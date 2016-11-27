//
//  Tentacle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/9/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Tentacle.hpp"

using namespace nautical;
using namespace climber;

Tentacle::Tentacle(nautical::Coordinate origin, nautical::Angle originAngle, nautical::Coordinate target) :
WorldObject(1, origin),
origin(origin),
originAngle(originAngle),
target(target) {
    Vector offset(originAngle, relaxedLength);
    tentacle[0].pos = origin;
    tentacle[0].angle = originAngle;
    tentacle[0].targetAngle = findAngle(origin, target);
    tentacle[0].tendon1Length = tentacle[0].tendon2Length = relaxedLength;
    for (int i = 1; i < TENTACLE_SEGMENT_COUNT; i++) {
        tentacle[i] = tentacle[i - 1];
        tentacle[i].pos += offset;
        tentacle[i].targetAngle = findAngle(tentacle[i].pos, target) - (M_PI_2 * i / TENTACLE_SEGMENT_COUNT);
    }
}

Tentacle::~Tentacle() { }

void Tentacle::update() {
    for (int i = 0; i < TENTACLE_SEGMENT_COUNT; i++) {
        Angle prevAngle = (i != 0) ? tentacle[i - 1].angle : originAngle;
        tentacle[i].pos = (i != 0) ? tentacle[i - 1].pos + Vector(prevAngle, relaxedLength) : origin;
        tentacle[i].targetAngle = findAngle(tentacle[i].pos, target);// - (M_PI_2 * i / TENTACLE_SEGMENT_COUNT);
        
        double targetAngleOffset = prevAngle.getValue() - tentacle[i].targetAngle.getValue();
        
        if (targetAngleOffset > 0) {
            tentacle[i].tendon1Length = fmax(relaxedLength / 2, tentacle[i].tendon1Length - muscleRelaxSpeed);
        } else if (targetAngleOffset < 0) {
            tentacle[i].tendon2Length = fmax(relaxedLength / 2, tentacle[i].tendon2Length - muscleRelaxSpeed);
        }
        
        tentacle[i].angle = prevAngle + Angle(muscleWidth, tentacle[i].tendon1Length - tentacle[i].tendon2Length);//fmin(fmax(relaxedLength, prevAngle.getValue() - M_PI_2), prevAngle.getValue() + M_PI_2);
        //tentacle[i].angle = Angle(angleVal);
    }
}

void Tentacle::draw() const {
    Coordinate prevCoor = tentacle[0].pos;
    for (int i = 1; i < TENTACLE_SEGMENT_COUNT; i++) {
        Line line(prevCoor, tentacle[i].pos);
        GraphicsManager::drawLine(line, Color(255, 153, 255));//, (127 * i / TENTACLE_SEGMENT_COUNT) + 127));
        prevCoor = tentacle[i].pos;
    }
    GraphicsManager::drawCoordinate(target, YELLOW);
}

//
//  GrapplingRope.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/5/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "GrapplingRope.hpp"

#include "Circle.hpp"
#include "Map.hpp"
#include "Player.hpp"

using namespace nautical;
using namespace climber;

GrapplingRope::GrapplingRope(nautical::Coordinate origin, double length, Vector extendVector, double retractSpeed) :
WorldObject(1, origin), //TODO what should the mass be?
StateMachine(EXTENDING),
origin(origin),
head(origin),
length(length),
extendVector(extendVector),
retractSpeed(retractSpeed),
hookAngle(extendVector.getAngle()) { }

GrapplingRope::~GrapplingRope() { }

Coordinate GrapplingRope::getOrigin() const {
    return origin;
}

GrapplingRope & GrapplingRope::setOrigin(nautical::Coordinate origin) {
    this->origin = origin;
    return *this;
}

Coordinate GrapplingRope::getHead() const {
    return head;
}

GrapplingRope & GrapplingRope::setHead(nautical::Coordinate head) {
    this->head = head;
    setPosition(head);
    return *this;
}

double GrapplingRope::getLength() const {
    return length;
}

GrapplingRope & GrapplingRope::setLength(double length) {
    this->length = length;
    return *this;
}

bool GrapplingRope::isTaught() const {
    return taught;
}

bool GrapplingRope::setState(int state) {
    if (getState() == EXTENDING) {
        if ((state == RETRACTING) && !overrideRetract) {
            shouldRetract = true;
            return true;
        }
    }
    
    return StateMachine::setState(state);
}

bool GrapplingRope::openState(int state) {
    switch (state) {
        case SET:
            Angle angle = findAngle(origin, head);
            head = origin + Vector(angle, length);
            break;
    }
    return true;
}

void GrapplingRope::update(World * p_world, double dt) { //TODO add dt
    switch (getState()) {
        case EXTENDING: {
            head += extendVector;
            if (findDistance(origin, head) > (length - 0.0001)) {
                setState((overrideRetract = shouldRetract) ? RETRACTING : SET);
            }
            
            wave = Path();
            Angle angle = findAngle(origin, head);
            double distance = findDistance(origin, head);
            Coordinate lastCoor = origin;
            for (double x = 0; x < distance; x++) {
                //Coordinate coor = origin + Vector(x, sin((8 * (length / distance)) * x) * ((64 * (distance - x)) / distance)); //TODO save this, this is awesome
                double amplitude = 16 * (1 - (x / distance));// * cos(distance / 64);
                double angularFrequency = ((((length - distance) / 3) + 1) / length);
                Coordinate coor = origin + Vector(x, amplitude * sin((angularFrequency * x) + (M_PI * 5))).rotate(angle);
                wave.addLine(Line(lastCoor, coor));
                lastCoor = coor;
            }
            break;
        }
        case SET: {
            double originHeadDistance = findDistance(origin, head);
            if (originHeadDistance > (length - 0.0001))
                taught = true;
            else if (originHeadDistance < (length - 0.0001))
                taught = false;
            break;
        }
        case RETRACTING: {
            if (length <= retractSpeed) {
                //getParent()->markObjectForRemoval(std::shared_ptr<WorldObject>(this)); //TODO?
                //p_parent->setRope(nullptr); //TODO TODO TODO
            } else {
                length -= retractSpeed;
                if (taught) {
                    head += Vector(findAngle(head, origin), retractSpeed);
                    hookAngle = findAngle(origin, head);
                } else {
                    double parabolaLengthDifference = findDistance(origin, head) - length;
                    if ((taught = (parabolaLengthDifference > 0))) { //purposely setting taught to result of equation
                        head += Vector(findAngle(head, origin), retractSpeed - parabolaLengthDifference);
                        hookAngle = findAngle(origin, head);
                    }
                }
            }
            break;
        }
    }
    line = Line(origin, head);
    parabola = Parabola(origin, head, length);
}

void GrapplingRope::draw() const {
    switch (getState()) {
        case EXTENDING: {
            wave.draw();
            break;
        }
        case SET: {
            if (World::DRAW_BUMPERS && DEBUG_MODE)
                Circle(head, length).setColor(Color(255, 255, 255, 64)).draw(); //DEBUGGING
            if (taught)
                GraphicsManager::drawLine(line, Color(255, 153, 153));
            else
                GraphicsManager::drawParabola(parabola, WHITE);
            break;
        }
        case RETRACTING: {
            if (taught)
                GraphicsManager::drawLine(line, WHITE);
            else
                GraphicsManager::drawParabola(parabola, WHITE);
            break;
        }
    }
    GraphicsManager::drawCoordinate(origin, WHITE);
    
    //draw hook
    static int arrowLength = 5;
    Angle angle(M_PI - hookAngle.getValue());// * ((angle < 0) ? -1 : 1));
    Angle angleMinus = angle - M_PI_4;
    Angle anglePlus = angle + M_PI_4;
    GraphicsManager::drawLine(Line(head.getX(), head.getY(), head.getX() - angleMinus.getSin(arrowLength), head.getY() - angleMinus.getCos(arrowLength)), getColor());
    GraphicsManager::drawLine(Line(head.getX(), head.getY(), head.getX() + anglePlus.getSin(arrowLength), head.getY() + anglePlus.getCos(arrowLength)), getColor());
}

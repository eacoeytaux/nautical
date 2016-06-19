//
//  Arc.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Arc.hpp"

#include "Utility.hpp"
#include "Vector.hpp"
#include "Circle.hpp"

using namespace nautical;

Arc::Arc(Coordinate origin, double originDistance, Angle startAngle, Angle endAngle, bool clockwise) :
origin(origin),
originDistance(originDistance),
startAngle(startAngle),
endAngle(endAngle),
clockwise(clockwise) {
    setDAngle();
}

Arc::~Arc() { }

Coordinate Arc::getOrigin() const {
    return origin;
}

Arc & Arc::setOrigin(Coordinate origin) {
    this->origin = origin;
    return *this;
}

double Arc::getOriginDistance() const {
    return originDistance;
}

Arc & Arc::setOriginDistance(double distance) {
    originDistance = distance;
    return *this;
}

Angle Arc::getStartAngle() const {
    return startAngle;
}

Arc & Arc::setStartAngle(Angle angle) {
    startAngle = angle;
    return setDAngle();
}

Angle Arc::getEndAngle() const {
    return endAngle;
}

Arc & Arc::setEndAngle(Angle angle) {
    endAngle = angle;
    return setDAngle();
}

Angle Arc::getDAngle() const {
    return dAngle;
}

Arc & Arc::setDAngle() {
    dAngle = (clockwise ? (startAngle - endAngle) : (endAngle - startAngle));
    return *this;
}

double Arc::getArcLength() const {
    Angle startAngle = this->startAngle;
    Angle endAngle = this->endAngle;
    if (clockwise)
        swap<Angle>(startAngle, endAngle);
    
    return dAngle.getValue() * originDistance;
}

bool Arc::isClockwise() const {
    return clockwise;
}

Arc & Arc::setClockwise(bool clockwise) {
    this->clockwise = clockwise;
    return *this;
}

Coordinate Arc::getStartCoor() const {
    return origin + Vector(startAngle, originDistance);
}

Coordinate Arc::getEndCoor() const {
    return origin + Vector(endAngle, originDistance);
}

bool Arc::intersectsArc(Arc arc, Queue<Coordinate> * p_intersections) const {
    Queue<Coordinate> intersections;
    Circle tempCircle(origin, originDistance);
    Circle(arc.origin, arc.originDistance).intersectsCircle(&tempCircle, &intersections);
    
    bool intersects = false;
    Coordinate intersection1;
    if (intersections.pop(&intersection1)) {
        if (inArc(findAngle(origin, intersection1)))
            intersects = true;
        
        Coordinate intersection2;
        if (intersections.pop(&intersection2)) {
            if (inArc(findAngle(origin, intersection1))) {
                if (p_intersections) {
                    if (intersects) {
                        Angle startAngle = (clockwise ? startAngle : endAngle);
                        if ((startAngle - findAngle(origin, intersection1)).getValue() <= (startAngle - findAngle(origin, intersection2)).getValue()) {
                            p_intersections->insert(intersection1);
                            p_intersections->insert(intersection2);
                        } else {
                            p_intersections->insert(intersection2);
                            p_intersections->insert(intersection1);
                        }
                    } else {
                        p_intersections->insert(intersection2);
                    }
                }
                intersects = true;
            }
        } else if (intersects) {
            if (p_intersections)
                p_intersections->insert(intersection1);
        }
    }
    return intersects;
}

bool Arc::intersectsLine(Line line, Queue<Coordinate> * p_intersections) const {
    Queue<Coordinate> intersections;
    Circle(origin, originDistance).intersectsLine(line, &intersections);
    
    bool intersects = false;
    Coordinate intersection1;
    if (intersections.pop(&intersection1)) {
        if (inArc(findAngle(origin, intersection1)))
            intersects = true;
        
        Coordinate intersection2;
        if (intersections.pop(&intersection2)) {
            if (inArc(findAngle(origin, intersection1))) {
                if (p_intersections) {
                    if (intersects) {
                        Angle startAngle = (clockwise ? startAngle : endAngle);
                        if ((startAngle - findAngle(origin, intersection1)).getValue() <= (startAngle - findAngle(origin, intersection2)).getValue()) {
                            p_intersections->insert(intersection1);
                            p_intersections->insert(intersection2);
                        } else {
                            p_intersections->insert(intersection2);
                            p_intersections->insert(intersection1);
                        }
                    } else {
                        p_intersections->insert(intersection2);
                    }
                }
                intersects = true;
            }
        } else if (intersects) {
            if (p_intersections)
                p_intersections->insert(intersection1);
        }
    }
    return intersects;
}

bool Arc::inArc(Angle angle) const {
    return ((clockwise ? startAngle : endAngle) - angle).getValue() <= dAngle.getValue();
}

bool Arc::inArc(Coordinate coor) const {
    return ((findDistance(origin, coor) <= originDistance) && inArc(findAngle(origin, coor)));
}

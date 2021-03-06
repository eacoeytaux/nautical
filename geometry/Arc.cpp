//
//  Arc.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Arc.hpp"

#include <algorithm>

#include "Utility.hpp"
#include "Vector.hpp"
#include "Circle.hpp"

using namespace nautical;

Arc::Arc(const Coordinate & origin, double originDistance, const Angle & startAngle, const Angle & endAngle, bool clockwise) :
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

Arc & Arc::setOrigin(const Coordinate & origin) {
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

Arc & Arc::setStartAngle(const Angle & angle) {
    startAngle = angle;
    setDAngle();
    return *this;
}

Angle Arc::getEndAngle() const {
    return endAngle;
}

Arc & Arc::setEndAngle(const Angle & angle) {
    endAngle = angle;
    setDAngle();
    return *this;
}

Angle Arc::getDAngle() const {
    return dAngle;
}

void Arc::setDAngle() {
    dAngle = (clockwise ? (startAngle - endAngle) : (endAngle - startAngle));
}

double Arc::getArcLength() const {
    Angle startAngle = this->startAngle;
    Angle endAngle = this->endAngle;
    if (clockwise)
        std::swap(startAngle, endAngle);
    
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

bool Arc::intersectsArc(const Arc & arc, std::vector<Coordinate> * p_intersections) const {
    std::vector<Coordinate> intersections;
    Circle tempCircle(origin, originDistance);
    Circle(arc.origin, arc.originDistance).intersectsCircle(&tempCircle, &intersections);
    
    bool intersects = false;
    if (intersections.size() > 0) {
        Coordinate intersection1 = intersections.at(0);
        if (inArc(findAngle(origin, intersection1)))
            intersects = true;
        
        if (intersections.size() > 1) {
            Coordinate intersection2 = intersections.at(1);
            if (inArc(findAngle(origin, intersection1))) {
                if (p_intersections) {
                    if (intersects) {
                        Angle startAngle = (clockwise ? startAngle : endAngle);
                        if ((startAngle - findAngle(origin, intersection1)).getValue() <= (startAngle - findAngle(origin, intersection2)).getValue()) {
                            p_intersections->push_back(intersection1);
                            p_intersections->push_back(intersection2);
                        } else {
                            p_intersections->push_back(intersection2);
                            p_intersections->push_back(intersection1);
                        }
                    } else {
                        p_intersections->push_back(intersection2);
                    }
                }
                intersects = true;
            }
        } else if (intersects) {
            if (p_intersections)
                p_intersections->push_back(intersection1);
        }
    }
    return intersects;
}

bool Arc::intersectsLine(const Line & line, std::vector<Coordinate> * p_intersections) const {
    std::vector<Coordinate> intersections;
    Circle(origin, originDistance).intersectsLine(line, &intersections);
    
    bool intersects = false;
    if (intersections.size() > 0) {
        Coordinate intersection1 = intersections.at(0);
        if (inArc(findAngle(origin, intersection1)))
            intersects = true;
        
        if (intersections.size() > 1) {
            Coordinate intersection2 = intersections.at(1);
            if (inArc(findAngle(origin, intersection1))) {
                if (p_intersections) {
                    if (intersects) {
                        Angle startAngle = (clockwise ? startAngle : endAngle);
                        if ((startAngle - findAngle(origin, intersection1)).getValue() <= (startAngle - findAngle(origin, intersection2)).getValue()) {
                            p_intersections->push_back(intersection1);
                            p_intersections->push_back(intersection2);
                        } else {
                            p_intersections->push_back(intersection2);
                            p_intersections->push_back(intersection1);
                        }
                    } else {
                        p_intersections->push_back(intersection2);
                    }
                }
                intersects = true;
            }
        } else if (intersects) {
            if (p_intersections)
                p_intersections->push_back(intersection1);
        }
    }
    return intersects;
}

bool Arc::inArc(const Angle & angle) const {
    return ((clockwise ? startAngle : endAngle) - angle).getValue() <= dAngle.getValue();
}

bool Arc::inArc(const Coordinate & coor) const {
    return ((findDistance(origin, coor) <= originDistance) && inArc(findAngle(origin, coor)));
}

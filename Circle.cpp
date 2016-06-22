//
//  Circle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Circle.hpp"

using namespace nautical;

bool Circle::circleVectorsCalculated = false;
Vector Circle::circleVectors[CIRCLE_PERCISION];

Circle::Circle(Coordinate center, double radius) :
center(center),
radius(radius) {
    appendTag(CIRCLE_TAG);
    
    if (!circleVectorsCalculated) {
        for (int i = 0; i < CIRCLE_PERCISION - 2; i++) {
            circleVectors[i] = Vector(cos((M_PI * 2) * ((float)i / (float)(CIRCLE_PERCISION - 1))), sin((M_PI * 2) * ((float)i / (float)(CIRCLE_PERCISION - 1))));
        }
        circleVectors[CIRCLE_PERCISION - 1] = circleVectors[0];
        circleVectorsCalculated = true;
    }
}

Circle::~Circle() { }

Coordinate Circle::getCenter() const {
    return center;
}

Circle & Circle::setCenter(Coordinate center) {
    this->center = center;
    return *this;
}

double Circle::getRadius() const {
    return radius;
}

Circle & Circle::setRadius(double radius) {
    this->radius = radius;
    return *this;
}

double Circle::convertToRadians(double distance) const {
    return distance / radius;
}

double Circle::convertToDistance(double radians) const {
    return radians * radius;
}

double Circle::getLowerBoundX() const {
    return center.getX() - radius;
}

double Circle::getLowerBoundY() const {
    return center.getY() - radius;
}

double Circle::getUpperBoundX() const {
    return center.getX() + radius;
}

double Circle::getUpperBoundY() const {
    return center.getY() + radius;
}

bool Circle::contains(Coordinate coor) const {
    return findDistance(center, coor) <= radius;
}

bool Circle::intersectsLine(Line line, std::vector<Coordinate> * p_intersections) const {
    bool lineIsVertical = line.isVertical(); //if line is vertical then m and b won't be properly set so the line is rotated by M_PI_4 to give it a proper m and b and then line and collisions are rotated back into place
    if (lineIsVertical)
        line.rotateAboutCoordinate(center, -M_PI_2);
    
    double a = (pow(line.getM(), 2) + 1);
    double b = ((-2 * center.getX()) + (2 * line.getB() * line.getM()) + (-2 * line.getM() * center.getY()));
    double c = (pow(center.getX(), 2) + pow(line.getB(), 2) + (-2 * line.getB() * center.getY()) + pow(center.getY(), 2) - pow(radius, 2));
    
    double xPlus = solveQuadraticPlus(a, b, c);
    double xMinus = solveQuadraticMinus(a, b, c);
    
    double yPlus = (line.getM() * xPlus) + line.getB();
    double yMinus = (line.getM() * xMinus) + line.getB();
    
    Coordinate plus(xPlus, yPlus);
    Coordinate minus(xMinus, yMinus);
    
    if (lineIsVertical) { //if line was rotated, undo rotation on line and collisions
        line.rotateAboutCoordinate(center, M_PI_2);
        plus.rotateAboutCoordinate(center, M_PI_2);
        minus.rotateAboutCoordinate(center, M_PI_2);
    }
    
    if (line.inBox(plus)) {
        if (p_intersections) {
            if (line.inBox(minus)) {
                if (findDistance(plus, line.getCoor1()) < findDistance(minus, line.getCoor1())) {
                    p_intersections->push_back(plus);
                    p_intersections->push_back(minus);
                } else {
                    p_intersections->push_back(minus);
                    p_intersections->push_back(plus);
                }
            } else {
                p_intersections->push_back(plus);
            }
        }
        return true;
    } else if (line.inBox(minus)) {
        if (p_intersections)
            p_intersections->push_back(minus);
        return true;
    } else {
        return false;
    }
}

bool Circle::intersectsShape(const Shape * p_shape, std::vector<Coordinate> * p_intersections) const {
    if (p_shape->hasTag(CIRCLE_TAG))
        return intersectsCircle(static_cast<const Circle*>(p_shape), p_intersections);
    
    return p_shape->intersectsShape(this, p_intersections);
}

bool Circle::intersectsCircle(const Circle * p_circle, std::vector<Coordinate> * p_intersections) const {
    double distance = findDistance(center, p_circle->center);
    Angle circleAngle = findAngle(center, p_circle->center);
    
    if (distance > radius + p_circle->radius) {
        return false;
    } else if (distance == radius + p_circle->radius) {
        if (p_intersections)
            p_intersections->push_back(center + Vector(circleAngle, radius));
        return true;
    }
    
    double x = (pow(distance, 2) - pow(p_circle->radius, 2) + pow(radius, 2)) / (distance * 2);
    double triangleAngle = acos(x / radius);
    
    if (p_intersections) {
        Angle angle1 = circleAngle + triangleAngle, angle2 = circleAngle - triangleAngle;
        p_intersections->push_back(center + Vector(angle1, radius));
        p_intersections->push_back(center + Vector(angle2, radius));
    }
    
    return true;
}

Circle & Circle::move(Vector vector) {
    setCenter(center += vector);
    return *this;
}

Circle & Circle::rotateAboutCoordinate(Coordinate coor, Angle angle) {
    center.rotateAboutCoordinate(coor, angle);
    return *this;
}

void Circle::draw() const {
    Coordinate prevCoor = center + (circleVectors[0] * radius);
    for (int i = 1; i < CIRCLE_PERCISION; i++) {
        Coordinate coor = center + (circleVectors[i] * radius);
        GraphicsManager::drawLine(Line(prevCoor, coor), getColor());
        prevCoor = coor;
    }
}

Circle * Circle::copyPtr_() const {
    return new Circle(*this);
}

bool Circle::operator==(const Shape & shape) const {
    if (getTag() == shape.getTag()) {
        Circle circle = (Circle&)shape;
        return ((center == circle.center) && (radius == circle.radius));
    }
    return false;
}


//
//  Coordinate.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Coordinate.hpp"

#include "Vector.hpp"

using namespace nautical;

Coordinate::Coordinate(double x, double y) :
x(x),
y(y) { }

double Coordinate::getX() const {
    return x;
}

Coordinate & Coordinate::setX(double x) {
    this->x = x;
    return *this;
}

Coordinate & Coordinate::moveX(double dx) {
    x += dx;
    return *this;
}

double Coordinate::getY() const {
    return y;
}

Coordinate & Coordinate::setY(double y) {
    this->y = y;
    return *this;
}

Coordinate & Coordinate::moveY(double dy) {
    y += dy;
    return *this;
}

Coordinate & Coordinate::rotateAboutCoordinate(const Coordinate & coor, const Angle & angle) {
    return *this = Vector(coor, *this).rotate(angle).getDestination();
}

bool Coordinate::operator==(const Coordinate & coor) const {
    return ((x == coor.x) && (y == coor.y));
}

bool Coordinate::operator!=(const Coordinate & coor) const {
    return !((x == coor.x) && (y == coor.y));
}

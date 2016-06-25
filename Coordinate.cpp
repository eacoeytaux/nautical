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

void Coordinate::setX(double x) {
    this->x = x;
}

void Coordinate::moveX(double dx) {
    x += dx;
}

double Coordinate::getY() const {
    return y;
}

void Coordinate::setY(double y) {
    this->y = y;
}

void Coordinate::moveY(double dy) {
    y += dy;
}

void Coordinate::rotateAboutCoordinate(Coordinate coor, Angle angle) {
    *this = Vector(coor, *this).rotate(angle).getDestination();
}

bool Coordinate::operator==(const Coordinate & coor) const {
    return ((x == coor.x) && (y == coor.y));
}

bool Coordinate::operator!=(const Coordinate & coor) const {
    return !((x == coor.x) && (y == coor.y));
}

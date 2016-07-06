//
//  Mass.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Mass.hpp"

using namespace nautical;
using namespace physics;

Mass::Mass(Coordinate pos, double m ) :
m(m),
pos(pos) { }

Mass::~Mass() { }

double Mass::getM() const {
    return m;
}

Mass & Mass::setM(double m) {
    this->m = m;
    return *this;
}

Coordinate Mass::getPosition() const {
    return pos;
}

Mass & Mass::setPosition(const Coordinate & pos) {
    this->pos = pos;
    return *this;
}

Mass & Mass::updatePosition() {
    pos += vel;
    return *this;
}

Vector Mass::getVelocity() const  {
    return vel;
}

Mass & Mass::setVelocity(const Vector & vel) {
    this->vel = vel;
    return *this;
}

Mass & Mass::addVelocity(const Vector & vel) {
    this->vel += vel;
    return *this;
}

Mass & Mass::updateVelocity()  {
    vel += force / m;
    force.setMagnitude(0);
    return *this;
}

Vector Mass::getForce() const {
    return force;
}

Mass & Mass::setForce(const Vector & force) {
    this->force = force;
    return *this;
}

Mass & Mass::addForce(const Vector & force) {
    this->force += force;
    return *this;
}

Mass & Mass::setAcceleration(const Vector & accel) {
    this->force = accel * m;
    return *this;
}

Mass & Mass::addAcceleration(const Vector & accel) {
    this->force += accel * m;
    return *this;
}

Mass & Mass::updateForce() {
    return *this;
}

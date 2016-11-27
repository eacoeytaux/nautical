//
//  Mass.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Mass.hpp"

using namespace nautical;

Mass::Mass(double m, Coordinate pos, Vector vel) :
m(m),
pos(pos),
vel(vel) {
    vel.setOrigin(pos);
}

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

Mass & Mass::movePosition(const Vector & dPos) {
    pos += dPos;
    return *this;
}

Vector Mass::getVelocity() const  {
    return vel;
}

Mass & Mass::setVelocity(const Vector & vel) {
    this->vel = vel;
    this->vel.setOrigin(pos);
    return *this;
}

Mass & Mass::addVelocity(const Vector & vel) {
    this->vel += vel;
    return *this;
}

bool Mass::isImmobile() const {
    return immobile;
}

Mass & Mass::setImmobile(bool b) {
    immobile = b;
    return *this;
}

Mass & Mass::addAccelerator(std::shared_ptr<Accelerator> accelerator) {
    accelerators.push_back(accelerator);
    return *this;
}

//RK4 implemented from http://gafferongames.com/game-physics/integration-basics/

std::pair<Coordinate, Vector> Mass::update(double dt) {
    if (immobile)
        return std::pair<Coordinate, Vector>();
    
    std::pair<Vector, Vector> a, b, c, d; //first = dPos/dt, second = dVel/dt
    
    a = evaluate(std::pair<Vector, Vector>(), 0);
    b = evaluate(a, 0.5 * dt);
    c = evaluate(b, 0.5 * dt);
    d = evaluate(c, dt);
    
    //Taylor Series expansion, number values should not be made variables
    Vector dPos = (a.first + ((b.first + c.first) * 2.0) + d.first) * (1.0 / 6.0);
    Vector dVel = (a.second + ((b.second + c.second) * 2.0) + d.second) * (1.0 / 6.0);
    
    pos += dPos * dt;
    vel += dVel * dt;
    
    if (vel.getMagnitude() < ROUNDING_ERROR_MARGIN) {
        vel.setMagnitude(0);
    }
    if (dPos.getMagnitude() < ROUNDING_ERROR_MARGIN) {
        dPos.setMagnitude(0);
    }
    if (dVel.getMagnitude() < ROUNDING_ERROR_MARGIN) {
        dVel.setMagnitude(0);
    }
    
    accelerators.clear();
    
    return std::pair<Coordinate, Vector>(pos, vel);
}

std::pair<Vector, Vector> Mass::evaluate(const std::pair<Vector, Vector> & derivative, double dt) { //helper function for update
    Mass temp(m, this->pos + derivative.first * dt, this->vel + derivative.second * dt);
    
    std::pair<Vector, Vector> ret;
    ret.first = temp.vel;
    for (std::vector<std::shared_ptr<Mass::Accelerator>>::iterator it = accelerators.begin(); it != accelerators.end(); it++) {
        ret.second += (*it)->accelerate(temp);
    }
    
    return ret;
}

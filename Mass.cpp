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

Mass::Mass(double m, Coordinate pos, Vector vel) :
m(m),
pos(pos),
vel(vel) { }

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

void Mass::update(double dt) {
    if (immobile)
        return;
    
    std::pair<Vector, Vector> a, b, c, d; //first = dPos/dt, second = dVel/dt
    
    a = evaluate(std::pair<Vector, Vector>(), 0);
    b = evaluate(a, 0.5 * dt);
    c = evaluate(b, 0.5 * dt);
    d = evaluate(c, dt);
    
    //Taylor Series expansion, should not be made variables
    Vector dPos = (a.first + ((b.first + c.first) * 2.0) + d.first) * (1.0 / 6.0);
    Vector dVel = (a.second + ((b.second + c.second) * 2.0) + d.second) * (1.0 / 6.0);
    
    printf("pos %f:%f\n", pos.getX(), pos.getY());
    printf("vel %f:%f\n", vel.getDx(), vel.getDy());
    printf("dPos %f:%f\n", dPos.getDx(), dPos.getDy());
    printf("dVel %f:%f\n\n", dVel.getDx(), dVel.getDy());
    
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

//
//  Spring.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Spring.hpp"

using namespace nautical;
using namespace physics;

Spring::Spring(Mass * p_mass1, Mass * p_mass2, double length, double k) :
p_mass1(p_mass1),
p_mass2(p_mass2),
length(length),
k(k) { }

Spring::~Spring() { }

const Mass * Spring::getMass1() {
    return p_mass1;
}

Spring & Spring::setMass1(Mass * p_mass) {
    p_mass1 = p_mass;
    return *this;
}

const Mass * Spring::getMass2() {
    return p_mass2;
}

Spring & Spring::setMass2(Mass * p_mass) {
    p_mass2 = p_mass;
    return *this;
}

double Spring::getK() const {
    return k;
}

Spring & Spring::setK(double k) {
    this->k = k;
    return *this;
}

double Spring::getLength() const {
    return length;
}

Spring & Spring::setLength(double length) {
    this->length = length;
    return *this;
}

void Spring::update() {
    Vector vec(p_mass1->getPosition(), p_mass2->getPosition());
    
    Vector force = vec;
    force.setMagnitude(k * (vec.getMagnitude() - length));
    
    force -= (p_mass1->getVelocity() - p_mass2->getVelocity()) * 0.2;
    
    p_mass1->addForce(force);
    p_mass2->addForce(force * -1);
}

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

Spring::Spring(Mass * p_mass1, Mass * p_mass2, double length, double k, double damper) :
p_mass1(p_mass1),
p_mass2(p_mass2),
length(length),
k(k),
damper(damper) { }

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
    struct SpringAccelerator : public Mass::Accelerator {
        SpringAccelerator(const Mass * p_otherMass, double length, double k, double damper) :
        length(length),
        k(k),
        damper(damper),
        p_otherMass(p_otherMass) { }
        
        const Mass * p_otherMass = nullptr;
        double length, k, damper;
        Vector accelerate(const Mass & mass) const {
            if (!p_otherMass) {
                Logger::writeLog(ERROR_MESSAGE, "Spring::SpringAccelerator::accelerate(): p_otherMass is null");
                return Vector();
            }
            
            Vector vec(mass.getPosition(), p_otherMass->getPosition());
            Vector force = vec;
            force.setMagnitude(k * (vec.getMagnitude() - length)) -= (mass.getVelocity() - p_otherMass->getVelocity()) * damper;
            return force * mass.getM();
        }
    };
    
    std::shared_ptr<SpringAccelerator> p_accelerator1(new SpringAccelerator(p_mass2, length, k, damper)),
    p_accelerator2(new SpringAccelerator(p_mass1, length, k, damper));
    
    p_mass1->addAccelerator(p_accelerator1);
    p_mass2->addAccelerator(p_accelerator2);
}

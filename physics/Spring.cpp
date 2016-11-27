//
//  Spring.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Spring.hpp"

using namespace nautical;

Spring::Spring(std::shared_ptr<Mass> p_mass1, std::shared_ptr<Mass> p_mass2, double length, double k, double damper) :
p_mass1(p_mass1),
p_mass2(p_mass2),
length(length),
k(k),
damper(damper) { }

Spring::~Spring() { }

std::shared_ptr<const Mass> Spring::getMass1() const {
    return p_mass1;
}

Spring & Spring::setMass1(std::shared_ptr<Mass> p_mass) {
    p_mass1 = p_mass;
    return *this;
}

std::shared_ptr<const Mass> Spring::getMass2() const {
    return p_mass2;
}

Spring & Spring::setMass2(std::shared_ptr<Mass> p_mass) {
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
        SpringAccelerator(std::shared_ptr<const Mass> p_otherMass, double length, double k, double damper) :
        Mass::Accelerator(Vector()),
        length(length),
        k(k),
        damper(damper),
        p_otherMass(p_otherMass) { }
        
        std::shared_ptr<const Mass> p_otherMass = nullptr;
        double length, k, damper;
        Vector accelerate(const Mass & mass) const {
            if (!p_otherMass) {
                Logger::writeLog(ERROR, "Spring::SpringAccelerator::accelerate(): p_otherMass is null");
                return Vector();
            }
            
            Vector vec(mass.getPosition(), p_otherMass->getPosition());
            Vector force = vec;
            //if (vec.getMagnitude() > length) {
                force.setMagnitude(k * (vec.getMagnitude() - length)) -= (mass.getVelocity() - p_otherMass->getVelocity()) * damper;
                return force * mass.getM();
            //} else {
            //    return Vector();
            //}
        }
    };
    
    std::shared_ptr<SpringAccelerator> p_accelerator1(new SpringAccelerator(p_mass2, length, k, damper)),
    p_accelerator2(new SpringAccelerator(p_mass1, length, k, damper));
    
    p_mass1->addAccelerator(p_accelerator1);
    p_mass2->addAccelerator(p_accelerator2);
}

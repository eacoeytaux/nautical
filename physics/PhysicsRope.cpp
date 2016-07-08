//
//  PhysicsRope.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "PhysicsRope.hpp"

#include "GraphicsManager.hpp" //TODO delete

using namespace nautical;
using namespace physics;

PhysicsRope::PhysicsRope(Coordinate anchor, double length, double segmentLength) {
    static double ropeMass = 0.05;
    
    Mass * p_prevMass = new Mass(ropeMass, anchor);
    p_prevMass->setImmobile(true);
    masses.push_back(p_prevMass);
    for (double i = 0; i < length; i += segmentLength) {
        Mass * p_mass = new Mass(ropeMass, anchor + Vector(i + segmentLength, 0));
        masses.push_back(p_mass);
        springs.push_back(new Spring(p_prevMass, p_mass, segmentLength, 50, 2));
        p_prevMass = p_mass;
    }
    //p_prevMass->setM(ropeMass * 3);
}

PhysicsRope::~PhysicsRope() { //TODO
    
}

PhysicsRope & PhysicsRope::setAnchor(const Coordinate & anchor) {
    masses.front()->setPosition(anchor);
    return *this;
}

void PhysicsRope::update() {
    for (std::vector<Spring*>::const_iterator it = springs.begin(); it != springs.end(); it++) {
        (*it)->update();
    }
    
    for (std::vector<Mass*>::const_iterator it = masses.begin() + 1; it != masses.end(); it++) {
        Mass * p_mass = *it;
        
        std::shared_ptr<Mass::ForceAccelerator> p_accelerator(new Mass::ForceAccelerator);
        p_accelerator->force = Vector(0, -0.3) / p_mass->getM();
        p_mass->addAccelerator(p_accelerator);
        p_mass->update();
        
        //p_mass->addForce(Vector(0, -0.3) * p_mass->getM());
        //p_mass->updateVelocity();
        //p_mass->setVelocity(p_mass->getVelocity() * 0.99);
        //p_mass->updatePosition();
    }
}

void PhysicsRope::draw() const {
    GraphicsManager::drawCoordinate(springs.front()->getMass1()->getPosition());
    for (std::vector<Spring*>::const_iterator it = springs.begin(); it != springs.end(); it++) {
        GraphicsManager::drawLine(Line((*it)->getMass1()->getPosition(), (*it)->getMass2()->getPosition()));
    }
//    for (std::vector<Mass*>::const_iterator it = masses.begin(); it != masses.end(); it++) {
//        GraphicsManager::drawCoordinate((*it)->getPosition());
//    }
}

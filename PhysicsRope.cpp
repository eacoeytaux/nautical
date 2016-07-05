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
    Mass * p_prevMass = new Mass(anchor);
    masses.push_back(p_prevMass);
    for (int i = 0; i < length; i += segmentLength) {
        Mass * p_mass = new Mass(anchor + Vector(i + segmentLength, 0));
        masses.push_back(p_mass);
        springs.push_back(new Spring(p_prevMass, p_mass, segmentLength, 0.5));
        p_prevMass = p_mass;
    }
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
        
        p_mass->addToForce(Vector(0, -0.3) * p_mass->getM());
        p_mass->updateVelocity();
        p_mass->setVelocity(p_mass->getVelocity() * 0.99);
        p_mass->updatePosition();
        p_mass->setForce(Vector(0, 0));
    }
}

void PhysicsRope::draw() const {
    GraphicsManager::drawCoordinate(springs.front()->getMass1()->getPosition());
    for (std::vector<Spring*>::const_iterator it = springs.begin(); it != springs.end(); it++) {
        GraphicsManager::drawLine(Line((*it)->getMass1()->getPosition(), (*it)->getMass2()->getPosition()));
    }
}

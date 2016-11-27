//
//  Rope.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Rope.hpp"

#include "GraphicsManager.hpp" //TODO delete

using namespace nautical;

Rope::Rope(Coordinate anchor, double length, double segmentLength) {
    static double ropeMass = 0.1;
    
    Mass prevMass(ropeMass, anchor);
    prevMass.setImmobile(true);
    masses.push_back(prevMass);
    for (double i = 0; i < length; i += segmentLength) {
        Mass mass(ropeMass, anchor + Vector(i + segmentLength, 0));
        masses.push_back(mass);
        springs.push_back(Spring(std::shared_ptr<Mass>(&prevMass), std::shared_ptr<Mass>(&mass), segmentLength, 25, 0.2));
        prevMass = mass;
    }
    //p_prevMass->setM(ropeMass * 3);
}

Rope::~Rope() { //TODO
    
}

Rope & Rope::setAnchor(const Coordinate & anchor) {
    masses.front().setPosition(anchor);
    return *this;
}

void Rope::update() {
    for (std::vector<Spring>::iterator it = springs.begin(); it != springs.end(); it++) {
        it->update();
    }
    
    for (std::vector<Mass>::iterator it = masses.begin() + 1; it != masses.end(); it++) {
        std::shared_ptr<Mass::ForceAccelerator> p_accelerator(new Mass::ForceAccelerator(Vector(0, -0.3))); //TODO change to gravity?
        p_accelerator->force = Vector(0, -0.3) * it->getM();
        it->addAccelerator(p_accelerator);
        it->update();
        
        //p_mass->addForce(Vector(0, -0.3) * p_mass->getM());
        //p_mass->updateVelocity();
        //p_mass->setVelocity(p_mass->getVelocity() * 0.99);
        //p_mass->updatePosition();
    }
}

void Rope::draw() const {
    GraphicsManager::drawCoordinate(springs.front().getMass1()->getPosition());
    for (std::vector<Spring>::const_iterator it = springs.begin(); it != springs.end(); it++) {
        GraphicsManager::drawLine(Line(it->getMass1()->getPosition(), it->getMass2()->getPosition()));
    }
    for (std::vector<Mass>::const_iterator it = masses.begin(); it != masses.end(); it++) {
        GraphicsManager::drawCoordinate(it->getPosition());
    }
}

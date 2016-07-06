//
//  Rope2.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Rope2.hpp"

#define MAX_DISTANCE 5.0

using namespace nautical;
using namespace climber;

struct Rope2::RopeJoint {
    Coordinate pos;
    physics::Vector vel, force;
};

Rope2::Rope2(Coordinate anchor, double length) :
WorldObject(anchor) {
    for (int i = 0; i < length; i++) {
        RopeJoint * p_joint = new RopeJoint();
        p_joint->pos = anchor + physics::Vector(Angle(Random::getRandDouble(M_PI * 2)), MAX_DISTANCE * i);
        joints.push_back(p_joint);
    }
}

Rope2::~Rope2() { }

void Rope2::update() {
    std::vector<RopeJoint*>::iterator it = joints.begin();
    for (it++; it != joints.end(); it++) {
        (*it)->force += physics::Vector(0, -0.3);
        
        if (Random::getRandBool(0.001f)) {
            (*it)->force += physics::Vector(15, 0);
        }
        
        (*it)->vel += (*it)->force;
        (*it)->pos += (*it)->vel;
        (*it)->force.setMagnitude(0);
    }
    
    it = joints.begin();
    RopeJoint * p_prevJoint = *it;
    for (it++; it != joints.end(); it++) {
        RopeJoint * p_joint = *it;
        
        double distance = findDistance(p_prevJoint->pos, p_joint->pos);
        
        if (distance != 0) {
            physics::Vector correction(p_prevJoint->pos, p_joint->pos);
            correction *= (MAX_DISTANCE / (distance - 1.0));
            
            if (p_prevJoint != joints.front()) {
                p_prevJoint->pos -= correction * 0.5;
                p_joint->pos += correction * 0.5;
            } else {
                p_joint->pos += correction;
            }
            
            //            Vector correction(p_prevJoint->pos, p_joint->pos);
            //            correction *= -(MAX_DISTANCE / (distance - 1.0));
            //
            //            p_joint->pos += correction;
            //
            //            p_prevJoint = p_joint;
        }
        p_prevJoint = p_joint;
    }
}

void Rope2::draw() const {
    std::vector<RopeJoint*>::const_iterator it = joints.begin();
    RopeJoint * p_prevJoint = *it;
    for (it++; it != joints.end(); it++) {
        GraphicsManager::drawLine(Line(p_prevJoint->pos, (*it)->pos));
        p_prevJoint = *it;
    }
}

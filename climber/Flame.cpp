//
//  Flame.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/24/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Flame.hpp"

#include "Random.hpp"

using namespace nautical;
using namespace climber;

//TODO use more defines
#define FADE_SPEED 2.0

Flame::Flame(nautical::Coordinate pos) :
WorldObject(0, pos) {
    appendTag(FLAME_TAG);
    
    p_spriteSheet = GraphicsManager::loadSpriteSheet("spritesheets/fire-particles.png", 2, 1, 10);
}

Flame::~Flame() {
    for (std::vector<Spark*>::iterator it = orangeParticles.begin(); it != orangeParticles.end(); it++) {
        delete *it;
    }
    for (std::vector<Spark*>::iterator it = yellowParticles.begin(); it != yellowParticles.end(); it++) {
        delete *it;
    }
    for (std::vector<Origin*>::iterator it = origins.begin(); it != origins.end(); it++) {
        delete *it;
    }
    
    delete p_spriteSheet;
}

Flame & Flame::addOrigin(double flameWidth, nautical::Vector offset) {
    Origin * p_origin = new Origin;
    p_origin->origin = getPosition() + offset;
    p_origin->flameWidth = flameWidth;
    origins.push_back(p_origin);
    return *this;
}

void Flame::update(World * p_world, double dt) {
    for (std::vector<Origin*>::iterator it = origins.begin(); it != origins.end(); it++) {
        Origin * p_origin = *it;
        
        //orange particles
        if (p_origin->orangeCountdown.check(dt)) {
            Spark * p_spark = new Spark;
            p_spark->center = p_origin->origin;
            p_spark->width = p_origin->flameWidth * (Random::getRandDouble(0.3) + 0.7);
            p_spark->alpha = 255;
            orangeParticles.push_back(p_spark);
            p_origin->orangeCountdown.reset(5 + Random::getRandInt(5));
        }
        
        //yellow particles
        if (p_origin->yellowCountdown.check(dt)) {
            Spark * p_spark = new Spark;
            p_spark->center = p_origin->origin;
            p_spark->width = p_origin->flameWidth * (Random::getRandDouble(0.2) + 0.3);
            p_spark->alpha = 255;
            yellowParticles.push_back(p_spark);
            p_origin->yellowCountdown.reset(7 + Random::getRandInt(5));
        }
    }
    
    //orange particles
    std::vector<Spark*> sparksToDelete;
    for (std::vector<Spark*>::iterator it = orangeParticles.begin(); it != orangeParticles.end(); it++) {
        Spark * p_spark = *it;
        
        p_spark->center += Vector(Angle(Random::getRandDouble(M_PI_2) + M_PI_4), Random::getRandDouble(0.5) + 0.5) * dt;
        p_spark->width *= (1.0 - (0.03 * dt));
        p_spark->alpha -= FADE_SPEED * dt;
        
        if ((p_spark->width < (1 / GraphicsManager::getZoom())) || (p_spark->alpha <= 0))
            sparksToDelete.push_back(p_spark);
    }
    for (std::vector<Spark*>::iterator it = sparksToDelete.begin(); it != sparksToDelete.end(); it++) {
        vector_helpers::removeElementByValue(orangeParticles, *it);
        delete *it;
    }
    
    //yellow particles
    sparksToDelete.clear();
    for (std::vector<Spark*>::iterator it = yellowParticles.begin(); it != yellowParticles.end(); it++) {
        Spark * p_spark = *it;
        
        p_spark->center += Vector(Angle(Random::getRandDouble(M_PI_2) + M_PI_4), Random::getRandDouble(0.5) + 0.5) * dt;
        p_spark->width *= (1.0 - (0.03 * dt));
        p_spark->alpha -= FADE_SPEED * dt;
        
        if ((p_spark->width < (1 / GraphicsManager::getZoom())) || (p_spark->alpha <= 0))
            sparksToDelete.push_back(p_spark);
    }
    for (std::vector<Spark*>::iterator it = sparksToDelete.begin(); it != sparksToDelete.end(); it++) {
        vector_helpers::removeElementByValue(yellowParticles, *it);
        delete *it;
    }
    
    if (DarknessOverlay::isInEffect()) {
        std::shared_ptr<Circle> p_circle = std::shared_ptr<Circle>(new Circle(getPosition(), 100));
        static Countdown count(1); //TODO don't make static
        static Vector moveVec[DARKNESS_LAYERS];
        if (count.check(dt)) {
            for (int i = 0; i < DARKNESS_LAYERS; i++) {
                moveVec[i] = Vector(Angle(Random::getRandDouble(M_PI * 2)), Random::getRandDouble(2));
            }
            count.reset();
        }
        for (int i = 0; i < DARKNESS_LAYERS; i++) {
            std::shared_ptr<Circle> p_circleToAdd = std::shared_ptr<Circle>(new Circle(*p_circle));
            p_circleToAdd->move(moveVec[i]);
            DarknessOverlay::addShape(p_circleToAdd, i);
            p_circle->setRadius(p_circle->getRadius() + 10);
        }
    }
}

void Flame::draw() const {
    static Angle rotationAngle(M_PI_4);
    
    if (p_spriteSheet) {
        for (std::vector<Spark*>::const_iterator it = orangeParticles.begin(); it != orangeParticles.end(); it++) {
            Spark * p_spark = *it;
            p_spriteSheet->setScale((float)(p_spark->width));
            GraphicsManager::drawImageFromSpriteSheet(p_spriteSheet, 0, p_spark->center, rotationAngle, false, false, (char)p_spark->alpha);
        }
        for (std::vector<Spark*>::const_iterator it = yellowParticles.begin(); it != yellowParticles.end(); it++) {
            Spark * p_spark = *it;
            p_spriteSheet->setScale((float)(p_spark->width));
            GraphicsManager::drawImageFromSpriteSheet(p_spriteSheet, 1, p_spark->center, rotationAngle, false, false, (char)p_spark->alpha);
        }
    }
}

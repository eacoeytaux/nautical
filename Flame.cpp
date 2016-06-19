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
#define FADE_SPEED 2

Flame::Flame(nautical::Coordinate pos) :
WorldObject(pos) {
    appendTag(FLAME_TAG);
    p_sheet = GraphicsManager::loadSpriteSheet("spritesheets/fire-particles.png", 2, 1, 10);
}

Flame::~Flame() {
    for (Iterator<Spark*> * iterator = orangeParticles.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
    for (Iterator<Spark*> * iterator = yellowParticles.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
    for (Iterator<Origin*> * iterator = origins.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
}

Flame & Flame::addOrigin(double flameWidth, nautical::Vector offset) {
    Origin * p_origin = new Origin;
    p_origin->origin = getCenter() + offset;
    p_origin->flameWidth = flameWidth;
    origins.insert(p_origin);
    return *this;
}

void Flame::update() {
    for (Iterator<Origin*> * iterator = origins.createIterator(); !iterator->complete(); iterator->next()) {
        Origin * p_origin = iterator->current();
        
        //orange particles
        if (p_origin->orangeCountdown.check()) {
            Spark * p_spark = new Spark;
            p_spark->center = p_origin->origin;
            p_spark->width = p_origin->flameWidth * (Random::getRandDouble(0.3) + 0.7);
            p_spark->alpha = 255;
            orangeParticles.insert(p_spark);
            p_origin->orangeCountdown.reset(5 + Random::getRandInt(5));
        }
        
        //yellow particles
        if (p_origin->yellowCountdown.check()) {
            Spark * p_spark = new Spark;
            p_spark->center = p_origin->origin;
            p_spark->width = p_origin->flameWidth * (Random::getRandDouble(0.2) + 0.3);
            p_spark->alpha = 255;
            yellowParticles.insert(p_spark);
            p_origin->yellowCountdown.reset(7 + Random::getRandInt(5));
        }
    }
    
    //orange particles
    LinkedList<Spark*> sparksToDelete;
    for (Iterator<Spark*> * iterator = orangeParticles.createIterator(); !iterator->complete(); iterator->next()) {
        Spark * p_spark = iterator->current();
        
        p_spark->center += (Vector(Angle(Random::getRandDouble(M_PI_2) + M_PI_4), Random::getRandDouble(0.5) + 0.5));
        p_spark->width *= 0.97;
        p_spark->alpha -= FADE_SPEED;
        
        if ((p_spark->width < (1 / GraphicsManager::getZoom())) || (p_spark->alpha <= 0))
            sparksToDelete.insert(p_spark);
    }
    for (Iterator<Spark*> * iterator = sparksToDelete.createIterator(); !iterator->complete(); iterator->next()) {
        Spark * p_spark = iterator->current();
        orangeParticles.remove(p_spark);
        delete p_spark;
    }
    
    //yellow particles
    sparksToDelete.clear();
    for (Iterator<Spark*> * iterator = yellowParticles.createIterator(); !iterator->complete(); iterator->next()) {
        Spark * p_spark = iterator->current();
        
        p_spark->center += (Vector(Angle(Random::getRandDouble(M_PI_2) + M_PI_4), Random::getRandDouble(0.5) + 0.5));
        p_spark->width *= 0.97;
        p_spark->alpha -= FADE_SPEED;
        
        if ((p_spark->width < (1 / GraphicsManager::getZoom())) || (p_spark->alpha <= 0))
            sparksToDelete.insert(p_spark);
    }
    for (Iterator<Spark*> * iterator = sparksToDelete.createIterator(); !iterator->complete(); iterator->next()) {
        Spark * p_spark = iterator->current();
        yellowParticles.remove(p_spark);
        delete p_spark;
    }
    
    if (DarknessOverlay::isInEffect()) {
        Circle * p_circle = new Circle(getCenter(), 100);
        static Countdown count(1);
        static Vector moveVec[DARKNESS_LAYERS];
        if (count.check()) {
            for (int i = 0; i < DARKNESS_LAYERS; i++) {
                moveVec[i] = Vector(Angle(Random::getRandDouble(M_PI * 2)), Random::getRandDouble(2));
            }
            count.reset();
        }
        for (int i = 0; i < DARKNESS_LAYERS; i++) {
            Circle * p_circleToAdd = new Circle(*p_circle);
            p_circleToAdd->move(moveVec[i]);
            DarknessOverlay::addShape(p_circleToAdd, i);
            p_circle->setRadius(p_circle->getRadius() + 10);
        }
    }
}

void Flame::draw() const {
    static Angle rotationAngle(M_PI_4);
    
    if (p_sheet) {
        for (Iterator<Spark*> * iterator = orangeParticles.createIterator(); !iterator->complete(); iterator->next()) {
            Spark * p_spark = iterator->current();
            p_sheet->setScale((float)(p_spark->width));
            GraphicsManager::drawImageFromSpriteSheet(p_sheet, 0, p_spark->center, rotationAngle, false, false, p_spark->alpha);
        }
        for (Iterator<Spark*> * iterator = yellowParticles.createIterator(); !iterator->complete(); iterator->next()) {
            Spark * p_spark = iterator->current();
            p_sheet->setScale((float)(p_spark->width));
            GraphicsManager::drawImageFromSpriteSheet(p_sheet, 1, p_spark->center, rotationAngle, false, false, p_spark->alpha);
        }
    }
}

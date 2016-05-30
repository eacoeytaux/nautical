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

#define FADE_SPEED 2

Flame::Flame(nautical::Coordinate pos) :
WorldObject(pos) {
    appendTag(FLAME_TAG);
}

Flame::~Flame() {
    for (Iterator<Shape*> * iterator = orangeParticles.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
    for (Iterator<Shape*> * iterator = yellowParticles.createIterator(); !iterator->complete(); iterator->next()) {
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
        updateOrigin(iterator->current());
    }
    
    LinkedList<Rectangle*> particlesToDelete;
    for (Iterator<Shape*> * iterator = orangeParticles.createIterator(); !iterator->complete(); iterator->next()) {
        Rectangle * p_particle = static_cast<Rectangle*>(iterator->current());
        p_particle->move(Vector(Angle(Random::getRandFloat(M_PI_2) + M_PI_4), Random::getRandFloat(0.5) + 0.5)).setWidth(p_particle->getWidth() * 0.97).setHeight(p_particle->getHeight() * 0.97);
        Color particleColor = p_particle->getColor();
        p_particle->setColor(particleColor.setA(particleColor.getA() - FADE_SPEED));
        if ((p_particle->getWidth() < (1 / GraphicsManager::getZoom())) || (particleColor.getA() <= FADE_SPEED))
            particlesToDelete.insert(p_particle);
    }
    for (Iterator<Rectangle*> * iterator = particlesToDelete.createIterator(); !iterator->complete(); iterator->next()) {
        Rectangle * p_particle = iterator->current();
        orangeParticles.remove(p_particle);
        delete p_particle;
    }
    
    particlesToDelete.clear();
    for (Iterator<Shape*> * iterator = yellowParticles.createIterator(); !iterator->complete(); iterator->next()) {
        Rectangle * p_particle = static_cast<Rectangle*>(iterator->current());
        p_particle->move(Vector(Angle(Random::getRandFloat(M_PI_2) + M_PI_4), Random::getRandFloat(0.5) + 0.5)).setWidth(p_particle->getWidth() * 0.97).setHeight(p_particle->getHeight() * 0.97);
        Color particleColor = p_particle->getColor();
        p_particle->setColor(particleColor.setA(particleColor.getA() - FADE_SPEED));
        if ((p_particle->getWidth() < (1 / GraphicsManager::getZoom())) || (particleColor.getA() <= FADE_SPEED))
            particlesToDelete.insert(p_particle);
    }
    for (Iterator<Rectangle*> * iterator = particlesToDelete.createIterator(); !iterator->complete(); iterator->next()) {
        Rectangle * p_particle = iterator->current();
        yellowParticles.remove(p_particle);
        delete p_particle;
    }
    
    Circle * p_circle = new Circle(getCenter(), 100);
    static Countdown count(1);
    static Vector moveVec[DARKNESS_LAYERS];
    if (count.check()) {
        for (int i = 0; i < DARKNESS_LAYERS; i++) {
            moveVec[i] = Vector(Angle(Random::getRandFloat(M_PI * 2)), Random::getRandFloat(2));
        }
        count.reset();
    }
    for (int i = 0; i < DARKNESS_LAYERS; i++) {
        Circle * p_circleToAdd = new Circle(*p_circle);
        p_circleToAdd->move(moveVec[i]);
        getParent()->addShapeToDarknessOverlay(p_circleToAdd, i);
        p_circle->setRadius(p_circle->getRadius() + 10);
    }
}

void Flame::updateOrigin(Origin * p_origin) {
    static Color fireColorOrange(255, 127, 0);
    static Color fireColorYellow(255, 255, 0);
    
    //orange particles
    if (p_origin->orangeCountdown.check()) {
        double flameWidth = p_origin->flameWidth * (Random::getRandFloat(0.3) + 0.7);
        Rectangle * p_particle = new Rectangle(p_origin->origin, flameWidth, flameWidth, M_PI_4);
        p_particle->setColor(fireColorOrange);
        orangeParticles.insert(p_particle);
        p_origin->orangeCountdown.reset(5 + Random::getRandInt(5));
    }
    
    //yellow particles
    if (p_origin->yellowCountdown.check()) {
        double flameWidth = p_origin->flameWidth * (Random::getRandFloat(0.15) + 0.3);
        Rectangle * p_particle = new Rectangle(p_origin->origin, flameWidth, flameWidth, M_PI_4);
        p_particle->setColor(fireColorYellow);
        yellowParticles.insert(p_particle);
        p_origin->yellowCountdown.reset(7 + Random::getRandInt(5));
    }
}

void Flame::draw() const {
    for (Iterator<Shape*> * iterator = orangeParticles.createIterator(); !iterator->complete(); iterator->next()) {
        iterator->current()->drawFilled();
    }
    for (Iterator<Shape*> * iterator = yellowParticles.createIterator(); !iterator->complete(); iterator->next()) {
        iterator->current()->drawFilled();
    }
}

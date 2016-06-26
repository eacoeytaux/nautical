//
//  Slimeball.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 6/25/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Slimeball.hpp"

#include "MapHitboxRectangle.hpp"

using namespace nautical;
using namespace climber;

Slimeball::Slimeball(Coordinate pos) :
Mob(pos) {
    appendTag(SLIMEBALL_TAG);
    
    MapHitboxRectangle * p_hitbox = new MapHitboxRectangle(Rectangle(pos, 48, 48));
    setMapHitbox(p_hitbox);
    delete p_hitbox;
    
    p_spriteSheet = GraphicsManager::loadSpriteSheet("spritesheets/slimeball.png", 1, 1, 1);
}

Slimeball::~Slimeball() {
    delete p_spriteSheet;
}

void Slimeball::update() {
    const MapElement * p_element = getMapHitbox()->getElement();
    if (!p_element || (p_element && !(p_element->isSticky())))
        addToForce(Vector(0, -0.3));
    
    addToVel(getForce());
    
    if (p_element) {
        setVel(getVel() * p_element->getFrictionCoefficient());
    } else {
        setVel(getVel() * getParent()->getMap()->getAirResistanceCoefficient());
    }
    
    World * p_parent = getParent();
    float percentageUsed = 1.f;
    do {
        Vector vel = getVel();
        Vector movement = p_parent->generatePath(&percentageUsed, &vel, getMapHitbox().get(), &p_element);
        setMapElement(p_element);
        move(movement);
        setVel(vel);
    } while (percentageUsed > 0);
    setForce(Vector(0, 0));
}

void Slimeball::draw() const {
    //getMapHitbox()->getShape()->draw();
    
    if (p_spriteSheet)
        GraphicsManager::drawImageFromSpriteSheet(p_spriteSheet, 0, getCenter(), rot);
}

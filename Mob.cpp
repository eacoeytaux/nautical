//
//  Mob.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/8/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Mob.hpp"

using namespace nautical;

Mob::Mob(Coordinate pos, int maxHealth) :
WorldObject(pos),
maxHealth(maxHealth),
health(maxHealth) {
    appendTag(MOB_TAG);
}

Mob::~Mob() { }

int Mob::getHealth() const {
    return health;
}

bool Mob::isAlive() const {
    return alive;
}

void Mob::kill(bool passively) {
    health = 0;
    alive = false;
}

int Mob::getMaxHealth() const {
    return maxHealth;
}

void Mob::setMaxHealth(int health) {
    maxHealth = health;
}

void Mob::raiseMaxHealth(int health) {
    setMaxHealth(health);
}

void Mob::setHealth(int health) {
    if (health <= 0)
        kill(false);
    else if (health > maxHealth)
        this->health = maxHealth;
    else
        this->health = health;
}

void Mob::damage(int health) {
    setHealth(this->health -= health);
}

void Mob::heal(int health) {
    setHealth(this->health += health);
}

void Mob::healFull() {
    setHealth(maxHealth);
}

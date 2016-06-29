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

Mob & Mob::kill(bool passively) {
    health = 0;
    alive = false;
    return *this;
}

int Mob::getMaxHealth() const {
    return maxHealth;
}

Mob & Mob::setMaxHealth(int health) {
    maxHealth = health;
    return *this;
}

Mob & Mob::raiseMaxHealth(int health) {
    return setMaxHealth(maxHealth + health);
}

Mob & Mob::setHealth(int health) {
    if (health <= 0)
        kill(false);
    else if (health > maxHealth)
        this->health = maxHealth;
    else
        this->health = health;
    return *this;
}

Mob & Mob::damage(int health) {
    return setHealth(this->health -= health);
}

Mob & Mob::heal(int health) {
    return setHealth(this->health += health);
}

Mob & Mob::healFull() {
    return setHealth(maxHealth);
}

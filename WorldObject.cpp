//
//  WorldObject.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/1/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "WorldObject.hpp"

using namespace nautical;

WorldObject::WorldObject(Coordinate pos) :
center(pos) {
    appendTag(WORLD_OBJECT_TAG);
    force.setOrigin(center);
    vel.setOrigin(center);
}

WorldObject::WorldObject(const WorldObject & other) :
p_parent(other.p_parent),
spectral(other.spectral),
priority(other.priority),
altitude(other.altitude),
center(other.center),
p_hitbox(other.p_hitbox->deepCopy()),
force(other.force),
vel(other.vel),
attachedObjects(other.attachedObjects),
subscribedEventTags(other.subscribedEventTags) { }

WorldObject::~WorldObject() { }

World * WorldObject::getParent() const {
    return p_parent;
}

void WorldObject::setParent(World * p_parent) {
    this->p_parent = p_parent;
}

Coordinate WorldObject::getCenter() const {
    return center;
}

std::shared_ptr<MapHitbox> WorldObject::getMapHitbox() const {
    if (p_hitbox)
        return std::shared_ptr<MapHitbox>(p_hitbox->deepCopy());
    else
        return std::shared_ptr<MapHitbox>(nullptr);
}

void WorldObject::setMapHitbox(MapHitbox * p_hitbox) {
    this->p_hitbox.reset(p_hitbox->deepCopy());
}

const MapElement * WorldObject::getMapElement() const {
    return p_hitbox->getElement();
}

void WorldObject::setMapElement(const MapElement * p_element) {
    p_hitbox->setElement(p_element);
}

Vector WorldObject::getForce() const {
    return force;
}

void WorldObject::setForce(Vector force) {
    this->force = force;
}

void WorldObject::addToForce(Vector force) {
    this->force += force;
    this->force.setOrigin(center);
}

Vector WorldObject::getVel() const {
    return vel;
}

void WorldObject::setVel(Vector vel) {
    this->vel = vel;
    this->vel.setOrigin(center);
}

void WorldObject::addToVel(Vector vel) {
    this->vel += vel;
}

void WorldObject::moveTo(Coordinate coor) {
    move(Vector(center, coor));
}

void WorldObject::move(Vector vec) {
    p_hitbox->move(vec);
    center += vec;
    force.setOrigin(center);
    vel.setOrigin(center);
    for (std::vector<WorldObject*>::iterator it = attachedObjects.begin(); it != attachedObjects.end(); it++) {
        (*it)->move(vec);
    }
}

const std::vector<WorldObject*> * WorldObject::getAttachedObjects() const {
    return &attachedObjects;
}

void WorldObject::attachObject(WorldObject * p_object) {
    attachedObjects.push_back(p_object);
}

void WorldObject::removeAttachedObject(WorldObject * p_object) {
    vector_helpers::removeElementByValue(attachedObjects, p_object);
}

const std::vector<std::string> * WorldObject::getSubscribedEventTags() const {
    return &subscribedEventTags;
}

void WorldObject::subscribeEvent(std::string eventTag) {
    subscribedEventTags.push_back(eventTag);
}

void WorldObject::unsubscribeEvent(std::string eventTag) {
    if (p_parent)
        p_parent->unsubscribeObject(eventTag, this);
    vector_helpers::removeElementByValue(subscribedEventTags, eventTag);
}

bool WorldObject::handleEvent(Event * p_event) {
    return true;
}

bool WorldObject::isSpectral() const {
    return spectral;
}

void WorldObject::setSpectral(bool spectral) {
    this->spectral = spectral;
}

int WorldObject::getPriority() const {
    return priority;
}

void WorldObject::setPriority(int priority) {
    if (priority < 0) {
        Logger::writeLog(WARNING_MESSAGE, "WorldObject::setPriority(): attempted to set priority to below 0");
        priority = 0;
    } else if (priority > MAX_PRIORITY) {
        Logger::writeLog(WARNING_MESSAGE, "WorldObject::setPriority(): attempted to set priority above max priority");
        priority = MAX_PRIORITY;
    }
    this->priority = priority;
}

int WorldObject::getAltitude() const {
    return altitude;
}

void WorldObject::setAltitude(int altitude) {
    if (altitude < -MAX_BELOW_ALTITUDE) {
        Logger::writeLog(WARNING_MESSAGE, "WorldObject::setAltitude(): attempted to set priority below min altitude");
        altitude = -MAX_BELOW_ALTITUDE;
    } else if (altitude > MAX_ABOVE_ALTITUDE) {
        Logger::writeLog(WARNING_MESSAGE, "WorldObject::setAltitude(): attempted to set priority above max altitude");
        altitude = MAX_PRIORITY;
    }
    this->altitude = altitude;
}

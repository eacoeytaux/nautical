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

WorldObject::~WorldObject() { }

World * WorldObject::getParent() const {
    return p_parent;
}

WorldObject & WorldObject::setParent(World * p_parent) {
    this->p_parent = p_parent;
    return *this;
}

Coordinate WorldObject::getCenter() const {
    return center;
}

double WorldObject::getMapWidth() const {
    return mapWidth;
}

WorldObject & WorldObject::setMapWidth(double width) {
    mapWidth = width;
    return *this;
}

double WorldObject::getMapHeight() const {
    return mapHeight;
}

WorldObject & WorldObject::setMapHeight(double height) {
    mapHeight = height;
    return *this;
}

Angle WorldObject::getMapAngle() const {
    return mapAngle;
}

WorldObject & WorldObject::setMapAngle(Angle angle) {
    mapAngle = angle;
    return *this;
}

Rectangle WorldObject::getHitbox() const {
    return Rectangle(center, mapWidth, mapHeight, mapAngle);
}

MapElement * WorldObject::getMapElement() const {
    return p_element;
}

WorldObject & WorldObject::setMapElement(MapElement * p_element) {
    this->p_element = p_element;
    return *this;
}

MapElement::ObjectPos WorldObject::getObjectPos() const {
    return objPos;
}

WorldObject & WorldObject::setObjectPos(MapElement::ObjectPos objPos) {
    this->objPos = objPos;
    return *this;
}

Vector WorldObject::getForce() const {
    return force;
}

WorldObject & WorldObject::setForce(Vector force) {
    this->force = force;
    return *this;
}

WorldObject & WorldObject::addToForce(Vector force) {
    this->force += force;
    return *this;
}

Vector WorldObject::getVel() const {
    return vel;
}

WorldObject & WorldObject::setVel(Vector vel) {
    this->vel = vel;
    return *this;
}

WorldObject & WorldObject::addToVel(Vector vel) {
    this->vel += vel;
    return *this;
}


WorldObject & WorldObject::moveTo(Coordinate coor) {
    return move(Vector(center, coor));
}

WorldObject & WorldObject::move(Vector vec) {
    center += vec;
    force.setOrigin(center);
    vel.setOrigin(center);
    for (Iterator<WorldObject*> * iterator = attachedObjects.createIterator(); !iterator->complete(); iterator->next()) {
        iterator->current()->move(vec);
    }
    return *this;
}

Iterator<WorldObject*> * WorldObject::getAttachedObjectsIterator() const {
    return attachedObjects.createIterator();
}

WorldObject & WorldObject::attachObject(WorldObject * p_object) {
    attachedObjects.insert(p_object);
    return *this;
}

WorldObject & WorldObject::removeAttachedObject(WorldObject * p_object) {
    attachedObjects.remove(p_object);
    return *this;
}

WorldObject & WorldObject::subscribeEvent(std::string eventTag) {
    subscribedEventTags.insert(eventTag);
    return *this;
}

WorldObject & WorldObject::unsubscribeEvent(std::string eventTag) {
    if (p_parent)
        p_parent->unsubscribeObject(eventTag, this);
    subscribedEventTags.remove(eventTag);
    return *this;
}

void WorldObject::handleEvent(Event * p_event) { }

Iterator<std::string> * WorldObject::getSubscribedEventTagsIterator() const {
    return subscribedEventTags.createIterator();
}

bool WorldObject::isSpectral() const {
    return spectral;
}

WorldObject & WorldObject::setSpectral(bool spectral) {
    this->spectral = spectral;
    return *this;
}

int WorldObject::getPriority() const {
    return priority;
}

WorldObject & WorldObject::setPriority(int priority) {
    if (priority < 0) {
        Logger::writeLog(WARNING, "WorldObject::setPriority(): attempted to set priority to below 0");
        priority = 0;
    } else if (priority > MAX_PRIORITY) {
        Logger::writeLog(WARNING, "WorldObject::setPriority(): attempted to set priority above max priority");
        priority = MAX_PRIORITY;
    }
    this->priority = priority;
    return *this;
}

int WorldObject::getAltitude() const {
    return altitude;
}

WorldObject & WorldObject::setAltitude(int altitude) {
    if (altitude < -MAX_BELOW_ALTITUDE) {
        Logger::writeLog(WARNING, "WorldObject::setAltitude(): attempted to set priority below min altitude");
        altitude = -MAX_BELOW_ALTITUDE;
    } else if (altitude > MAX_ABOVE_ALTITUDE) {
        Logger::writeLog(WARNING, "WorldObject::setAltitude(): attempted to set priority above max altitude");
        altitude = MAX_PRIORITY;
    }
    this->altitude = altitude;
    return *this;
}

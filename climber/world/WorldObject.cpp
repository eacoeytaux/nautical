//
//  WorldObject.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/1/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "WorldObject.hpp"

using namespace nautical;

WorldObject::WorldObject(double m, Coordinate pos) :
Mass(m, pos) {
    appendTag(WORLD_OBJECT_TAG);
}

WorldObject::~WorldObject() { }

WorldObject & WorldObject::movePosition(const Vector & dPos) {
    p_hitbox->move(dPos);
    Mass::setPosition(p_hitbox->getCenter());
    return *this;
}

std::shared_ptr<MapHitbox> WorldObject::getMapHitbox() const {
    return p_hitbox;
}

WorldObject & WorldObject::setMapHitbox(std::shared_ptr<MapHitbox> p_hitbox) {
    this->p_hitbox = p_hitbox;
    return *this;
}

std::shared_ptr<const MapElement> WorldObject::getMapElement() const {
    return p_hitbox->getElement();
}

WorldObject & WorldObject::setMapElement(std::shared_ptr<const MapElement> p_element) {
    p_hitbox->setElement(p_element);
    return *this;
}

const std::vector<std::shared_ptr<WorldObject>> & WorldObject::getAttachedObjects() const {
    return attachedObjects;
}

WorldObject & WorldObject::attachObject(std::shared_ptr<WorldObject> p_object) {
    attachedObjects.push_back(p_object);
    return *this;
}

WorldObject & WorldObject::removeAttachedObject(std::shared_ptr<WorldObject> p_object) {
    vector_helpers::removeElementByValue(attachedObjects, p_object);
    return *this;
}

const std::vector<std::string> & WorldObject::getSubscribedEventTags() const {
    return subscribedEventTags;
}

WorldObject & WorldObject::subscribeEvent(std::string eventTag) {
    subscribedEventTags.push_back(eventTag);
    return *this;
}

WorldObject & WorldObject::unsubscribeEvent(std::string eventTag) {
    //TODO TODO TODO
//    if (p_parent)
//        p_parent->unsubscribeObject(eventTag, std::shared_ptr<WorldObject>(this));
    vector_helpers::removeElementByValue(subscribedEventTags, eventTag);
    return *this;
}

bool WorldObject::handleEvent(std::shared_ptr<Event> p_event) {
    return true;
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
        Logger::writeLog(MESSAGE, "WorldObject::setPriority(): attempted to set priority to below 0");
        priority = 0;
    } else if (priority > MAX_PRIORITY) {
        Logger::writeLog(MESSAGE, "WorldObject::setPriority(): attempted to set priority above max priority");
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
        Logger::writeLog(MESSAGE, "WorldObject::setAltitude(): attempted to set priority below min altitude");
        altitude = -MAX_BELOW_ALTITUDE;
    } else if (altitude > MAX_ABOVE_ALTITUDE) {
        Logger::writeLog(MESSAGE, "WorldObject::setAltitude(): attempted to set priority above max altitude");
        altitude = MAX_PRIORITY;
    }
    this->altitude = altitude;
    return *this;
}

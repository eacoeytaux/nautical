//
//  Level.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "World.hpp"

#include "Logger.hpp"
#include "MaxMinValue.hpp"
#include "Path.hpp"
#include "MapHitbox.hpp"
#include "WorldObject.hpp"

//DEBUGGING
#include "KeyboardEvent.hpp"
#include "Player.hpp" //TODO delete

using namespace nautical;

bool World::DRAW_BUMPERS = false;

//functions used for weighing WorldObjects in SortedLists
double weighWorldObjectByX(WorldObject * const * p_object) {
    return (*p_object)->getPosition().getX();
}

double weighWorldObjectByY(WorldObject * const * p_object) {
    return (*p_object)->getPosition().getY();
}

World::World(bool verticalWorld) {
    static int id = 0;
    this->id = id++;
    
    /*allObjects = SortedList<WorldObject*>(verticalWorld ? &weighWorldObjectByY : &weighWorldObjectByX);
     objectsToDelete = SortedList<WorldObject*>(verticalWorld ? &weighWorldObjectByY : &weighWorldObjectByX);
     for (int i = 0; i < MAX_PRIORITY + 1; i++) {
     objectsToUpdate[i] = SortedList<WorldObject*>(verticalWorld ? &weighWorldObjectByY : &weighWorldObjectByX);
     }
     for (int i = 0; i < MAX_BELOW_ALTITUDE + MAX_ABOVE_ALTITUDE + 1; i++) {
     objectsToDraw[i] = SortedList<WorldObject*>(verticalWorld ? &weighWorldObjectByY : &weighWorldObjectByX);
     }*/
    
    //generate map //TODO move this somewhere else
    map = Map(verticalWorld);
    std::shared_ptr<MapVertex> v0 = map.createVertex(Coordinate(100, 640));
    std::shared_ptr<MapVertex> v1 = map.createVertex(Coordinate(10, 340));
    std::shared_ptr<MapVertex> v2 = map.createVertex(Coordinate(175, 220));
    //std::shared_ptr<MapVertex> v3 = map.createVertex(Coordinate(250, 220));
    std::shared_ptr<MapVertex> v4 = map.createVertex(Coordinate(400, 290));
    std::shared_ptr<MapVertex> v5 = map.createVertex(Coordinate(450, 265));
    std::shared_ptr<MapVertex> v6 = map.createVertex(Coordinate(500, 290));
    std::shared_ptr<MapVertex> v7 = map.createVertex(Coordinate(800, 240));
    std::shared_ptr<MapVertex> v8 = map.createVertex(Coordinate(750, 640));
    
    map.createEdge(v0, v1, false);
    map.createEdge(v1, v2, true);
    //createEdge(v2, v3, true);
    map.createEdge(v2, v4, true);
    map.createEdge(v4, v5, true);
    map.createEdge(v5, v6, true);
    map.createEdge(v6, v7, true);
    map.createEdge(v7, v8, false);
}

World::~World() { }

int World::getID() const {
    return id;
}

int World::getUpdateTimestamp() const {
    return updateTimestamp;
}

int World::getDrawTimestamp() const {
    return drawTimestamp;
}

bool World::isVertical() const {
    return isVertical();
}

const Map & World::getMap() const {
    return map;
}

double World::getTimeRatio() const {
    return timeRatio;
}

World & World::setTimeRatio(double timeRatio) {
    this->timeRatio = timeRatio;
    return *this;
}

World & World::addObject(std::shared_ptr<WorldObject> p_object, bool shouldUpdate, bool shouldDraw) {
    if (p_object) {
        //p_object->setParent(std::shared_ptr<World>(this));
        
        allObjects.push_back(p_object);
        if (shouldUpdate)
            objectsToUpdate[p_object->getPriority()].push_back(p_object);
        if (shouldDraw)
            objectsToDraw[p_object->getAltitude() + MAX_BELOW_ALTITUDE].push_back(p_object);
        
        for (std::vector<std::string>::const_iterator it = p_object->getSubscribedEventTags().begin(); it != p_object->getSubscribedEventTags().end(); it++) {
            subscribeObject(*it, p_object);
        }
        
        Logger::writeLog(INFO, "World::addObject(): added object[%d] to world[%d]", p_object->getID(), id);
    } else {
        Logger::writeLog(MESSAGE, "World::addObject(): attempted to add nullptr");
    }
    
    return *this;
}

World & World::markObjectForRemoval(std::shared_ptr<WorldObject> p_object) {
    objectsToDelete.push_back(p_object);
    return *this;
}

//TODO this should be done by saving indexes
World & World::removeObject(std::shared_ptr<WorldObject> p_object) {
    if (p_object) {
        if (vector_helpers::containsElement(allObjects, p_object)) {
            vector_helpers::removeElementByValue(allObjects, p_object);
            if (vector_helpers::containsElement(objectsToUpdate[p_object->getPriority()], p_object))
                vector_helpers::removeElementByValue(objectsToUpdate[p_object->getPriority()], p_object);
            if (vector_helpers::containsElement(objectsToDraw[p_object->getAltitude() + MAX_BELOW_ALTITUDE], p_object))
                vector_helpers::removeElementByValue(objectsToDraw[p_object->getAltitude() + MAX_BELOW_ALTITUDE], p_object);
            
            for (std::vector<std::string>::const_iterator it = p_object->getSubscribedEventTags().begin(); it != p_object->getSubscribedEventTags().end(); it++) {
                unsubscribeObject(*it, p_object);
            }
        } else {
            Logger::writeLog(MESSAGE,"World::removeObject(): attempted to remove object not in world's list of all objects");
        }
    } else {
        Logger::writeLog(MESSAGE, "World::removeObject(): attempted to remove nullptr");
    }
    
    return *this;
}

World & World::subscribeObject(std::string eventTag, std::shared_ptr<WorldObject> p_object) {
    for (std::vector<EventPairing>::iterator it = subscribedObjects.begin(); it != subscribedObjects.end(); it++) {
        if (it->eventTag == eventTag) {
            it->subscribedObjects.push_back(p_object);
            return *this;
        }
    }
    
    //event has not yet been registered
    EventPairing newPair;
    newPair.eventTag = eventTag;
    newPair.subscribedObjects.push_back(p_object);
    subscribedObjects.push_back(newPair);
    return *this;
}

World & World::unsubscribeObject(std::string eventTag, std::shared_ptr<WorldObject> p_object) {
    for (std::vector<EventPairing>::iterator it = subscribedObjects.begin(); it != subscribedObjects.end(); it++) {
        if (it->eventTag == eventTag) {
            vector_helpers::removeElementByValue(it->subscribedObjects, p_object);
            if (it->subscribedObjects.size() == 0) //if no objects are subscribed to event, remove event
                vector_helpers::removeElementByValue(subscribedObjects, *it);
            return *this;
        }
    }
    Logger::writeLog(MESSAGE, "World::unregisterObjects(): attempted to unregister objects from non-existant event");
    return *this;
}

World & World::handleEvent(std::shared_ptr<Event> p_event) {
    for (std::vector<EventPairing>::iterator it = subscribedObjects.begin(); it != subscribedObjects.end(); it++) {
        EventPairing pair = *it;
        if (p_event->hasTag(pair.eventTag)) {
            for (std::vector<std::shared_ptr<WorldObject>>::iterator subIt = pair.subscribedObjects.begin(); subIt != pair.subscribedObjects.end(); subIt++) {
                (*subIt)->handleEvent(p_event);
            }
        }
    }
    return *this;
}

/*void World::generatePath(WorldObject * p_object) {
 Path path(p_object->getCenter());
 
 Coordinate center = p_object->getCenter();
 Rectangle hitbox = p_object->getHitbox();
 Vector vel = p_object->getVel();
 vel.setOrigin(center);
 MapElement * p_element = p_object->getMapElement();
 MapElement * p_prevElement = nullptr;
 MapElement::ObjectPos objPos = p_object->getObjectPos();
 
 float percentage = 1.f;
 
 while (percentage > 0) {
 LinkedList<MapElement*> elementsNotToCheck;
 elementsNotToCheck.insert(p_prevElement);
 elementsNotToCheck.insert(p_element);
 
 hitbox.setCenter(center);
 
 bool velAdjusted = false;
 if (p_element) {
 if (!p_element->adjustVectorRectangle(hitbox, &vel, objPos)) {
 p_element = nullptr;
 } else {
 velAdjusted = true;
 }
 }
 
 Vector adjustedVel = vel * percentage;
 
 if (adjustedVel.getMagnitude() == 0) {
 path.addVector(adjustedVel);
 break;
 }
 
 Coordinate nextCenter = center;
 Vector nextVel = adjustedVel;
 MapElement * p_nextElement = p_element;
 MapElement::ObjectPos nextObjPos = objPos;
 
 int lowerBound, upperBound;
 lowerBound = center.getX() - (p_object->getMapWidth() / 2);
 upperBound = center.getX() + (p_object->getMapWidth() / 2);
 (adjustedVel.isDxPositive() ? upperBound : lowerBound) += adjustedVel.getDx();
 
 MinValue distance;
 
 if (p_element) { //if touching element, check only element catches
 LinkedList<MapCatch> catches = p_element->findCatches(hitbox, &map);
 for (Iterator<MapCatch> * iterator = catches.createIterator(); !iterator->complete(); iterator->next()) {
 MapCatch mapCatch = iterator->current();
 
 Line adjustedVelLine = Line(center, center + adjustedVel);
 
 if (mapCatch.getLine().intersects(adjustedVelLine)) {
 Coordinate collision = mapCatch.getCollision();
 Vector collisionVel(center, collision);
 if (distance.update(collisionVel.getMagnitude())) {
 nextCenter = collision;
 nextVel = collisionVel;
 p_nextElement = mapCatch.getElement(p_element);
 }
 }
 }
 } else { //free fall, check all elements
 //check all vertices for collision
 for (Iterator<MapVertex*> * iterator = map.getVerticesListIterator(lowerBound, upperBound); !iterator->complete(); iterator->next()) {
 MapVertex * p_vertex = iterator->current();
 
 if (elementsNotToCheck.contains(p_vertex))
 continue;
 
 Queue<Coordinate> collisions;
 Line adjustedVelLine = Line(center, center + adjustedVel);
 Rectangle * p_rec = p_vertex->generateBumperRectangle(hitbox);
 if (p_rec->intersectsLine(adjustedVelLine, &collisions)) {
 Coordinate collision;
 if (collisions.pop(&collision)) {
 Vector collisionVel(center, collision);
 if (distance.update(collisionVel.getMagnitude())) {
 nextCenter = collision;
 nextVel = collisionVel;
 p_nextElement = p_vertex;
 }
 } else {
 Logger::writeLog(ERROR, "World::generatePath(): collisions is empty");
 }
 }
 delete p_rec;
 }
 
 //check all edges for collision
 for (Iterator<MapEdge*> * iterator = map.getEdgesListIterator(lowerBound, upperBound); !iterator->complete(); iterator->next()) {
 MapEdge * p_edge = iterator->current();
 
 if (elementsNotToCheck.contains(p_edge))
 continue;
 
 Queue<Coordinate> collisions;
 Line adjustedVelLine(center, center + adjustedVel);
 LineShape * p_lineShape = p_edge->generateBumperRectangle(hitbox);
 if (p_lineShape->intersectsLine(adjustedVelLine, &collisions)) {
 Coordinate collision;
 if (collisions.pop(&collision)) {
 Vector collisionVel(center, collision);
 if (distance.update(collisionVel.getMagnitude())) {
 nextCenter = collision;
 nextVel = collisionVel;
 p_nextElement = p_edge;
 }
 } else {
 Logger::writeLog(ERROR, "World::generatePath(): collisions is empty");
 }
 }
 delete p_lineShape;
 }
 }
 
 //finished checking collisions
 
 if (distance.getValue() < INFINITY) {
 float percentageUsed = distance.getValue() / adjustedVel.getMagnitude();
 
 float prevPercentage = percentage;
 percentage *= (1.f - percentageUsed);
 if (percentage == prevPercentage) {
 Logger::writeLog(ERROR, "World::generatePath(): percentage did not progress");
 percentage = 0; //TODO this is still a problem
 }
 } else {
 percentage = 0;
 }
 
 center = nextCenter;
 if (p_element != p_nextElement) {
 p_prevElement = p_element;
 p_element = p_nextElement;
 }
 objPos = nextObjPos;
 
 path.addVector(nextVel);
 }
 
 p_object->setVel(vel);
 p_object->moveTo(path.getEndPoint());
 p_object->setMapElement(p_element);
 p_object->setObjectPos(objPos);
 }*/

Vector World::generatePath(float & percentage, Vector & vel, std::shared_ptr<MapHitbox> p_hitbox, std::shared_ptr<const MapElement> & p_nextElement) {
    //Vector vel = *p_vel;
    //if (vel.getMagnitude() < 0)
    //    return 0;
    
    std::shared_ptr<Shape> p_shape = p_hitbox->getShape();
    Coordinate center = p_hitbox->getCenter();
    std::shared_ptr<const MapElement> p_element = p_hitbox->getElement();
    std::shared_ptr<const MapElement> p_prevElement = p_element;
    
    vel *= percentage;
    
    if (p_element && !p_hitbox->adjustVector(vel))
        p_element = nullptr;
    
    p_nextElement = p_element;
    
    Coordinate nextCenter = center;
    Vector nextVel = vel;
    
    int lowerBound, upperBound;
    lowerBound = (int)(verticalWorld ? p_shape->getLowerBoundY() : p_shape->getLowerBoundX());
    upperBound = (int)(verticalWorld ? p_shape->getUpperBoundY() : p_shape->getUpperBoundX());
    if (verticalWorld)
        (vel.isDyPositive() ? upperBound : lowerBound) += vel.getDy();
    else
        (vel.isDxPositive() ? upperBound : lowerBound) += vel.getDx();
    
    MinValue distance;
    
    if (p_element) { //if touching element, check only element catches
        std::vector<MapCatch> catches = p_hitbox->findCatches(map.getVerticesList(), map.getEdgesList()); //TODO this should be &map
        for (std::vector<MapCatch>::iterator it = catches.begin(); it != catches.end(); it++) {
            MapCatch mapCatch = *it;
            
            Line adjustedVelLine = Line(center, center + vel);
            
            if (mapCatch.getLine().intersectsLine(adjustedVelLine)) {
                Coordinate collision = mapCatch.getCollision();
                Vector collisionVel(center, collision);
                if ((collisionVel.getMagnitude() > 0) && distance.update(collisionVel.getMagnitude())) {
                    nextCenter = collision;
                    nextVel = collisionVel;
                    
                    Vector tempVector = vel;
                    tempVector.setOrigin(collision);
                    p_nextElement = (p_hitbox->adjustVectorAmbigious(mapCatch.getElement(p_element), tempVector)) ? mapCatch.getElement(p_element) : nullptr;
                }
            }
        }
    } else { //free fall, check all elements
        //check all vertices for collision
        for (std::vector<std::shared_ptr<MapVertex>>::const_iterator it = map.getVerticesList().begin(); it != map.getVerticesList().end(); it++) {//TODO = map.getVerticesListIterator(lowerBound, upperBound); !iterator->complete(); iterator->next()) {
            std::shared_ptr<const MapVertex> p_vertex = *it;
            
            if (p_vertex == p_prevElement)
                continue;
            
            std::vector<Coordinate> collisions;
            Line adjustedVelLine = Line(center, center + vel);
            std::shared_ptr<Shape> p_bumper = p_hitbox->createBumper(p_vertex);
            if (p_bumper->intersectsLine(adjustedVelLine, &collisions)) {
                Coordinate collision = collisions.front();
                Vector tempVector = vel;
                tempVector.setOrigin(collision);
                if (p_hitbox->adjustVector(p_vertex, tempVector)) {
                    Vector collisionVel(center, collision);
                    if ((collisionVel.getMagnitude() > 0) && distance.update(collisionVel.getMagnitude())) {
                        nextCenter = collision;
                        nextVel = collisionVel;
                        p_nextElement = p_vertex;
                    }
                }
            }
        }
        
        //check all edges for collision
        for (std::vector<std::shared_ptr<MapEdge>>::const_iterator it = map.getEdgesList().begin(); it != map.getEdgesList().end(); it++) {//TODO = map.getEdgesListIterator(lowerBound, upperBound); !iterator->complete(); iterator->next()) {
            std::shared_ptr<const MapEdge> p_edge = *it;
            
            if (p_edge == p_prevElement)
                continue;
            
            std::vector<Coordinate> collisions;
            Line adjustedVelLine(center, center + vel);
            std::shared_ptr<Shape> p_bumper = p_hitbox->createBumper(p_edge);
            if (p_bumper->intersectsLine(adjustedVelLine, &collisions)) {
                Coordinate collision = collisions.front();
                Vector tempVector = vel;
                tempVector.setOrigin(collision);
                if (p_hitbox->adjustVector(p_edge, tempVector)) {
                    Vector collisionVel(center, collision);
                    if ((collisionVel.getMagnitude() > 0) && distance.update(collisionVel.getMagnitude())) {
                        nextCenter = collision;
                        nextVel = collisionVel;
                        p_nextElement = p_edge;
                    }
                }
            }
        }
    }
    
    //finished checking collisions
    
    vel = vel / percentage;
    
    percentage = 0.f;
    if ((distance.getValue() < INFINITY) && (vel.getMagnitude() != 0)) {
        percentage = 1.f - (float)(distance.getValue() / vel.getMagnitude());
    }
    
    return nextVel;
}

void World::update(std::vector<std::shared_ptr<Event>> & events) {
    Logger::writeLog(INFO, "starting world[%d] update %d", id, updateTimestamp);
    
    for (std::vector<std::shared_ptr<Event>>::iterator it = events.begin(); it != events.end(); it++) {
        if ((*it)->hasTag(KEYBOARD_EVENT_TAG)) { //DEBUGGING
            std::shared_ptr<KeyboardEvent> p_keyEvent = std::static_pointer_cast<KeyboardEvent>(*it);
            switch (p_keyEvent->getAction()) {
                case KeyboardEvent::KEY_PRESSED: {
                    switch (p_keyEvent->getKey()) {
                        case KeyboardEvent::O: {
                            DarknessOverlay::setInEffect(!DarknessOverlay::isInEffect());
                            break;
                        }
                        case KeyboardEvent::B: {
                            DRAW_BUMPERS = !DRAW_BUMPERS;
                            MapEdge::DRAW_NORMALS = !MapEdge::DRAW_NORMALS;
                            break;
                        }
                        case KeyboardEvent::EQUALS: {
                            setTimeRatio(getTimeRatio() / 0.9);
                            break;
                        }
                        case KeyboardEvent::MINUS: {
                            setTimeRatio(getTimeRatio() * 0.9);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
        }
        
        handleEvent(*it);
    }
    
    for (int i = MAX_PRIORITY; i >= 0; i--) {
        for (std::vector<std::shared_ptr<WorldObject>>::iterator it = objectsToUpdate[i].begin(); it != objectsToUpdate[i].end(); it++) {
            std::shared_ptr<WorldObject> p_object = *it;
            
            if (p_object->getPriority() != i) { //if object's priority has changed, move object to appropriate priority but still update object //TODO this may need to be done after loop
                vector_helpers::removeElementByValue(objectsToUpdate[i], p_object);
                objectsToUpdate[p_object->getPriority()].push_back(p_object);
            }
            
            updateObject(p_object);
        }
    }
    
    for (std::vector<std::shared_ptr<WorldObject>>::iterator it = objectsToDelete.begin(); it != objectsToDelete.end(); it++) {
        removeObject(*it);
    }
    objectsToDelete.clear();
    
    updateTimestamp++;
}

World & World::updateObject(std::shared_ptr<WorldObject> p_object) {
    p_object->Updatable::updateTimestamp(updateTimestamp, this, timeRatio);
    return *this;
}

void World::draw() {
    //Logger::writeLog(INFO, "starting world[%d] draw %d", id, drawTimestamp);
    
    for (int i = 0; i <= MAX_BELOW_ALTITUDE + MAX_ABOVE_ALTITUDE; i++) {
        if (i == MAX_BELOW_ALTITUDE) {
            map.draw();
            if (DRAW_BUMPERS && DEBUG_MODE)
                map.drawBumpers(climber::Player(Coordinate(0, 0)).getMapHitbox());
        }
        
        for (std::vector<std::shared_ptr<WorldObject>>::iterator it = objectsToDraw[i].begin(); it != objectsToDraw[i].end(); it++) {
            std::shared_ptr<WorldObject> p_object = *it;
            
            if ((p_object->getAltitude() + MAX_BELOW_ALTITUDE) != i) { //if object's altitude has changed, move object to appropriate altitude but still draw object
                vector_helpers::removeElementByValue(objectsToDraw[i], p_object);
                objectsToDraw[p_object->getAltitude() + MAX_BELOW_ALTITUDE].push_back(p_object);
            }
            
            p_object->Drawable::draw(drawTimestamp);
        }
    }
    
    DarknessOverlay::draw();
    DarknessOverlay::clearShapes();
    
    drawTimestamp++;
}

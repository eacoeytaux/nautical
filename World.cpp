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
#include "Queue.hpp"
#include "Path.hpp"
#include "MapHitbox.hpp"
#include "WorldObject.hpp"

#include "KeyboardEvent.hpp" //TODO delete
#include "Player.hpp" //TODO delete

using namespace nautical;

bool World::drawBumpers = false;

World::World() {
    static int id = 0;
    this->id = id++;
    
    Map * p_map = &map;
    MapVertex * v0 = p_map->createVertex(Coordinate(100, 640));
    MapVertex * v1 = p_map->createVertex(Coordinate(10, 340));
    MapVertex * v2 = p_map->createVertex(Coordinate(175, 220));
    //MapVertex * v3 = p_map->createVertex(Coordinate(250, 220));
    MapVertex * v4 = p_map->createVertex(Coordinate(400, 290));
    MapVertex * v5 = p_map->createVertex(Coordinate(450, 265));
    MapVertex * v6 = p_map->createVertex(Coordinate(500, 290));
    MapVertex * v7 = p_map->createVertex(Coordinate(800, 240));
    MapVertex * v8 = p_map->createVertex(Coordinate(750, 640));
    
    p_map->createEdge(v0, v1, false);
    p_map->createEdge(v1, v2, true);
    //createEdge(v2, v3, true);
    p_map->createEdge(v2, v4, true);
    p_map->createEdge(v4, v5, true);
    p_map->createEdge(v5, v6, true);
    p_map->createEdge(v6, v7, true);
    p_map->createEdge(v7, v8, false);
}

World::~World() {
    for (Iterator<WorldObject*> * iterator = allObjects.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
}

int World::getID() const {
    return id;
}

int World::getUpdateTimestamp() const {
    return updateTimestamp;
}

int World::getDrawTimestamp() const {
    return drawTimestamp;
}

const Map * World::getMap() const {
    return &map;
}

Map * World::getMap() {
    return &map;
}

bool World::isDarknessInEffect() const {
    return darknessInEffect;
}

World & World::setDarknessInEffect(bool darkness) {
    darknessInEffect = darkness;
    return *this;
}

float World::getDarknessOverlayPercentage() const {
    return darknessOverlay.getPercentage();
}

World & World::setDarknessOverlayPercentage(float percentage) {
    darknessOverlay.setPercentage(percentage);
    return *this;
}

World & World::addShapeToDarknessOverlay(Shape * p_shape, int layer) {
    darknessOverlay.addShape(p_shape, layer);
    return *this;
}

World & World::addObject(WorldObject * p_object, bool shouldUpdate, bool shouldDraw) {
    if (p_object) {
        p_object->setParent(this);
        
        allObjects.insert(p_object);
        if (shouldUpdate)
            objectsToUpdate[p_object->getPriority()].insert(p_object);
        if (shouldDraw)
            objectsToDraw[p_object->getAltitude() + MAX_BELOW_ALTITUDE].insert(p_object);
        
        for (Iterator<std::string> * iterator = p_object->getSubscribedEventTagsIterator(); !iterator->complete(); iterator->next()) {
            subscribeObject(iterator->current(), p_object);
        }
        
        Logger::writeLog(PLAIN_MESSAGE, "World::addObject(): added object[%d] to world[%d]", p_object->getID(), id);
    } else {
        Logger::writeLog(WARNING_MESSAGE, "World::addObject(): attempted to add nullptr");
    }
    
    return *this;
}

World & World::markObjectForRemoval(WorldObject * p_object) {
    objectsToDelete.insert(p_object);
    return *this;
}

World & World::removeObject(WorldObject * p_object) {
    if (p_object) {
        if (allObjects.contains(p_object)) {
            allObjects.remove(p_object);
            if (objectsToUpdate[p_object->getPriority()].contains(p_object))
                objectsToUpdate[p_object->getPriority()].remove(p_object);
            if (objectsToDraw[p_object->getAltitude() + MAX_BELOW_ALTITUDE].contains(p_object))
                objectsToDraw[p_object->getAltitude() + MAX_BELOW_ALTITUDE].remove(p_object);
            
            for (Iterator<std::string> * iterator = p_object->getSubscribedEventTagsIterator(); !iterator->complete(); iterator->next()) {
                unsubscribeObject(iterator->current(), p_object);
            }
        } else {
            Logger::writeLog(WARNING_MESSAGE,"World::removeObject(): attempted to remove object not in world's list of all objects");
        }
        delete p_object;
    } else {
        Logger::writeLog(WARNING_MESSAGE, "World::removeObject(): attempted to remove nullptr");
    }
    return *this;
}

World & World::subscribeObject(std::string eventTag, WorldObject * p_object) {
    LinkedList<WorldObject*> objects;
    objects.insert(p_object);
    return subscribeObjects(eventTag, objects);
}

World & World::subscribeObjects(std::string eventTag, LinkedList<WorldObject*> objects) {
    for (Iterator<EventPairing> * iterator = subscribedObjects.createIterator(); !iterator->complete(); iterator->next()) {
        if (iterator->current().eventTag == eventTag) {
            iterator->current().subscribedObjects.Collection<WorldObject*>::insert(&objects);
            return *this;
        }
    }
    
    //event has not yet been registered
    EventPairing newPair;
    newPair.eventTag = eventTag;
    newPair.subscribedObjects.Collection<WorldObject*>::insert(&objects);
    subscribedObjects.insert(newPair);
    return *this;
}

World & World::unsubscribeObject(std::string eventTag, WorldObject * p_object) {
    LinkedList<WorldObject*> objects;
    objects.insert(p_object);
    return unsubscribeObjects(eventTag, objects);
}

World & World::unsubscribeObjects(std::string eventTag, LinkedList<WorldObject*> objects) {
    for (Iterator<EventPairing> * iterator = subscribedObjects.createIterator(); !iterator->complete(); iterator->next()) {
        if (iterator->current().eventTag == eventTag) {
            iterator->current().subscribedObjects.Collection<WorldObject*>::remove(&objects);
            if (iterator->current().subscribedObjects.empty()) //if no objects are subscribed to event, remove event
                subscribedObjects.remove(iterator->current());
            return *this;
        }
    }
    Logger::writeLog(WARNING_MESSAGE, "World::unregisterObjects(): attempted to unregister objects from non-existant event");
    return *this;
}

World & World::handleEvent(Event * p_event) {
    for (Iterator<EventPairing> * iterator = subscribedObjects.createIterator(); !iterator->complete(); iterator->next()) {
        EventPairing pair = iterator->current();
        if (p_event->hasTag(pair.eventTag)) {
            for (Iterator<WorldObject*> * subIterator = pair.subscribedObjects.createIterator(); !subIterator->complete(); subIterator->next()) {
                subIterator->current()->handleEvent(p_event);
            }
        }
    }
    delete p_event;
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

float World::generatePath(WorldObject * p_object, float percentage) {
    Vector * p_vel = &(p_object->vel);
    MapHitbox * p_hitbox = p_object->p_hitbox;
    
    if (p_vel->getMagnitude() < 0.000000001)
        return 1;
    
    Shape * p_shape = p_hitbox->getShape();
    Coordinate center = p_hitbox->getCenter();
    const MapElement * p_element = p_hitbox->getElement();
    const MapElement * p_prevElement = p_element;
    
    *p_vel *= percentage;
    
    bool velAdjusted = false;
    if (!p_hitbox->adjustVector(p_vel)) {
        p_element = nullptr;
    } else {
        velAdjusted = true;
    }
    
    Coordinate nextCenter = center;
    Vector nextVel = *p_vel;
    const MapElement * p_nextElement = p_element;
    
    int lowerBound, upperBound;
    lowerBound = p_shape->getLowerBoundX();
    upperBound = p_shape->getUpperBoundX();
    (p_vel->isDxPositive() ? upperBound : lowerBound) += p_vel->getDx();
    
    MinValue distance;
    
    if (p_element) { //if touching element, check only element catches
        LinkedList<MapCatch> catches = p_hitbox->findCatches(&map);
        for (Iterator<MapCatch> * iterator = catches.createIterator(); !iterator->complete(); iterator->next()) {
            MapCatch mapCatch = iterator->current();
            
            Line adjustedVelLine = Line(center, center + *p_vel);
            
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
            
            if (p_vertex == p_prevElement)
                continue;
            
            Queue<Coordinate> collisions;
            Line adjustedVelLine = Line(center, center + *p_vel);
            Shape * p_bumper = p_hitbox->createBumper(p_vertex);
            if (p_bumper->intersectsLine(adjustedVelLine, &collisions)) {
                Coordinate collision;
                if (collisions.pop(&collision)) {
                    Vector collisionVel(center, collision);
                    if (distance.update(collisionVel.getMagnitude())) {
                        nextCenter = collision;
                        nextVel = collisionVel;
                        p_nextElement = p_vertex;
                    }
                } else {
                    Logger::writeLog(ERROR_MESSAGE, "World::generatePath(): collisions is empty");
                }
            }
            delete p_bumper;
        }
        
        //check all edges for collision
        for (Iterator<MapEdge*> * iterator = map.getEdgesListIterator(lowerBound, upperBound); !iterator->complete(); iterator->next()) {
            MapEdge * p_edge = iterator->current();
            
            if (p_edge == p_prevElement)
                continue;
            
            Queue<Coordinate> collisions;
            Line adjustedVelLine(center, center + *p_vel);
            Shape * p_bumper = p_hitbox->createBumper(p_edge);
            if (p_bumper->intersectsLine(adjustedVelLine, &collisions)) {
                Coordinate collision;
                if (collisions.pop(&collision)) {
                    Vector collisionVel(center, collision);
                    if (distance.update(collisionVel.getMagnitude())) {
                        nextCenter = collision;
                        nextVel = collisionVel;
                        p_nextElement = p_edge;
                    }
                } else {
                    Logger::writeLog(ERROR_MESSAGE, "World::generatePath(): collisions is empty");
                }
            }
            delete p_bumper;
        }
    }
    
    //finished checking collisions
    
    p_object->move(nextVel);
    p_hitbox->setElement(p_nextElement);
    
    *p_vel /= percentage;//nextVel / percentage;
    
    
    float percentageUsed = 1.f;
    if ((distance.getValue() < INFINITY) && (p_vel->getMagnitude() != 0))
        percentageUsed = distance.getValue() / p_vel->getMagnitude();
    return percentageUsed;
}

void World::update(Collection<Event*> & events) {
    Logger::writeLog(PLAIN_MESSAGE, "starting world[%d] update %d", id, updateTimestamp);
    
    for (Iterator<Event*> * iterator = events.createIterator(); !iterator->complete(); iterator->next()) {
        if (iterator->current()->hasTag(KEYBOARD_EVENT_TAG)) { //TODO for debugging
            if ((static_cast<KeyboardEvent*>(iterator->current())->getAction() == KeyboardEvent::KEY_PRESSED) && (static_cast<KeyboardEvent*>(iterator->current())->getKey() == KeyboardEvent::O))
                setDarknessInEffect(!isDarknessInEffect());
            else if ((static_cast<KeyboardEvent*>(iterator->current())->getAction() == KeyboardEvent::KEY_PRESSED) && (static_cast<KeyboardEvent*>(iterator->current())->getKey() == KeyboardEvent::B))
                drawBumpers = !drawBumpers;
        }
        
        handleEvent(iterator->current());
    }
    
    for (int i = MAX_PRIORITY; i >= 0; i--) {
        for (Iterator<WorldObject*> * iterator = objectsToUpdate[i].createIterator(); !iterator->complete(); iterator->next()) {
            WorldObject* p_object = iterator->current();
            
            if (p_object->getPriority() != i) { //if object's priority has changed, move object to appropriate priority but still update object
                objectsToUpdate[i].remove(p_object);
                objectsToUpdate[p_object->getPriority()].insert(p_object);
            }
            
            updateObject(p_object);
        }
    }
    
    for (Iterator<WorldObject*> * iterator = objectsToDelete.createIterator(); !iterator->complete(); iterator->next()) {
        removeObject(iterator->current());
    }
    objectsToDelete.clear();
    
    updateTimestamp++;
}

void World::updateObject(WorldObject * p_object) {
    p_object->Updatable::update(updateTimestamp);
}

void World::draw() {
    Logger::writeLog(PLAIN_MESSAGE, "starting world[%d] draw %d", id, drawTimestamp);
    
    for (int i = 0; i <= MAX_BELOW_ALTITUDE + MAX_ABOVE_ALTITUDE; i++) {
        if (i == MAX_BELOW_ALTITUDE) {
            map.draw();
            if (drawBumpers && DEBUG_MODE)
                map.drawBumpers(climber::Player(Coordinate(0, 0)).getMapHitbox());
        }
        
        for (Iterator<WorldObject*> * iterator = objectsToDraw[i].createIterator(); !iterator->complete(); iterator->next()) {
            WorldObject* p_object = iterator->current();
            
            if ((p_object->getAltitude() + MAX_BELOW_ALTITUDE) != i) { //if object's altitude has changed, move object to appropriate altitude but still draw object
                objectsToDraw[i].remove(p_object);
                objectsToDraw[p_object->getAltitude() + MAX_BELOW_ALTITUDE].insert(p_object);
            }
            
            p_object->Drawable::draw(drawTimestamp);
        }
    }
    
    if (darknessInEffect)
        darknessOverlay.draw();
    darknessOverlay.clearShapes();
    
    drawTimestamp++;
}

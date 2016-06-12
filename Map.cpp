//
//  Map.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Map.hpp"

#include "MapHitbox.hpp"

using namespace nautical;

float Map::defaultAirResistanceCoefficient = 0.995;
float MapElement::defaultFrictionCoefficient = 0.9;

//these functions used to sort edges and vertices
double weighMapVertex(MapVertex * const * pp_vertex) {
    return (*pp_vertex)->getCoor().getX();
}

double weighMapEdge(MapEdge * const * pp_edge) {
    return (*pp_edge)->getVertexFront()->getCoor().getX();
}

Map::Map() :
vertices(SortedList<MapVertex*>(&weighMapVertex)),
edges((SortedList<MapEdge*>(&weighMapEdge))),
airResistanceCoefficient(defaultAirResistanceCoefficient) { }

Map::~Map() {
    for (Iterator<MapEdge*> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
    
    for (Iterator<MapVertex*> * iterator = vertices.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
}

MapVertex * Map::createVertex(Coordinate coor) {
    MapVertex * p_vertex = new MapVertex(coor);
    p_vertex->setParent(this);
    vertices.insert(p_vertex);
    return p_vertex;
}

MapEdge * Map::createEdge(MapVertex * p_vertexBack, MapVertex * p_vertexFront, bool sticky) {
    MapEdge * p_edge = new MapEdge(p_vertexBack, p_vertexFront, sticky);
    p_edge->setParent(this);
    double edgeDx = p_edge->getVertexFront()->getCoor().getX() - p_edge->getVertexBack()->getCoor().getX();
    edges.insert(p_edge, edgeDx, 0);
    return p_edge;
}

Iterator<MapVertex*> * Map::getVerticesListIterator() const {
    return vertices.createIterator();
}

Iterator<MapVertex*> * Map::getVerticesListIterator(double lowerBound, double upperBound) const {
    return vertices.createIterator(lowerBound, upperBound);
}

Iterator<MapEdge*> * Map::getEdgesListIterator() const {
    return edges.createIterator();
}

Iterator<MapEdge*> * Map::getEdgesListIterator(double lowerBound, double upperBound) const {
    return edges.createIterator(lowerBound, upperBound);
}

double Map::getAirResistanceCoefficient(double value) const {
    return airResistanceCoefficient * value;
}

Map & Map::setAirResistanceCoefficient(float airResistanceCoefficient) {
    this->airResistanceCoefficient = airResistanceCoefficient;
    return *this;
}

double Map::getDefaultAirResistanceCoefficient(double value) {
    return Map::defaultAirResistanceCoefficient * value;
}

void Map::setDefaultAirResistanceCoefficient(float airResistanceCoefficient) {
    Map::defaultAirResistanceCoefficient = airResistanceCoefficient;
}

void Map::draw() const {
    for (Iterator<MapEdge*> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        iterator->current()->draw();
    }
    if (DEBUG_MODE) {
        for (Iterator<MapVertex*> * iterator = vertices.createIterator(); !iterator->complete(); iterator->next()) {
            iterator->current()->draw();
        }
    }
}

void Map::drawBumpers(MapHitbox * p_hitbox, bool drawCatches) const {
    for (Iterator<MapEdge*> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        Shape * p_bumper = p_hitbox->createBumper(iterator->current());
        p_bumper->Drawable::draw(Color(CYAN).setA(127));
        delete p_bumper;
        if (drawCatches) {
            LinkedList<MapCatch> catches = p_hitbox->findCatches(iterator->current(), this);
            for (Iterator<MapCatch> * subIterator = catches.createIterator(); !subIterator->complete(); subIterator->next()) {
                GraphicsManager::drawLine(subIterator->current().getLine(), Color(YELLOW).setA(64));
            }
        }
    }
    for (Iterator<MapVertex*> * iterator = vertices.createIterator(); !iterator->complete(); iterator->next()) {
        Shape * p_bumper = p_hitbox->createBumper(iterator->current());
        p_bumper->Drawable::draw(Color(CYAN).setA(127));
        delete p_bumper;
        if (drawCatches) {
            LinkedList<MapCatch> catches = p_hitbox->findCatches(iterator->current(), this);
            for (Iterator<MapCatch> * subIterator = catches.createIterator(); !subIterator->complete(); subIterator->next()) {
                GraphicsManager::drawLine(subIterator->current().getLine(), Color(YELLOW).setA(64));
            }
        }
    }
}

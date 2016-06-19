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

double Map::defaultAirResistanceCoefficient = 0.995;
double MapElement::defaultFrictionCoefficient = 0.9;

//these functions used to sort edges and vertices
double weighMapVertexX(MapVertex * const * pp_vertex) {
    return (*pp_vertex)->getCoor().getX();
}

double weighMapVertexY(MapVertex * const * pp_vertex) {
    return (*pp_vertex)->getCoor().getY();
}

double weighMapEdgeX(MapEdge * const * pp_edge) {
    return (*pp_edge)->getVertexFront()->getCoor().getX();
}

double weighMapEdgeY(MapEdge * const * pp_edge) {
    return (*pp_edge)->getVertexFront()->getCoor().getY();
}

Map::Map(bool verticalMap) :
verticalMap(verticalMap),
vertices(SortedList<MapVertex*>(verticalMap ? &weighMapVertexY : &weighMapVertexX)),
edges((SortedList<MapEdge*>(verticalMap ? &weighMapEdgeY : &weighMapEdgeX))),
airResistanceCoefficient(defaultAirResistanceCoefficient) { }

Map::~Map() {
    for (Iterator<MapEdge*> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
    
    for (Iterator<MapVertex*> * iterator = vertices.createIterator(); !iterator->complete(); iterator->next()) {
        delete iterator->current();
    }
}

bool Map::isVertical() const {
    return verticalMap;
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
    double largestEdgeLength = fabs(verticalMap ? (p_edge->getVertexFront()->getCoor().getY() - p_edge->getVertexBack()->getCoor().getY()) : (p_edge->getVertexFront()->getCoor().getX() - p_edge->getVertexBack()->getCoor().getX())); //TODO does this need to be fabsed?
    edges.insert(p_edge, largestEdgeLength, 0);
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

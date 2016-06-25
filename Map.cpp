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
airResistanceCoefficient(defaultAirResistanceCoefficient) { }

Map::~Map() {
    for (std::vector<MapEdge*>::iterator it = edges.begin(); it != edges.end(); it++) {
        delete *it;
    }
    
    for (std::vector<MapVertex*>::iterator it = vertices.begin(); it != vertices.end(); it++) {
        delete *it;
    }
}

bool Map::isVertical() const {
    return verticalMap;
}

MapVertex * Map::createVertex(Coordinate coor) {
    MapVertex * p_vertex = new MapVertex(coor);
    p_vertex->setParent(this);
    vertices.push_back(p_vertex);
    return p_vertex;
}

MapEdge * Map::createEdge(MapVertex * p_vertexBack, MapVertex * p_vertexFront, bool sticky) {
    MapEdge * p_edge = new MapEdge(p_vertexBack, p_vertexFront, sticky);
    p_edge->setParent(this);
    double largestEdgeLength = fabs(verticalMap ? (p_edge->getVertexFront()->getCoor().getY() - p_edge->getVertexBack()->getCoor().getY()) : (p_edge->getVertexFront()->getCoor().getX() - p_edge->getVertexBack()->getCoor().getX())); //TODO does this need to be fabsed?
    edges.push_back(p_edge);
    return p_edge;
}

const std::vector<MapVertex*> * Map::getVerticesList() const {
    return &vertices;
}

const std::vector<MapEdge*> * Map::getEdgesList() const {
    return &edges;
}

double Map::getAirResistanceCoefficient(double value) const {
    return airResistanceCoefficient * value;
}

void Map::setAirResistanceCoefficient(float airResistanceCoefficient) {
    this->airResistanceCoefficient = airResistanceCoefficient;
}

double Map::getDefaultAirResistanceCoefficient(double value) {
    return Map::defaultAirResistanceCoefficient * value;
}

void Map::setDefaultAirResistanceCoefficient(float airResistanceCoefficient) {
    Map::defaultAirResistanceCoefficient = airResistanceCoefficient;
}

void Map::draw() const {
    for (std::vector<MapEdge*>::const_iterator it = edges.begin(); it != edges.end(); it++) {
        (*it)->draw();
    }
    if (DEBUG_MODE) {
        for (std::vector<MapVertex*>::const_iterator it = vertices.begin(); it != vertices.end(); it++) {
            (*it)->draw();
        }
    }
}

void Map::drawBumpers(MapHitbox * p_hitbox, bool drawCatches) const {
    for (std::vector<MapEdge*>::const_iterator it = edges.begin(); it != edges.end(); it++) {
        p_hitbox->createBumper(*it)->Drawable::draw(Color(CYAN).setA(127));
        if (drawCatches) {
            std::vector<MapCatch> catches = p_hitbox->findCatches(*it, this);
            for (std::vector<MapCatch>::iterator subIt = catches.begin(); subIt != catches.end(); subIt++) {
                GraphicsManager::drawLine(subIt->getLine(), Color(YELLOW).setA(64));
            }
        }
    }
    for (std::vector<MapVertex*>::const_iterator it = vertices.begin(); it != vertices.end(); it++) {
        p_hitbox->createBumper(*it)->Drawable::draw(Color(CYAN).setA(127));
        if (drawCatches) {
            std::vector<MapCatch> catches = p_hitbox->findCatches(*it, this);
            for (std::vector<MapCatch>::iterator subIt = catches.begin(); subIt != catches.end(); subIt++) {
                GraphicsManager::drawLine(subIt->getLine(), Color(YELLOW).setA(64));
            }
        }
    }
}

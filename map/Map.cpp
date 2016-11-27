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
//double weighMapVertexX(MapVertex * const * pp_vertex) {
//    return (*pp_vertex)->getCoor().getX();
//}
//
//double weighMapVertexY(MapVertex * const * pp_vertex) {
//    return (*pp_vertex)->getCoor().getY();
//}
//
//double weighMapEdgeX(MapEdge * const * pp_edge) {
//    return (*pp_edge)->getVertexFront()->getCoor().getX();
//}
//
//double weighMapEdgeY(MapEdge * const * pp_edge) {
//    return (*pp_edge)->getVertexFront()->getCoor().getY();
//}

Map::Map(bool verticalMap) :
verticalMap(verticalMap),
airResistanceCoefficient(defaultAirResistanceCoefficient) { }

Map::~Map() { }

bool Map::isVertical() const {
    return verticalMap;
}

std::shared_ptr<MapVertex> Map::createVertex(Coordinate coor) {
    std::shared_ptr<MapVertex> p_vertex = std::shared_ptr<MapVertex>(new MapVertex(coor));
    //p_vertex->setParent(std::shared_ptr<Map>(this));
    p_vertices.push_back(p_vertex);
    return p_vertex;
}

std::shared_ptr<MapEdge> Map::createEdge(std::shared_ptr<MapVertex> p_vertexBack, std::shared_ptr<MapVertex> p_vertexFront, bool sticky) {
    std::shared_ptr<MapEdge> p_edge = std::shared_ptr<MapEdge>(new MapEdge(p_vertexBack, p_vertexFront, sticky));
    //p_edge->setParent(std::shared_ptr<Map>(this));
    double largestEdgeLength = fabs(verticalMap ? (p_edge->getVertexFront()->getCoor().getY() - p_edge->getVertexBack()->getCoor().getY()) : (p_edge->getVertexFront()->getCoor().getX() - p_edge->getVertexBack()->getCoor().getX())); //TODO does this need to be fabsed?
    p_edges.push_back(p_edge);
    return p_edge;
}

const std::vector<std::shared_ptr<MapVertex>> & Map::getVerticesList() const {
    return p_vertices;
}

const std::vector<std::shared_ptr<MapEdge>> & Map::getEdgesList() const {
    return p_edges;
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
    for (std::vector<std::shared_ptr<MapEdge>>::const_iterator it = p_edges.begin(); it != p_edges.end(); it++) {
        (*it)->draw();
    }
    if (DEBUG_MODE) {
        for (std::vector<std::shared_ptr<MapVertex>>::const_iterator it = p_vertices.begin(); it != p_vertices.end(); it++) {
            (*it)->draw();
        }
    }
}

void Map::drawBumpers(std::shared_ptr<MapHitbox> p_hitbox, bool drawCatches) const {
    for (std::vector<std::shared_ptr<MapEdge>>::const_iterator it = p_edges.begin(); it != p_edges.end(); it++) {
        p_hitbox->createBumper(*it)->Drawable::draw(Color(CYAN).setA(127));
        if (drawCatches) {
//            std::vector<MapCatch> catches = p_hitbox->findCatches(*it, p_vertices, p_edges);
//            for (std::vector<MapCatch>::iterator subIt = catches.begin(); subIt != catches.end(); subIt++) {
//                GraphicsManager::drawLine(subIt->getLine(), Color(YELLOW).setA(64));
//            }
        }
    }
    for (std::vector<std::shared_ptr<MapVertex>>::const_iterator it = p_vertices.begin(); it != p_vertices.end(); it++) {
        p_hitbox->createBumper(*it)->Drawable::draw(Color(CYAN).setA(127));
        if (drawCatches) {
//            std::vector<MapCatch> catches = p_hitbox->findCatches(*it, p_vertices, p_edges);
//            for (std::vector<MapCatch>::iterator subIt = catches.begin(); subIt != catches.end(); subIt++) {
//                GraphicsManager::drawLine(subIt->getLine(), Color(YELLOW).setA(64));
//            }
        }
    }
}

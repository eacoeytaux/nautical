//
//  MapVertex.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/16/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "MapVertex.hpp"

#include "Utility.hpp"
#include "Map.hpp"
#include "MapEdge.hpp"

using namespace nautical;

MapVertex::MapVertex(Coordinate coor, bool sticky) :
MapElement(sticky),
coor(coor) {
    appendTag(MAP_VERTEX_TAG);
    
    setColor(YELLOW);
}

MapVertex::~MapVertex() { }

Coordinate MapVertex::getCoor() const {
    return coor;
}

std::shared_ptr<const MapEdge> MapVertex::getEdgeFront() const {
    return p_edgeFront.lock();
}

MapVertex & MapVertex::setEdgeFront(std::shared_ptr<const MapEdge> p_edge) {
    p_edgeFront = p_edge;
    return *this;
}

std::shared_ptr<const MapEdge> MapVertex::getEdgeBack() const {
    return p_edgeBack.lock();
}

MapVertex & MapVertex::setEdgeBack(std::shared_ptr<const MapEdge> p_edge) {
    p_edgeBack = p_edge;
    return *this;
}

void MapVertex::draw() const {
    GraphicsManager::drawCoordinate(coor, getColor());
}

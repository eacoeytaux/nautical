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

MapVertex::~MapVertex() {
    if (p_edgeFront)
        delete p_edgeFront;
    if (p_edgeBack)
        delete p_edgeBack;
}

Coordinate MapVertex::getCoor() const {
    return coor;
}

MapEdge * MapVertex::getEdgeFront() const {
    return p_edgeFront;
}

void MapVertex::setEdgeFront(MapEdge * p_edge) {
    p_edgeFront = p_edge;
}

MapEdge * MapVertex::getEdgeBack() const {
    return p_edgeBack;
}

void MapVertex::setEdgeBack(MapEdge * p_edge) {
    p_edgeBack = p_edge;
}

void MapVertex::draw() const {
    GraphicsManager::drawCoordinate(coor, getColor());
}

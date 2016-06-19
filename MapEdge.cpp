//
//  MapEdge.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/16/15.
//  Copyright (c) 2015 EthanCo. All rights reserved.
//

#include "MapEdge.hpp"

#include "Queue.hpp"
#include "LineShape.hpp"
#include "Map.hpp"
#include "MapVertex.hpp"

using namespace nautical;

bool MapEdge::DRAW_NORMALS = false;

MapEdge::MapEdge(MapVertex * p_vertexBack, MapVertex * p_vertexFront, bool sticky) :
MapElement(sticky),
p_vertexBack(p_vertexBack),
p_vertexFront(p_vertexFront) {
    appendTag(MAP_EDGE_TAG);
    
    setColor(isSticky() ? GREEN : RED);
    line = Line(p_vertexBack->getCoor(), p_vertexFront->getCoor());
    normal = Angle(line.getAngle() + M_PI_2);
    
    p_vertexBack->setEdgeFront(this);
    p_vertexFront->setEdgeBack(this);
}

MapEdge::~MapEdge() {
    p_vertexFront->setEdgeBack(nullptr);
    p_vertexBack->setEdgeFront(nullptr);
}

Line MapEdge::getLine() const {
    return line;
}

Angle MapEdge::getNormal() const {
    return normal;
}

MapVertex * MapEdge::getVertex(MapVertex * p_other) const {
    if (p_other == p_vertexFront)
        return p_vertexBack;
    else if (p_other == p_vertexBack)
        return p_vertexFront;
    else
        return nullptr;
}

MapVertex * MapEdge::getVertexFront() const {
    return p_vertexFront;
}

MapEdge & MapEdge::setVertexFront(MapVertex * p_vertex) {
    this->p_vertexFront = p_vertex;
    return *this;
}

MapVertex * MapEdge::getVertexBack() const {
    return p_vertexBack;
}

MapEdge & MapEdge::setVertexBack(MapVertex * p_vertex) {
    this->p_vertexBack = p_vertex;
    return *this;
}

void MapEdge::draw() const {
    GraphicsManager::drawLine(line, getColor());
    
    if (DRAW_NORMALS && DEBUG_MODE) {
        Coordinate origin = p_vertexBack->getCoor() + (Vector(p_vertexBack->getCoor(), p_vertexFront->getCoor()) / 2);
        Vector(normal, 5).setOrigin(origin).Drawable::draw(getColor().setA(127));
    }
}

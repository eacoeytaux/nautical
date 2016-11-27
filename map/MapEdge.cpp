//
//  MapEdge.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/16/15.
//  Copyright (c) 2015 EthanCo. All rights reserved.
//

#include "MapEdge.hpp"

#include "LineShape.hpp"
#include "Map.hpp"
#include "MapVertex.hpp"

using namespace nautical;

bool MapEdge::DRAW_NORMALS = false;

MapEdge::MapEdge(std::shared_ptr<MapVertex> p_vertexBack, std::shared_ptr<MapVertex> p_vertexFront, bool sticky) :
MapElement(sticky),
p_vertexBack(std::weak_ptr<MapVertex>(p_vertexBack)),
p_vertexFront(std::weak_ptr<MapVertex>(p_vertexFront)) {
    appendTag(MAP_EDGE_TAG);
    
    setColor(isSticky() ? GREEN : RED);
    line = Line(p_vertexBack->getCoor(), p_vertexFront->getCoor());
    normal = Angle(line.getAngle() + M_PI_2);
    
    //p_vertexBack->setEdgeFront(shared_from_this());
    //p_vertexFront->setEdgeBack(shared_from_this());
}

MapEdge::~MapEdge() { }

Line MapEdge::getLine() const {
    return line;
}

Angle MapEdge::getNormal() const {
    return normal;
}

std::shared_ptr<MapVertex> MapEdge::getVertex(std::shared_ptr<MapVertex> p_other) const {
    if (p_other == p_vertexFront.lock())
        return p_vertexBack.lock();
    else if (p_other == p_vertexBack.lock())
        return p_vertexFront.lock();
    else
        return nullptr;
}

std::shared_ptr<MapVertex> MapEdge::getVertexFront() const {
    return p_vertexFront.lock();
}

MapEdge & MapEdge::setVertexFront(std::shared_ptr<MapVertex> p_vertex) {
    this->p_vertexFront = p_vertex;
    return *this;
}

std::shared_ptr<MapVertex> MapEdge::getVertexBack() const {
    return p_vertexBack.lock();
}

MapEdge & MapEdge::setVertexBack(std::shared_ptr<MapVertex> p_vertex) {
    this->p_vertexBack = p_vertex;
    return *this;
}

void MapEdge::draw() const {
    GraphicsManager::drawLine(line, getColor());
    
    if (DRAW_NORMALS && DEBUG_MODE) {
        Coordinate origin = p_vertexBack.lock()->getCoor() + (Vector(p_vertexBack.lock()->getCoor(), p_vertexFront.lock()->getCoor()) / 2);
        Vector(normal, 5, origin).Drawable::draw(getColor().setA(127));
    }
}

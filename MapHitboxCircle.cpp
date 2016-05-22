//
//  MapHitboxCircle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/22/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "MapHitboxCircle.hpp"
#include "Queue.hpp"
#include "SortedList.hpp"

using namespace nautical;

MapHitboxCircle::MapHitboxCircle(Circle circle) : circle(circle) { }

MapHitboxCircle::~MapHitboxCircle() { }

Shape * MapHitboxCircle::getShape() const {
    return new Circle(circle);
}

Circle MapHitboxCircle::getCircle() const {
    return circle;
}

MapHitboxCircle & MapHitboxCircle::setCircle(Circle circle) {
    this->circle = circle;
    return *this;
}

bool MapHitboxCircle::adjustVector(const MapVertex * p_vertex, Vector * p_vector) const {
    return false; //TODO
}

Circle * MapHitboxCircle::createBumper(const MapVertex * p_vertex) const {
    return new Circle(p_vertex->getCoor(), circle.getRadius());
}

LinkedList<MapCatch> MapHitboxCircle::findCatches(const MapVertex * p_vertex, const Map * p_map) const {
    return LinkedList<MapCatch>(); //TODO
}

bool MapHitboxCircle::adjustVector(const MapEdge * p_edge, Vector * p_vector) const {
    return false; //TODO
}

LineShape * MapHitboxCircle::createBumper(const MapEdge * p_edge) const {
    LineShape * bumper = new LineShape(p_edge->getLine());
    Vector bumperOffset(p_edge->getNormal(), circle.getRadius());
    bumper->move(bumperOffset);
    return bumper;
}

LinkedList<MapCatch> MapHitboxCircle::findCatches(const MapEdge * p_edge, const Map * p_map) const {
    return LinkedList<MapCatch>(); //TODO
}

Vector MapHitboxCircle::getOffset(const MapEdge * p_edge) const {
    return Vector(0, 0); //TODO
}

MapCatch MapHitboxCircle::getCatchFront(const MapEdge * p_edge) const {
    Coordinate center = p_edge->getVertexFront()->getCoor();
    Vector offset = getOffset(p_edge);
    return MapCatch(center + offset, Line(center, center + (offset * 2)), (MapElement*)p_edge, (MapElement*)p_edge->getVertexFront());
}

MapCatch MapHitboxCircle::getCatchBack(const MapEdge * p_edge) const {
    Coordinate center = p_edge->getVertexBack()->getCoor();
    Vector offset = getOffset(p_edge);
    return MapCatch(center + offset, Line(center, center + (offset * 2)), (MapElement*)p_edge, (MapElement*)p_edge->getVertexBack());
}

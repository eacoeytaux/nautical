//
//  MapHitboxCircle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/22/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "MapHitboxCircle.hpp"

using namespace nautical;

MapHitboxCircle::MapHitboxCircle(Circle circle) : circle(circle) {
    center = circle.getCenter();
}

MapHitboxCircle::~MapHitboxCircle() { }

MapHitboxCircle & MapHitboxCircle::move(Vector vec) {
    MapHitbox::move(vec);
    circle.move(vec);
    return *this;
}

Shape * MapHitboxCircle::getShape_() const {
    return new Circle(circle);
}

Circle MapHitboxCircle::getCircle() const {
    return circle;
}

MapHitboxCircle & MapHitboxCircle::setCircle(Circle circle) {
    this->circle = circle;
    center = circle.getCenter();
    return *this;
}

bool MapHitboxCircle::adjustVector(const MapVertex * p_vertex, Vector * p_vector) const {
    return false; //TODO
}

Circle * MapHitboxCircle::createBumper_(const MapVertex * p_vertex) const {
    return new Circle(p_vertex->getCoor(), circle.getRadius());
}

std::vector<MapCatch> MapHitboxCircle::findCatches(const MapVertex * p_vertex, const Map * p_map) const {
    return std::vector<MapCatch>(); //TODO
}

bool MapHitboxCircle::adjustVector(const MapEdge * p_edge, Vector * p_vector) const {
    return false; //TODO
}

LineShape * MapHitboxCircle::createBumper_(const MapEdge * p_edge) const {
    LineShape * bumper = new LineShape(p_edge->getLine());
    Vector bumperOffset(p_edge->getNormal(), circle.getRadius());
    bumper->move(bumperOffset);
    return bumper;
}

std::vector<MapCatch> MapHitboxCircle::findCatches(const MapEdge * p_edge, const Map * p_map) const {
    return std::vector<MapCatch>(); //TODO
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

MapHitboxCircle * MapHitboxCircle::copyPtr_() const {
    return new MapHitboxCircle(*this);
}

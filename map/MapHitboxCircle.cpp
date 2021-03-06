//
//  MapHitboxCircle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/22/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "MapHitboxCircle.hpp"

#include "LineShape.hpp"

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

std::shared_ptr<Shape> MapHitboxCircle::getShape() const {
    return std::shared_ptr<Shape>(new Circle(circle));
}

Circle MapHitboxCircle::getCircle() const {
    return circle;
}

MapHitboxCircle & MapHitboxCircle::setCircle(Circle circle) {
    this->circle = circle;
    center = circle.getCenter();
    return *this;
}

bool MapHitboxCircle::adjustVector(std::shared_ptr<const MapVertex> p_vertex, Vector & vector) const {
    return false; //TODO
}

std::shared_ptr<Shape> MapHitboxCircle::createBumper(std::shared_ptr<const MapVertex> p_vertex) const {
    return std::shared_ptr<Shape>(new Circle(p_vertex->getCoor(), circle.getRadius()));
}

std::vector<MapCatch> MapHitboxCircle::findCatches(std::shared_ptr<const MapVertex> p_vertex, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const {
    return std::vector<MapCatch>(); //TODO
}

bool MapHitboxCircle::adjustVector(std::shared_ptr<const MapEdge> p_edge, Vector & vector) const {
    return false; //TODO
}

std::shared_ptr<Shape> MapHitboxCircle::createBumper(std::shared_ptr<const MapEdge> p_edge) const {
    std::shared_ptr<Shape> bumper(new LineShape(p_edge->getLine()));
    Vector bumperOffset(p_edge->getNormal(), circle.getRadius());
    bumper->move(bumperOffset);
    return bumper;
}

std::vector<MapCatch> MapHitboxCircle::findCatches(std::shared_ptr<const MapEdge> p_edge, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const {
    return std::vector<MapCatch>(); //TODO
}

Vector MapHitboxCircle::getOffset(std::shared_ptr<const MapEdge> p_edge) const {
    return Vector(0, 0); //TODO
}

MapCatch MapHitboxCircle::getCatchFront(std::shared_ptr<const MapEdge> p_edge) const {
    Coordinate center = p_edge->getVertexFront()->getCoor();
    Vector offset = getOffset(p_edge);
    return MapCatch(center + offset, Line(center, center + (offset * 2)), std::static_pointer_cast<const MapElement>(p_edge), std::static_pointer_cast<const MapElement>(p_edge->getVertexFront()));
}

MapCatch MapHitboxCircle::getCatchBack(std::shared_ptr<const MapEdge> p_edge) const {
    Coordinate center = p_edge->getVertexBack()->getCoor();
    Vector offset = getOffset(p_edge);
    return MapCatch(center + offset, Line(center, center + (offset * 2)), std::static_pointer_cast<const MapElement>(p_edge), std::static_pointer_cast<const MapElement>(p_edge->getVertexBack()));
}

//MapHitboxCircle * MapHitboxCircle::deepCopy() const {
//    return new MapHitboxCircle(*this);
//}

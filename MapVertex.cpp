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

MapVertex & MapVertex::setEdgeFront(MapEdge * p_edge) {
    p_edgeFront = p_edge;
    return *this;
}

MapEdge * MapVertex::getEdgeBack() const {
    return p_edgeBack;
}

MapVertex & MapVertex::setEdgeBack(MapEdge * p_edge) {
    p_edgeBack = p_edge;
    return *this;
}

bool MapVertex::adjustVectorRectangle(Rectangle rectangle, Vector * p_vector, ObjectPos pos) const {
    Angle recAngle = rectangle.getAngle();
    double recAngleValue = recAngle.getValue();
    
    if (pos == UNDEFINED) {
        Coordinate recCenter = rectangle.getCenter();
        Angle recDiagonalAngle = Angle(rectangle.getWidth(), rectangle.getHeight());
        
        Angle angleToOrigin = findAngle(coor, recCenter);
        
        if ((angleToOrigin.getValue() > M_PI - recDiagonalAngle.getValue() + recAngleValue) || (angleToOrigin.getValue() < -M_PI + recDiagonalAngle.getValue() + recAngleValue)) {
            pos = LEFT;
        } else if (angleToOrigin.getValue() > recDiagonalAngle.getValue() + recAngleValue) {
            pos = BELOW;
        } else if (angleToOrigin.getValue() > -recDiagonalAngle.getValue() + recAngleValue) {
            pos = RIGHT;
        } else {
            pos = ABOVE;
        }
    }
    
    switch (pos) {
        case ABOVE: {
            return p_vector->subtractAngle(Angle(-M_PI_2) + recAngle);
        } case RIGHT: {
            return p_vector->subtractAngle(recAngle);
        } case BELOW: {
            return p_vector->subtractAngle(Angle(M_PI_2) + recAngle);
        } case LEFT: {
            return p_vector->subtractAngle(Angle(M_PI) + rectangle.getAngle());
        } default: {
            Logger::writeLog(ERROR, "MapVertex::adjustVectorRectangle(): pos not defined");
            return false;
        }
    }
}

Rectangle * MapVertex::generateBumperRectangle(Rectangle rectangle) const {
    return new Rectangle(coor, rectangle.getWidth(), rectangle.getHeight(), rectangle.getAngle());
}

Circle * MapVertex::generateBumperCircle(Circle circle) const {
    return new Circle(coor, circle.getRadius());
}

LinkedList<MapCatch> MapVertex::findCatches(Rectangle rectangle, const Map * p_map) const {
    LinkedList<MapCatch> catches;
    if (p_edgeFront)
        catches.insert(p_edgeFront->getCatchBackRectangle(rectangle));
    if (p_edgeBack)
        catches.insert(p_edgeBack->getCatchFrontRectangle(rectangle));
    return catches;
}

void MapVertex::draw() const {
    GraphicsManager::drawCoordinate(coor, getColor());
}

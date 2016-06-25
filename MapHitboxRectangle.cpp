//
//  MapHitboxRectangle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/22/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "MapHitboxRectangle.hpp"

#include <algorithm>

#include "Random.hpp"
#include "LineShape.hpp"

using namespace nautical;

MapHitboxRectangle::MapHitboxRectangle(Rectangle rec) : rec(rec) {
    center = rec.getCenter();
}

MapHitboxRectangle::~MapHitboxRectangle() { }

void MapHitboxRectangle::move(Vector vec) {
    MapHitbox::move(vec);
    rec.move(vec);
}

std::shared_ptr<Shape> MapHitboxRectangle::getShape() const {
    return std::shared_ptr<Shape>(new Rectangle(rec));
}

Rectangle MapHitboxRectangle::getRectangle() const {
    return rec;
}

void MapHitboxRectangle::setRectangle(Rectangle rec) {
    this->rec = rec;
    center = rec.getCenter();
}

bool MapHitboxRectangle::adjustVector(const MapVertex * p_vertex, Vector * p_vector) const {
    Angle recAngle = rec.getAngle();
    double recAngleValue = rec.getAngle().getValue();
    double recDiagonalAngleValue = Angle(rec.getWidth(), rec.getHeight()).getValue();
    double angleToOriginValue = findAngle(p_vertex->getCoor(), p_vector->getOrigin()).getValue();
    
    enum MapEdgeRelativePosition {
        UNDEFINED = 0,
        ABOVE,
        RIGHT,
        BELOW,
        LEFT
    } edgePos = UNDEFINED;
    
    if ((angleToOriginValue > M_PI - recDiagonalAngleValue + recAngleValue) || (angleToOriginValue < -M_PI + recDiagonalAngleValue + recAngleValue)) {
        edgePos = LEFT;
    } else if (angleToOriginValue > recDiagonalAngleValue + recAngleValue) {
        edgePos = ABOVE;
    } else if (angleToOriginValue > -recDiagonalAngleValue + recAngleValue) {
        edgePos = RIGHT;
    } else { //if (angleToOrigin.getValue() > -M_PI + recDiagonalAngleValue + recAngleValue) {
        return BELOW;
    }
    
    //check to see if edgePos is in invalid pos
    MapEdge * p_edgeFront = p_vertex->getEdgeFront(),
    * p_edgeBack = p_vertex->getEdgeBack();
    if (p_edgeFront) {
        double normalAngleValue = p_edgeFront->getNormal().getValue();
        
        if (normalAngleValue > M_PI_2) {
            if (edgePos == BELOW)
                edgePos = RIGHT;
        } else if (normalAngleValue > 0) {
            if (edgePos == RIGHT)
                edgePos = ABOVE;
        } else if (normalAngleValue > -M_PI_2) {
            if (edgePos == ABOVE)
                edgePos = LEFT;
        } else {
            if (edgePos == LEFT)
                edgePos = BELOW;
        }
    }
    if (p_edgeBack) {
        double normalAngleValue = p_edgeBack->getNormal().getValue();
        
        if (normalAngleValue > M_PI_2) {
            if (edgePos == LEFT)
                edgePos = ABOVE;
        } else if (normalAngleValue > 0) {
            if (edgePos == ABOVE)
                edgePos = RIGHT;
        } else if (normalAngleValue > -M_PI_2) {
            if (edgePos == RIGHT)
                edgePos = BELOW;
        } else {
            if (edgePos == BELOW)
                edgePos = LEFT;
        }
    }
    
    //adjust vector according to angle
    switch (edgePos) {
        case ABOVE:
            return p_vector->subtractAngle(Angle(M_PI_2) + recAngle);
        case RIGHT:
            return p_vector->subtractAngle(recAngle);
        case BELOW:
            return p_vector->subtractAngle(Angle(-M_PI_2) + recAngle);
        case LEFT:
            return p_vector->subtractAngle(Angle(M_PI) + recAngle);
        case UNDEFINED:
        default:
            return false;
    }
    
    // perserved if needed to revert to previous code
    //if ((angleToOrigin.getValue() > M_PI - recDiagonalAngleValue + recAngleValue) || (angleToOrigin.getValue() < -M_PI + recDiagonalAngleValue + recAngleValue)) {
    //    return p_vector->subtractAngle(Angle(M_PI) + recAngle);
    //} else if (angleToOrigin.getValue() > recDiagonalAngleValue + recAngleValue) {
    //    return p_vector->subtractAngle(Angle(M_PI_2) + recAngle);
    //} else if (angleToOrigin.getValue() > -recDiagonalAngleValue + recAngleValue) {
    //    return p_vector->subtractAngle(recAngle);
    //} else { //if (angleToOrigin.getValue() > -M_PI + recDiagonalAngleValue + recAngleValue) {
    //    return p_vector->subtractAngle(Angle(-M_PI_2) + recAngle);
    //}
}

std::shared_ptr<Shape> MapHitboxRectangle::createBumper(const MapVertex * p_vertex) const {
    return std::shared_ptr<Shape>(new Rectangle(p_vertex->getCoor(), rec.getWidth(), rec.getHeight(), rec.getAngle()));
}

std::vector<MapCatch> MapHitboxRectangle::findCatches(const MapVertex * p_vertex, const Map * p_map) const {
    std::vector<MapCatch> catches;
    if (p_vertex->getEdgeFront())
        catches.push_back(getCatchBack(p_vertex->getEdgeFront()));
    if (p_vertex->getEdgeBack())
        catches.push_back(getCatchFront(p_vertex->getEdgeBack()));
    return catches;
}

bool MapHitboxRectangle::adjustVector(const MapEdge * p_edge, Vector * p_vector) const {
    return p_vector->subtractAngle(p_edge->getNormal());
}

std::shared_ptr<Shape> MapHitboxRectangle::createBumper(const MapEdge * p_edge) const {
    std::shared_ptr<Shape> bumper(new LineShape(p_edge->getLine()));
    bumper->move(getOffset(p_edge));
    return bumper;
}

std::vector<MapCatch> MapHitboxRectangle::findCatches(const MapEdge * p_edge, const Map * p_map) const {
    std::vector<MapCatch> catches;
    const std::vector<MapEdge*> * p_edgesList = p_map->getEdgesList();
    for (std::vector<MapEdge*>::const_iterator it = p_edgesList->begin(); it != p_edgesList->end(); it++) {//= p_map->getEdgesListIterator(fmin(p_edge->getVertexBack()->getCoor().getX(), p_edge->getVertexFront()->getCoor().getX()) - rec.getWidth(), fmax(p_edge->getVertexBack()->getCoor().getX(), p_edge->getVertexFront()->getCoor().getX()) + rec.getWidth()); !iterator->complete(); iterator->next()) { //TODO filter by lower/upper bounds
        MapEdge * p_edge2 = *it;
        if (p_edge2 == p_edge)
            continue;
        
        std::shared_ptr<Shape> p_lineShape = createBumper(p_edge);
        std::shared_ptr<Shape> p_lineShape2 = createBumper(p_edge2);
        std::vector<Coordinate> collisions;
        if (p_lineShape->intersectsShape(p_lineShape2.get(), &collisions))
            catches.push_back(MapCatch(collisions.front(), std::dynamic_pointer_cast<LineShape>(p_lineShape2)->getLine(), (MapElement*)p_edge, (MapElement*)p_edge2));
    }
    catches.push_back(getCatchBack(p_edge));
    catches.push_back(getCatchFront(p_edge));
    return catches;
}

Vector MapHitboxRectangle::getOffset(const MapEdge * p_edge) const {
    double distance = rec.getDiagonalLength() / 2;
    Angle angleOffset(rec.getWidth(), rec.getHeight());
    Angle recAngle = rec.getAngle();
    double recAngleValue = recAngle.getValue();
    double normalValue = p_edge->getNormal().getValue();
    
    std::vector<Angle> angles = {Angle(M_PI) + recAngle,
        Angle(M_PI_2) + recAngle,
        recAngle,
        Angle(-M_PI_2) + recAngle};
    std::sort(angles.begin(), angles.end());
    
    Vector ret;
    if ((normalValue >= angles.at(0).getValue()) && (normalValue < angles.at(1).getValue())) {
        if (recAngleValue > M_PI_2) {
            ret = Vector((angleOffset * -1), -distance);
        } else if (recAngleValue > 0) {
            ret = Vector(angleOffset, -distance);
        } else if (recAngleValue >= -M_PI_2) {
            ret = Vector((angleOffset * -1), distance);
        } else {
            ret = Vector(angleOffset, distance);
        }
    } else if ((normalValue >= angles.at(1).getValue()) && (normalValue < angles.at(2).getValue())) {
        if (recAngleValue > M_PI_2) {
            ret = Vector(angleOffset, -distance);
        } else if (recAngleValue > 0) {
            ret = Vector((angleOffset * -1), distance);
        } else if (recAngleValue >= -M_PI_2) {
            ret = Vector(angleOffset, distance);
        } else {
            ret = Vector((angleOffset * -1), -distance);
        }
    } else if ((normalValue >= angles.at(2).getValue()) && (normalValue < angles.at(3).getValue())) {
        if (recAngleValue > M_PI_2) {
            ret = Vector((angleOffset * -1), distance);
        } else if (recAngleValue > 0) {
            ret = Vector(angleOffset, distance);
        } else if (recAngleValue >= -M_PI_2) {
            ret = Vector((angleOffset * -1), -distance);
        } else {
            ret = Vector(angleOffset, -distance);
        }
    } else {
        if (recAngleValue > M_PI_2) {
            ret = Vector(angleOffset, distance);
        } else if (recAngleValue > 0) {
            ret = Vector((angleOffset * -1), -distance);
        } else if (recAngleValue >= -M_PI_2) {
            ret = Vector(angleOffset, -distance);
        } else {
            ret = Vector((angleOffset * -1), distance);
        }
    }
    return ret.rotate(recAngle);
}

MapCatch MapHitboxRectangle::getCatchFront(const MapEdge * p_edge) const {
    Coordinate center = p_edge->getVertexFront()->getCoor();
    Vector offset = getOffset(p_edge);
    return MapCatch(center + offset, Line(center, center + (offset * 2)), (MapElement*)p_edge, (MapElement*)p_edge->getVertexFront());
}

MapCatch MapHitboxRectangle::getCatchBack(const MapEdge * p_edge) const {
    Coordinate center = p_edge->getVertexBack()->getCoor();
    Vector offset = getOffset(p_edge);
    return MapCatch(center + offset, Line(center, center + (offset * 2)), (MapElement*)p_edge, (MapElement*)p_edge->getVertexBack());
}

MapHitboxRectangle * MapHitboxRectangle::deepCopy() const {
    return new MapHitboxRectangle(*this);
}

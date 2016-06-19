//
//  MapHitboxRectangle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/22/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "MapHitboxRectangle.hpp"

#include "Random.hpp"
#include "Queue.hpp"
#include "SortedList.hpp"

using namespace nautical;

MapHitboxRectangle::MapHitboxRectangle(Rectangle rec) : rec(rec) {
    center = rec.getCenter();
}

MapHitboxRectangle::~MapHitboxRectangle() { }

MapHitboxRectangle & MapHitboxRectangle::move(Vector vec) {
    MapHitbox::move(vec);
    rec.move(vec);
    return *this;
}

Shape * MapHitboxRectangle::getShape() const {
    return new Rectangle(rec);
}

Rectangle MapHitboxRectangle::getRectangle() const {
    return rec;
}

MapHitboxRectangle & MapHitboxRectangle::setRectangle(Rectangle rec) {
    this->rec = rec;
    center = rec.getCenter();
    return *this;
}

bool MapHitboxRectangle::adjustVector(const MapVertex * p_vertex, Vector * p_vector) const {
    Angle recAngle = rec.getAngle();
    double recAngleValue = recAngle.getValue();
    
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

Rectangle * MapHitboxRectangle::createBumper(const MapVertex * p_vertex) const {
    return new Rectangle(p_vertex->getCoor(), rec.getWidth(), rec.getHeight(), rec.getAngle());
}

LinkedList<MapCatch> MapHitboxRectangle::findCatches(const MapVertex * p_vertex, const Map * p_map) const {
    LinkedList<MapCatch> catches;
    if (p_vertex->getEdgeFront())
        catches.insert(getCatchBack(p_vertex->getEdgeFront()));
    if (p_vertex->getEdgeBack())
        catches.insert(getCatchFront(p_vertex->getEdgeBack()));
    return catches;
}

bool MapHitboxRectangle::adjustVector(const MapEdge * p_edge, Vector * p_vector) const {
    return p_vector->subtractAngle(p_edge->getNormal());
}

LineShape * MapHitboxRectangle::createBumper(const MapEdge * p_edge) const {
    LineShape * bumper = new LineShape(p_edge->getLine());
    bumper->move(getOffset(p_edge));
    return bumper;
}

LinkedList<MapCatch> MapHitboxRectangle::findCatches(const MapEdge * p_edge, const Map * p_map) const {
    LinkedList<MapCatch> catches;
    for (Iterator<MapEdge*> * iterator = p_map->getEdgesListIterator(fmin(p_edge->getVertexBack()->getCoor().getX(), p_edge->getVertexFront()->getCoor().getX()) - rec.getWidth(), fmax(p_edge->getVertexBack()->getCoor().getX(), p_edge->getVertexFront()->getCoor().getX()) + rec.getWidth()); !iterator->complete(); iterator->next()) {
        MapEdge * p_edge2 = iterator->current();
        if (p_edge2 == p_edge)
            continue;
        
        Queue<Coordinate> collisions;
        LineShape * p_lineShape = createBumper(p_edge);
        LineShape * p_lineShape2 = createBumper(p_edge2);
        if (p_lineShape->intersectsShape(p_lineShape2, &collisions)) {
            Coordinate collision;
            if (collisions.pop(&collision))
                catches.insert(MapCatch(collision, p_lineShape2->getLine(), (MapElement*)p_edge, (MapElement*)p_edge2));
            else
                Logger::writeLog(ERROR_MESSAGE, "MapHitboxRectangle::findCatches(MapEdge*): collisions is empty");
        }
        delete p_lineShape;
        delete p_lineShape2;
    }
    catches.insert(getCatchBack(p_edge));
    catches.insert(getCatchFront(p_edge));
    return catches;
}

Vector MapHitboxRectangle::getOffset(const MapEdge * p_edge) const {
    double distance = rec.getDiagonalLength() / 2;
    Angle angleOffset(rec.getWidth(), rec.getHeight());
    Angle recAngle = rec.getAngle();
    double recAngleValue = recAngle.getValue();
    double normalValue = p_edge->getNormal().getValue();
    
    Angle angle1;
    Angle angle2;
    Angle angle3;
    Angle angle4;
    SortedList<Angle> angles(&Angle::weighAngle);
    angles.insert(Angle(M_PI) + recAngle);
    angles.insert(Angle(M_PI_2) + recAngle);
    angles.insert(recAngle);
    angles.insert(Angle(-M_PI_2) + recAngle);
    angles.getElementByIndex(0, &angle1);
    angles.getElementByIndex(1, &angle2);
    angles.getElementByIndex(2, &angle3);
    angles.getElementByIndex(3, &angle4);
    
    Vector ret;
    if ((normalValue >= angle1.getValue()) && (normalValue < angle2.getValue())) {
        if (recAngleValue > M_PI_2) {
            ret = Vector((angleOffset * -1), -distance);
        } else if (recAngleValue > 0) {
            ret = Vector(angleOffset, -distance);
        } else if (recAngleValue >= -M_PI_2) {
            ret = Vector((angleOffset * -1), distance);
        } else {
            ret = Vector(angleOffset, distance);
        }
    } else if ((normalValue >= angle2.getValue()) && (normalValue < angle3.getValue())) {
        if (recAngleValue > M_PI_2) {
            ret = Vector(angleOffset, -distance);
        } else if (recAngleValue > 0) {
            ret = Vector((angleOffset * -1), distance);
        } else if (recAngleValue >= -M_PI_2) {
            ret = Vector(angleOffset, distance);
        } else {
            ret = Vector((angleOffset * -1), -distance);
        }
    } else if ((normalValue >= angle3.getValue()) && (normalValue < angle4.getValue())) {
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

MapHitboxRectangle * MapHitboxRectangle::copyPtr() const {
    return new MapHitboxRectangle(*this);
}

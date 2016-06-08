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

#define DRAW_NORMALS false

using namespace nautical;

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

Vector MapEdge::getOffsetRectangle(Rectangle rectangle) const {
    double distance = rectangle.getDiagonalLength() / 2;
    Angle angleOffset(rectangle.getWidth(), rectangle.getHeight());
    Angle recAngle = rectangle.getAngle();
    double recAngleValue = recAngle.getValue();
    double normalValue = normal.getValue();
    
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
    
    /*if (normal.getValue() > M_PI_2) {
     return Vector((angleOffset * -1) + M_PI, distance);
     } else if (normal.getValue() > 0) {
     return Vector(angleOffset, distance);
     } else if (normal.getValue() > -M_PI_2) {
     return Vector((angleOffset * -1) + M_PI, -distance);
     } else {
     return Vector(angleOffset, -distance);
     }*/
}

bool MapEdge::adjustVectorRectangle(Rectangle rectangle, Vector * p_vector, ObjectPos pos) const {
    return p_vector->subtractAngle(normal);
}

LineShape * MapEdge::generateBumperRectangle(Rectangle rectangle) const {
    LineShape * bumper = new LineShape(line);
    bumper->move(getOffsetRectangle(rectangle));
    return bumper;
}

LineShape * MapEdge::generateBumperCircle(Circle circle) const {
    LineShape * bumper = new LineShape(line);
    Vector bumperOffset(normal, circle.getRadius());
    bumper->move(bumperOffset);
    return bumper;
}

MapCatch MapEdge::getCatchFrontRectangle(Rectangle rectangle) const {
    Coordinate center = p_vertexFront->getCoor();
    Vector offset = getOffsetRectangle(rectangle);
    return MapCatch(center + offset, Line(center, center + (offset * 2)), (MapElement*)this, (MapElement*)p_vertexFront);
}

MapCatch MapEdge::getCatchBackRectangle(Rectangle rectangle) const {
    Coordinate center = p_vertexBack->getCoor();
    Vector offset = getOffsetRectangle(rectangle);
    return MapCatch(center + offset, Line(center, center + (offset * 2)), (MapElement*)this, (MapElement*)p_vertexBack);
}

LinkedList<MapCatch> MapEdge::findCatches(Rectangle rectangle, const Map * p_map) const {
    LinkedList<MapCatch> catches;
    for (Iterator<MapEdge*> * iterator = p_map->getEdgesListIterator(fmin(p_vertexBack->getCoor().getX(), p_vertexFront->getCoor().getX()) - rectangle.getWidth(), fmax(p_vertexBack->getCoor().getX(), p_vertexFront->getCoor().getX()) + rectangle.getWidth()); !iterator->complete(); iterator->next()) {
        MapEdge * p_edge = iterator->current();
        if (p_edge == this)
            continue;
        
        Queue<Coordinate> collisions;
        LineShape * p_lineShape = generateBumperRectangle(rectangle);
        LineShape * p_lineShape2 = p_edge->generateBumperRectangle(rectangle);
        if (p_lineShape->intersectsShape(p_lineShape2, &collisions)) {
            Coordinate collision;
            if (collisions.pop(&collision))
                catches.insert(MapCatch(collision, p_lineShape2->getLine(), (MapElement*)this, (MapElement*)p_edge));
            else
                Logger::writeLog(ERROR_MESSAGE, "World::generatePath(): collisions is empty");
        }
        delete p_lineShape;
        delete p_lineShape2;
    }
    catches.insert(getCatchBackRectangle(rectangle));
    catches.insert(getCatchFrontRectangle(rectangle));
    return catches;
}

void MapEdge::draw() const {
    GraphicsManager::drawLine(line, getColor());
    
    if (DRAW_NORMALS && DEBUG_MODE) {
        Coordinate origin = p_vertexBack->getCoor() + (Vector(p_vertexBack->getCoor(), p_vertexFront->getCoor()) / 2);
        Vector(normal, 5).setOrigin(origin).Drawable::draw(getColor().setA(127));
    }
}

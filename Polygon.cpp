//
//  Polygon.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 10/1/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Polygon.hpp"

using namespace nautical;

Polygon::Polygon(Queue<Coordinate> * p_coors, bool checkForConcave) {
    appendTag(POLYGON_TAG);
    init(p_coors, checkForConcave);
}

Polygon::~Polygon() { }

void Polygon::init(Queue<Coordinate> * p_coors, bool checkForConcave) {
    if (p_coors == nullptr) {
        Logger::writeLog(ERROR_MESSAGE, "Polygon::Polygon(): attempted to create polygon with null list of Coordinates");
        return;
    }
    
    if (p_coors->size() < 3) {
        Logger::writeLog(ERROR_MESSAGE, "Polygon::Polygon(): attempted to create polygon with less than 3 vertices");
        return;
    }
    
    //create polygon
    Iterator<Coordinate> * iterator = p_coors->createIterator();
    Coordinate firstCoor = iterator->current();
    lowerBoundX.update(firstCoor.getX());
    lowerBoundY.update(firstCoor.getY());
    upperBoundX.update(firstCoor.getX());
    upperBoundY.update(firstCoor.getY());
    Coordinate prevCoor = firstCoor;
    vertices.insert(firstCoor);
    for (iterator->next(); !iterator->complete(); iterator->next()) {
        Coordinate coor = iterator->current();
        lowerBoundX.update(coor.getX());
        lowerBoundY.update(coor.getY());
        upperBoundX.update(coor.getX());
        upperBoundY.update(coor.getY());
        vertices.insert(coor);
        edges.insert(Line(prevCoor, coor));
        prevCoor = coor;
    }
    edges.insert(Line(prevCoor, firstCoor));
    
    if (checkForConcave) {
        //create convex outline
        convexVertices = vertices;
        bool outlineConvex;
        do {
            outlineConvex = true;
            Coordinate coorToRemove;
            Queue<Coordinate> subtractedCoors; //to be used to remove a triangle
            
            Iterator<Coordinate> * iterator = convexVertices.createIterator();
            Coordinate firstCoor = iterator->current();
            iterator->next();
            Coordinate prevCoor = iterator->current();
            Line firstLine(firstCoor, prevCoor);
            Line prevLine = firstLine;
            for (iterator->next(); !iterator->complete(); iterator->next()) {
                Coordinate coor = iterator->current();
                Line line(prevCoor, coor);
                
                if (!prevLine.isOnOrBelow(coor)) { //interior angle
                    outlineConvex = false;
                    coorToRemove = prevCoor;
                    subtractedCoors.insert(prevLine.getCoor1());
                    subtractedCoors.insert(coor);
                    subtractedCoors.insert(prevCoor);
                    break;
                }
                
                prevCoor = coor;
                prevLine = line;
            }
            
            if (outlineConvex) {
                //check angle created with prevLine and line from first and last vertices
                Line line(prevCoor, firstCoor);
                if (!prevLine.isOnOrBelow(firstCoor)) { //interior angle
                    outlineConvex = false;
                    coorToRemove = prevCoor;
                    subtractedCoors.insert(prevLine.getCoor1());
                    subtractedCoors.insert(firstCoor);
                    subtractedCoors.insert(prevCoor);
                }
                //check angle create with last and first line
                if (!line.isOnOrBelow(firstLine.getCoor2())) { //interior angle
                    outlineConvex = false;
                    coorToRemove = firstCoor;
                    subtractedCoors.insert(prevCoor);
                    subtractedCoors.insert(firstLine.getCoor2());
                    subtractedCoors.insert(firstCoor);
                }
            }
            
            if (!outlineConvex) {
                convex = false;
                if (!convexVertices.remove(coorToRemove))
                    Logger::writeLog(ERROR_MESSAGE, "Polygon::Polygon(): attempted to remove non-existant coordinate from convexVertices");
                Triangle triangle;
                if (subtractedCoors.pop(&triangle.coor1) && subtractedCoors.pop(&triangle.coor2) && subtractedCoors.pop(&triangle.coor3))
                    subtractedTrianglesStruct.insert(triangle);
                else
                    Logger::writeLog(ERROR_MESSAGE, "Polygon::init(): subtractedCoors does not contain 3 coordinates");
            }
        } while (!outlineConvex);
        
        if (!convex) {
            //fill convexEdges
            iterator = convexVertices.createIterator();
            firstCoor = iterator->current();
            prevCoor = firstCoor;
            for (iterator->next(); !iterator->complete(); iterator->next()) {
                Coordinate coor = iterator->current();
                convexEdges.insert(Line(prevCoor, coor));
                prevCoor = coor;
            }
            convexEdges.insert(Line(prevCoor, firstCoor));
        } else {
            convexEdges = edges;
        }
    } else {
        convex = true;
        convexVertices = vertices;
        convexEdges = edges;
    }
}

bool Polygon::isConvex() const {
    return convex;
}

Iterator<Polygon> * Polygon::getSubtractedTrianglesIterator() const {
    if (!subtractedTrianglesSet) {
        for (Iterator<Triangle> * iterator = subtractedTrianglesStruct.createIterator(); !iterator->complete(); iterator->next()) {
            Triangle triangle = iterator->current();
            Queue<Coordinate> coors;
            coors.insert(triangle.coor1);
            coors.insert(triangle.coor2);
            coors.insert(triangle.coor3);
            subtractedTriangles.insert(Polygon(&coors));
        }
        subtractedTrianglesSet = true;
    }
    return subtractedTriangles.createIterator();
}

double Polygon::getLowerBoundX() const {
    return lowerBoundX.getValue();
}

double Polygon::getLowerBoundY() const {
    return lowerBoundY.getValue();
}

double Polygon::getUpperBoundX() const {
    return upperBoundX.getValue();
}

double Polygon::getUpperBoundY() const {
    return upperBoundY.getValue();
}

bool Polygon::contains(Coordinate coor) const {
    for (Iterator<Line> * iterator = convexEdges.createIterator(); !iterator->complete(); iterator->next()) {
        if (!iterator->current().isOnOrBelow(coor))
            return false;
    }
    
    if (convex) {
        return true;
    } else {
        for (Iterator<Polygon> * iterator = getSubtractedTrianglesIterator(); !iterator->complete(); iterator->next()) {
            if (iterator->current().contains(coor))
                return false;
        }
        return true;
    }
}

bool Polygon::intersectsLine(Line line, Queue<Coordinate> * p_intersections) const {
    LinkedList<Coordinate> intersections(false);
    for (Iterator<Line> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        Coordinate intersection;
        if (iterator->current().intersectsLine(line, &intersection))
            intersections.insert(intersection);
    }
    if (intersections.size() == 0)
        return false;
    
    Coordinate origin = line.getCoor1();
    while (intersections.size() > 0) { //TODO there HAS to be a more efficient way to do this
        Iterator<Coordinate> * iterator = intersections.createIterator();
        Coordinate closestCoor = iterator->current();
        MinValue distance(findDistance(origin, closestCoor));
        for (iterator->next(); !iterator->complete(); iterator->next()) {
            Coordinate coor = iterator->current();
            if (distance.update(findDistance(origin, coor)))
                closestCoor = coor;
        }
        p_intersections->insert(closestCoor);
        intersections.remove(closestCoor);
    }
    return true;
}

bool Polygon::intersectsShape(const Shape * p_shape, Queue<Coordinate> * p_intersections) const {
    bool intersects = false;
    for (Iterator<Line> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        intersects |= p_shape->intersectsLine(iterator->current(), p_intersections);
    }
    return intersects;
}

Polygon & Polygon::move(Vector vector) {
    Queue<Coordinate> newVertices;
    for (Iterator<Coordinate> * iterator = vertices.createIterator(); !iterator->complete(); iterator->next()) {
        newVertices.insert(iterator->current() + vector);
    }
    vertices = newVertices;
    Queue<Line> newEdges;
    for (Iterator<Line> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        newEdges.insert(iterator->current() + vector);
    }
    edges = newEdges;
    
    if (!convex) {
        Queue<Coordinate> newConvexVertices;
        for (Iterator<Coordinate> * iterator = convexVertices.createIterator(); !iterator->complete(); iterator->next()) {
            newConvexVertices.insert(iterator->current() + vector);
        }
        convexVertices = newConvexVertices;
        Queue<Line> newConvexEdges;
        for (Iterator<Line> * iterator = convexEdges.createIterator(); !iterator->complete(); iterator->next()) {
            newConvexEdges.insert(iterator->current() + vector);
        }
        convexEdges = newConvexEdges;
    } else {
        convexVertices = vertices;
        convexEdges = edges;
    }
    
    return *this;
}

Polygon & Polygon::rotateAboutCoordinate(Angle angle, Coordinate coor) {
    Queue<Coordinate> newVertices;
    for (Iterator<Coordinate> * iterator = vertices.createIterator(); !iterator->complete(); iterator->next()) {
        newVertices.insert(iterator->current().rotateAboutCoordinate(angle, coor));
    }
    vertices = newVertices;
    Queue<Line> newEdges;
    for (Iterator<Line> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        newEdges.insert(iterator->current().rotateAboutCoordinate(angle, coor));
    }
    edges = newEdges;
    
    if (!convex) {
        Queue<Coordinate> newConvexVertices;
        for (Iterator<Coordinate> * iterator = convexVertices.createIterator(); !iterator->complete(); iterator->next()) {
            newConvexVertices.insert(iterator->current().rotateAboutCoordinate(angle, coor));
        }
        convexVertices = newConvexVertices;
        Queue<Line> newConvexEdges;
        for (Iterator<Line> * iterator = convexEdges.createIterator(); !iterator->complete(); iterator->next()) {
            newConvexEdges.insert(iterator->current().rotateAboutCoordinate(angle, coor));
        }
        convexEdges = newConvexEdges;
    } else {
        convexVertices = vertices;
        convexEdges = edges;
    }
    
    return *this;
}

void Polygon::draw() const {
    for (Iterator<Line> * iterator = edges.createIterator(); !iterator->complete(); iterator->next()) {
        GraphicsManager::drawLine(iterator->current(), getColor());
    }
    if (DEBUG_MODE) {
        for (Iterator<Line> * iterator = convexEdges.createIterator(); !iterator->complete(); iterator->next()) {
            GraphicsManager::drawLine(iterator->current(), getColor().setA(127));
        }
    }
}

bool Polygon::operator==(const Shape & shape) const {
    if (getTag() == shape.getTag()) {
        Polygon poly = (Polygon&)shape;
        if (vertices.size() != poly.vertices.size())
            return false;
        for (Iterator<Coordinate> * iterator = vertices.createIterator(), * otherIterator = poly.vertices.createIterator(); (!iterator->complete() && !  otherIterator->complete()); iterator->next(), otherIterator->next()) {
            if (!(iterator->current() == otherIterator->current()))
                return false;
        }
    }
    return true;
}

Polygon * Polygon::copyPtr() const {
    return new Polygon(*this);
}

bool Polygon::Triangle::operator==(const Triangle & triangle) {
    return ((coor1 == triangle.coor1) && (coor2 == triangle.coor2) && (coor3 == triangle.coor3));
}

//
//  Polygon.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 10/1/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Polygon.hpp"

using namespace nautical;

Polygon::Polygon(std::vector<Coordinate> * p_coors, bool checkForConcave) {
    appendTag(POLYGON_TAG);
    init(p_coors, checkForConcave);
}

Polygon::~Polygon() { }

void Polygon::init(std::vector<Coordinate> * p_coors, bool checkForConcave) {
    if (p_coors == nullptr) {
        Logger::writeLog(ERROR_MESSAGE, "Polygon::Polygon(): attempted to create polygon with null list of Coordinates");
        return;
    }
    
    if (p_coors->size() < 3) {
        Logger::writeLog(ERROR_MESSAGE, "Polygon::Polygon(): attempted to create polygon with less than 3 vertices");
        return;
    }
    
    //create polygon
    std::vector<Coordinate>::iterator it = p_coors->begin();
    Coordinate firstCoor = *it;
    lowerBoundX.update(firstCoor.getX());
    lowerBoundY.update(firstCoor.getY());
    upperBoundX.update(firstCoor.getX());
    upperBoundY.update(firstCoor.getY());
    Coordinate prevCoor = firstCoor;
    vertices.push_back(firstCoor);
    for (it++; it != p_coors->end(); it++) {
        lowerBoundX.update(it->getX());
        lowerBoundY.update(it->getY());
        upperBoundX.update(it->getX());
        upperBoundY.update(it->getY());
        vertices.push_back(*it);
        edges.push_back(Line(prevCoor, *it));
        prevCoor = *it;
    }
    edges.push_back(Line(prevCoor, firstCoor));
    
    if (checkForConcave) {
        //create convex outline
        convexVertices = vertices;
        bool outlineConvex;
        do {
            outlineConvex = true;
            Coordinate coorToRemove;
            std::vector<Coordinate> subtractedCoors; //to be used to remove a triangle
            
            std::vector<Coordinate>::iterator it = convexVertices.begin();
            Coordinate firstCoor = *it;
            Coordinate prevCoor = *(++it);
            Line firstLine(firstCoor, prevCoor);
            Line prevLine = firstLine;
            for (it++; it != convexVertices.end(); it++) {
                Line line(prevCoor, *it);
                if (!prevLine.isOnOrBelow(*it)) { //interior angle
                    outlineConvex = false;
                    coorToRemove = prevCoor;
                    subtractedCoors.push_back(prevLine.getCoor1());
                    subtractedCoors.push_back(*it);
                    subtractedCoors.push_back(prevCoor);
                    break;
                }
                prevCoor = *it;
                prevLine = line;
            }
            
            if (outlineConvex) {
                //check angle created with prevLine and line from first and last vertices
                Line line(prevCoor, firstCoor);
                if (!prevLine.isOnOrBelow(firstCoor)) { //interior angle
                    outlineConvex = false;
                    coorToRemove = prevCoor;
                    subtractedCoors.push_back(prevLine.getCoor1());
                    subtractedCoors.push_back(firstCoor);
                    subtractedCoors.push_back(prevCoor);
                }
                //check angle create with last and first line
                if (!line.isOnOrBelow(firstLine.getCoor2())) { //interior angle
                    outlineConvex = false;
                    coorToRemove = firstCoor;
                    subtractedCoors.push_back(prevCoor);
                    subtractedCoors.push_back(firstLine.getCoor2());
                    subtractedCoors.push_back(firstCoor);
                }
            }
            
            if (!outlineConvex) {
                convex = false;
                vector_helpers::removeElementByValue(convexVertices, coorToRemove);
                if (subtractedCoors.size() >= 3) {
                    Triangle triangle;
                    triangle.coor1 = subtractedCoors.at(0);
                    triangle.coor2 = subtractedCoors.at(1);
                    triangle.coor3 = subtractedCoors.at(2);
                } else {
                    Logger::writeLog(ERROR_MESSAGE, "Polygon::init(): subtractedCoors does not contain 3 coordinates");
                }            }
        } while (!outlineConvex);
        
        if (!convex) {
            //fill convexEdges
            it = convexVertices.begin();
            firstCoor = prevCoor = *it;
            for (it++; it != convexVertices.end(); it++) {
                convexEdges.push_back(Line(prevCoor, *it));
                prevCoor = *it;
            }
            convexEdges.push_back(Line(prevCoor, firstCoor));
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

std::vector<Polygon>::iterator Polygon::getSubtractedTrianglesIterator() const {
    if (!subtractedTrianglesSet) {
        for (std::vector<Triangle>::const_iterator it = subtractedTrianglesStruct.begin(); it != subtractedTrianglesStruct.end(); it++) {
            Triangle triangle = *it;
            std::vector<Coordinate> coors;
            coors.push_back(triangle.coor1);
            coors.push_back(triangle.coor2);
            coors.push_back(triangle.coor3);
            subtractedTriangles.push_back(Polygon(&coors));
        }
        subtractedTrianglesSet = true;
    }
    return subtractedTriangles.begin();
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
    for (std::vector<Line>::const_iterator it = convexEdges.begin(); it != convexEdges.end(); it++) {
        if (!(it->isOnOrBelow(coor)))
            return false;
    }
    
    if (convex) {
        return true;
    } else {
        for (std::vector<Polygon>::iterator it = getSubtractedTrianglesIterator(); it != subtractedTriangles.end(); it++) {
            if (it->contains(coor))
                return false;
        }
        return true;
    }
}

bool Polygon::intersectsLine(Line line, std::vector<Coordinate> * p_intersections) const {
    std::vector<Coordinate> intersections;
    for (std::vector<Line>::const_iterator it = edges.begin(); it != edges.end(); it++) {
        Coordinate intersection;
        if (it->intersectsLine(line, &intersection))
            intersections.push_back(intersection);
    }
    if (intersections.size() == 0)
        return false;
    
    if (p_intersections) {
        Coordinate origin = line.getCoor1();
        while (intersections.size() > 0) { //TODO there HAS to be a more efficient way to do this
            std::vector<Coordinate>::iterator it = intersections.begin();
            Coordinate closestCoor = *it;
            MinValue distance(findDistance(origin, closestCoor));
            for (it++; it != intersections.end(); it++) {
                if (distance.update(findDistance(origin, *it)))
                    closestCoor = *it;
            }
            p_intersections->push_back(closestCoor);
            vector_helpers::removeElementByValue(intersections, closestCoor);
        }
    }
    return true;
}

bool Polygon::intersectsShape(const Shape * p_shape, std::vector<Coordinate> * p_intersections) const {
    bool intersects = false;
    for (std::vector<Line>::const_iterator it = edges.begin(); it != edges.end(); it++) {
        intersects |= p_shape->intersectsLine(*it, p_intersections);
    }
    return intersects;
}

Polygon & Polygon::move(Vector vector) {
    std::vector<Coordinate> newVertices;
    for (std::vector<Coordinate>::iterator it = vertices.begin(); it != vertices.end(); it++) {
        newVertices.push_back(*it + vector);
    }
    vertices = newVertices;
    std::vector<Line> newEdges;
    for (std::vector<Line>::iterator it = edges.begin(); it != edges.end(); it++) {
        newEdges.push_back(*it + vector);
    }
    edges = newEdges;
    
    if (!convex) {
        std::vector<Coordinate> newConvexVertices;
        for (std::vector<Coordinate>::iterator it = convexVertices.begin(); it != convexVertices.end(); it++) {
            newConvexVertices.push_back(*it + vector);
        }
        convexVertices = newConvexVertices;
        std::vector<Line> newConvexEdges;
        for (std::vector<Line>::iterator it = convexEdges.begin(); it != convexEdges.end(); it++) {
            newConvexEdges.push_back(*it + vector);
        }
        convexEdges = newConvexEdges;
    } else {
        convexVertices = vertices;
        convexEdges = edges;
    }
    
    return *this;
}

Polygon & Polygon::rotateAboutCoordinate(Coordinate coor, Angle angle) {
    std::vector<Coordinate> newVertices;
    for (std::vector<Coordinate>::iterator it = vertices.begin(); it != vertices.end(); it++) {
        newVertices.push_back(it->rotateAboutCoordinate(coor, angle));
    }
    vertices = newVertices;
    std::vector<Line> newEdges;
    for (std::vector<Line>::iterator it = edges.begin(); it != edges.end(); it++) {
        newEdges.push_back(it->rotateAboutCoordinate(coor, angle));
    }
    edges = newEdges;
    
    if (!convex) {
        std::vector<Coordinate> newConvexVertices;
        for (std::vector<Coordinate>::iterator it = convexVertices.begin(); it != convexVertices.end(); it++) {
            newConvexVertices.push_back(it->rotateAboutCoordinate(coor, angle));
        }
        convexVertices = newConvexVertices;
        std::vector<Line> newConvexEdges;
        for (std::vector<Line>::iterator it = convexEdges.begin(); it != convexEdges.end(); it++) {
            newConvexEdges.push_back(it->rotateAboutCoordinate(coor, angle));
        }
        convexEdges = newConvexEdges;
    } else {
        convexVertices = vertices;
        convexEdges = edges;
    }
    
    return *this;
}

void Polygon::draw() const {
    for (std::vector<Line>::const_iterator it = edges.begin(); it != edges.end(); it++) {
        GraphicsManager::drawLine(*it, getColor());
    }
    if (DEBUG_MODE) {
        for (std::vector<Line>::const_iterator it = convexEdges.begin(); it != convexEdges.end(); it++) {
            GraphicsManager::drawLine(*it, getColor().setA(127));
        }
    }
}

bool Polygon::operator==(const Shape & shape) const {
    if (getTag() == shape.getTag()) {
        Polygon poly = (Polygon&)shape;
        if (vertices.size() != poly.vertices.size())
            return false;
        for (std::vector<Coordinate>::const_iterator it = vertices.begin(), otherIt = poly.vertices.begin(); it != vertices.end(); it++, otherIt++) {
            if (!(*it == *otherIt))
                return false;
        }
    }
    return true;
}

Polygon * Polygon::copyPtr_() const {
    return new Polygon(*this);
}

bool Polygon::Triangle::operator==(const Triangle & triangle) {
    return ((coor1 == triangle.coor1) && (coor2 == triangle.coor2) && (coor3 == triangle.coor3));
}

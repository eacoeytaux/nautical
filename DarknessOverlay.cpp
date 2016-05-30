//
//  DarknessOverlay.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/23/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "DarknessOverlay.hpp"

#include "Stack.hpp"
#include "SortedList.hpp"
#include "Shape.hpp"

#define DEFAULT_COLOR BLACK

using namespace nautical;

DarknessOverlay::DarknessOverlay() {
    setColor(DEFAULT_COLOR);
    for (int i = 0; i < DARKNESS_LAYERS; i++) {
        for (Iterator<Shape*> * iterator = subtractedShapes[i].createIterator(); !iterator->complete(); iterator->next()) {
            lowerBoundX.update(iterator->current()->getLowerBoundX());
        }
    }
}

DarknessOverlay::DarknessOverlay(const DarknessOverlay & overlay) {
    setColor(DEFAULT_COLOR);
    lowerBoundX = overlay.lowerBoundX;
    for (int i = 0; i < DARKNESS_LAYERS; i++) {
        for (Iterator<Shape*> * iterator = overlay.subtractedShapes[i].createIterator(); !iterator->complete(); iterator->next()) {
            subtractedShapes[i].insert(iterator->current()->copyPtr());
        }
    }
}

DarknessOverlay::~DarknessOverlay() {
    for (int i = 0; i < DARKNESS_LAYERS; i++) {
        for (Iterator<Shape*> * iterator = subtractedShapes[i].createIterator(); !iterator->complete(); iterator->next()) {
            delete iterator->current();
        }
    }
}

float DarknessOverlay::getPercentage() const {
    return percentage;
}

DarknessOverlay & DarknessOverlay::setPercentage(float percentage) {
    this->percentage = percentage;
    return *this;
}

DarknessOverlay & DarknessOverlay::addShape(Shape * p_shape, int layer) {
    //TODO layer bounds checking
    subtractedShapes[layer].insert(p_shape);
    lowerBoundX.update(p_shape->getLowerBoundX());
    upperBoundY.update(p_shape->getLowerBoundY());
    lowerBoundX.update(p_shape->getUpperBoundX());
    upperBoundY.update(p_shape->getUpperBoundY());
    return *this;
}

DarknessOverlay & DarknessOverlay::clearShapes() {
    for (int i = 0; i < DARKNESS_LAYERS; i++) {
        for (Iterator<Shape*> * iterator = subtractedShapes[i].createIterator(); !iterator->complete(); iterator->next()) {
            delete iterator->current();
        }
        subtractedShapes[i].clear();
    }
    lowerBoundX.reset();
    lowerBoundY.reset();
    upperBoundX.reset();
    upperBoundY.reset();
    return *this;
}

void DarknessOverlay::draw() const { //TODO optimize (only needs to check between yLowerBound and yUpperBound)
    for (int i = 0; i < DARKNESS_LAYERS; i++) {
        int screenWidth = GraphicsManager::getScreenWidth();
        int screenHeight = GraphicsManager::getScreenHeight();
        int screenLowerBoundX = fmin(0, GraphicsManager::worldToScreenX(lowerBoundX.getValue()));
        int screenUpperBoundX = fmax(GraphicsManager::worldToScreenX(upperBoundX.getValue()), screenWidth);
        for (int y = 0; y <= screenHeight; y++) {
            double yWorld = GraphicsManager::screenToWorldY(y);
            Line screenLine(GraphicsManager::screenToWorld(Coordinate(screenLowerBoundX, y)).moveX(-1), GraphicsManager::screenToWorld(Coordinate(screenUpperBoundX, y)));
            
            SortedList<Shape::IntersectionLine> lineIntersections(&Shape::IntersectionLine::weighIntersectionLine);
            for (Iterator<Shape*> * iterator = subtractedShapes[i].createIterator(); !iterator->complete(); iterator->next()) {
                Queue<Coordinate> intersections;
                if (iterator->current()->intersectsLine(screenLine, &intersections)) {
                    Shape::IntersectionLine line;
                    Coordinate coor;
                    while (intersections.size() > 0) {
                        if (intersections.pop(&coor)) {
                            line.xIn = coor.getX();
                            if (intersections.pop(&coor))
                                line.xOut = coor.getX();
                        }
                        lineIntersections.insert(line);
                    }
                }
            }
            
            Stack<Line> linesToDraw;
            linesToDraw.insert(screenLine);
            for (Iterator<Shape::IntersectionLine> * iterator = lineIntersections.createIterator(); !iterator->complete(); iterator->next()) {
                Shape::IntersectionLine line = iterator->current();
                Line prevLineToDraw;
                if (linesToDraw.pop(&prevLineToDraw)) {
                    if (line.xIn > prevLineToDraw.getCoor1().getX()) {
                        linesToDraw.insert(Line(prevLineToDraw.getCoor1(), Coordinate(line.xIn, yWorld)));
                        linesToDraw.insert(Line(Coordinate(line.xOut, yWorld), prevLineToDraw.getCoor2()));
                    } else if (line.xIn < prevLineToDraw.getCoor1().getX()) {
                        linesToDraw.insert(Line(Coordinate(fmax(line.xOut, prevLineToDraw.getCoor1().getX()), yWorld), prevLineToDraw.getCoor2()));
                    } else {
                        linesToDraw.insert(prevLineToDraw);
                    }
                } else {
                    Logger::writeLog(ERROR, "DarknessOverlay::draw(): linesToDraw is empty");
                }
            }
            
            for (Iterator<Line> * iterator = linesToDraw.createIterator(); !iterator->complete(); iterator->next()) {
                GraphicsManager::drawLine(iterator->current(), Color(getColor()).setA(fmin(51 * (i + i + 1), 255) * percentage)); //TODO better way to calculate alpha?
            }
        }
    }
}

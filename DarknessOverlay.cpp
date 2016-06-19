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

bool DarknessOverlay::inEffect = false;
float DarknessOverlay::percentage = 1.f;
LinkedList<Shape*> DarknessOverlay::subtractedShapes[DARKNESS_LAYERS];
MinValue DarknessOverlay::lowerBoundX;
MinValue DarknessOverlay::lowerBoundY;
MaxValue DarknessOverlay::upperBoundX;
MaxValue DarknessOverlay::upperBoundY;

bool DarknessOverlay::isInEffect() {
    return inEffect;
}

void DarknessOverlay::setInEffect(bool b) {
    inEffect = b;
}

float DarknessOverlay::getPercentage() {
    return percentage;
}

void DarknessOverlay::setPercentage(float percentage) {
    DarknessOverlay::percentage = percentage;
}

void DarknessOverlay::addShape(Shape * p_shape, int layer) {
    //TODO layer bounds checking
    subtractedShapes[layer].insert(p_shape);
    lowerBoundX.update(p_shape->getLowerBoundX());
    upperBoundY.update(p_shape->getLowerBoundY());
    lowerBoundX.update(p_shape->getUpperBoundX());
    upperBoundY.update(p_shape->getUpperBoundY());
}

void DarknessOverlay::clearShapes() {
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
}

void DarknessOverlay::draw() { //TODO optimize (only needs to check between yLowerBound and yUpperBound)
    if (!inEffect)
        return;
    
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
                    Logger::writeLog(ERROR_MESSAGE, "DarknessOverlay::draw(): linesToDraw is empty");
                }
            }
            
            for (Iterator<Line> * iterator = linesToDraw.createIterator(); !iterator->complete(); iterator->next()) {
                GraphicsManager::drawLine(iterator->current(), Color(BLACK).setA(fmin(51 * (i + i + 1), 255) * percentage)); //TODO better way to calculate alpha?
            }
        }
    }
}

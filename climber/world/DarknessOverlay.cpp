//
//  DarknessOverlay.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/23/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "DarknessOverlay.hpp"

#include "Utility.hpp"
#include "Shape.hpp"

#define DEFAULT_COLOR BLACK

using namespace nautical;

bool DarknessOverlay::inEffect = false;
float DarknessOverlay::percentage = 1.f;
std::vector<std::shared_ptr<Shape>> DarknessOverlay::subtractedShapes[DARKNESS_LAYERS];
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

void DarknessOverlay::addShape(std::shared_ptr<Shape> p_shape, int layer) {
    //TODO layer bounds checking
    subtractedShapes[layer].push_back(p_shape);
    lowerBoundX.update(p_shape->getLowerBoundX());
    upperBoundY.update(p_shape->getLowerBoundY());
    lowerBoundX.update(p_shape->getUpperBoundX());
    upperBoundY.update(p_shape->getUpperBoundY());
}

void DarknessOverlay::clearShapes() {
    for (int i = 0; i < DARKNESS_LAYERS; i++) {
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
        int screenLowerBoundX = (int)fmin(0, GraphicsManager::worldToScreenX(lowerBoundX.getValue()));
        int screenUpperBoundX = (int)fmax(GraphicsManager::worldToScreenX(upperBoundX.getValue()), screenWidth);
        for (int y = 0; y <= screenHeight; y++) {
            double yWorld = GraphicsManager::screenToWorldY(y);
            Line screenLine(GraphicsManager::screenToWorld(Coordinate(screenLowerBoundX, y)), GraphicsManager::screenToWorld(Coordinate(screenUpperBoundX, y)));
            
            std::vector<Shape::IntersectionLine> lineIntersections;
            for (std::vector<std::shared_ptr<Shape>>::iterator it = subtractedShapes[i].begin(); it != subtractedShapes[i].end(); it++) {
                std::vector<Coordinate> intersections;
                if ((*it)->intersectsLine(screenLine, &intersections)) {
                    Shape::IntersectionLine line;
                    while (intersections.size() > 0) {
                        line.xIn = intersections.front().getX();
                        vector_helpers::removeElementByIndex(intersections, 0);
                        if (intersections.size() > 0) {
                            line.xOut = intersections.front().getX();
                            vector_helpers::removeElementByIndex(intersections, 0);
                        } else {
                            line.xOut = screenUpperBoundX;
                        }
                        lineIntersections.push_back(line);
                    }
                }
            }
            
            std::sort(lineIntersections.begin(), lineIntersections.end(), Shape::IntersectionLine::Comp());
            
            std::vector<Line> linesToDraw;
            linesToDraw.push_back(screenLine);
            for (std::vector<Shape::IntersectionLine>::iterator it = lineIntersections.begin(); it != lineIntersections.end(); it++) {
                Line prevLineToDraw;
                if (linesToDraw.size() > 0) {
                    prevLineToDraw = linesToDraw.back();
                    vector_helpers::removeElementByValue(linesToDraw, prevLineToDraw);
                    if (it->xIn > prevLineToDraw.getCoor1().getX()) {
                        linesToDraw.push_back(Line(prevLineToDraw.getCoor1(), Coordinate(it->xIn, yWorld)));
                        linesToDraw.push_back(Line(Coordinate(it->xOut, yWorld), prevLineToDraw.getCoor2()));
                    } else if (it->xIn < prevLineToDraw.getCoor1().getX()) {
                        linesToDraw.push_back(Line(Coordinate(fmax(it->xOut, prevLineToDraw.getCoor1().getX()), yWorld), prevLineToDraw.getCoor2()));
                    } else {
                        linesToDraw.push_back(prevLineToDraw);
                    }
                } else {
                    Logger::writeLog(ERROR, "DarknessOverlay::draw(): linesToDraw is empty");
                }
            }
            
            for (std::vector<Line>::iterator it = linesToDraw.begin(); it != linesToDraw.end(); it++) {
                GraphicsManager::drawLine(*it, Color(BLACK).setA((unsigned char)(fmin(51 * (i + i + 1), 255) * percentage)));
            }
        }
    }
}

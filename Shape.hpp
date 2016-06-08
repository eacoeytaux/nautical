//
//  Shape.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Shape__
#define __Nautical__Shape__

#include "Drawable.hpp" //base class
#include "Tagable.hpp" //base class

#include <string>

#include "Logger.hpp"
#include "Utility.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "SortedList.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"
#include "Vector.hpp"

namespace nautical {
    const std::string SHAPE_TAG = "shape";
    
    class Shape : public Drawable, public Tagable {
    public:
        Shape() { appendTag(SHAPE_TAG); }
        virtual ~Shape() { }
        
        //bounds for box containing shape
        virtual double getLowerBoundX() const = 0;
        virtual double getLowerBoundY() const = 0;
        virtual double getUpperBoundX() const = 0;
        virtual double getUpperBoundY() const = 0;
        
        virtual bool contains(Coordinate coor) const = 0;
        virtual bool intersectsLine(Line line, Queue<Coordinate> * p_intersections = nullptr) const = 0;
        virtual bool intersectsShape(const Shape * p_shape, Queue<Coordinate> * p_intersections = nullptr) const = 0;
        
        virtual Shape & move(Vector vector) = 0;
        virtual Shape & rotateAboutCoordinate(Angle angle, Coordinate coor = Coordinate(0, 0)) = 0;
        
        virtual void draw() const = 0;
        
        struct IntersectionLine {
            double xIn = -INFINITY;
            double xOut = -INFINITY;
            
            bool operator==(const IntersectionLine & line) {
                return ((xIn == line.xIn) && (xOut == line.xOut));
            }
            
            static double weighIntersectionLine(const IntersectionLine * p_line) {
                return p_line->xIn;
            }
        };
        
        virtual void drawFilled(Color color) const {
            setColor(color);
            drawFilled();
        }
        
        virtual void drawFilled() const { //TODO move to .cpp file
            int screenLowerBoundX = (int)GraphicsManager::worldToScreenX(getLowerBoundX()) - 1;
            int screenLowerBoundY = (int)GraphicsManager::worldToScreenY(getLowerBoundY()) + 1;
            int screenUpperBoundX = (int)GraphicsManager::worldToScreenX(getUpperBoundX()) + 1;
            int screenUpperBoundY = (int)GraphicsManager::worldToScreenY(getUpperBoundY()) - 1;
            for (int y = screenUpperBoundY; y <= screenLowerBoundY; y++) { //TODO optimize (does not to check 0 to screenHeight)
                double yWorld = GraphicsManager::screenToWorldY(y);
                Line screenLine(GraphicsManager::screenToWorld(Coordinate(screenLowerBoundX, y)).moveX(-1), GraphicsManager::screenToWorld(Coordinate(screenUpperBoundX, y).moveX(1)));
                
                SortedList<IntersectionLine> lineIntersections(&IntersectionLine::weighIntersectionLine);
                Queue<Coordinate> intersections;
                if (intersectsLine(screenLine, &intersections)) {
                    IntersectionLine line;
                    Coordinate coor;
                    while (intersections.size() > 0) {
                        if (intersections.pop(&coor)) {
                            line.xIn = coor.getX();
                            if (intersections.pop(&coor)) {
                                line.xOut = coor.getX();
                                lineIntersections.insert(line); //only add line if line intersections at least twice
                            } else {
                                Logger::writeLog(ERROR_MESSAGE, "Shape::drawFilled(): horizontal line intersections shape odd number of times");
                            }
                        }
                    }
                }
                
                Stack<Line> linesToDraw;
                for (Iterator<IntersectionLine> * iterator = lineIntersections.createIterator(); !iterator->complete(); iterator->next()) {
                    IntersectionLine line = iterator->current();
                    Line prevLineToDraw;
                    if (linesToDraw.pop(&prevLineToDraw)) {
                        if (line.xIn < prevLineToDraw.getCoor2().getX()) {
                            linesToDraw.insert(Line(prevLineToDraw.getCoor1(), Coordinate(line.xOut, yWorld)));
                        } else {
                            linesToDraw.insert(prevLineToDraw);
                            linesToDraw.insert(Line(Coordinate(line.xIn, yWorld), Coordinate(line.xOut, yWorld)));
                        }
                    } else {
                        linesToDraw.insert(Line(Coordinate(line.xIn, yWorld), Coordinate(line.xOut, yWorld)));
                    }
                }
                
                for (Iterator<Line> * iterator = linesToDraw.createIterator(); !iterator->complete(); iterator->next()) {
                    GraphicsManager::drawLine(iterator->current(), getColor());
                }
            }
        }
        
        virtual Shape * copyPtr() const = 0;
        virtual bool operator==(const Shape & shape) const = 0;
        virtual bool operator!=(const Shape & shape) const { return !((*this) == shape); }
    };
}

#endif /* defined(__Nautical__Shape__) */

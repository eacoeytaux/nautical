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
#include <vector>

#include "Logger.hpp"
#include "Utility.hpp"
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
        virtual bool intersectsLine(Line line, std::vector<Coordinate> * p_intersections = nullptr) const = 0;
        virtual bool intersectsShape(const Shape * p_shape, std::vector<Coordinate> * p_intersections = nullptr) const = 0;
        
        virtual Shape & move(Vector vector) = 0;
        virtual Shape & rotateAboutCoordinate(Coordinate coor, Angle angle) = 0;
        
        virtual void draw() const = 0;
        
        struct IntersectionLine {
            double xIn, xOut;
            
            struct IntersectionLineComp {
                inline bool operator()(IntersectionLine line1, IntersectionLine line2) { return (line1.xIn < line2.xIn); }
            };
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
            for (int y = screenUpperBoundY; y <= screenLowerBoundY; y++) { //TODO optimize (does not need to check 0 to screenHeight)
                double yWorld = GraphicsManager::screenToWorldY(y);
                Line screenLine(GraphicsManager::screenToWorld(Coordinate(screenLowerBoundX, y)).moveX(-1), GraphicsManager::screenToWorld(Coordinate(screenUpperBoundX, y).moveX(1)));
                
                std::vector<IntersectionLine> lineIntersections;
                std::vector<Coordinate> intersections;
                if (intersectsLine(screenLine, &intersections)) {
                    while (intersections.size() > 0) {
                        IntersectionLine line;
                        
                        line.xIn = intersections.front().getX();
                        intersections.pop_back();
                        
                        if (intersections.size() > 0) {
                            line.xOut = intersections.front().getX();
                            intersections.pop_back();
                            
                            lineIntersections.push_back(line);
                        } else {
                            Logger::writeLog(ERROR_MESSAGE, "Shape::drawFilled(): horizontal line intersections shape odd number of times");
                        }
                    }
                }
                
                std::vector<Line> linesToDraw;
                for (std::vector<IntersectionLine>::iterator it = lineIntersections.begin(); it != lineIntersections.end(); it++) {
                    IntersectionLine line = *it;
                    if (linesToDraw.size() > 0) {
                        Line prevLineToDraw = linesToDraw.front();
                        if (line.xIn < prevLineToDraw.getCoor2().getX()) {
                            linesToDraw.push_back(Line(prevLineToDraw.getCoor1(), Coordinate(line.xOut, yWorld)));
                        } else {
                            linesToDraw.push_back(prevLineToDraw);
                            linesToDraw.push_back(Line(Coordinate(line.xIn, yWorld), Coordinate(line.xOut, yWorld)));
                        }
                    } else {
                        linesToDraw.push_back(Line(Coordinate(line.xIn, yWorld), Coordinate(line.xOut, yWorld)));
                    }
                }
                
                for (std::vector<Line>::iterator it = linesToDraw.begin(); it != linesToDraw.end(); it++) {
                    GraphicsManager::drawLine(*it, getColor());
                }
            }
        }
        
        virtual Shape * copyPtr_() const = 0;
        virtual bool operator==(const Shape & shape) const = 0;
        virtual bool operator!=(const Shape & shape) const { return !((*this) == shape); }
    };
}

#endif /* defined(__Nautical__Shape__) */

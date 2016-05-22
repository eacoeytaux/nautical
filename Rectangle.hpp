//
//  Rectangle.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Rectangle__
#define __Nautical__Rectangle__

#include "Shape.hpp" //base class

namespace nautical {
    const std::string RECTANGLE_TAG = "rectangle";
    
    class Rectangle : public Shape {
    public:
        Rectangle(Coordinate center = Coordinate(0, 0), double width = 0, double height = 0, Angle angle = Angle(0));
        virtual ~Rectangle();
        
        Coordinate getCenter() const;
        Rectangle & setCenter(Coordinate center);
        double getWidth() const;
        Rectangle & setWidth(double width);
        double getHeight() const;
        Rectangle & setHeight(double height);
        Angle getAngle() const;
        Rectangle & setAngle(Angle angle); //TODO add rotate
        
        Coordinate getTopRightCoor() const;
        Coordinate getBottomRightCoor() const;
        Coordinate getBottomLeftCoor() const;
        Coordinate getTopLeftCoor() const;
        
        Line getTopLine() const;
        Line getRightLine() const;
        Line getBottomLine() const;
        Line getLeftLine() const;
        
        double getDiagonalLength() const;
        
        bool contains(Coordinate coor) const;
        bool intersectsLine(Line line, Queue<Coordinate> * p_intersections = nullptr) const;
        bool intersectsShape(const Shape * p_shape, Queue<Coordinate> * p_intersections = nullptr) const;
        
        Rectangle & move(Vector vector);
        Rectangle & rotateAboutCoordinate(Angle angle, Coordinate coor = Coordinate(0, 0));
        
        void draw() const;
        
        bool operator==(const Shape & shape) const;
        
    private:
        Coordinate center;
        double width, halfWidth, height, halfHeight;
        Angle angle;
    };
}

#endif /* defined(__Nautical__Rectangle__) */

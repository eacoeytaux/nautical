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
        void setCenter(Coordinate center);
        double getWidth() const;
        void setWidth(double width);
        double getHeight() const;
        void setHeight(double height);
        Angle getAngle() const;
        void setAngle(Angle angle);
        
        Coordinate getTopRightCoor() const;
        Coordinate getBottomRightCoor() const;
        Coordinate getBottomLeftCoor() const;
        Coordinate getTopLeftCoor() const;
        
        Line getTopLine() const;
        Line getRightLine() const;
        Line getBottomLine() const;
        Line getLeftLine() const;
        
        double getDiagonalLength() const;
        
        double getLowerBoundX() const;
        double getLowerBoundY() const;
        double getUpperBoundX() const;
        double getUpperBoundY() const;
        
        bool contains(Coordinate coor) const;
        bool intersectsLine(Line line, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool intersectsShape(const Shape * p_shape, std::vector<Coordinate> * p_intersections = nullptr) const;
        
        void move(Vector vector);
        void rotateAboutCoordinate(Coordinate coor, Angle angle);
        
        void draw() const;
        
        std::shared_ptr<Shape> deepCopy() const;
        bool operator==(const Shape & shape) const;
        
    private:
        Coordinate center;
        double width, halfWidth, height, halfHeight;
        Angle angle;
    };
}

#endif /* defined(__Nautical__Rectangle__) */

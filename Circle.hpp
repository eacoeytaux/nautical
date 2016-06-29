//
//  Circle.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Circle__
#define __Nautical__Circle__

#define CIRCLE_PERCISION 100

#include "Shape.hpp" //base class

namespace nautical {
    const std::string CIRCLE_TAG = "circle";
    
    class Circle : public Shape {
    public:
        Circle(Coordinate center, double radius);
        virtual ~Circle();
        
        Coordinate getCenter() const;
        Circle & setCenter(Coordinate center);
        double getRadius() const;
        Circle & setRadius(double radius);
        
        double convertToRadians(double distance) const;
        double convertToDistance(double radians) const;
        
        double getLowerBoundX() const;
        double getLowerBoundY() const;
        double getUpperBoundX() const;
        double getUpperBoundY() const;
        
        bool contains(Coordinate coor) const;
        bool intersectsLine(Line line, std::vector<Coordinate> * intersections) const;
        bool intersectsShape(const Shape * p_shape, std::vector<Coordinate> * intersections) const;
        
        bool intersectsCircle(const Circle * p_circle, std::vector<Coordinate> * p_intersections = nullptr) const;
        
        Circle & move(Vector vector);
        Circle & rotateAboutCoordinate(Coordinate coor, Angle angle);
        
        void draw() const;
        
        std::shared_ptr<Shape> deepCopy() const;
        bool operator==(const Shape & shape) const;
        
    private:
        Coordinate center;
        double radius;
        
        static bool circleVectorsCalculated;
        static Vector circleVectors[CIRCLE_PERCISION];
    };
}

#endif /* defined(__Nautical__Circle__) */

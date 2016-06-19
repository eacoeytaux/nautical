//
//  Parabola.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Parabola__
#define __Nautical__Parabola__

#include "Queue.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"

namespace nautical {
    class Parabola {
    public:
        Parabola(double x1 = 0, double y1 = 0, double x2 = 0, double y2 = 0, double length = 0);
        Parabola(Coordinate coor1, Coordinate coor2, double length = 0);
        
        Coordinate getVertex() const;
        Coordinate getCoor1() const;
        Coordinate getCoor2() const;
        Coordinate getLeftCoor() const;
        Coordinate getRightCoor() const;
        double getLength() const;
        
        double calculateY(double x) const;
        bool inRangeX(double x) const;
        bool intersects(Line line, Queue<Coordinate> * p_intersections = nullptr) const;
        
        static double calculateArcLength(double a, double b, double c, double x1, double y1, double x2, double y2); //calculates arc length of parabola a(x^2) + bx + c between x1 and x2
        static double calculateArcLength(double b, double h); //calculates arc length from vertex to coordinate given base (dx) and height (dy)
        
    private:
        void init(double x1, double y1, double x2, double y2, double length);
        
        double x1, y1, x2, y2;
        double xLeft, yLeft, xRight, yRight;
        double length, a, b, c;
    };
}

#endif /* defined(__Nautical__Parabola__) */

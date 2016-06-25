//
//  Line.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Line__
#define __Nautical__Line__

#include <vector>

#include "Angle.hpp"
#include "Coordinate.hpp"

namespace nautical {
    class Line {
    public:
        Line(double x1 = 0, double y1 = 0, double x2 = 0, double y2 = 0);
        Line(Coordinate coor1, Coordinate coor2);
        
        bool isVertical() const;
        bool isHorizontal() const;
        Angle getAngle() const;
        Coordinate getCoor1() const;
        void setCoor1(Coordinate coor);
        Coordinate getCoor2() const;
        void setCoor2(Coordinate coor);
        Coordinate getCoorLow() const;
        Coordinate getCoorHigh() const;
        double getLength();
        double getM() const;
        double getB() const;
        double getXAdjustment() const;
        double getYAdjustment() const;
        
        bool inBoxX(double x) const;
        bool inBoxY(double y) const;
        bool inBox(double x, double y) const;
        bool inBox(Coordinate coor) const;
        bool isOnOrBelow(Coordinate coor) const;
        bool intersectsLine(Line line, std::vector<Coordinate> * p_intersections = nullptr) const;
        Coordinate closestCoordinate(Coordinate coor) const;
        
        void rotateAboutCoordinate(Coordinate coor, Angle angle);
        
        bool operator==(const Line & line) const;
        bool operator!=(const Line & line) const;
        
    private:
        bool vertical, horizontal;
        Angle angle;
        double x1, y1, x2, y2;
        double xLow, yLow, xHigh, yHigh; //NOTE: yLow is not necessarily lower than yHigh, as yLow corresponds to xLow and yHigh corresponds to xHigh
        double m, b;
        double xAdjustment, yAdjustment;
        
        void init(double x1, double y1, double x2, double y2);
    };
}

#endif /* defined(__Nautical__Line__) */

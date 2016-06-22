//
//  Line.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Line.hpp"

#include "Utility.hpp"

using namespace nautical;

Line::Line(double x1, double y1, double x2, double y2) {
    init(x1, y1, x2, y2);
}

Line::Line(Coordinate coor1, Coordinate coor2) {
    init(coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY());
}

void Line::init(double x1, double y1, double x2, double y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    
    if (x1 <= x2) {
        xLow = x1;
        xHigh = x2;
        if ((x1 == x2) && (y1 > y2)) { //if is vertical
            yLow = y2;
            yHigh = y1;
        } else {
            yLow = y1;
            yHigh = y2;
        }
    } else {
        xLow = x2;
        yLow = y2;
        xHigh = x1;
        yHigh = y1;
    }
    
    if (xLow != xHigh) {
        if (yLow != yHigh) {
            vertical = false;
            horizontal = false;
            double dx = (x2 - x1);
            double dy = (y2 - y1);
            m = dy / dx;
            b = yLow - (m * xLow);
            angle.setValue(dx, dy);
            xAdjustment = angle.getCos();
            yAdjustment = angle.getSin();
        } else {
            vertical = false;
            horizontal = true;
            m = 0;
            b = y1;
            angle = (x1 > x2) ? M_PI : 0;
            xAdjustment = 1;
            yAdjustment = 0;
        }
    } else {
        vertical = true;
        horizontal = false;
        m = 0;
        b = 0;
        angle = (y1 > y2) ? -M_PI_2 : M_PI_2;
        xAdjustment = 0;
        yAdjustment = 1;
    }
}

bool Line::isVertical() const {
    return vertical;
}

bool Line::isHorizontal() const {
    return horizontal;
}

Angle Line::getAngle() const {
    return angle;
}

Coordinate Line::getCoor1() const {
    return Coordinate(x1, y1);
}

Line & Line::setCoor1(Coordinate coor) {
    init(coor.getX(), coor.getY(), x2, y2);
    return *this;
}

Coordinate Line::getCoor2() const {
    return Coordinate(x2, y2);
}

Line & Line::setCoor2(Coordinate coor) {
    init(x1, y1, coor.getX(), coor.getY());
    return *this;
}

Coordinate Line::getCoorLow() const {
    return Coordinate(xLow, yLow);
}

Coordinate Line::getCoorHigh() const {
    return Coordinate(xHigh, yHigh);
}

double Line::getM() const {
    return m;
}

double Line::getB() const {
    return b;
}

double Line::getXAdjustment() const {
    return xAdjustment;
}

double Line::getYAdjustment() const {
    return yAdjustment;
}

bool Line::inBoxX(double x) const {
    if (vertical)
        return (x == xLow);
    else
        return inRange(x, xLow, xHigh);
}

bool Line::inBoxY(double y) const {
    if (horizontal)
        return (doubleEqual(y, yLow));
    else if (vertical)
        return inRange(y, yLow, yHigh);
    else
        return (m >= 0) ? inRange(y, yLow, yHigh) : inRange(y, yHigh, yLow);
}

bool Line::inBox(double x, double y) const {
    return inBoxX(x) && inBoxY(y);
}

bool Line::inBox(Coordinate coor) const {
    return inBoxX(coor.getX()) && inBoxY(coor.getY());
}

bool Line::isOnOrBelow(Coordinate coor) const {
    return ((((x2 - x1) * (coor.getY() - y1)) - ((y2 - y1) * (coor.getX() - x1))) <= 0);
}

bool Line::intersectsLine(Line line, Coordinate * p_intersection) const {
    if (vertical && line.vertical) {
        if (x1 == line.x1) {
            if (p_intersection)
                p_intersection->setX(x1).setY(y1);
            return true;
        } else {
            return false;
        }
    } else if (horizontal && line.horizontal) {
        if (y1 == line.y1) {
            if (p_intersection)
                p_intersection->setX(x1).setY(y1);
            return true;
        } else {
            return false;
        }
    } else if ((m == line.m) && (b == line.b)) {
        if (p_intersection)
            p_intersection->setX(x1).setY(y1);
        return true;
    }
    
    double xIntersect;
    double yIntersect;
    
    if (vertical) {
        xIntersect = xLow;
        yIntersect = (line.horizontal ? line.yLow : (line.m * xLow) + line.b);
    } else if (line.vertical) {
        xIntersect = line.xLow;
        yIntersect = (horizontal ? yLow : (m * line.xLow) + b);
    } else if (horizontal) {
        xIntersect = (yLow - line.b) / line.m;
        yIntersect = yLow;
    } else if (line.horizontal) {
        xIntersect = (line.yLow - b) / m;
        yIntersect = line.yLow;
    } else {
        xIntersect = (line.b - b) / (m - line.m);
        yIntersect = (m * xIntersect) + b;
    }
    
    if (inBox(xIntersect, yIntersect) && line.inBox(xIntersect, yIntersect)) {
        if (p_intersection)
            p_intersection->setX(xIntersect).setY(yIntersect);
        return true;
    } else {
        return false;
    }
}

/*bool Line::isLeftOfLine(Coordinate coor) const {
 if (isHorizontal()) {
 return ((x1 > x2) ? (coor.getY() >= y1) : (coor.getY() <= y1));
 } else if (isVertical()) {
 return ((y1 > y2) ? (coor.getX() <= x1) : (coor.getX() >= x1));
 } else {
 Angle angleToCoor = findAngle(getCoor1(), coor);
 return ((angle.getValue() > 0) ?
 ((angleToCoor.getValue() <= angle.getValue()) && (angleToCoor.getValue() >= (angle + M_PI).getValue())) :
 ((angleToCoor.getValue() <= angle.getValue()) || (angleToCoor.getValue() >= (angle + M_PI).getValue())));
 }
 }*/

Coordinate Line::closestCoordinate(Coordinate coor) const {
    if (vertical) {
        if (inBoxY(coor.getY()))
            return Coordinate(xLow, coor.getY());
        else
            return (coor.getY() > yHigh) ? getCoorHigh() : getCoorLow();
    } else if (horizontal) {
        if (inBoxX(coor.getX()))
            return Coordinate(coor.getX(), y1);
        else
            return (coor.getX() > xHigh) ? getCoorHigh() : getCoorLow();
    } else {
        double otherM = -1 / m;
        double otherB = coor.getY() - (otherM * coor.getX());
        
        double x = (b - otherB) / (otherM - m);
        double y = (m * x) + b;
        
        if (inBox(x, y))
            return Coordinate(x, y);
        else
            return ((x > xHigh) ? getCoorHigh() : getCoorLow());
    }
}

Line & Line::rotateAboutCoordinate(Coordinate coor, Angle angle) {
    Coordinate coor1 = getCoor1().rotateAboutCoordinate(coor, angle);
    Coordinate coor2 = getCoor2().rotateAboutCoordinate(coor, angle);
    init(coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY());
    return *this;
}

bool Line::operator==(const Line & line) const {
    return ((x1 == line.x1) && (y1 == line.y1) && (x2 == line.x2) && (y2 == line.y2));
}

bool Line::operator!=(const Line & line) const {
    return !((x1 == line.x1) && (y1 == line.y1) && (x2 == line.x2) && (y2 == line.y2));
}

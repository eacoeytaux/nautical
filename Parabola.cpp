//
//  Parabola.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Parabola.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#include "Logger.hpp"
#include "Utility.hpp"

#define T_PERCISION 10

using namespace nautical;

Parabola::Parabola(double x1, double y1, double x2, double y2, double length) {
    init(x1, y1, x2, y2, length);
}

Parabola::Parabola(Coordinate coor1, Coordinate coor2, double length) {
    init(coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY(), length);
}

void Parabola::init(double x1, double y1, double x2, double y2, double length) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    
    if (x2 > x1) {
        xLeft = x1;
        yLeft = y1;
        xRight = x2;
        yRight = y2;
    } else {
        xLeft = x2;
        yLeft = y2;
        xRight = x1;
        yRight = y1;
    }
    
    if (findDistance(Coordinate(x1, y1), Coordinate(x2, y2)) > (length - 0.0001)) {
        if (length > 0)
            Logger::writeLog(WARNING_MESSAGE, "Parabola::init(): distance between coordinates is greater than length of parabola");
        
        double dx = x2 - x1;
        //set a, b, c to straight line
        a = 0;
        b = ((dx != 0) ? ((y2 - y1) / (x2 - x1)) : 0); //ensures no dividing by zero //TODO account for vertical lines
        c = y1 - (b * x1);
    } else {
        if (x1 > x2) { //switches coordinates if coor1 is further right than coor2
            swap<double>(x1, x2);
            swap<double>(y1, y2);
        }
        
        double dx = x2 - x1;
        double dy = y2 - y1;
        
        //calculates b and c of f(x)
        double bf = (dy - (pow(x2, 2) - pow(x1, 2))) / dx;
        double cf = y1 - (pow(x1, 2) + (bf * x1));
        
        //calculates b and c of g(x)
        double bg = (dy + (pow(x2, 2) - pow(x1, 2))) / dx;
        double cg = y1 - (-pow(x1, 2) + (bg * x1));
        
        //all parabolas containing coor1 and coor2 have equation ht(x) = t*f(x) + (1-t)*g(x)
        double t = 0.5; //t = 0.5 is straight line, all values lower have parabola opening down
        double dt = 100; //rate at which t changes when approximating
        
        for (int i = 0; i < T_PERCISION; i++) { //calculates t value with T_PERCISION digits of precision
            a = -1 + (2 * t);
            b = (bg * (1 - t)) + (bf * t);
            c = (cg * (1 - t)) + (cf * t);
            
            while ((calculateArcLength(a, b, c, x1, y1, x2, y2) - length) < 0) {
                t += dt;
                a = -1 + (2 * t);
                b = (bg * (1 - t)) + (bf * t);
                c = (cg * (1 - t)) + (cf * t);
            }
            
            t -= dt; //un-does last change which pushes t over threshold
            dt /= 10;
        }
        
        //final calculation of a, b, and c
        a = -1 + (2 * t);
        b = (bg * (1 - t)) + (bf * t);
        c = (cg * (1 - t)) + (cf * t);
    }
}

Coordinate Parabola::getVertex() const {
    double x = -b / (2 * a);
    return Coordinate(x, calculateY(x));
}

Coordinate Parabola::getCoor1() const {
    return Coordinate(x1, y1);
}

Coordinate Parabola::getCoor2() const {
    return Coordinate(x2, y2);
}

Coordinate Parabola::getLeftCoor() const {
    return Coordinate(xLeft, yLeft);
}

Coordinate Parabola::getRightCoor() const {
    return Coordinate(xRight, yRight);
}

double Parabola::getLength() const {
    return length;
}

double Parabola::calculateY(double x) const {
    return (a * pow(x, 2)) + (b * x) + c;
}

bool Parabola::inRangeX(double x) const {
    return inRange(x, xLeft, xRight);
}

bool Parabola::intersects(Line line, Coordinate * p_intersection) const {
    double newB = b - line.getM();
    double newC = c - line.getB();
    
    double x1 = solveQuadraticPlus(a, newB, newC);
    double x2 = solveQuadraticMinus(a, newB, newC);
    
    Coordinate coor1(x1, calculateY(x1));
    Coordinate coor2(x2, calculateY(x2));
    
    bool coor1Intersects = (line.inBox(coor1) && inRangeX(coor1.getX()));
    bool coor2Intersects = (line.inBox(coor2) && inRangeX(coor2.getX()));
    
    if (coor1Intersects || coor2Intersects) {
        if (p_intersection) {
            if (!coor2Intersects) {
                *p_intersection = coor1;
            } else if (!coor1Intersects) {
                *p_intersection = coor2;
            } else {
                double xDistance1 = fabs(line.getCoor1().getX() - coor1.getX());
                double xDistance2 = fabs(line.getCoor1().getX() - coor2.getX());
                
                if (xDistance1 <= xDistance2)
                    *p_intersection = coor1;
                else
                    *p_intersection = coor2;
            }
        }
        return true;
    } else {
        return false;
    }
}

double Parabola::calculateArcLength(double a, double b, double c, double x1, double y1, double x2, double y2) {
    if (a == 0)
        return nautical::findDistance(Coordinate(x1, y1), Coordinate(x2, y2));
    
    if (x1 > x2) { //switches coordinates if coor1 is further right than coor2 (should already be done)
        double tempx = x1;
        double tempy = y1;
        x1 = x2;
        y1 = y2;
        x2 = tempx;
        y2 = tempy;
    }
    
    //calculates vertex
    double vertexX = -b / (2 * a);
    double vertexY = (a * pow(vertexX, 2)) + (b * vertexX) + c;
    
    double arc1 = calculateArcLength(x1 - vertexX, y1 - vertexY) * ((x1 < vertexX) ? 1 : -1);
    double arc2 = calculateArcLength(x2 - vertexX, y2 - vertexY) * ((x2 > vertexX) ? 1 : -1);
    
    return arc1 + arc2;
}

double Parabola::calculateArcLength(double b, double h) {
    b = fabs(b);
    h = fabs(h);
    
    double twoH = 2 * h;
    double bSquared = pow(b, 2);
    double root = sqrt(pow(twoH, 2) + bSquared);
    
    return (root + ((bSquared / twoH) * log((twoH + root) / b))) / 2;
}

//
//  Utility.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Utility__
#define __Nautical__Utility__

#include <string>
#include <cmath>

#include "Angle.hpp"
#include "Coordinate.hpp"
#include "LinkedList.hpp"

#define ROUNDING_ERROR_MARGIN 0.0001

namespace nautical {
    extern bool DEBUG_MODE;
    
    inline double solveQuadraticPlus(double a, double b, double c) { //solves quadratic equation
        return (-b + sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
    }
    
    inline double solveQuadraticMinus(double a, double b, double c) { //solves quadratic equation
        return (-b + sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
    }
    
    inline bool inRange(double value, double min, double max, bool inclusive = true) { //checks whether or not value is inbetween min and max
        return inclusive ? ((value >= min) && (value <= max)) : ((value > min) && (value < max));
    }
    
    inline double findDistance(double dx, double dy) { //returns distance of dx and dy
        return sqrt(pow(dx, 2) + pow(dy, 2));
    }
    
    inline double findDistance(Coordinate coor1, Coordinate coor2 = Coordinate()) { //returns distance between coordinates
        double x1 = coor1.getX(), y1 = coor1.getY(), x2 = coor2.getX(), y2 = coor2.getY();
        
        double dx = x1 - x2;
        double dy = y1 - y2;
        
        if (dx == 0)
            return fabs(dy);
        else if (dy == 0)
            return fabs(dx);
        
        return findDistance(dx, dy);
    }
    
    inline Angle findAngle(Coordinate coor1, Coordinate coor2) { //finds angle between coordinates
        return Angle(coor2.getX() - coor1.getX(), coor2.getY() - coor1.getY());
    }
    
    inline bool doubleEqual(double d1, double d2, double margin = ROUNDING_ERROR_MARGIN) { //checks whether or not two doubles are equal with a set margin of error
        return fabs(d1 - d2) <= margin;
    }
    
    inline bool stringEqual(std::string str1, std::string str2) { //checks whether or not two strings have the same value
        return (str1.compare(str2) == 0);
    }
    
    template<typename T>
    T flip(T element, bool b = true) { //TODO name this better?
        return element * (b ? -1 : 1);
    }
    
    template<typename T>
    void swap(T * p_t1, T * p_t2) { //swaps generic elements
        T temp = *p_t1;
        *p_t1 = *p_t2;
        *p_t2 = temp;
    }
}

#endif /* defined(__Nautical__Utility__) */

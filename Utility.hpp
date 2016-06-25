//
//  Utility.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Utility__
#define __Nautical__Utility__

#define _USE_MATH_DEFINES
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

#ifndef M_PI
#define M_PI       3.14159265358979323846264338328      // Pi
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923132169164      // Pi/2
#endif

#ifndef M_PI_4
#define M_PI_4     0.78539816339744830961566084582      // Pi/4
#endif

#include "Logger.hpp"
#include "Angle.hpp"
#include "Coordinate.hpp"

#define ROUNDING_ERROR_MARGIN 0.00001

namespace nautical {
    extern bool DEBUG_MODE;
    
    inline double solveQuadraticPlus(double a, double b, double c) { //solves quadratic equation
        return (-b + sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
    }
    
    inline double solveQuadraticMinus(double a, double b, double c) { //solves quadratic equation
        return (-b - sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
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
    
    namespace vector_helpers {
        template<typename T>
        inline void removeElementByValue(std::vector<T> & vector, T value, bool removeAll = false) {
            if (removeAll) {
                vector.erase(std::remove(vector.begin(), vector.end(), value), vector.end());
            } else {
                typename std::vector<T>::iterator it = std::find(vector.begin(), vector.end(), value);
                if (it != vector.end())
                    vector.erase(it);
                else
                    Logger::writeLog(ERROR_MESSAGE, "removeElementByValue(): attempted to remove non-existant element from vector");
            }
        }
        
        template<typename T>
        inline void removeElementByIndex(std::vector<T> & vector, int index) {
            //TODO index bounds checking
            vector.erase(vector.begin() + index);
        }
        
        template<typename T>
        inline int getIndexOfElement(std::vector<T> & vector, T value) { //returns -1 if not found
            int index = (int)(std::find(vector.begin(), vector.end(), value) - vector.begin());
            return (index >= vector.size()) ? -1 : index;
        }
        
        template<typename T>
        inline bool containsElement(std::vector<T> & vector, T value) {
            return getIndexOfElement(vector, value) != -1;
        }
    }
}

#endif /* defined(__Nautical__Utility__) */

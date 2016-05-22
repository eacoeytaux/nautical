//
//  Angle.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Angle__
#define __Nautical__Angle__

#include "Reusable.hpp"

namespace nautical {
    class Angle {
    public:
        Angle(double angleVal = 0, bool truncate = true);
        Angle(double dx, double dy, bool truncate = true);
        
        bool truncating() const;
        Angle & setTruncating(bool truncate);
        
        double getValue() const;
        Angle & setValue(double angleVal);
        Angle & setValue(double dx, double dy);
        double getSin(double scale = 1) const;
        double getCos(double scale = 1) const;
        
        Angle operator+(const Angle & angle) const;
        Angle & operator+=(const Angle & angle);
        Angle operator-(const Angle & angle) const;
        Angle & operator-=(const Angle & angle);
        
        Angle & operator=(double angleVal);
        Angle operator+(const double & angleVal) const;
        Angle & operator+=(const double & angleVal);
        Angle operator-(const double & angleVal) const;
        Angle & operator-=(const double & angleVal);
        Angle operator*(const double & angleVal) const;
        Angle & operator*=(const double & angleVal);
        Angle operator/(const double & angleVal) const;
        Angle & operator/=(const double & angleVal);
        
        bool operator==(const Angle & angle) const;
        bool operator!=(const Angle & angle) const;
        
        static double radiansToDegrees(double radians);
        static double degreesToRadians(double degrees);
        
        static double weighAngle(const Angle * p_angle);
        
    private:
        bool truncate;
        double angleVal,
        angleValDegrees; //DEBUGGING
        mutable Reusable<double> sinValue = Reusable<double>(0, false), cosValue = Reusable<double>(0, false);
    };
}

#endif /* defined(__Nautical__Angle__) */

//
//  Vector.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Vector__
#define __Nautical__Vector__

#include "Drawable.hpp" //base class

#include "Utility.hpp"
#include "Angle.hpp"

namespace nautical {
    class Vector : public Drawable {
    public:
        Vector(double dx = 0, double dy = 0, Coordinate origin = Coordinate());
        Vector(Angle angle, double magnitude, Coordinate origin = Coordinate());
        Vector(Coordinate coor1, Coordinate coor2);
        
        double getDx() const;
        Vector & setDx(double dx);
        bool isDxPositive();
        double getDy() const;
        Vector & setDy(double dy);
        bool isDyPositive();
        Coordinate getOrigin() const;
        Vector & setOrigin(Coordinate origin);
        Coordinate getDestination() const;
        Vector & setDestination(Coordinate destination);
        double getMagnitude() const;
        Vector & setMagnitude(double magnitude);
        Vector & extend(double length);
        
        Angle getAngle() const;
        Vector & rotateToAngle(Angle angle);
        Vector & rotate(Angle angle);
        Vector & mirror();
        Vector & mirrorHorizontal();
        Vector & mirrorVertical();
        
        bool subtractAngle(Angle angle);
        
        //Line toLine() const;
        //Line toLine(const Coordinate coor, bool subtracted = false) const;
        
        void draw() const;
        void draw(int magnitude) const;
        void draw(Coordinate origin, int magnitude) const;
        
        Vector operator+(const Vector & vec) const;
        Vector & operator+=(const Vector & vec);
        Vector operator-(const Vector & vec) const;
        Vector & operator-=(const Vector & vec);
        Vector operator*(const double & d) const;
        Vector & operator*=(const double & d);
        Vector operator/(const double & d) const;
        Vector & operator/=(const double & d);
        
        bool operator==(const Vector & vec) const;
        bool operator!=(const Vector & vec) const;
        
    private:
        Coordinate origin;
        double dx, dy;
        mutable Reusable<Angle> angle = Reusable<Angle>(0, 0);
        mutable Reusable<double> magnitude = Reusable<double>(0, false);
    };
    
    //these are declared here because declaring them inside Coordinate.hpp requires including Vector.hpp which causes circular inclusion
    inline Coordinate operator+(const Coordinate & coor, const Vector & vec) {
        return Coordinate(coor.getX() + vec.getDx(), coor.getY() + vec.getDy());
    }
    
    inline Coordinate & operator+=(Coordinate & coor, const Vector & vec) {
        coor.setX(coor.getX() + vec.getDx());
        coor.setY(coor.getY() + vec.getDy());
        return coor;
    }
    
    inline Coordinate operator-(const Coordinate & coor, const Vector & vec) {
        return Coordinate(coor.getX() - vec.getDx(), coor.getY() - vec.getDy());
    }
    
    inline Coordinate & operator-=(Coordinate & coor, const Vector & vec) {
        coor.setX(coor.getX() - vec.getDx());
        coor.setY(coor.getY() - vec.getDy());
        return coor;
    }
    
    inline Line operator+(const Line & line, const Vector & vec) {
        return Line(line.getCoor1() + vec, line.getCoor1() + vec);
    }
    
    inline Line operator-(const Line & line, const Vector & vec) {
        return Line(line.getCoor1() - vec, line.getCoor1() - vec);
    }
    
    inline bool Vector::operator==(const Vector & vec) const {
        return ((origin == vec.origin) && (dx == vec.dx) && (dy == vec.dy));
    }
    
    inline bool Vector::operator!=(const Vector & vec) const {
        return !((origin == vec.origin) && (dx == vec.dx) && (dy == vec.dy));
    }
}

#endif /* defined(__Nautical__Vector__) */

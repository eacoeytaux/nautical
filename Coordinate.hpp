//
//  Coordinate.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Coordinate__
#define __Nautical__Coordinate__

#include "Angle.hpp"

namespace nautical {
    class Coordinate {
    public:
        Coordinate(double x = 0, double y = 0);
        
        double getX() const;
        void setX(double x);
        void moveX(double dx);
        
        double getY() const;
        void setY(double y);
        void moveY(double dy);
        
        void rotateAboutCoordinate(Coordinate coor, Angle angle);
        
        bool operator==(const Coordinate & coor) const;
        bool operator!=(const Coordinate & coor) const;
        
    private:
        double x, y;
    };
}

#endif /* defined(__Nautical__Coordinate__) */

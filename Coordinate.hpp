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
        Coordinate & setX(double x);
        Coordinate & moveX(double dx);
        
        double getY() const;
        Coordinate & setY(double y);
        Coordinate & moveY(double dy);
        
        Coordinate & rotateAboutCoordinate(Angle angle, Coordinate coor = Coordinate(0, 0));
        
        bool operator==(const Coordinate & coor) const;
        bool operator!=(const Coordinate & coor) const;
        
    private:
        double x, y;
    };
}

#endif /* defined(__Nautical__Coordinate__) */

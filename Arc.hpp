//
//  Arc.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Arc_hpp
#define Arc_hpp

#include "Angle.hpp"
#include "Coordinate.hpp"
#include "Circle.hpp"

namespace nautical {
    /*class Arc {
    public:
        Arc(Coordinate origin, Angle startAngle, Angle endAngle, bool clockwise = true);
        virtual ~Arc();
        
        double getLength() const;
        Coordinate getStartCoor() const;
        Coordinate getEndCoor() const;
        Angle getStartAngle() const;
        Angle getEndAngle() const;
        
        bool intersects(Line line, Queue<Coordinate> * p_intersections) const;
        
    private:
        Circle circle;
        Angle startAngle,
        endAngle;
        bool clockwise;
    };*/
}

#endif /* Arc_hpp */

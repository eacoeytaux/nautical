//
//  Arc.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Arc_hpp
#define Arc_hpp

#include "Queue.hpp"
#include "Angle.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"

namespace nautical {
    class Arc {
    public:
        Arc(Coordinate origin = Coordinate(), double originDistance = 0, Angle startAngle = Angle(), Angle endAngle = Angle(), bool clockwise = true);
        virtual ~Arc();
        
        Coordinate getOrigin() const;
        Arc & setOrigin(Coordinate origin);
        double getOriginDistance() const;
        Arc & setOriginDistance(double distance);
        Angle getStartAngle() const;
        Arc & setStartAngle(Angle angle);
        Angle getEndAngle() const;
        Arc & setEndAngle(Angle angle);
        Angle getDAngle() const;
        double getArcLength() const;
        bool isClockwise() const;
        Arc & setClockwise(bool clockwise);
        Coordinate getStartCoor() const;
        Coordinate getEndCoor() const;
        
        bool intersectsArc(Arc arc, Queue<Coordinate> * p_intersections) const;
        bool intersectsLine(Line line, Queue<Coordinate> * p_intersections) const;
        bool inArc(Angle angle) const;
        bool inArc(Coordinate coor) const;
        
    private:
        Coordinate origin;
        double originDistance;
        Angle startAngle,
        endAngle,
        dAngle;
        bool clockwise;
        
        Arc & setDAngle();
    };
}

#endif /* Arc_hpp */

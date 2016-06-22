//
//  Arc.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Arc_hpp
#define Arc_hpp

#include <vector>

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
        
        bool intersectsArc(Arc arc, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool intersectsLine(Line line, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool inArc(Angle angle) const;
        bool inArc(Coordinate coor) const;
        
        Arc & rotateAboutCoordinate(Coordinate coor, Angle angle);
        
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

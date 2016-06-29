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
        Arc(const Coordinate & origin = Coordinate(), double originDistance = 0, const Angle & startAngle = Angle(), const Angle & endAngle = Angle(), bool clockwise = true);
        virtual ~Arc();
        
        Coordinate getOrigin() const;
        Arc & setOrigin(const Coordinate & origin);
        double getOriginDistance() const;
        Arc & setOriginDistance(double distance);
        Angle getStartAngle() const;
        Arc & setStartAngle(const Angle & angle);
        Angle getEndAngle() const;
        Arc & setEndAngle(const Angle & angle);
        Angle getDAngle() const;
        double getArcLength() const;
        bool isClockwise() const;
        Arc & setClockwise(bool clockwise);
        Coordinate getStartCoor() const;
        Coordinate getEndCoor() const;
        
        bool intersectsArc(const Arc & arc, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool intersectsLine(const Line & line, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool inArc(const Angle & angle) const;
        bool inArc(const Coordinate & coor) const;
        
        Arc & rotateAboutCoordinate(const Coordinate & coor, const Angle & angle);
        
    private:
        Coordinate origin;
        double originDistance;
        Angle startAngle,
        endAngle,
        dAngle;
        bool clockwise;
        
        void setDAngle();
    };
}

#endif /* Arc_hpp */

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
        void setOrigin(Coordinate origin);
        double getOriginDistance() const;
        void setOriginDistance(double distance);
        Angle getStartAngle() const;
        void setStartAngle(Angle angle);
        Angle getEndAngle() const;
        void setEndAngle(Angle angle);
        Angle getDAngle() const;
        double getArcLength() const;
        bool isClockwise() const;
        void setClockwise(bool clockwise);
        Coordinate getStartCoor() const;
        Coordinate getEndCoor() const;
        
        bool intersectsArc(Arc arc, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool intersectsLine(Line line, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool inArc(Angle angle) const;
        bool inArc(Coordinate coor) const;
        
        void rotateAboutCoordinate(Coordinate coor, Angle angle);
        
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

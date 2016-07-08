//
//  Tentacle.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/9/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Tentacle_hpp
#define Tentacle_hpp

#include "WorldObject.hpp"

#define TENTACLE_SEGMENT_COUNT 20

namespace climber {
    class Tentacle : public nautical::WorldObject {
    public:
        Tentacle(nautical::Coordinate origin, nautical::Angle originAngle, nautical::Coordinate target);
        virtual ~Tentacle();
        
        void update();
        void draw() const;
        
    private:
        nautical::Coordinate origin;
        nautical::Angle originAngle;
        nautical::Coordinate target;
        
        double relaxedLength = 10,
        muscleWidth = 5,
        muscleRelaxSpeed = 0.5;
        struct Muscle {
            nautical::Coordinate pos;
            nautical::Angle angle;
            nautical::Angle targetAngle;
            double tendon1Length;
            double tendon2Length;
        };
        Muscle tentacle[TENTACLE_SEGMENT_COUNT];
    };
}

#endif /* Tentacle_hpp */

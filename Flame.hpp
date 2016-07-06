//
//  Flame.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/24/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Flame_hpp
#define Flame_hpp

#include "WorldObject.hpp"

#include "Countdown.hpp"

namespace climber {
    const std::string FLAME_TAG = "flame";
    
    class Flame : public nautical::WorldObject {
    public:
        Flame(nautical::Coordinate pos);
        virtual ~Flame();
        
        Flame & addOrigin(double flameWidth, nautical::physics::Vector offset = nautical::physics::Vector(0, 0));
        
        void update();
        void draw() const;
        
    private:
        nautical::SpriteSheet * p_spriteSheet = nullptr;
        
        struct Origin {
            nautical::Coordinate origin;
            double flameWidth;
            nautical::Countdown orangeCountdown;
            nautical::Countdown yellowCountdown;
        };
        std::vector<Origin*> origins;
        
        struct Spark {
            nautical::Coordinate center;
            double width;
            int alpha;
        };
        std::vector<Spark*> orangeParticles;
        std::vector<Spark*> yellowParticles;
    };
}

#endif /* Flame_hpp */

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
        
        Flame & addOrigin(double flameWidth, nautical::Vector offset = nautical::Vector(0, 0));
        
        void update();
        
        void draw() const;
        
    private:
        nautical::SpriteSheet * p_sheet = nullptr;
        
        struct Origin {
            nautical::Coordinate origin;
            double flameWidth;
            nautical::Countdown orangeCountdown;
            nautical::Countdown yellowCountdown;
        };
        nautical::LinkedList<Origin*> origins;
        
        struct Spark {
            nautical::Coordinate center;
            double width;
            int alpha;
        };
        nautical::LinkedList<Spark*> orangeParticles;
        nautical::LinkedList<Spark*> yellowParticles;
    };
}

#endif /* Flame_hpp */

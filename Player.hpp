//
//  Player.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/16/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Mob.hpp"

#include "Rope.hpp"

namespace climber {
    class Player : public nautical::Mob {
    public:
        Player(nautical::Coordinate pos);
        virtual ~Player();
        
        Rope * getRope();
        Player & setRope(Rope * p_rope);
        
        Player & move(nautical::Vector vec);
        
        bool handleEvent(nautical::Event * p_event);
        
        void update();
        void draw() const;
        
    private:
        nautical::Rectangle trap; //used to center camera on player
        
        Rope * p_rope = nullptr;
    };
}

#endif /* Player_hpp */

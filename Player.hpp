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

namespace climber {
    class Player : public nautical::Mob {
    public:
        Player(nautical::Coordinate pos);
        virtual ~Player();
        
        Player & move(nautical::Vector vec);
        
        void handleEvent(nautical::Event * p_event);
        
        void update();
        void draw() const;
        
    private:
        nautical::Rectangle trap;
    };
}

#endif /* Player_hpp */

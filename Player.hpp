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

#include "Countdown.hpp"
#include "Rope.hpp"

namespace climber {
    const std::string PLAYER_TAG = "player";
    
    class Player : public nautical::Mob {
    public:
        Player(nautical::Coordinate pos);
        virtual ~Player();
        
        Player & setMapElement(const nautical::MapElement * p_element);
        
        Rope * getRope();
        Player & setRope(Rope * p_rope);
        
        bool isFacingRight() const;
        Player & setFacingRight(bool facingRight);
        bool isMovingRight() const;
        Player & setMovingRight(bool movingRight);
        bool isMovingLeft() const;
        Player & setMovingLeft(bool movingLeft);
        
        
        
        Player & move(nautical::Vector vec);
        
        bool handleEvent(nautical::Event * p_event);
        
        void update();
        void draw() const;
        
    private:
        nautical::Rectangle trap; //used to center camera on player
        
        bool facingRight = true, //false implies facing left
        movingRight = false,
        movingLeft = false;
        
        bool jumping = false,
        canJump = false,
        canGhostJump = false;
        nautical::Countdown jumpingCountdown; //countdown of how long player can hold jump button and it will still have effect
        nautical::Countdown ghostJumpCountdown; //countdown from leaving ground where player can still jump
        
        nautical::Angle aimAngle;
        Rope * p_rope = nullptr;
    };
}

#endif /* Player_hpp */

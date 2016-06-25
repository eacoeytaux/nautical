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
        
        void setMapElement(const nautical::MapElement * p_element);
        
        Rope * getRope();
        void setRope(Rope * p_rope);
        
        bool isFacingRight() const;
        void setFacingRight(bool facingRight);
        bool isMovingRight() const;
        void setMovingRight(bool movingRight);
        bool isMovingLeft() const;
        void setMovingLeft(bool movingLeft);
        
        bool isJumping() const;
        void setJumping(bool b);
        bool canJump() const;
        void setCanJump(bool b);
        bool canGhostJump() const;
        void setCanGhostJump(bool b);
        
        void move(nautical::Vector vec);
        
        bool handleEvent(nautical::Event * p_event);
        
        void update();
        void draw() const;
        
    private:
        nautical::Rectangle trap; //used to center camera on player
        
        bool facingRight = true, //false implies facing left
        movingRight = false,
        movingLeft = false;
        
        bool jumping = false,
        jumpCapable = false;
        int jumpValueIndex = 0;
        std::vector<double> jumpValues = {4, 3.5, 3, 2.5}; //array of jump values
        bool ghostJumpCapable = false;
        nautical::Countdown ghostJumpCountdown; //countdown from leaving ground where player can still jump
        
        nautical::Angle aimAngle = nautical::Angle(M_PI_2);
        Rope * p_rope = nullptr;
    };
}

#endif /* Player_hpp */

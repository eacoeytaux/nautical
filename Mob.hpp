//
//  Mob.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/8/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Mob_hpp
#define Mob_hpp

#include "WorldObject.hpp"

namespace nautical {
    const std::string MOB_TAG = "mob";
    
    class Mob : public WorldObject {
    public:
        Mob(Coordinate pos, int maxHealth = 1);
        virtual ~Mob();
        
        bool isAlive() const;
        virtual void kill(bool passively = false); //passively is bool used to kill mob without death triggers
        
        int getMaxHealth() const;
        virtual Mob & setMaxHealth(int health);
        virtual Mob & raiseMaxHealth(int health);
        int getHealth() const;
        virtual Mob & setHealth(int health);
        virtual Mob & damage(int health);
        virtual Mob & heal(int health);
        virtual Mob & healFull();
        
        bool isFacingRight() const;
        Mob & setFacingRight(bool facingRight);
        bool isMovingRight() const;
        Mob & setMovingRight(bool movingRight);
        bool isMovingLeft() const;
        Mob & setMovingLeft(bool movingLeft);
        
    private:
        bool alive = true;
        int maxHealth,
        health;
        bool facingRight = true, //false implies facing left
        movingRight = false,
        movingLeft = false;
    };
}

#endif /* Mob_hpp */

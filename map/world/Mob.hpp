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
        
        virtual bool isAlive() const;
        virtual Mob & kill(bool passively = false); //passively is bool used to kill mob without death triggers
        
        virtual int getMaxHealth() const;
        virtual Mob & setMaxHealth(int health);
        virtual Mob & raiseMaxHealth(int health);
        virtual int getHealth() const;
        virtual Mob & setHealth(int health);
        virtual Mob & damage(int health);
        virtual Mob & heal(int health);
        virtual Mob & healFull();
        
    private:
        bool alive = true;
        int maxHealth,
        health;
    };
}

#endif /* Mob_hpp */

//
//  WorldObject.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/1/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef WorldObject_hpp
#define WorldObject_hpp

#include "Updatable.hpp"
#include "Drawable.hpp"
#include "Tagable.hpp"

#include "SpriteSheet.hpp"
#include "Utility.hpp"
#include "Random.hpp"
#include "Coordinate.hpp"
#include "Vector.hpp"
#include "Rectangle.hpp"
#include "MapHitbox.hpp"
#include "DarknessOverlay.hpp"
#include "World.hpp"

namespace nautical { //TODO add spritesheet?
    const std::string WORLD_OBJECT_TAG = "world_object";
    
    class WorldObject : public Updatable, public Drawable, public Tagable {
    public:
        WorldObject(Coordinate pos);
        WorldObject(const WorldObject & other);
        virtual ~WorldObject();
        
        virtual World * getParent() const;
        virtual WorldObject & setParent(World * p_parent);
        virtual Coordinate getCenter() const;
        virtual std::shared_ptr<MapHitbox> getMapHitbox() const; //should delete pointer after use
        virtual WorldObject & setMapHitbox(MapHitbox * p_hitbox); //should delete pointer after use
        virtual const MapElement * getMapElement() const;
        virtual WorldObject & setMapElement(const MapElement * p_element);
        virtual Vector getForce() const;
        virtual WorldObject & setForce(Vector force);
        virtual WorldObject & addToForce(Vector force);
        virtual Vector getVel() const;
        virtual WorldObject & setVel(Vector vel);
        virtual WorldObject & addToVel(Vector vel);
        
        virtual WorldObject & moveTo(Coordinate coor);
        virtual WorldObject & move(Vector vec);
        
        //TODO make sure WorldObjects* cannot be accessed
        virtual const std::vector<WorldObject*> * getAttachedObjects() const;
        virtual WorldObject & attachObject(WorldObject * p_object);
        virtual WorldObject & removeAttachedObject(WorldObject * p_object);
        
        const std::vector<std::string> * getSubscribedEventTags() const;
        WorldObject & subscribeEvent(std::string eventTag);
        WorldObject & unsubscribeEvent(std::string eventTag);
        
        virtual bool handleEvent(Event * p_event); //returns whether or not object did something with event
        
        bool isSpectral() const;
        WorldObject & setSpectral(bool spectral);
        int getPriority() const;
        WorldObject & setPriority(int priority);
        int getAltitude() const;
        WorldObject & setAltitude(int altitude);
        
    private:
        World * p_parent = nullptr;
        
        bool spectral = false;
        int priority = 0;
        int altitude = -MAX_BELOW_ALTITUDE;
        
        Coordinate center;
        std::unique_ptr<MapHitbox> p_hitbox;
        Vector force, vel;
        
        std::vector<WorldObject*> attachedObjects;
        
        std::vector<std::string> subscribedEventTags;
    };
}

#endif /* WorldObject_hpp */

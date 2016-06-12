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
#include "LinkedList.hpp"
#include "Coordinate.hpp"
#include "Vector.hpp"
#include "Rectangle.hpp"
#include "MapHitbox.hpp"
#include "World.hpp"

namespace nautical {
    const std::string WORLD_OBJECT_TAG = "world_object";
    
    class WorldObject : public Updatable, public Drawable, public Tagable {
        friend class World;
        
    public:
        WorldObject(Coordinate pos);
        WorldObject(const WorldObject & other);
        virtual ~WorldObject();
        
        World * getParent() const;
        WorldObject & setParent(World * p_parent);
        Coordinate getCenter() const;
        MapHitbox * getMapHitbox() const; //should delete pointer after use
        WorldObject & setMapHitbox(MapHitbox * p_hitbox); //should delete pointer after use
        Vector getForce() const;
        WorldObject & setForce(Vector force);
        WorldObject & addToForce(Vector force);
        Vector getVel() const;
        WorldObject & setVel(Vector vel);
        WorldObject & addToVel(Vector vel);
        
        WorldObject & moveTo(Coordinate coor);
        virtual WorldObject & move(Vector vec);
        
        Iterator<WorldObject*> * getAttachedObjectsIterator() const;
        WorldObject & attachObject(WorldObject * p_object);
        WorldObject & removeAttachedObject(WorldObject * p_object);
        
        WorldObject & subscribeEvent(std::string eventTag);
        WorldObject & unsubscribeEvent(std::string eventTag);
        Iterator<std::string> * getSubscribedEventTagsIterator() const;
        
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
        MapHitbox * p_hitbox = nullptr;
        Vector force, vel;
        
        LinkedList<WorldObject*> attachedObjects;
        
        LinkedList<std::string> subscribedEventTags = LinkedList<std::string>(false);
    };
}

#endif /* WorldObject_hpp */

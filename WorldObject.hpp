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

namespace nautical {
    const std::string WORLD_OBJECT_TAG = "world_object";
    
    class WorldObject : public Updatable, public Drawable, public Tagable {
    public:
        WorldObject(Coordinate pos);
        WorldObject(const WorldObject & other);
        virtual ~WorldObject();
        
        virtual World * getParent() const;
        virtual void setParent(World * p_parent);
        virtual Coordinate getCenter() const;
        virtual std::shared_ptr<MapHitbox> getMapHitbox() const; //should delete pointer after use
        virtual void setMapHitbox(MapHitbox * p_hitbox); //should delete pointer after use
        virtual const MapElement * getMapElement() const;
        virtual void setMapElement(const MapElement * p_element);
        virtual Vector getForce() const;
        virtual void setForce(Vector force);
        virtual void addToForce(Vector force);
        virtual Vector getVel() const;
        virtual void setVel(Vector vel);
        virtual void addToVel(Vector vel);
        
        virtual void moveTo(Coordinate coor);
        virtual void move(Vector vec);
        
        //TODO make sure WorldObjects* cannot be accessed
        virtual const std::vector<WorldObject*> * getAttachedObjects() const;
        virtual void attachObject(WorldObject * p_object);
        virtual void removeAttachedObject(WorldObject * p_object);
        
        const std::vector<std::string> * getSubscribedEventTags() const;
        void subscribeEvent(std::string eventTag);
        void unsubscribeEvent(std::string eventTag);
        
        virtual bool handleEvent(Event * p_event); //returns whether or not object did something with event
        
        bool isSpectral() const;
        void setSpectral(bool spectral);
        int getPriority() const;
        void setPriority(int priority);
        int getAltitude() const;
        void setAltitude(int altitude);
        
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

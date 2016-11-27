//
//  WorldObject.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/1/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef WorldObject_hpp
#define WorldObject_hpp

#include "Mass.hpp"
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
    
    class WorldObject : public Mass, public Updatable, public Drawable, public Tagable {
    public:
        WorldObject(double m, Coordinate pos);
        //WorldObject(const WorldObject & other);
        virtual ~WorldObject();
        
        virtual WorldObject & movePosition(const Vector & dPos);
        
        virtual std::shared_ptr<MapHitbox> getMapHitbox() const;
        virtual WorldObject & setMapHitbox(std::shared_ptr<MapHitbox> p_hitbox);
        virtual std::shared_ptr<const MapElement> getMapElement() const;
        virtual WorldObject & setMapElement(std::shared_ptr<const MapElement> p_element);
        
        //TODO make sure WorldObjects* cannot be accessed
        virtual const std::vector<std::shared_ptr<WorldObject>> & getAttachedObjects() const;
        virtual WorldObject & attachObject(std::shared_ptr<WorldObject> p_object);
        virtual WorldObject & removeAttachedObject(std::shared_ptr<WorldObject> p_object);
        
        const std::vector<std::string> & getSubscribedEventTags() const;
        WorldObject & subscribeEvent(std::string eventTag);
        WorldObject & unsubscribeEvent(std::string eventTag);
        
        virtual bool handleEvent(std::shared_ptr<Event> p_event); //returns whether or not object did something with event
        
        bool isSpectral() const;
        WorldObject & setSpectral(bool spectral);
        int getPriority() const;
        WorldObject & setPriority(int priority);
        int getAltitude() const;
        WorldObject & setAltitude(int altitude);
        
    protected:
        std::shared_ptr<MapHitbox> p_hitbox; //TODO should this be a unique_ptr?
        
    private:
        bool spectral = false;
        int priority = 0;
        int altitude = -MAX_BELOW_ALTITUDE;
        
        std::vector<std::shared_ptr<WorldObject>> attachedObjects;
        
        std::vector<std::string> subscribedEventTags;
    };
}

#endif /* WorldObject_hpp */

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

#include "Utility.hpp"
#include "LinkedList.hpp"
#include "Coordinate.hpp"
#include "Vector.hpp"
#include "Rectangle.hpp"
#include "World.hpp"

namespace nautical {
    const std::string WORLD_OBJECT_TAG = "world_object";
    
    class WorldObject : public Updatable, public Drawable, public Tagable {
    public:
        WorldObject(Coordinate pos);
        virtual ~WorldObject();
        
        World * getParent() const;
        WorldObject & setParent(World * p_parent);
        Coordinate getCenter() const;
        double getMapWidth() const;
        WorldObject & setMapWidth(double width);
        double getMapHeight() const;
        WorldObject & setMapHeight(double height);
        Angle getMapAngle() const;
        WorldObject & setMapAngle(Angle angle);
        Rectangle getHitbox() const;
        MapElement * getMapElement() const;
        WorldObject & setMapElement(MapElement * p_element);
        MapElement::ObjectPos getObjectPos() const;
        WorldObject & setObjectPos(MapElement::ObjectPos objPos);
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
        
        virtual void handleEvent(Event * p_event);
        
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
        double mapWidth, mapHeight;
        Angle mapAngle;
        MapElement * p_element = nullptr;
        MapElement::ObjectPos objPos = MapElement::UNDEFINED;
        Vector force, vel;
        
        LinkedList<WorldObject*> attachedObjects;
        
        LinkedList<std::string> subscribedEventTags = LinkedList<std::string>(false);
    };
}

#endif /* WorldObject_hpp */

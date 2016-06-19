//
//  Level.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Level_hpp
#define Level_hpp

#include "LinkedList.hpp"
#include "Event.hpp"
#include "DarknessOverlay.hpp"
#include "Map.hpp"

#define MAX_PRIORITY 4
#define MAX_BELOW_ALTITUDE 3
#define MAX_ABOVE_ALTITUDE 3

namespace nautical {
    class MapHitbox;
    class WorldObject;
    
    class World {
    public:
        World(bool verticalWorld = false);
        virtual ~World();
        
        int getID() const;
        int getUpdateTimestamp() const;
        int getDrawTimestamp() const;
        
        bool isVertical() const;
        
        const Map * getMap() const;
        Map * getMap();
        
        World & addObject(WorldObject * p_object, bool shouldUpdate = true, bool shouldDraw = true);
        World & markObjectForRemoval(WorldObject * p_object); //TODO when should object be deleted?
        
        World & subscribeObject(std::string eventTag, WorldObject * p_object);
        World & subscribeObjects(std::string eventTag, LinkedList<WorldObject*> objects);
        World & unsubscribeObject(std::string eventTag, WorldObject * p_object);
        World & unsubscribeObjects(std::string eventTag, LinkedList<WorldObject*> objects);
        
        World & handleEvent(Event * p_event);
        
        Vector generatePath(float * p_percentage, Vector * p_vel, MapHitbox * p_hitbox, const MapElement ** p_nextElement); //TODO rename function
        
        virtual void update(Collection<Event*> & events);
        virtual void draw();
        
        static bool DRAW_BUMPERS; //DEBUGGING
        
    protected:
        World & removeObject(WorldObject * p_object);
        
        virtual void updateObject(WorldObject * p_object);
        
    private:
        int id,
        updateTimestamp = 0,
        drawTimestamp = 0;
        
        bool verticalWorld;
        
        Map map;
        
        SortedList<WorldObject*> allObjects,
        objectsToDelete,
        objectsToUpdate[MAX_PRIORITY + 1],
        objectsToDraw[MAX_BELOW_ALTITUDE + MAX_ABOVE_ALTITUDE + 1];
        
        struct EventPairing {
            std::string eventTag;
            LinkedList<WorldObject*> subscribedObjects;
            
            inline bool operator==(const EventPairing & other) const { return (eventTag == other.eventTag); }
        };
        LinkedList<EventPairing> subscribedObjects;
    };
}

#endif /* Level_hpp */

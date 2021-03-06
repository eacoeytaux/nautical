//
//  Level.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Level_hpp
#define Level_hpp

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
        
        const Map & getMap() const;
        
        double getTimeRatio() const;
        World & setTimeRatio(double timeRatio);
        
        World & addObject(std::shared_ptr<WorldObject> p_object, bool shouldUpdate = true, bool shouldDraw = true);
        World & markObjectForRemoval(std::shared_ptr<WorldObject> p_object); //TODO when should object be deleted?
        
        World & subscribeObject(std::string eventTag, std::shared_ptr<WorldObject> p_object);
        World & unsubscribeObject(std::string eventTag, std::shared_ptr<WorldObject> p_object);
        
        World & handleEvent(std::shared_ptr<Event> p_event);
        
        Vector generatePath(float & p_percentage, Vector & vel, std::shared_ptr<MapHitbox> p_hitbox, std::shared_ptr<const MapElement> & p_nextElement); //TODO rename function
        
        virtual void update(std::vector<std::shared_ptr<Event>> & events);
        virtual void draw();
        
        static bool DRAW_BUMPERS; //DEBUGGING
        
    protected:
        World & removeObject(std::shared_ptr<WorldObject> p_object);
        
        virtual World & updateObject(std::shared_ptr<WorldObject> p_object);
        
    private:
        int id,
        updateTimestamp = 0,
        drawTimestamp = 0;
        
        bool verticalWorld;
        
        Map map;
        double timeRatio = 1; //TODO stuff with this
        
        std::vector<std::shared_ptr<WorldObject>> allObjects,
        objectsToDelete,
        objectsToUpdate[MAX_PRIORITY + 1],
        objectsToDraw[MAX_BELOW_ALTITUDE + MAX_ABOVE_ALTITUDE + 1];
        
        struct EventPairing {
            std::string eventTag;
            std::vector<std::shared_ptr<WorldObject>> subscribedObjects;
            
            inline bool operator==(const EventPairing & other) const { return (eventTag == other.eventTag); }
        };
        std::vector<EventPairing> subscribedObjects;
    };
}

#endif /* Level_hpp */

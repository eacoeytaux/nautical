//
//  GameManager.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/23/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <vector>

#include "Event.hpp"

namespace nautical {
    class GameManager {
    public:
        static bool startup();
        static bool shutdown();
        
        static void run();
        static void switchPause();
        static void setPause(bool pause);
        
    private:
        static bool init, //prevents game from re-initialized unnecessarily
        running; //whether or not game is running
        
        static void pollEvents(std::vector<Event*> & events);
        
        static void runTests(); //this function for debugging only, put test code in here so it can be easily removed later
    };
}

#endif /* GameManager_hpp */

//
//  Event.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Event_hpp
#define Event_hpp

#include "Tagable.hpp"

namespace nautical {
    const std::string EVENT_TAG = "event";
    
    class Event : public Tagable {
    public:
        Event() { appendTag(EVENT_TAG); }
        Event(std::string tag) {
            appendTag(EVENT_TAG);
            appendTag(tag);
        }
        virtual ~Event() { }
    };
}

#endif /* Event_hpp */

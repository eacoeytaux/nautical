//
//  MouseEvent.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/2/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef MouseEvent_hpp
#define MouseEvent_hpp

#include "Event.hpp"

namespace nautical {
    const std::string MOUSE_EVENT_TAG = "mouse_event";
    
    class MouseEvent : public Event {
    public:
        enum MouseAction {
            UNDEFINED_MOUSE_ACTION,
            LEFT_BUTTON_PRESS,
            LEFT_BUTTON_RELEASE,
            RIGHT_BUTTON_PRESS,
            RIGHT_BUTTON_RELEASE
        };
        
        MouseEvent(MouseAction action = UNDEFINED_MOUSE_ACTION);
        virtual ~MouseEvent();
        
        MouseAction getAction() const;
        MouseEvent & setAction(MouseAction action);
        
    private:
        MouseAction action;
    };
}

#endif /* MouseEvent_hpp */

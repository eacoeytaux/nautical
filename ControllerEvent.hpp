//
//  ControllerEvent.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 6/19/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef ControllerEvent_hpp
#define ControllerEvent_hpp

#include "Event.hpp"

#include "Angle.hpp"

namespace nautical {
    const std::string CONTROLLER_EVENT_TAG = "controller_event";
    
    class ControllerEvent : public Event {
    public:
        enum ControllerAction {
            UNDEFINED_CONTROLLER_ACTION,
            BUTTON_PRESS,
            BUTTON_RELEASE,
            HAT_VALUE_CHANGE,
            JOYSTICK_MOVEMENT
        };
        
        ControllerEvent(int controllerIndex, ControllerAction action = UNDEFINED_CONTROLLER_ACTION);
        virtual ~ControllerEvent();
        
        int getControllerIndex() const;
        void setControllerIndex(int index);
        
        ControllerAction getAction() const;
        void setAction(ControllerAction action);
        
        //for buttons
        int getButtonIndex() const;
        void setButtonIndex(int index);
        
        //for hat
        bool isUpPressed() const;
        bool isRightPressed() const;
        bool isDownPressed() const;
        bool isLeftPressed() const;
        void setUpPressed(bool b);
        void setRightPressed(bool b);
        void setDownPressed(bool b);
        void setLeftPressed(bool b);
        
        //for joystick
        int getJoystickIndex() const;
        void setJoystickIndex(int index);
        bool isOutsideDeadzone() const;
        void setOutsideDeadzone(bool b);
        Angle getJoystickAngle() const;
        void setJoystickAngle(Angle angle);
        
    private:
        int controllerIndex; //which controller is creating event
        ControllerAction action;
        
        //for buttons
        int buttonIndex = -1;
        
        //for ps4 controller:
        //  0: square
        //  1: x
        //  2: circle
        //  3: triangle
        //  4: l1
        //  5: r1
        //  6: l2
        //  7: r2
        
        //for hat
        bool upPressed = false;
        bool rightPressed = false;
        bool downPressed = false;
        bool leftPressed = false;
        
        //for joystick
        int joystickIndex = -1;
        bool outsideDeadzone = false;
        Angle joystickAngle;
    };
}

#endif /* ControllerEvent_hpp */

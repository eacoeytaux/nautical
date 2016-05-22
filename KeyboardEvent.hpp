//
//  KeyboardEvent.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/13/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef KeyboardEvent_hpp
#define KeyboardEvent_hpp

#include "Event.hpp"

namespace nautical {
    const std::string KEYBOARD_EVENT_TAG = "keyboard_event";
    
    class KeyboardEvent : public Event {
    public:
        enum KeyboardAction {
            UNDEFINED_KEYBOARD_ACTION,
            KEY_PRESSED,
            KEY_RELEASED
        };
        
        enum Key { //which key being pressed
            UNDEFINED_KEY, SPACE, RETURN, ESCAPE, TAB, LEFTARROW, RIGHTARROW, UPARROW, DOWNARROW, MINUS, PLUS, PERIOD, COMMA, SLASH, LEFTCONTROL, RIGHTCONTROL, LEFTSHIFT, RIGHTSHIFT, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24
        };
        
        KeyboardEvent(KeyboardAction action = UNDEFINED_KEYBOARD_ACTION, Key key = UNDEFINED_KEY);
        virtual ~KeyboardEvent();
        
        KeyboardAction getAction() const;
        KeyboardEvent & setAction(KeyboardAction action);
        Key getKey() const;
        KeyboardEvent & setKey(Key key);
        
    private:
        KeyboardAction action;
        Key key;
    };
}

#endif /* KeyboardEvent_hpp */

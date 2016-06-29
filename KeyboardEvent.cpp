//
//  KeyboardEvent.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/13/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "KeyboardEvent.hpp"

using namespace nautical;

KeyboardEvent::KeyboardEvent(Key key, KeyboardAction action) :
key(key),
action(action) {
    appendTag(KEYBOARD_EVENT_TAG);
}

KeyboardEvent::~KeyboardEvent() { }

KeyboardEvent::Key KeyboardEvent::getKey() const {
    return key;
}

KeyboardEvent & KeyboardEvent::setKey(Key key) {
    this->key = key;
    return *this;
}

KeyboardEvent::KeyboardAction KeyboardEvent::getAction() const {
    return action;
}

KeyboardEvent & KeyboardEvent::setAction(KeyboardAction action) {
    this->action = action;
    return *this;
}

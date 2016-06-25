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

void KeyboardEvent::setKey(Key key) {
    this->key = key;
}

KeyboardEvent::KeyboardAction KeyboardEvent::getAction() const {
    return action;
}

void KeyboardEvent::setAction(KeyboardAction action) {
    this->action = action;
}

//
//  MouseEvent.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/2/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "MouseEvent.hpp"

using namespace nautical;

MouseEvent::MouseEvent(MouseAction action) : action(action) {
    appendTag(MOUSE_EVENT_TAG);
}

MouseEvent::~MouseEvent() { }

MouseEvent::MouseAction MouseEvent::getAction() const {
    return action;
}

MouseEvent & MouseEvent::setAction(MouseAction action) {
    this->action = action;
    return *this;
}

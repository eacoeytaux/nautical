//
//  MouseEvent.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/2/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "MouseEvent.hpp"

#include "GraphicsManager.hpp"

using namespace nautical;

MouseEvent::MouseEvent(Coordinate mousePosScreen, MouseAction action) :
mousePosScreen(mousePosScreen),
action(action) {
    appendTag(MOUSE_EVENT_TAG);
}

MouseEvent::~MouseEvent() { }

Coordinate MouseEvent::getMousePos() const {
    return GraphicsManager::screenToWorld(mousePosScreen);
}

MouseEvent & MouseEvent::setMousePosScreen(Coordinate mousePosScreen) {
    mousePosScreen = mousePosScreen;
    return *this;
}

MouseEvent::MouseAction MouseEvent::getAction() const {
    return action;
}

MouseEvent & MouseEvent::setAction(MouseAction action) {
    this->action = action;
    return *this;
}

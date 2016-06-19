//
//  ControllerEvent.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 6/19/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "ControllerEvent.hpp"

using namespace nautical;

ControllerEvent::ControllerEvent(int controllerIndex, ControllerAction action) :
controllerIndex(controllerIndex),
action(action) {
    appendTag(CONTROLLER_EVENT_TAG);
}

ControllerEvent::~ControllerEvent() { }

int ControllerEvent::getControllerIndex() const {
    return controllerIndex;
}

ControllerEvent & ControllerEvent::setControllerIndex(int index) {
    controllerIndex = index;
    return *this;
}

ControllerEvent::ControllerAction ControllerEvent::getAction() const {
    return action;
}

ControllerEvent & ControllerEvent::setAction(ControllerAction action) {
    this->action = action;
    return *this;
}

//for buttons
int ControllerEvent::getButtonIndex() const {
    return buttonIndex;
}

ControllerEvent & ControllerEvent::setButtonIndex(int index) {
    buttonIndex = index;
    return *this;
}

//for hat
bool ControllerEvent::isUpPressed() const {
    return upPressed;
}

bool ControllerEvent::isRightPressed() const {
    return rightPressed;
}

bool ControllerEvent::isDownPressed() const {
    return downPressed;
}

bool ControllerEvent::isLeftPressed() const {
    return leftPressed;
}

ControllerEvent & ControllerEvent::setUpPressed(bool b) {
    upPressed = b;
    return *this;
}

ControllerEvent & ControllerEvent::setRightPressed(bool b) {
    rightPressed = b;
    return *this;
}

ControllerEvent & ControllerEvent::setDownPressed(bool b) {
    downPressed = b;
    return *this;
}

ControllerEvent & ControllerEvent::setLeftPressed(bool b) {
    leftPressed = b;
    return *this;
}

//for joystick
int ControllerEvent::getJoystickIndex() const {
    return joystickIndex;
}

ControllerEvent & ControllerEvent::setJoystickIndex(int index) {
    joystickIndex = index;
    return *this;
}

bool ControllerEvent::isOutsideDeadzone() const {
    return outsideDeadzone;
}

ControllerEvent & ControllerEvent::setOutsideDeadzone(bool b) {
    outsideDeadzone = b;
    return *this;
}

Angle ControllerEvent::getJoystickAngle() const {
    return joystickAngle;
}

ControllerEvent & ControllerEvent::setJoystickAngle(Angle angle) {
    this->joystickAngle = angle;
    return *this;
}

double ControllerEvent::getJoystickMagnitude() const {
    return joystickMagnitude;
}

ControllerEvent & ControllerEvent::setJoystickMagnitude(double magnitude) {
    joystickMagnitude = magnitude;
    return *this;
}

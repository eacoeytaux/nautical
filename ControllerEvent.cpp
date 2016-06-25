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

void ControllerEvent::setControllerIndex(int index) {
    controllerIndex = index;
}

ControllerEvent::ControllerAction ControllerEvent::getAction() const {
    return action;
}

void ControllerEvent::setAction(ControllerAction action) {
    this->action = action;
}

//for buttons
int ControllerEvent::getButtonIndex() const {
    return buttonIndex;
}

void ControllerEvent::setButtonIndex(int index) {
    buttonIndex = index;
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

void ControllerEvent::setUpPressed(bool b) {
    upPressed = b;
}

void ControllerEvent::setRightPressed(bool b) {
    rightPressed = b;
}

void ControllerEvent::setDownPressed(bool b) {
    downPressed = b;
}

void ControllerEvent::setLeftPressed(bool b) {
    leftPressed = b;
}

//for joystick
int ControllerEvent::getJoystickIndex() const {
    return joystickIndex;
}

void ControllerEvent::setJoystickIndex(int index) {
    joystickIndex = index;
}

bool ControllerEvent::isOutsideDeadzone() const {
    return outsideDeadzone;
}

void ControllerEvent::setOutsideDeadzone(bool b) {
    outsideDeadzone = b;
}

Angle ControllerEvent::getJoystickAngle() const {
    return joystickAngle;
}

void ControllerEvent::setJoystickAngle(Angle angle) {
    this->joystickAngle = angle;
}

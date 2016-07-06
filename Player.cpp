//
//  Player.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/16/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Player.hpp"

#include "ControllerEvent.hpp"
#include "KeyboardEvent.hpp"
#include "MouseEvent.hpp"
#include "MapHitboxRectangle.hpp"

using namespace nautical;
using namespace climber;

Player::Player(Coordinate pos) : Mob(pos) {
    appendTag(PLAYER_TAG);
    subscribeEvent(CONTROLLER_EVENT_TAG);
    subscribeEvent(KEYBOARD_EVENT_TAG);
    subscribeEvent(MOUSE_EVENT_TAG);
    
    MapHitboxRectangle * p_hitbox = new MapHitboxRectangle(Rectangle(pos, 60, 80)); //TODO does this need to be 3 lines?
    setMapHitbox(p_hitbox);
    delete p_hitbox;
    
    trap = Rectangle(getCenter(), 30, 160);
    trap.setColor(Color(YELLOW).setA(127));
}

Player::~Player() { }

Player & Player::setMapElement(const nautical::MapElement * p_element) {
    WorldObject::setMapElement(p_element);
    if (p_element) {
        setCanJump(true);
    } else {
        setCanJump(false);
    }
    return *this;
}

Rope * Player::getRope() {
    return p_rope;
}

Player & Player::setRope(Rope * p_rope) {
    if (p_rope)
        delete p_rope;
    this->p_rope = p_rope;
    return *this;
}

bool Player::isFacingRight() const {
    return facingRight;
}

Player & Player::setFacingRight(bool facingRight) {
    this->facingRight = facingRight;
    return *this;
}

bool Player::isMovingRight() const {
    return movingRight;
}

Player & Player::setMovingRight(bool movingRight) {
    Logger::writeLog(PLAIN_MESSAGE, "Player::setMovingRight(): movingRight set to %s", movingRight ? "true" : "false");
    this->movingRight = movingRight;
    return *this;
}

bool Player::isMovingLeft() const {
    return movingLeft;
}

Player & Player::setMovingLeft(bool movingLeft) {
    Logger::writeLog(PLAIN_MESSAGE, "Player::setMovingRight(): movingLeft set to %s", movingLeft ? "true" : "false");
    this->movingLeft = movingLeft;
    return *this;
}

bool Player::isJumping() const {
    return jumping;
}

Player & Player::setJumping(bool b) {
    if (b) {
        if (!jumping)
            jumpValueIndex = 0;
        setCanJump(false);
        setCanGhostJump(false);
    }
    jumping = b;
    return *this;
}

bool Player::canJump() const {
    return jumpCapable;
}

Player & Player::setCanJump(bool b) {
    if (jumpCapable && !b) {
        ghostJumpCapable = true;
        ghostJumpCountdown.reset(15);
    }
    jumpCapable = b;
    return *this;
}

bool Player::canGhostJump() const {
    return ghostJumpCapable;
}

Player & Player::setCanGhostJump(bool b) {
    ghostJumpCapable = b;
    return *this;
}


Player & Player::move(physics::Vector vec) {
    // move camera using trap
    physics::Vector vecHorizontal = physics::Vector(vec);
    vecHorizontal.subtractAngle(M_PI_2);
    vecHorizontal.subtractAngle(-M_PI_2);
    
    physics::Vector vecVertical = physics::Vector(vec);
    vecVertical.subtractAngle(0);
    vecVertical.subtractAngle(M_PI);
    
    Coordinate newCenterHorizontal = getCenter() + vecHorizontal;
    Coordinate newCenterVertical = getCenter() + vecVertical;
    
    if (!trap.getRightLine().isOnOrBelow(newCenterHorizontal) || !trap.getLeftLine().isOnOrBelow(newCenterHorizontal)) {
        GraphicsManager::setCenter(GraphicsManager::getCenterSet() + vecHorizontal);
        trap.move(vecHorizontal);
    }
    if (!trap.getTopLine().isOnOrBelow(newCenterVertical) || !trap.getBottomLine().isOnOrBelow(newCenterVertical)) {
        GraphicsManager::setCenter(GraphicsManager::getCenterSet() + vecVertical);
        trap.move(vecVertical);
    }
    // end trap code
    
    WorldObject::move(vec);
    
    return *this;
}

bool Player::handleEvent(Event * p_event) {
    if (p_event->hasTag(CONTROLLER_EVENT_TAG)) {
        ControllerEvent * p_controllerEvent = static_cast<ControllerEvent*>(p_event);
        
        if (p_controllerEvent->getControllerIndex() != 0)
            return false;
        
        switch (p_controllerEvent->getAction()) {
            case ControllerEvent::JOYSTICK_MOVEMENT: {
                if (p_controllerEvent->getJoystickIndex() == 0) {
                    if (p_controllerEvent->isOutsideDeadzone()) {
                        double joystickAngleValue = p_controllerEvent->getJoystickAngle().getValue();
                        if ((joystickAngleValue > M_PI_2 + M_PI_4) || (joystickAngleValue <= -(M_PI_2 + M_PI_4))) {
                            setMovingLeft(true);
                            setMovingRight(false);
                        } else if (joystickAngleValue > M_PI_4) {
                            setMovingLeft(false);
                            setMovingRight(false);
                        } else if (joystickAngleValue > -M_PI_4) {
                            setMovingLeft(false);
                            setMovingRight(true);
                        } else {
                            setMovingLeft(false);
                            setMovingRight(false);
                        }
                        break;
                    } else {
                        setMovingLeft(false);
                        setMovingRight(false);
                    }
                    
                    return true;
                } else if (p_controllerEvent->getJoystickIndex() == 1) {
                    aimAngle = p_controllerEvent->getJoystickAngle();
                    return true;
                }
            }
            case ControllerEvent::HAT_VALUE_CHANGE: {
                if (p_controllerEvent->isRightPressed())
                    setMovingRight(true);
                else
                    setMovingRight(false);
                
                if (p_controllerEvent->isLeftPressed())
                    setMovingLeft(true);
                else
                    setMovingLeft(false);
                
                return true;
            }
            case ControllerEvent::BUTTON_PRESS: {
                switch (p_controllerEvent->getButtonIndex()) {
                    case 5: {
                        if (jumpCapable || ghostJumpCapable) {
                            setJumping(true);
                            return true;
                        }
                        break;
                    }
                    case 7: {
                        if (!p_rope) {
                            p_rope = new Rope(this, getCenter(), 250, getVel() + physics::Vector(aimAngle, 10), 20); //TODO add current velocity of player to rope's velocity
                            getParent()->addObject(p_rope);
                            Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): p_rope set to EXTENDING at angle %f degrees", Angle::radiansToDegrees(aimAngle.getValue()));
                            return true;
                        }
                        return true;
                    }
                }
                break;
            }
            case ControllerEvent::BUTTON_RELEASE: {
                switch (p_controllerEvent->getButtonIndex()) {
                    case 5: {
                        setJumping(false);
                        break;
                    }
                    case 7: {
                        if (p_rope) {
                            p_rope->setState(Rope::RETRACTING);
                            Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): p_rope set to RETRACTING");
                            return true;
                        }
                    }
                }
                break;
            }
            default:
                break;
        }
    } else if (p_event->hasTag(KEYBOARD_EVENT_TAG)) {
        KeyboardEvent * p_keyboardEvent = static_cast<KeyboardEvent*>(p_event);
        
        switch (p_keyboardEvent->getKey()) {
            case KeyboardEvent::A:
            case KeyboardEvent::LEFTARROW: {
                switch (p_keyboardEvent->getAction()) {
                    case KeyboardEvent::KEY_PRESSED:
                        setMovingLeft(true);
                        return true;
                    default:
                        setMovingLeft(false);
                        return true;
                }
                break;
            }
            case KeyboardEvent::D:
            case KeyboardEvent::RIGHTARROW: {
                switch (p_keyboardEvent->getAction()) {
                    case KeyboardEvent::KEY_PRESSED:
                        setMovingRight(true);
                        return true;
                    default:
                        setMovingRight(false);
                        return true;
                }
                break;
            }
            case KeyboardEvent::SPACE: {
                switch (p_keyboardEvent->getAction()) {
                    case KeyboardEvent::KEY_PRESSED:
                        if (jumpCapable || ghostJumpCapable) {
                            setJumping(true);
                            return true;
                        }
                        break;
                    default:
                        setJumping(false);
                        return true;
                }
            }
            default:
                break;
        }
    } else if (p_event->hasTag(MOUSE_EVENT_TAG)) {
        MouseEvent * p_mouseEvent = static_cast<MouseEvent*>(p_event);
        
        switch (p_mouseEvent->getAction()) {
            case MouseEvent::MOUSE_MOVEMENT: {
                Coordinate target = p_mouseEvent->getMousePos();
                aimAngle = findAngle(getCenter(), target);
                break;
            }
            case MouseEvent::LEFT_BUTTON_PRESS: {
                if (!p_rope) {
                    p_rope = new Rope(this, getCenter(), 250, getVel() + physics::Vector(aimAngle, 10), 20); //TODO add current velocity of player to rope's velocity
                    getParent()->addObject(p_rope);
                    Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): p_rope set to EXTENDING at angle %f degrees", Angle::radiansToDegrees(aimAngle.getValue()));
                    return true;
                }
            }
            case MouseEvent::LEFT_BUTTON_RELEASE: {
                if (p_rope) {
                    p_rope->setState(Rope::RETRACTING);
                    Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): p_rope set to RETRACTING");
                    return true;
                }
            }
            default:
                break;
        }
    }
    return false;
}

void Player::update() {
    //update countdowns
    if (ghostJumpCountdown.check())
        setCanGhostJump(false);
    
    //add force to object
    std::shared_ptr<MapHitbox> p_hitbox = getMapHitbox();
    const MapElement * p_element = p_hitbox->getElement();
    
    if (!p_element || (p_element && !(p_element->isSticky())))
        addToForce(physics::Vector(0, -0.3));
    
    if (isMovingLeft())
        addToForce(physics::Vector(-0.3, 0));
    if (isMovingRight())
        addToForce(physics::Vector(0.3, 0));
    
    if (jumping) {
        if (jumpValueIndex >= jumpValues.size()) {
            setJumping(false);
        } else {
            addToForce(physics::Vector(0, jumpValues.at((unsigned long)jumpValueIndex++)));
        }
    }
    
    /*if (p_rope) { //TODO
     if (p_rope->isTaught() && (p_rope->getState() == Rope::SET)) {
     //F = -kx - bv
     static double k = 0.0000000001, b = 2; //k and b are constants in equation
     double x = findDistance(getCenter(), p_rope->getHead()) - p_rope->getLength(); //distance from end to end
     double v = (getVel() + getForce()).getMagnitude(); //relative velocity
     
     static double damper = 0.6;
     
     addToForce(Vector(findAngle(p_rope->getHead(), getCenter()), (-(k * x) - (b * v)) * damper));
     }
     }*/
    
    addToVel(getForce());
    
    if (p_element) {
        setVel(getVel() * p_element->getFrictionCoefficient());
    } else {
        setVel(getVel() * getParent()->getMap()->getAirResistanceCoefficient());
    }
    
    World * p_parent = getParent();
    float percentageUsed = 1.f;
    do {
        physics::Vector vel = getVel();
        physics::Vector movement = p_parent->generatePath(&percentageUsed, &vel, getMapHitbox().get(), &p_element);
        
        //check movement here
        if (p_rope) {
            if (p_rope->getState() == Rope::SET) {
                double distance = findDistance(p_rope->getHead(), movement.getDestination());
                if (distance > p_rope->getLength()) {
                    std::vector<Coordinate> intersections;
                    Circle(p_rope->getHead(), p_rope->getLength()).intersectsLine(Line(movement.getOrigin(), movement.getDestination()), &intersections);
                    if (intersections.size() > 0) {
                        //TODO stop movement
                    } else {
                        Logger::writeLog(ERROR_MESSAGE, "Player::update(): did not find collision with rope boundaries");
                    }
                }
            }
        }
        
        setMapElement(p_element);
        move(movement);
        setVel(vel);
    } while (percentageUsed > 0);
    setForce(physics::Vector(0, 0));
    
    if (p_rope)
        p_rope->setOrigin(getCenter());
    
    //lighting
    if (DarknessOverlay::isInEffect()) {
        Circle * p_circle = new Circle(getCenter(), 150);
        static Countdown count(1); //TODO make these not static
        static physics::Vector moveVec[DARKNESS_LAYERS];
        if (count.check()) {
            for (int i = 0; i < DARKNESS_LAYERS; i++) {
                moveVec[i] = physics::Vector(Angle(Random::getRandDouble(M_PI * 2)), Random::getRandDouble(2));
            }
            count.reset();
        }
        for (int i = 0; i < DARKNESS_LAYERS; i++) {
            Circle * p_circleToAdd = new Circle(*p_circle);
            p_circleToAdd->move(moveVec[i]);
            DarknessOverlay::addShape(p_circleToAdd, i);
            p_circle->setRadius(p_circle->getRadius() + 10);
        }
        delete p_circle;
    }
}

void Player::draw() const {
    if (DEBUG_MODE) {
        trap.draw();
        physics::Vector(aimAngle, 20, getCenter()).Drawable::draw(MAGENTA);
        getVel().setColor((canJump() || canGhostJump()) ? WHITE : Color(255, 191, 191)).draw(5);
    }
    getMapHitbox()->getShape()->Drawable::draw(CYAN);
}

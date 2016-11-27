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
    
    std::shared_ptr<MapHitboxRectangle> p_hitbox = std::shared_ptr<MapHitboxRectangle>(new MapHitboxRectangle(Rectangle(pos, 60, 80))); //TODO does this need to be 2 lines?
    setMapHitbox(p_hitbox);
    
    trap = Rectangle(getPosition(), 30, 160);
    trap.setColor(Color(YELLOW).setA(127));
}

Player::~Player() { }

Player & Player::setMapElement(std::shared_ptr<const MapElement> p_element) {
    WorldObject::setMapElement(p_element);
    if (p_element) {
        setCanJump(true);
    } else {
        setCanJump(false);
    }
    return *this;
}

std::shared_ptr<GrapplingRope> Player::getRope() {
    return p_rope;
}

Player & Player::setRope(std::shared_ptr<GrapplingRope> p_rope) {
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
    Logger::writeLog(INFO, "Player::setMovingRight(): movingRight set to %s", movingRight ? "true" : "false");
    this->movingRight = movingRight;
    return *this;
}

bool Player::isMovingLeft() const {
    return movingLeft;
}

Player & Player::setMovingLeft(bool movingLeft) {
    Logger::writeLog(INFO, "Player::setMovingRight(): movingLeft set to %s", movingLeft ? "true" : "false");
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


Player & Player::movePosition(Vector vec) {
    // move camera using trap
    Vector vecHorizontal = Vector(vec);
    vecHorizontal.subtractAngle(M_PI_2);
    vecHorizontal.subtractAngle(-M_PI_2);
    
    Vector vecVertical = Vector(vec);
    vecVertical.subtractAngle(0);
    vecVertical.subtractAngle(M_PI);
    
    Coordinate newCenterHorizontal = getPosition() + vecHorizontal;
    Coordinate newCenterVertical = getPosition() + vecVertical;
    
    if (!trap.getRightLine().isOnOrBelow(newCenterHorizontal) || !trap.getLeftLine().isOnOrBelow(newCenterHorizontal)) {
        GraphicsManager::setCenter(GraphicsManager::getCenterTarget() + vecHorizontal);
        trap.move(vecHorizontal);
    }
    if (!trap.getTopLine().isOnOrBelow(newCenterVertical) || !trap.getBottomLine().isOnOrBelow(newCenterVertical)) {
        GraphicsManager::setCenter(GraphicsManager::getCenterTarget() + vecVertical);
        trap.move(vecVertical);
    }
    // end trap code
    
    WorldObject::movePosition(vec);
    
    return *this;
}

bool Player::handleEvent(std::shared_ptr<Event> p_event) {
    if (p_event->hasTag(CONTROLLER_EVENT_TAG)) {
        std::shared_ptr<ControllerEvent> p_controllerEvent = std::static_pointer_cast<ControllerEvent>(p_event);
        
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
                            p_rope = std::shared_ptr<GrapplingRope>(new GrapplingRope(getPosition(), 250, getVelocity() + Vector(aimAngle, 10), 20)); //TODO add current velocity of player to rope's velocity
                            Logger::writeLog(INFO, "Player::handleEvent(): p_rope set to EXTENDING at angle %f degrees", Angle::radiansToDegrees(aimAngle.getValue()));
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
                            p_rope->setState(GrapplingRope::RETRACTING);
                            Logger::writeLog(INFO, "Player::handleEvent(): p_rope set to RETRACTING");
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
        std::shared_ptr<KeyboardEvent> p_keyboardEvent = std::static_pointer_cast<KeyboardEvent>(p_event);
        
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
        std::shared_ptr<MouseEvent> p_mouseEvent = std::static_pointer_cast<MouseEvent>(p_event);
        
        switch (p_mouseEvent->getAction()) {
            case MouseEvent::MOUSE_MOVEMENT: {
                Coordinate target = p_mouseEvent->getMousePos();
                aimAngle = findAngle(getPosition(), target);
                break;
            }
            case MouseEvent::LEFT_BUTTON_PRESS: {
                if (!p_rope) {
                    p_rope = std::shared_ptr<GrapplingRope>(new GrapplingRope(getPosition(), 250, getVelocity() + Vector(aimAngle, 10), 20)); //TODO add current velocity of player to rope's velocity
                    Logger::writeLog(INFO, "Player::handleEvent(): p_rope set to EXTENDING at angle %f degrees", Angle::radiansToDegrees(aimAngle.getValue()));
                    return true;
                }
            }
            case MouseEvent::LEFT_BUTTON_RELEASE: {
                if (p_rope) {
                    p_rope->setState(GrapplingRope::RETRACTING);
                    Logger::writeLog(INFO, "Player::handleEvent(): p_rope set to RETRACTING");
                    return true;
                }
            }
            default:
                break;
        }
    }
    return false;
}

void Player::update(World * p_world, double dt) { //TODO implement dt
    //update countdowns
    if (ghostJumpCountdown.check())
        setCanGhostJump(false);
    
    //add force to object
    //std::shared_ptr<MapHitbox> p_hitbox = getMapHitbox();
    std::shared_ptr<const MapElement> p_element = p_hitbox->getElement();
    
    if (!p_element || (p_element && !(p_element->isSticky()))) {
        addAccelerator(std::shared_ptr<Mass::Accelerator>(new Mass::ForceAccelerator(Vector(0, -0.3)))); //TODO change to world gravity
    }
    if (isMovingLeft()) {
        addAccelerator(std::shared_ptr<Mass::Accelerator>(new Mass::ForceAccelerator(Vector(-0.3, 0))));
    }
    if (isMovingRight()) {
        addAccelerator(std::shared_ptr<Mass::Accelerator>(new Mass::ForceAccelerator(Vector(0.3, 0))));
    }
    
    if (jumping) {
        if (jumpValueIndex >= jumpValues.size()) {
            setJumping(false);
        } else {
            addAccelerator(std::shared_ptr<Mass::Accelerator>(new Mass::ForceAccelerator(Vector(0, jumpValues.at((unsigned long)jumpValueIndex++)))));
        }
    }
    
    std::pair<Coordinate, Vector> updated = Mass::update(dt);
    
    //TODO TODO TODO addToVel(getForce());
    
//    Vector vel = updated.second;
    
    if (p_element) {
        setVelocity(vel * p_element->getFrictionCoefficient());
    } else {
        setVelocity(vel * p_world->getMap().getAirResistanceCoefficient());
    }
    
    float percentageUsed = 1.f;
    do {
        Vector tempVel = vel * dt;
        Vector movement = p_world->generatePath(percentageUsed, tempVel, p_hitbox, p_element);
        
        //check movement here
        if (p_rope) {
            if (p_rope->getState() == GrapplingRope::SET) {
                double distance = findDistance(p_rope->getHead(), movement.getDestination());
                if (distance > p_rope->getLength()) {
                    std::vector<Coordinate> intersections;
                    Circle(p_rope->getHead(), p_rope->getLength()).intersectsLine(Line(movement.getOrigin(), movement.getDestination()), &intersections);
                    if (intersections.size() > 0) {
                        //TODO stop movement
                    } else {
                        Logger::writeLog(ERROR, "Player::update(): did not find collision with rope boundaries");
                    }
                }
            }
        }
        
        setMapElement(p_element);
        movePosition(movement);
        setVelocity(tempVel / dt);
    } while (percentageUsed > 0);
    
    //setForce(Vector(0, 0));
    
    if (p_rope)
        p_rope->setOrigin(getPosition());
    
    //lighting
    if (DarknessOverlay::isInEffect()) {
        std::shared_ptr<Circle> p_circle = std::shared_ptr<Circle>(new Circle(getPosition(), 150));
        static Countdown count(1); //TODO make these not static
        static Vector moveVec[DARKNESS_LAYERS];
        if (count.check()) {
            for (int i = 0; i < DARKNESS_LAYERS; i++) {
                moveVec[i] = Vector(Angle(Random::getRandDouble(M_PI * 2)), Random::getRandDouble(2));
            }
            count.reset();
        }
        for (int i = 0; i < DARKNESS_LAYERS; i++) {
            std::shared_ptr<Circle> p_circleToAdd = std::shared_ptr<Circle>(new Circle(*p_circle));
            p_circleToAdd->move(moveVec[i]);
            DarknessOverlay::addShape(p_circleToAdd, i);
            p_circle->setRadius(p_circle->getRadius() + 10);
        }
    }
}

void Player::draw() const {
    if (DEBUG_MODE) {
        trap.draw();
        Vector(aimAngle, 20, getPosition()).Drawable::draw(MAGENTA);
        getVelocity().setColor((canJump() || canGhostJump()) ? WHITE : Color(255, 191, 191)).draw(5);
    }
    getMapHitbox()->getShape()->Drawable::draw(CYAN);
}

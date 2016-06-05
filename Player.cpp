//
//  Player.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/16/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Player.hpp"

#include "KeyboardEvent.hpp"
#include "MouseEvent.hpp"

#include "DarknessOverlay.hpp" //TODO delete
#include "Random.hpp" //TODO delete
#include "Countdown.hpp" //TODO delete

using namespace nautical;
using namespace climber;

Player::Player(Coordinate pos) : Mob(pos) {
    subscribeEvent(KEYBOARD_EVENT_TAG);
    subscribeEvent(MOUSE_EVENT_TAG);
    setMapWidth(60);
    setMapHeight(80);
    setMapAngle(Angle::degreesToRadians(0));
    trap = Rectangle(getCenter(), getMapWidth() / 2, getMapHeight() * 2, getMapAngle());
    trap.setColor(Color(YELLOW).setA(127));
}

Player::~Player() { }

Rope * Player::getRope() {
    return p_rope;
}

Player & Player::setRope(Rope * p_rope) {
    this->p_rope = p_rope;
    return *this;
}

Player & Player::move(Vector vec) {
    /* move camera using trap */
    Vector vecHorizontal = Vector(vec);
    vecHorizontal.subtractAngle(getMapAngle() + M_PI_2);
    vecHorizontal.subtractAngle(getMapAngle() - M_PI_2);
    
    Vector vecVertical = Vector(vec);
    vecVertical.subtractAngle(getMapAngle());
    vecVertical.subtractAngle(getMapAngle() + M_PI);
    
    Coordinate newCenterHorizontal = getCenter() + vecHorizontal;
    Coordinate newCenterVertical = getCenter() + vecVertical;
    
    if (!trap.contains(newCenterHorizontal)) {
        GraphicsManager::setCenter(GraphicsManager::getCenterSet() + vecHorizontal);
        trap.move(vecHorizontal);
    }
    if (!trap.contains(newCenterVertical)) {
        GraphicsManager::setCenter(GraphicsManager::getCenterSet() + vecVertical);
        trap.move(vecVertical);
    }
    /* end trap code */
    
    WorldObject::move(vec);
    return *this;
}

bool Player::handleEvent(Event * p_event) {
    if (p_event->hasTag(KEYBOARD_EVENT_TAG)) {
        KeyboardEvent * p_keyboardEvent = static_cast<KeyboardEvent*>(p_event);
        
        switch (p_keyboardEvent->getKey()) {
            case KeyboardEvent::A:
            case KeyboardEvent::LEFTARROW: {
                switch (p_keyboardEvent->getAction()) {
                    case nautical::KeyboardEvent::KEY_PRESSED:
                        setMovingLeft(true);
                        return true;
                    default:
                        setMovingLeft(false);
                        return true;
                }
            } case KeyboardEvent::D:
            case KeyboardEvent::RIGHTARROW: {
                switch (p_keyboardEvent->getAction()) {
                    case nautical::KeyboardEvent::KEY_PRESSED:
                        setMovingRight(true);
                        return true;
                    default:
                        setMovingRight(false);
                        return true;
                }
            }
            default:
                break;
        }
    } else if (p_event->hasTag(MOUSE_EVENT_TAG)) {
        MouseEvent * p_mouseEvent = static_cast<MouseEvent*>(p_event);
        
        switch (p_mouseEvent->getAction()) {
            case MouseEvent::LEFT_BUTTON_PRESS:
                if (!p_rope) {
                    p_rope = new Rope(this, getCenter(), 250, findAngle(getCenter(), GraphicsManager::screenToWorld(GraphicsManager::getMouseCoor())), 10, 20);
                    getParent()->addObject(p_rope);
                    return true;
                }
            case MouseEvent::LEFT_BUTTON_RELEASE:
                if (p_rope) {
                    p_rope->setState(Rope::RETRACTING);
                    return true;
                }
            default:
                break;
        }
    }
    return false;
}

void Player::update() {
    if (!getMapElement() || (getMapElement() && !(getMapElement()->isSticky())))
        setForce(Vector(0, -0.3));
    else
        setForce(Vector(0, 0));
    
    if (isMovingLeft())
        addToForce(Vector(-0.3, 0));
    if (isMovingRight())
        addToForce(Vector(0.3, 0));
    
    if (p_rope) {
        //if (p_rope->getState() == Rope::TAUGHT)
        //addToForce(Vector(findAngle(getCenter(), p_rope->getHead()), (findDistance(getCenter(), p_rope->getHead()) - p_rope->getLength()) * 2));
    }
    
    addToVel(getForce());
    
    if (getMapElement()) {
        setVel(getVel() * getMapElement()->getFrictionCoefficient());
    } else {
        setVel(getVel() * getParent()->getMap()->getAirResistanceCoefficient());
    }
    
    getParent()->generatePath(this);
    
    if (p_rope)
        p_rope->setOrigin(getCenter());
    
    //lighting
    if (getParent()->isDarknessInEffect()) {
        Circle * p_circle = new Circle(getCenter(), 150);
        static Countdown count(1);
        static Vector moveVec[DARKNESS_LAYERS];
        if (count.check()) {
            for (int i = 0; i < DARKNESS_LAYERS; i++) {
                moveVec[i] = Vector(Angle(Random::getRandFloat(M_PI * 2)), Random::getRandFloat(2));
            }
            count.reset();
        }
        for (int i = 0; i < DARKNESS_LAYERS; i++) {
            Circle * p_circleToAdd = new Circle(*p_circle);
            p_circleToAdd->move(moveVec[i]);
            getParent()->addShapeToDarknessOverlay(p_circleToAdd, i);
            p_circle->setRadius(p_circle->getRadius() + 10);
        }
    }
}

void Player::draw() const {
    getHitbox().draw();
    if (nautical::DEBUG_MODE) {
        getVel().draw(5);
        trap.draw();
    }
}


//
//  Player.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/16/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Player.hpp"

#include "Random.hpp"
#include "Countdown.hpp"
#include "KeyboardEvent.hpp"
#include "MouseEvent.hpp"
#include "DarknessOverlay.hpp"
#include "MapHitboxRectangle.hpp"

using namespace nautical;
using namespace climber;

Player::Player(Coordinate pos) : Mob(pos) {
    appendTag(PLAYER_TAG);
    subscribeEvent(KEYBOARD_EVENT_TAG);
    subscribeEvent(MOUSE_EVENT_TAG);
    
    MapHitboxRectangle * p_hitbox = new MapHitboxRectangle(Rectangle(pos, 60, 80));
    setMapHitbox(p_hitbox);
    delete p_hitbox;
    
    trap = Rectangle(getCenter(), 30, 160);
    trap.setColor(Color(YELLOW).setA(127));
}

Player::~Player() { }

Rope * Player::getRope() {
    return p_rope;
}

Player & Player::setRope(Rope * p_rope) {
    if (p_rope)
        delete p_rope;
    this->p_rope = p_rope;
    return *this;
}

Player & Player::move(Vector vec) {
    /* move camera using trap */
    Vector vecHorizontal = Vector(vec);
    vecHorizontal.subtractAngle(M_PI_2);
    vecHorizontal.subtractAngle(M_PI_2);
    
    Vector vecVertical = Vector(vec);
    vecVertical.subtractAngle(0);
    vecVertical.subtractAngle(M_PI);
    
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
                        if (!isMovingLeft())
                            Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): movingLeft set to true");
                        setMovingLeft(true);
                        return true;
                    default:
                        if (isMovingLeft())
                            Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): movingLeft set to false");
                        setMovingLeft(false);
                        return true;
                }
            } case KeyboardEvent::D:
            case KeyboardEvent::RIGHTARROW: {
                switch (p_keyboardEvent->getAction()) {
                    case nautical::KeyboardEvent::KEY_PRESSED:
                        if (!isMovingRight())
                            Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): movingRight set to true");
                        setMovingRight(true);
                        return true;
                    default:
                        if (isMovingRight())
                            Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): movingRight set to false");
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
                    Coordinate target = GraphicsManager::screenToWorld(GraphicsManager::getMouseCoor());
                    p_rope = new Rope(this, getCenter(), 250, findAngle(getCenter(), target), 10, 20); //TODO add current velocity of player to rope's velocity
                    getParent()->addObject(p_rope);
                    Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): p_rope set to EXTENDING, target set to (%f:%f)", target.getX(), target.getY());
                    return true;
                }
            case MouseEvent::LEFT_BUTTON_RELEASE:
                if (p_rope) {
                    p_rope->setState(Rope::RETRACTING);
                    Logger::writeLog(PLAIN_MESSAGE, "Player::handleEvent(): p_rope set to RETRACTING");
                    return true;
                }
            default:
                break;
        }
    }
    return false;
}

void Player::update() {
    MapHitbox * p_hitbox = getMapHitbox();
    const MapElement * p_element = p_hitbox->getElement();
    
    if (!p_element || (p_element && !(p_element->isSticky())))
        setForce(Vector(0, -0.3));
    else
        setForce(Vector(0, 0));
    
    if (isMovingLeft())
        addToForce(Vector(-0.3, 0));
    if (isMovingRight())
        addToForce(Vector(0.3, 0));
    
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
        Vector vel = getVel();
        MapHitbox * p_hitbox = getMapHitbox();
        Vector movement = p_parent->generatePath(&percentageUsed, &vel, p_hitbox, &p_element);
        delete p_hitbox;
        
        //check movement here
        if (p_rope) {
            if (p_rope->getState() == Rope::SET) {
                double distance = findDistance(p_rope->getHead(), movement.getDestination());
                if (distance > p_rope->getLength()) {
                    Queue<Coordinate> intersections;
                    Circle(p_rope->getHead(), p_rope->getLength()).intersectsLine(Line(movement.getOrigin(), movement.getDestination()), &intersections);
                    Coordinate coor;
                    if (intersections.pop(&coor)) {
                        int x = 5;
                        x++;
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
    delete p_hitbox;
    
    if (p_rope)
        p_rope->setOrigin(getCenter());
    
    //lighting
    if (DarknessOverlay::isInEffect()) {
        Circle * p_circle = new Circle(getCenter(), 150);
        static Countdown count(1); //TODO make these not static
        static Vector moveVec[DARKNESS_LAYERS];
        if (count.check()) {
            for (int i = 0; i < DARKNESS_LAYERS; i++) {
                moveVec[i] = Vector(Angle(Random::getRandDouble(M_PI * 2)), Random::getRandDouble(2));
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
    Shape * p_hitboxShape = getMapHitbox()->getShape();
        p_hitboxShape->draw();
    delete p_hitboxShape;
    if (nautical::DEBUG_MODE) {
        getVel().draw(5);
        trap.draw();
    }
}

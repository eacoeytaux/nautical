//
//  Rope.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/5/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Rope.hpp"

#include "Circle.hpp"
#include "Map.hpp"

using namespace nautical;
using namespace climber;

Rope::Rope(Coordinate * p_origin, Coordinate * p_head, double length) :
StateMachine(EXTENDING),
p_origin(p_origin),
p_head(p_head),
length(length) { }

Rope::~Rope() { }

Coordinate Rope::getOrigin() const {
    return *p_origin;
}

Coordinate Rope::getHead() const {
    return *p_head;
}

double Rope::getLength() const {
    return length;
}

void Rope::setLength(double length) {
    this->length = length;
}

void Rope::update() {
    if (findDistance(*p_origin, *p_head) > (length - 0.0001)) {
        setState(TAUGHT);
        Angle angle = findAngle(*p_origin, *p_head);
        *p_head = *p_origin + Vector(angle, length);
        
        //TODO ensure rope is of proper length by adjusting p_head
    }
    
    switch (getState()) {
        case EXTENDING: {
            wave = Path(*p_origin);
            hookAngle = findAngle(*p_origin, *p_head);
            double distance = findDistance(*p_origin, *p_head);
            Coordinate lastCoor = *p_origin;
            for (double x = 0; x < distance; x++) {
                //Coordinate coor = *p_origin + Vector(x, sin((8 * (length / distance)) * x) * ((64 * (distance - x)) / distance)); //save this, this is awesome
                double amplitude = 16 * (1 - (x / distance));// * cos(distance / 64);
                double angularFrequency = ((((length - distance) / 3) + 1) / length);
                Coordinate coor = *p_origin + Vector(x, amplitude * sin((angularFrequency * x) + (M_PI * 5))).rotate(hookAngle);
                wave.addVector(Vector(lastCoor, coor));
                lastCoor = coor;
            }
            break;
        } case RETRACTING: {
            parabola = Parabola(*p_origin, *p_head, length);
            break;
        } case SET: {
            parabola = Parabola(*p_origin, *p_head, length);
            break;
        } case TAUGHT: {
            line = Line(*p_origin, *p_head);
            break;
        }
    }
}

void Rope::draw() const {
    switch (getState()) {
        case EXTENDING: {
            wave.draw();
            break;
        } case RETRACTING: {
            //parabola.setColor(WHITE).draw(); TODO add to GraphicsManager
            break;
        } case SET: {
            Circle(*p_head, length).setColor(Color(WHITE).setA(64)).draw(); //for debugging
            //parabola.setColor(WHITE).draw();
            break;
        } case TAUGHT: {
            Circle(*p_head, length).setColor(Color(WHITE).setA(64)).draw(); //for debugging
            GraphicsManager::drawLine(line, Color(255, 153, 153));
            break;
        }
    }
    
    static int arrowLength = 5;
    Angle angle(M_PI - hookAngle.getValue());// * ((angle < 0) ? -1 : 1));
    Angle angleMinus = angle - M_PI_4;
    Angle anglePlus = angle + M_PI_4;
    GraphicsManager::drawLine(Line(p_head->getX(), p_head->getY(), p_head->getX() - angleMinus.getSin(arrowLength), p_head->getY() - angleMinus.getCos(arrowLength)), getColor());
    GraphicsManager::drawLine(Line(p_head->getX(), p_head->getY(), p_head->getX() + anglePlus.getSin(arrowLength), p_head->getY() + anglePlus.getCos(arrowLength)), getColor());
}

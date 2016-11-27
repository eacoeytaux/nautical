//
//  Slider.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/8/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Slider.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace nautical;

//Slider

Slider::Slider(double currVal, double dVal) :
currVal(currVal),
targetVal(currVal),
dVal(dVal) { }

Slider::~Slider() { }

double Slider::getValue() const {
    return currVal;
}

double Slider::getTargetValue() const {
    return targetVal;
}

double Slider::getDValue() const {
    return dVal;
}

Slider & Slider::setValue(double val, bool hardSet) {
    if (hardSet) {
        currVal = val;
    }
    targetVal = val;
    return *this;
}

Slider & Slider::setDValue(double dVal) {
    this->dVal = dVal;
    return *this;
}

void Slider::update(double dt) {
    currVal += (targetVal - currVal) * (1.0 - pow(1.0 - dVal, dt));
}

void Slider::updateFull() {
    currVal = targetVal;
}

//Slider2D

Slider2D::Slider2D(Coordinate currVal, double dVal) :
currVal(currVal),
targetVal(currVal),
dVal(dVal) { }

Slider2D::~Slider2D() { }

Coordinate Slider2D::getValue() const {
    return currVal;
}

Coordinate Slider2D::getTargetValue() const {
    return targetVal;
}

double Slider2D::getDValue() const {
    return dVal;
}

Slider2D & Slider2D::setValue(Coordinate val, bool hardSet) {
    if (hardSet) {
        currVal = val;
    }
    targetVal = val;
    return *this;
}

Slider2D & Slider2D::setDValue(double dVal) {
    this->dVal = dVal;
    return *this;
}

void Slider2D::update(double dt) {
    currVal += Vector(currVal, targetVal) * (1.0 - pow(1.0 - dVal, dt));
}

void Slider2D::updateFull() {
    currVal = targetVal;
}

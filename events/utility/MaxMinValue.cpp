//
//  MaxMinValue.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/19/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "MaxMinValue.hpp"

using namespace nautical;

MaxValue::MaxValue(double initial) :
value(initial) { }

double MaxValue::getValue() const {
    return value;
}

bool MaxValue::update(double value) {
    if (value > this->value) {
        this->value = value;
        return true;
    } else {
        return false;
    }
}

MaxValue & MaxValue::reset(double initial) {
    value = initial;
    return *this;
}

MinValue::MinValue(double initial) :
value(initial) { }

double MinValue::getValue() const {
    return value;
}

bool MinValue::update(double value) {
    if (value < this->value) {
        this->value = value;
        return true;
    } else {
        return false;
    }
}

MinValue & MinValue::reset(double initial) {
    value = initial;
    return *this;
}

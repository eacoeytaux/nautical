//
//  Angle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Angle.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include "Utility.hpp"

using namespace nautical;

Angle::Angle(double angleVal, bool truncate) :
truncate(truncate) {
    setValue(angleVal);
}

Angle::Angle(double dx, double dy, bool truncate) :
truncate(truncate) {
    setValue(atan2(dy, dx));
}

bool Angle::truncating() const {
    return truncate;
}

Angle & Angle::setTruncating(bool truncate) {
    this->truncate = truncate;
    return *this;
}

double Angle::getValue() const {
    return angleVal;
}

Angle & Angle::setValue(double angleVal) {
    if (this->angleVal != angleVal) {
        if (truncate) {
            while (angleVal > M_PI)
                angleVal -= (M_PI * 2);
            while (angleVal < -M_PI)
                angleVal += (M_PI * 2);
        }
        
        this->angleVal = angleVal;
        sinValue.setSet();
        cosValue.setSet();
    }
    angleValDegrees = radiansToDegrees(angleVal);
    return *this;
}

Angle & Angle::setValue(double dx, double dy) {
    return setValue(atan2(dy, dx));
}

double Angle::getSin(double scale) const {
    if (!sinValue.isSet())
        sinValue.setValue(sin(angleVal));
    return sinValue.getValue() * scale;
}

double Angle::getCos(double scale) const {
    if (!cosValue.isSet())
        cosValue.setValue(cos(angleVal));
    return cosValue.getValue() * scale;
}

Angle Angle::operator+(const Angle & angle) const {
    return Angle(angleVal + angle.getValue());
}

Angle & Angle::operator+=(const Angle & angle) {
    setValue(angleVal + angle.getValue());
    return *this;
}

Angle Angle::operator-(const Angle & angle) const {
    return Angle(angleVal - angle.getValue());
}

Angle & Angle::operator-=(const Angle & angle) {
    setValue(angleVal - angle.getValue());
    return *this;
}

Angle & Angle::operator=(double angleVal) {
    setValue(angleVal);
    return *this;
}

Angle Angle::operator+(const double & angleVal) const {
    return Angle(this->angleVal + angleVal);
}

Angle & Angle::operator+=(const double & angleVal) {
    setValue(this->angleVal + angleVal);
    return *this;
}

Angle Angle::operator-(const double & angleVal) const {
    return Angle(this->angleVal - angleVal);
}

Angle & Angle::operator-=(const double & angleVal) {
    setValue(this->angleVal - angleVal);
    return *this;
}

Angle Angle::operator*(const double & angleVal) const {
    return Angle(this->angleVal * angleVal);
}

Angle & Angle::operator*=(const double & angleVal) {
    setValue(this->angleVal * angleVal);
    return *this;
}

Angle Angle::operator/(const double & angleVal) const {
    return Angle(this->angleVal / angleVal);
}

Angle & Angle::operator/=(const double & angleVal) {
    setValue(this->angleVal / angleVal);
    return *this;
}

bool Angle::operator==(const Angle & angle) const {
    return (angleVal == angle.getValue());
}

bool Angle::operator!=(const Angle & angle) const {
    return !(angleVal == angle.getValue());
}

bool Angle::operator<(const Angle & angle) const {
    return angleVal < angle.getValue();
}

bool Angle::operator<=(const Angle & angle) const {
    return angleVal <= angle.getValue();
}

bool Angle::operator>(const Angle & angle) const {
    return angleVal > angle.getValue();
}

bool Angle::operator>=(const Angle & angle) const {
    return angleVal >= angle.getValue();
}

double Angle::radiansToDegrees(double radians) {
    return radians * 180 / M_PI;
}

double Angle::degreesToRadians(double degrees) {
    return degrees * M_PI / 180;
}

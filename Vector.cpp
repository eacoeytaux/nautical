//
//  Vector.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Vector.hpp"

#include "Utility.hpp"

using namespace nautical;
using namespace physics;

Vector::Vector(double dx, double dy, Coordinate origin) :
origin(origin) {
    setDx(dx);
    setDy(dy);
    angle.setValue(Angle(dx, dy));
}

Vector::Vector(Angle angle, double magnitude, Coordinate origin) :
origin(origin) {
    if (magnitude < 0) {
        magnitude *= -1;
        angle += M_PI;
    }
    
    setDx(angle.getCos(magnitude));
    setDy(angle.getSin(magnitude));
    this->angle.setValue(angle);
    this->magnitude.setValue(magnitude);
}

Vector::Vector(Coordinate coor1, Coordinate coor2) {
    this->origin = coor1;
    setDx(coor2.getX() - coor1.getX());
    setDy(coor2.getY() - coor1.getY());
}

double Vector::getDx() const {
    return dx;
}

Vector & Vector::setDx(double dx) {
    this->dx = dx;
    angle.setSet();
    magnitude.setSet();
    return *this;
}

double Vector::getDy() const {
    return dy;
}

Vector & Vector::setDy(double dy) {
    this->dy = dy;
    angle.setSet();
    magnitude.setSet();
    return *this;
}

bool Vector::isDxPositive() {
    return dx >= 0;
}

bool Vector::isDyPositive() {
    return dy >= 0;
}

Coordinate Vector::getOrigin() const {
    return origin;
}

Vector & Vector::setOrigin(Coordinate origin) {
    this->origin = origin;
    return *this;
}

Coordinate Vector::getDestination() const {
    return origin + *this;
}

Vector & Vector::setDestination(Coordinate destination) {
    setDx(destination.getX() - origin.getX());
    setDy(destination.getY() - origin.getY());
    return *this;
}

double Vector::getMagnitude() const {
    if (!magnitude.isSet())
        magnitude.setValue(findDistance(dx, dy));
    return magnitude.getValue();
}

Vector & Vector::setMagnitude(double magnitude) {
    Angle currAngle = getAngle();
    setDx(currAngle.getCos(magnitude));
    setDy(currAngle.getSin(magnitude));
    this->magnitude.setValue(magnitude);
    return *this;
}

Vector & Vector::extend(double length) {
    return setMagnitude(getMagnitude() + length);
}

Angle Vector::getAngle() const {
    if (!angle.isSet())
        angle.setValue(Angle(getDx(), getDy()));
    return angle.getValue();
}

Vector & Vector::rotateToAngle(Angle angle) {
    if (getAngle() != angle) {
        double magnitude = getMagnitude();
        setDx(angle.getCos(magnitude));
        setDy(angle.getSin(magnitude));
        this->angle.setSet();
    }
    return *this;
}

Vector & Vector::rotate(Angle angle) {
    rotateToAngle(getAngle() + angle);
    return *this;
}

Vector & Vector::mirror() {
    dx *= -1;
    dy *= -1;
    angle.setSet();
    return *this;
}

Vector & Vector::mirrorHorizontal() {
    dx *= -1;
    angle.setSet();
    return *this;
}

Vector & Vector::mirrorVertical() {
    dy *= -1;
    angle.setSet();
    return *this;
}

bool Vector::subtractAngle(Angle angle) {
    Angle vectorAngle = getAngle();
    
    Angle angleDifference;
    if ((vectorAngle.getValue() > 0) ^ (angle.getValue() > 0)) {
        angleDifference = Angle(fabs(vectorAngle.getValue()) + fabs(angle.getValue()), false);
        if (angleDifference.getValue() > M_PI)
            angleDifference.setValue(((M_PI * 2) - angleDifference.getValue()) * ((vectorAngle.getValue() > 0) ? 1 : -1));
        else
            angleDifference *= ((vectorAngle.getValue() > 0) ? -1 : 1);
    } else {
        angleDifference = angle - vectorAngle;
    }
    
    if (fabs(angleDifference.getValue()) - M_PI_2 < -0.00001) //account for rounding error (prevents bug where object falls through map when moving at very slow speeds > 0)
        return false;
    
    angleDifference += (angleDifference.getValue() > 0) ? -M_PI_2 : M_PI_2;
    
    Angle newAngle = vectorAngle + angleDifference; //angle of new vector relative to cartesian plane
    double newMagnitude = fabs(angleDifference.getCos(getMagnitude()));
    
    setDx(newAngle.getCos(newMagnitude));
    setDy(newAngle.getSin(newMagnitude));
    this->angle.setValue(newAngle);
    magnitude.setValue(newMagnitude);
    
    return true;
}

/*Line Vector::toLine() const {
 if (p_origin == nullptr)
 return toLine(Coordinate());
 else
 return toLine(*p_origin);
 }
 
 Line Vector::toLine(Coordinate coor, bool subtracted) const {
 return Line(coor.getX(), coor.getY(), coor.getX() + (subtracted ? -dx : dx), coor.getY() + (subtracted ? -dy : dy));
 }
 
 void Vector::moveLine(Line * line) const {
 if (line)
 *line = Line(line->getCoor1() + *this, line->getCoor2() + *this);
 }*/

void Vector::draw() const {
    draw(origin, 1);
}

void Vector::draw(int magntitude) const {
    draw(origin, magntitude);
}

void Vector::draw(Coordinate origin, int magnitude) const {
    GraphicsManager::drawCoordinate(origin, getColor());
    
    if ((fabs(dx) + fabs(dy)) < 0.0001) //account for rounding errors
        return;
    
    Coordinate destination(origin.getX() + (dx * 5), origin.getY() + (dy * 5));
    GraphicsManager::drawLine(Line(origin.getX(), origin.getY(), destination.getX(), destination.getY()), getColor());
    
    static int arrowLength = 5;
    Angle angle(M_PI - Angle(dx, dy).getValue());// * ((angle < 0) ? -1 : 1));
    Angle angleMinus = angle - M_PI_4;
    Angle anglePlus = angle + M_PI_4;
    GraphicsManager::drawLine(Line(destination.getX(), destination.getY(), destination.getX() - angleMinus.getSin(arrowLength), destination.getY() - angleMinus.getCos(arrowLength)), getColor());
    GraphicsManager::drawLine(Line(destination.getX(), destination.getY(), destination.getX() + anglePlus.getSin(arrowLength), destination.getY() + anglePlus.getCos(arrowLength)), getColor());
}

Vector Vector::operator+(const Vector & vec) const {
    return Vector(dx + vec.getDx(), dy + vec.getDy(), origin);
}

Vector & Vector::operator+=(const Vector & vec) {
    setDx(dx + vec.dx);
    setDy(dy + vec.dy);
    return *this;
}

Vector Vector::operator-(const Vector & vec) const {
    return Vector(dx - vec.getDx(), dy - vec.getDy(), origin);
}

Vector & Vector::operator-=(const Vector & vec) {
    setDx(dx - vec.dx);
    setDy(dy - vec.dy);
    return *this;
}

Vector Vector::operator*(const double & d) const {
    return Vector(dx * d, dy * d, origin);
}

Vector & Vector::operator*=(const double & d) {
    setDx(dx * d);
    setDy(dy * d);
    return *this;
}

Vector Vector::operator/(const double & d) const {
    return Vector(dx / d, dy / d, origin);
}

Vector & Vector::operator/=(const double & d) {
    setDx(dx / d);
    setDy(dy / d);
    return *this;
}

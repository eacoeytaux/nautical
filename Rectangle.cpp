//
//  Rectangle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Rectangle.hpp"

using namespace nautical;

Rectangle::Rectangle(Coordinate center, double width, double height, Angle angle) :
center(center),
width(width),
halfWidth(width / 2),
height(height),
halfHeight(height / 2),
angle(angle) {
    appendTag(RECTANGLE_TAG);
}

Rectangle::~Rectangle() { }

Coordinate Rectangle::getCenter() const {
    return center;
}

Rectangle & Rectangle::setCenter(Coordinate center) {
    this->center = center;
    return *this;
}

double Rectangle::getWidth() const {
    return width;
}

Rectangle & Rectangle::setWidth(double width) {
    this->width = width;
    halfWidth = width / 2;
    return *this;
}

double Rectangle::getHeight() const {
    return height;
}

Rectangle & Rectangle::setHeight(double height) {
    this->height = height;
    halfHeight = height / 2;
    return *this;
}

Angle Rectangle::getAngle() const {
    return angle;
}

Rectangle & Rectangle::setAngle(Angle angle) {
    this->angle = angle;
    return *this;
}

Coordinate Rectangle::getTopRightCoor() const {
    return center + Vector(halfWidth, -halfHeight).rotate(angle);
}

Coordinate Rectangle::getBottomRightCoor() const {
    return center + Vector(halfWidth, halfHeight).rotate(angle);
}

Coordinate Rectangle::getBottomLeftCoor() const {
    return center + Vector(-halfWidth, halfHeight).rotate(angle);
}

Coordinate Rectangle::getTopLeftCoor() const {
    return center + Vector(-halfWidth, -halfHeight).rotate(angle);
}

Line Rectangle::getTopLine() const {
    return Line(getTopRightCoor(), getTopLeftCoor());
}

Line Rectangle::getRightLine() const {
    return Line(getBottomRightCoor(), getTopRightCoor());
}

Line Rectangle::getBottomLine() const {
    return Line(getBottomLeftCoor(), getBottomRightCoor());
}

Line Rectangle::getLeftLine() const {
    return Line(getTopLeftCoor(), getBottomLeftCoor());
}

double Rectangle::getDiagonalLength() const {
    return findDistance(getTopLeftCoor(), getBottomRightCoor());
}

bool Rectangle::contains(Coordinate coor) const {
    return (getTopLine().isOnOrBelow(coor) && getRightLine().isOnOrBelow(coor) && getBottomLine().isOnOrBelow(coor) && getLeftLine().isOnOrBelow(coor));
}

bool Rectangle::intersectsLine(Line line, Queue<Coordinate> * p_intersections) const {
    Coordinate coor1, coor2;
    bool intersects = false, intersectsTwice = false;
    
    (intersects ? intersectsTwice : intersects) = line.intersects(getTopLine(), intersects ? &coor2 : &coor1);
    (intersects ? intersectsTwice : intersects) = line.intersects(getRightLine(), intersects ? &coor2 : &coor1);
    (intersects ? intersectsTwice : intersects) = line.intersects(getBottomLine(), intersects ? &coor2 : &coor1);
    (intersects ? intersectsTwice : intersects) = line.intersects(getLeftLine(), intersects ? &coor2 : &coor1);
    
    if (intersects) {
        if (p_intersections) {
            if (intersectsTwice) {
                double xDistance1 = fabs(line.getCoor1().getX() - coor1.getX());
                double xDistance2 = fabs(line.getCoor1().getX() - coor2.getX());
                
                if (xDistance1 <= xDistance2) {
                    p_intersections->insert(coor1);
                    p_intersections->insert(coor2);
                } else {
                    p_intersections->insert(coor2);
                    p_intersections->insert(coor1);
                }
            } else {
                p_intersections->insert(coor1);
            }
        }
        return true;
    } else {
        return false;
    }
}

bool Rectangle::intersectsShape(const Shape * p_shape, Queue<Coordinate> * p_intersections) const {
    bool intersects = false;
    intersects |= p_shape->intersectsLine(getTopLine(), p_intersections);
    intersects |= p_shape->intersectsLine(getRightLine(), p_intersections);
    intersects |= p_shape->intersectsLine(getBottomLine(), p_intersections);
    intersects |= p_shape->intersectsLine(getLeftLine(), p_intersections);
    intersects |= p_shape->contains(center);
    return intersects;
}


Rectangle & Rectangle::move(Vector vector) {
    setCenter(center += vector);
    return *this;
}

Rectangle & Rectangle::rotateAboutCoordinate(Angle angle, Coordinate coor) {
    center.rotateAboutCoordinate(angle, coor);
    this->angle += angle;
    return *this;
}

void Rectangle::draw() const {
    GraphicsManager::drawLine(getTopLine(), color);
    GraphicsManager::drawLine(getRightLine(), color);
    GraphicsManager::drawLine(getBottomLine(), color);
    GraphicsManager::drawLine(getLeftLine(), color);
}

bool Rectangle::operator==(const Shape & shape) const {
    if (getTag() == shape.getTag()) {
        Rectangle rec = (Rectangle&)shape;
        return ((center == rec.center) && (width == rec.width) && (height == rec.height) && (angle == rec.angle));
    }
    return false;
}

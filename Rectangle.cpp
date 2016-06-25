//
//  Rectangle.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Rectangle.hpp"

#include "MaxMinValue.hpp"

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

void Rectangle::setCenter(Coordinate center) {
    this->center = center;
}

double Rectangle::getWidth() const {
    return width;
}

void Rectangle::setWidth(double width) {
    this->width = width;
    halfWidth = width / 2;
}

double Rectangle::getHeight() const {
    return height;
}

void Rectangle::setHeight(double height) {
    this->height = height;
    halfHeight = height / 2;
}

Angle Rectangle::getAngle() const {
    return angle;
}

void Rectangle::setAngle(Angle angle) {
    this->angle = angle;
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

double Rectangle::getLowerBoundX() const {
    return center.getX() - (angle.getCos(halfWidth) + angle.getSin(halfHeight));
}

double Rectangle::getLowerBoundY() const {
    return center.getY() - (angle.getCos(halfHeight) + angle.getSin(halfWidth));
}

double Rectangle::getUpperBoundX() const {
    return center.getX() + (angle.getCos(halfWidth) + angle.getSin(halfHeight));
}

double Rectangle::getUpperBoundY() const {
    return center.getY() + (angle.getCos(halfHeight) + angle.getSin(halfWidth));
}

bool Rectangle::contains(Coordinate coor) const {
    return (getTopLine().isOnOrBelow(coor) && getRightLine().isOnOrBelow(coor) && getBottomLine().isOnOrBelow(coor) && getLeftLine().isOnOrBelow(coor));
}

bool Rectangle::intersectsLine(Line line, std::vector<Coordinate> * p_intersections) const {
    struct Comp {
        Coordinate origin;
        inline bool operator()(Coordinate coor1, Coordinate coor2) {
            return (findDistance(origin, coor1) < findDistance(origin, coor2));
        }
    };
    
    if (p_intersections) {
        if (line.intersectsLine(getTopLine(), p_intersections) ||
            line.intersectsLine(getRightLine(), p_intersections) ||
            line.intersectsLine(getBottomLine(), p_intersections) ||
            line.intersectsLine(getLeftLine(), p_intersections)) {
            
            Comp comp;
            comp.origin = line.getCoor1();
            std::sort(p_intersections->begin(), p_intersections->end(), comp);
            
            return true;
        }
        return false;
    }
    
    return (line.intersectsLine(getTopLine()) || line.intersectsLine(getRightLine()) ||
            line.intersectsLine(getBottomLine()) || line.intersectsLine(getLeftLine()));
}

bool Rectangle::intersectsShape(const Shape * p_shape, std::vector<Coordinate> * p_intersections) const {
    bool intersects = false;
    intersects |= p_shape->intersectsLine(getTopLine(), p_intersections);
    intersects |= p_shape->intersectsLine(getRightLine(), p_intersections);
    intersects |= p_shape->intersectsLine(getBottomLine(), p_intersections);
    intersects |= p_shape->intersectsLine(getLeftLine(), p_intersections);
    intersects |= p_shape->contains(center);
    return intersects;
}

void Rectangle::move(Vector vector) {
    setCenter(center += vector);
}

void Rectangle::rotateAboutCoordinate(Coordinate coor, Angle angle) {
    center.rotateAboutCoordinate(coor, angle);
    this->angle += angle;
}

void Rectangle::draw() const {
    GraphicsManager::drawLine(getTopLine(), getColor());
    GraphicsManager::drawLine(getRightLine(), getColor());
    GraphicsManager::drawLine(getBottomLine(), getColor());
    GraphicsManager::drawLine(getLeftLine(), getColor());
}

std::shared_ptr<Shape> Rectangle::deepCopy() const {
    return std::shared_ptr<Shape>(new Rectangle(*this));
}

bool Rectangle::operator==(const Shape & shape) const {
    if (getTag() == shape.getTag()) {
        Rectangle rec = (Rectangle&)shape;
        return ((center == rec.center) && (width == rec.width) && (height == rec.height) && (angle == rec.angle));
    }
    return false;
}

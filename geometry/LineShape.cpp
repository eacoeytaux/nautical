//
//  LineShape.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/16/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "LineShape.hpp"

using namespace nautical;

LineShape::LineShape(Line line) :
line(line) {
    appendTag(LINESHAPE_TAG);
}

LineShape::~LineShape() { }

Line LineShape::getLine() const {
    return line;
}

LineShape & LineShape::setLine(Line line) {
    this->line = line;
    return *this;
}

double LineShape::getLowerBoundX() const {
    return line.getCoorLow().getX();
}

double LineShape::getLowerBoundY() const {
    return line.getCoorLow().getY();
}

double LineShape::getUpperBoundX() const {
    return line.getCoorHigh().getX();
}

double LineShape::getUpperBoundY() const {
    return line.getCoorHigh().getY();
}

bool LineShape::contains(Coordinate coor) const {
    return false;
}

bool LineShape::intersectsLine(Line line, std::vector<Coordinate> * p_intersections) const {
    return this->line.intersectsLine(line, p_intersections);
}

bool LineShape::intersectsShape(const Shape * p_shape, std::vector<Coordinate> * p_intersections) const {
    return p_shape->intersectsLine(line, p_intersections);
}

LineShape & LineShape::move(Vector vector) {
    line = Line(line.getCoor1() + vector, line.getCoor2() + vector);
    return *this;
}

LineShape & LineShape::rotateAboutCoordinate(Coordinate coor, Angle angle) {
    line.rotateAboutCoordinate(coor, angle);
    return *this;
}

void LineShape::draw() const {
    GraphicsManager::drawLine(line, getColor());
}

void LineShape::drawFilled() const {
    draw();
}

std::shared_ptr<Shape> LineShape::deepCopy() const {
    return std::shared_ptr<Shape>(new LineShape(*this));
}

bool LineShape::operator==(const Shape & shape) const {
    if (getTag() == shape.getTag())
        return (line == ((LineShape&)shape).line);
    return false;
}

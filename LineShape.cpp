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

LineShape & LineShape::move(Vector vector) {
    line = Line(line.getCoor1() + vector, line.getCoor2() + vector);
    return *this;
}

LineShape & LineShape::rotateAboutCoordinate(Angle angle, Coordinate coor) {
    line.rotateAboutCoordinate(angle, coor);
    return *this;
}

bool LineShape::contains(Coordinate coor) const {
    return false; //TODO maybe if Line.inLine() is implemented?
}

bool LineShape::intersectsLine(Line line, Queue<Coordinate> * p_intersections) const {
    Coordinate intersection;
    if (this->line.intersects(line, &intersection)) {
        if (p_intersections)
            p_intersections->insert(intersection);
        return true;
    } else {
        return false;
    }
}

bool LineShape::intersectsShape(const Shape * p_shape, Queue<Coordinate> * intersections) const {
    return p_shape->intersectsLine(line, intersections);
}

void LineShape::draw() const {
    GraphicsManager::drawLine(line, getColor());
}

bool LineShape::operator==(const Shape & shape) const {
    if (getTag() == shape.getTag())
        return (line == ((LineShape&)shape).line);
    return false;
}

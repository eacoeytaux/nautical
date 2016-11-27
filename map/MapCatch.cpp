//
//  MapCatch.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/11/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "MapCatch.hpp"

#include "MapElement.hpp"

using namespace nautical;

MapCatch::MapCatch(Coordinate collision, Line line, std::shared_ptr<const MapElement> p_element1, std::shared_ptr<const MapElement> p_element2) :
collision(collision),
line(line),
p_element1(p_element1),
p_element2(p_element2) { }

MapCatch::~MapCatch() { }

Coordinate MapCatch::getCollision() const {
    return collision;
}

MapCatch & MapCatch::setCollision(Coordinate collision) {
    this->collision = collision;
    return *this;
}

Line MapCatch::getLine() const {
    return line;
}

MapCatch & MapCatch::setLine(Line line) {
    this->line = line;
    return *this;
}

bool MapCatch::containsElement(std::shared_ptr<const MapElement> p_element) const {
    return ((p_element1 == p_element) || (p_element2 == p_element));
}

std::shared_ptr<const MapElement> MapCatch::getElement(std::shared_ptr<const MapElement> p_other) const {
    if (p_element1 == p_other)
        return p_element2;
    else if (p_element2 == p_other)
        return p_element1;
    else
        return nullptr;
}

std::shared_ptr<const MapElement> MapCatch::getElement1() const {
    return p_element1;
}

MapCatch & MapCatch::setElement1(std::shared_ptr<const MapElement> p_element) {
    this->p_element1 = p_element;
    return *this;
}

std::shared_ptr<const MapElement> MapCatch::getElement2() const {
    return p_element2;
}

MapCatch & MapCatch::setElement2(std::shared_ptr<const MapElement> p_element) {
    this->p_element2 = p_element;
    return *this;
}

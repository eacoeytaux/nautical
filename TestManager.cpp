//
//  TestManager.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 10/1/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "TestManager.hpp"

#include "LinkedList.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"
#include "Rectangle.hpp"
#include "Polygon.hpp"
#include "Trigger.hpp"

using namespace nautical;

bool testLinkedList() {
    LinkedList<Coordinate> ll1;
    ll1.insert(Coordinate(3, 5));
    ll1.insertHead(Coordinate(0, 0));
    ll1.insert(Coordinate(5, 6));
    ll1.remove(Coordinate(3, 5));
    
    /*if (ll1.size() != 2)
        return false;
    if (!(ll1.getFirstElement() == Coordinate(0, 0)))
        return false;
    if (!(ll1.getLastElement() == Coordinate(5, 6)))
        return false;*/
    
    return true;
}

bool testTagable() {
    Rectangle rec;
    if (!(rec.hasTag(SHAPE_TAG)) && (rec.hasTag(RECTANGLE_TAG)))
        return false;
    
    return true;
}

bool testPolygon() {
    Queue<Coordinate> coors;
    coors.insert(Coordinate(10, 10));
    Polygon polygon(&coors);
    
    return true;
}

bool testLine() {
    Line line(10, 20, 50, 40);
    Coordinate coor1(30, 30);
    Coordinate coor2(30, 50);
    
    if (line.isOnOrBelow(coor1))
        return false;
    if (!line.isOnOrBelow(coor2))
        return false;
    
    return true;
}

bool TestManager::runTests() {
    bool success = true;
    
    success &= testTagable();
    //success &= testLine();
    
    Rectangle rec(Coordinate(0, 0), 5, 8);
    rec.setHeight(8).setWidth(10);
    
    return success;
}

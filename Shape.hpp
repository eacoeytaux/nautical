//
//  Shape.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Shape__
#define __Nautical__Shape__

#include "Drawable.hpp" //base class
#include "Tagable.hpp" //base class

#include <string>

#include "Logger.hpp"
#include "Utility.hpp"
#include "Queue.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"
#include "Vector.hpp"

namespace nautical {
    const std::string SHAPE_TAG = "shape";
    
    class Shape : public Drawable, public Tagable {
    public:
        Shape() { appendTag(SHAPE_TAG); }
        virtual ~Shape() { }
        
        virtual bool contains(Coordinate coor) const = 0;
        virtual bool intersectsLine(Line line, Queue<Coordinate> * p_intersections = nullptr) const = 0;
        virtual bool intersectsShape(const Shape * p_shape, Queue<Coordinate> * p_intersections = nullptr) const = 0;
        
        virtual Shape & move(Vector vector) = 0;
        virtual Shape & rotateAboutCoordinate(Angle angle, Coordinate coor = Coordinate(0, 0)) = 0;
        
        virtual void draw() const = 0;
        
        virtual bool operator==(const Shape & shape) const = 0;
        bool operator!=(const Shape & shape) const { return !((*this) == shape); }
    };
}

#endif /* defined(__Nautical__Shape__) */

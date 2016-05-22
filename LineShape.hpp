//
//  LineShape.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/16/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__LineShape__
#define __Nautical__LineShape__

#include "Shape.hpp" //base class

namespace nautical {
    const std::string LINESHAPE_TAG = "lineshape";
    
    class LineShape : public Shape {
    public:
        LineShape(Line line = Line());
        virtual ~LineShape();
        
        Line getLine() const;
        LineShape & setLine(Line line);
        
        LineShape & move(Vector vector);
        LineShape & rotateAboutCoordinate(Angle angle, Coordinate coor = Coordinate(0, 0));
        
        bool contains(Coordinate coor) const;
        bool intersectsLine(Line line, Queue<Coordinate> * p_intersections = nullptr) const;
        bool intersectsShape(const Shape * p_shape, Queue<Coordinate> * p_intersections = nullptr) const;
        
        void draw() const;
        
        bool operator==(const Shape & shape) const;
        
    private:
        Line line;
    };
}

#endif /* defined(__Nautical__LineShape__) */

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
        
        double getLowerBoundX() const;
        double getLowerBoundY() const;
        double getUpperBoundX() const;
        double getUpperBoundY() const;
        
        bool contains(Coordinate coor) const;
        bool intersectsLine(Line line, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool intersectsShape(const Shape * p_shape, std::vector<Coordinate> * p_intersections = nullptr) const;
        
        LineShape & move(Vector vector);
        LineShape & rotateAboutCoordinate(Coordinate coor, Angle angle);
        
        void draw() const;
        void drawFilled() const;
        
        std::shared_ptr<Shape> deepCopy() const;
        bool operator==(const Shape & shape) const;
        
    private:
        Line line;
    };
}

#endif /* defined(__Nautical__LineShape__) */

//
//  Polygon.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 10/1/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef Polygon_hpp
#define Polygon_hpp

#include "Shape.hpp" //base class

#include "MaxMinValue.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"

namespace nautical {
    const std::string POLYGON_TAG = "polygon";
    
    class Polygon : public Shape {
    public:
        Polygon(std::vector<Coordinate> * p_coors, bool checkForConcave = true); //checkForConcave should only be set to tru if polygon is definitely convex
        virtual ~Polygon();
        
        bool isConvex() const;
        std::vector<Polygon>::iterator getSubtractedTrianglesIterator() const;
        
        double getLowerBoundX() const;
        double getLowerBoundY() const;
        double getUpperBoundX() const;
        double getUpperBoundY() const;
        
        bool contains(Coordinate coor) const;
        bool intersectsLine(Line line, std::vector<Coordinate> * p_intersections = nullptr) const;
        bool intersectsShape(const Shape * p_shape, std::vector<Coordinate> * p_intersections = nullptr) const;
        
        Polygon & move(Vector vector);
        Polygon & rotateAboutCoordinate(Coordinate coor, Angle angle);
        
        void draw() const;
        
        std::shared_ptr<Shape> deepCopy() const;
        bool operator==(const Shape & shape) const;
        
    private:
        //stored in clockwise order
        std::vector<Coordinate> vertices;
        std::vector<Line> edges;
        
        bool convex = true;
        //following variables only needed if polygon is not convex
        std::vector<Coordinate> convexVertices; //Coordinates that create convex outline
        std::vector<Line> convexEdges; //Lines that create convex outline
        
        struct Triangle {
            Coordinate coor1;
            Coordinate coor2;
            Coordinate coor3;
            bool operator==(const Triangle & triangle);
        };;
        std::vector<Triangle> subtractedTrianglesStruct;
        mutable bool subtractedTrianglesSet = false;
        mutable std::vector<Polygon> subtractedTriangles; //triangles subtracted from polygon to create a convex polygon
        
        MinValue lowerBoundX, lowerBoundY;
        MaxValue upperBoundX, upperBoundY;
        
        void init(std::vector<Coordinate> * coors, bool checkForConcave = true);
    };
}

#endif /* Polygon_hpp */

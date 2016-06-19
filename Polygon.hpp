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
#include "Collection.hpp"
#include "Queue.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"

namespace nautical {
    const std::string POLYGON_TAG = "polygon";
    
    class Polygon : public Shape {
    public:
        Polygon(Queue<Coordinate> * p_coors, bool checkForConcave = true); //checkForConcave should only be set to tru if polygon is definitely convex
        virtual ~Polygon();
        
        bool isConvex() const;
        Iterator<Polygon> * getSubtractedTrianglesIterator() const;
        
        double getLowerBoundX() const;
        double getLowerBoundY() const;
        double getUpperBoundX() const;
        double getUpperBoundY() const;
        
        bool contains(Coordinate coor) const;
        bool intersectsLine(Line line, Queue<Coordinate> * p_intersections = nullptr) const;
        bool intersectsShape(const Shape * p_shape, Queue<Coordinate> * p_intersections = nullptr) const;
        
        Polygon & move(Vector vector);
        Polygon & rotateAboutCoordinate(Coordinate coor, Angle angle);
        
        void draw() const;
        
        Polygon * copyPtr() const;
        bool operator==(const Shape & shape) const;
        
    private:
        //stored in clockwise order
        Queue<Coordinate> vertices;
        Queue<Line> edges;
        
        bool convex = true;
        //following variables only needed if polygon is not convex
        Queue<Coordinate> convexVertices; //Coordinates that create convex outline
        Queue<Line> convexEdges; //Lines that create convex outline
        
        struct Triangle {
            Coordinate coor1;
            Coordinate coor2;
            Coordinate coor3;
            bool operator==(const Triangle & triangle);
        };;
        Queue<Triangle> subtractedTrianglesStruct;
        mutable bool subtractedTrianglesSet = false;
        mutable Queue<Polygon> subtractedTriangles; //triangles subtracted from polygon to create a convex polygon
        
        MinValue lowerBoundX, lowerBoundY;
        MaxValue upperBoundX, upperBoundY;
        
        void init(Queue<Coordinate> * coors, bool checkForConcave = true);
    };
}

#endif /* Polygon_hpp */

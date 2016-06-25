//
//  Map.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Map__
#define __Nautical__Map__

#include "Drawable.hpp"

#include "MaxMinValue.hpp"
#include "MapVertex.hpp"
#include "MapEdge.hpp"

namespace nautical {
    class World;
    class MapHitbox;
    
    class Map : public Drawable {
    public:
        Map(bool verticalMap = false);
        virtual ~Map();
        
        bool isVertical() const;
        
        virtual MapVertex * createVertex(Coordinate coor);
        virtual MapEdge * createEdge(MapVertex * p_vertexBack, MapVertex * p_vertexFront, bool sticky = false);
        
        //TODO check to make sure MapElements* cannot be accessed
        const std::vector<MapVertex*> * getVerticesList() const;
        const std::vector<MapEdge*> * getEdgesList() const;
        
        double getAirResistanceCoefficient(double value = 1) const;
        void setAirResistanceCoefficient(float airResistanceCoefficient);
        
        static double getDefaultAirResistanceCoefficient(double value = 1);
        static void setDefaultAirResistanceCoefficient(float airResistanceCoefficient);
        
        void draw() const; //for debugging only
        void drawBumpers(MapHitbox * p_hitbox, bool drawCatches = true) const; //for debugging on;y
        
    private:
        World * p_parent;
        
        bool verticalMap;
        
        std::vector<MapVertex*> vertices;
        std::vector<MapEdge*> edges;
        
        double airResistanceCoefficient;
        static double defaultAirResistanceCoefficient;
    };
}

#endif /* defined(__Nautical__Map__) */

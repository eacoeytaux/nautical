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
        
        virtual std::shared_ptr<MapVertex> createVertex(Coordinate coor);
        virtual std::shared_ptr<MapEdge> createEdge(std::shared_ptr<MapVertex> p_vertexBack, std::shared_ptr<MapVertex> p_vertexFront, bool sticky = false);
        
        //TODO check to make sure MapElements* cannot be accessed
        const std::vector<std::shared_ptr<MapVertex>> & getVerticesList() const;
        const std::vector<std::shared_ptr<MapEdge>> & getEdgesList() const;
        
        double getAirResistanceCoefficient(double value = 1) const;
        Map & setAirResistanceCoefficient(float airResistanceCoefficient);
        
        static double getDefaultAirResistanceCoefficient(double value = 1);
        static void setDefaultAirResistanceCoefficient(float airResistanceCoefficient);
        
        void draw() const; //for debugging only
        void drawBumpers(std::shared_ptr<MapHitbox> p_hitbox, bool drawCatches = true) const; //for debugging on;y
        
    private:
        bool verticalMap;
        
        std::vector<std::shared_ptr<MapVertex>> p_vertices;
        std::vector<std::shared_ptr<MapEdge>> p_edges;
        
        double airResistanceCoefficient;
        static double defaultAirResistanceCoefficient;
    };
}

#endif /* defined(__Nautical__Map__) */

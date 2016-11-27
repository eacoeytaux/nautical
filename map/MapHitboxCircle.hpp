//
//  MapHitboxCircle.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/22/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef MapHitboxCircle_hpp
#define MapHitboxCircle_hpp

#include "MapHitbox.hpp"

namespace nautical {
    class MapHitboxCircle : public MapHitbox {
    public:
        MapHitboxCircle(Circle circle);
        virtual ~MapHitboxCircle();
        
        MapHitboxCircle & move(Vector vec);
        
        std::shared_ptr<Shape> getShape() const;
        Circle getCircle() const;
        MapHitboxCircle & setCircle(Circle circle);
        
        bool adjustVector(std::shared_ptr<const MapVertex> p_vertex, Vector & vector) const;
        std::shared_ptr<Shape> createBumper(std::shared_ptr<const MapVertex> p_vertex) const; //always returns LineShape
        std::vector<MapCatch> findCatches(std::shared_ptr<const MapVertex> p_vertex, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const;
        
        bool adjustVector(std::shared_ptr<const MapEdge> p_edge, Vector & vector) const;
        std::shared_ptr<Shape> createBumper(std::shared_ptr<const MapEdge> p_edge) const; //always returns Circle
        std::vector<MapCatch> findCatches(std::shared_ptr<const MapEdge> p_edge, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const;
        
        //helper functions for edges
        Vector getOffset(std::shared_ptr<const MapEdge> p_edge) const;
        MapCatch getCatchFront(std::shared_ptr<const MapEdge> p_edge) const;
        MapCatch getCatchBack(std::shared_ptr<const MapEdge> p_edge) const;
        
        //MapHitboxCircle * deepCopy() const;
        
    private:
        Circle circle;
    };
}

#endif /* MapHitboxCircle_hpp */

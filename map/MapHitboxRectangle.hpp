//
//  MapHitboxRectangle.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/22/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef MapHitboxRectangle_hpp
#define MapHitboxRectangle_hpp

#include "MapHitbox.hpp"

namespace nautical {
    class MapHitboxRectangle : public MapHitbox {
    public:
        MapHitboxRectangle(Rectangle rec);
        virtual ~MapHitboxRectangle();
        
        MapHitboxRectangle & move(Vector vec);
        
        std::shared_ptr<Shape> getShape() const;
        Rectangle getRectangle() const;
        MapHitboxRectangle & setRectangle(Rectangle rec);
        
        bool adjustVector(std::shared_ptr<const MapVertex> p_vertex, Vector & vector) const;
        std::shared_ptr<Shape> createBumper(std::shared_ptr<const MapVertex> p_vertex) const; //always returns LineShape
        std::vector<MapCatch> findCatches(std::shared_ptr<const MapVertex> p_vertex, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const;
        
        bool adjustVector(std::shared_ptr<const MapEdge> p_edge, Vector & vector) const;
        std::shared_ptr<Shape> createBumper(std::shared_ptr<const MapEdge> p_edge) const; //always return Rectangle
        std::vector<MapCatch> findCatches(std::shared_ptr<const MapEdge> p_edge, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const;
        
        //helper functions for edges
        Vector getOffset(std::shared_ptr<const MapEdge> p_edge) const;
        MapCatch getCatchFront(std::shared_ptr<const MapEdge> p_edge) const;
        MapCatch getCatchBack(std::shared_ptr<const MapEdge> p_edge) const;
        
        //MapHitboxRectangle * deepCopy() const;
        
    private:
        Rectangle rec;
    };
}

#endif /* MapHitboxRectangle_hpp */

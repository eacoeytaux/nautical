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
        
        void move(Vector vec);
        
        std::shared_ptr<Shape> getShape() const;
        Rectangle getRectangle() const;
        void setRectangle(Rectangle rec);
        
        bool adjustVector(const MapVertex * p_vertex, Vector * p_vector) const;
        std::shared_ptr<Shape> createBumper(const MapVertex * p_vertex) const; //always returns LineShape
        std::vector<MapCatch> findCatches(const MapVertex * p_vertex, const Map * p_map) const;
        
        bool adjustVector(const MapEdge * p_edge, Vector * p_vector) const;
        std::shared_ptr<Shape> createBumper(const MapEdge * p_edge) const; //always return Rectangle
        std::vector<MapCatch> findCatches(const MapEdge * p_edge, const Map * p_map) const;
        
        //helper functions for edges
        Vector getOffset(const MapEdge * p_edge) const;
        MapCatch getCatchFront(const MapEdge * p_edge) const;
        MapCatch getCatchBack(const MapEdge * p_edge) const;
        
        MapHitboxRectangle * deepCopy() const;
        
    private:
        Rectangle rec;
    };
}

#endif /* MapHitboxRectangle_hpp */

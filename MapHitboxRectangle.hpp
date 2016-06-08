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
        
        Shape * getShape() const;
        Rectangle getRectangle() const;
        MapHitboxRectangle & setRectangle(Rectangle rec);
        
        bool adjustVector(const MapVertex * p_vertex, Vector * p_vector) const;
        Rectangle * createBumper(const MapVertex * p_vertex) const;
        LinkedList<MapCatch> findCatches(const MapVertex * p_vertex, const Map * p_map) const;
        
        bool adjustVector(const MapEdge * p_edge, Vector * p_vector) const;
        LineShape * createBumper(const MapEdge * p_edge) const;
        LinkedList<MapCatch> findCatches(const MapEdge * p_edge, const Map * p_map) const;
        
        //helper functions for edges
        Vector getOffset(const MapEdge * p_edge) const;
        MapCatch getCatchFront(const MapEdge * p_edge) const;
        MapCatch getCatchBack(const MapEdge * p_edge) const;
        
        MapHitboxRectangle * copyPtr() const;
        
    private:
        Rectangle rec;
    };
}

#endif /* MapHitboxRectangle_hpp */

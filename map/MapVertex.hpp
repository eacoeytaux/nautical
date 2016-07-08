//
//  MapVertex.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/16/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__MapVertex__
#define __Nautical__MapVertex__

#include "MapElement.hpp" //base class

namespace nautical {
    class MapEdge; //forward declaration to avoid circular inclusion
    
    const std::string MAP_VERTEX_TAG = "map_vertex";
    
    class MapVertex : public MapElement {
    public:
        MapVertex(Coordinate coor, bool sticky = false);
        virtual ~MapVertex();
        
        Coordinate getCoor() const;
        MapEdge * getEdgeFront() const;
        MapVertex & setEdgeFront(MapEdge * p_edge);
        MapEdge * getEdgeBack() const;
        MapVertex & setEdgeBack(MapEdge * p_edge);
        
        virtual void draw() const;
        
    private:
        Coordinate coor;
        MapEdge * p_edgeFront = nullptr, * p_edgeBack = nullptr;
    };
}

#endif /* defined(__Nautical__MapVertex__) */

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
        std::shared_ptr<const MapEdge> getEdgeFront() const;
        MapVertex & setEdgeFront(std::shared_ptr<const MapEdge> p_edge);
        std::shared_ptr<const MapEdge> getEdgeBack() const;
        MapVertex & setEdgeBack(std::shared_ptr<const MapEdge> p_edge);
        
        virtual void draw() const;
        
    private:
        Coordinate coor;
        std::weak_ptr<const MapEdge> p_edgeFront, p_edgeBack;
    };
}

#endif /* defined(__Nautical__MapVertex__) */

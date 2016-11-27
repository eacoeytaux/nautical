//
//  MapEdge.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/16/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__MapEdge__
#define __Nautical__MapEdge__

#include "MapElement.hpp" //base class

namespace nautical {
    class MapVertex;
    
    const std::string MAP_EDGE_TAG = "map_edge";
    
    class MapEdge : public MapElement {
    public:
        MapEdge(std::shared_ptr<MapVertex> p_vertexBack, std::shared_ptr<MapVertex> p_vertexFront, bool sticky = false);
        virtual ~MapEdge();
        
        Line getLine() const;
        Angle getNormal() const;
        std::shared_ptr<MapVertex> getVertex(std::shared_ptr<MapVertex> p_other) const;
        std::shared_ptr<MapVertex> getVertexFront() const;
        MapEdge & setVertexFront(std::shared_ptr<MapVertex> p_vertex);
        std::shared_ptr<MapVertex> getVertexBack() const;
        MapEdge & setVertexBack(std::shared_ptr<MapVertex> p_vertex);
        
        virtual void draw() const;
        
        static bool DRAW_NORMALS; //DEBUGGING
        
    private:
        Line line;
        Angle normal;
        std::weak_ptr<MapVertex> p_vertexFront, p_vertexBack;
    };
}

#endif /* defined(__Nautical__MapEdge__) */

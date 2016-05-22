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

#include "LineShape.hpp" //this has to be included so the compiler knows LineShape extends Shape

namespace nautical {
    class MapVertex;
    
    const std::string MAP_EDGE_TAG = "map_edge";
    
    class MapEdge : public MapElement {
    public:
        MapEdge(MapVertex * p_vertexBack, MapVertex * p_vertexFront, bool sticky = false);
        virtual ~MapEdge();
        
        Line getLine() const;
        Angle getNormal() const;
        MapVertex * getVertex(MapVertex * p_other) const;
        MapVertex * getVertexFront() const;
        MapEdge & setVertexFront(MapVertex * p_vertex);
        MapVertex * getVertexBack() const;
        MapEdge & setVertexBack(MapVertex * p_vertex);
        
        bool adjustVectorRectangle(Rectangle rectangle, Vector * p_vector, ObjectPos pos = UNDEFINED) const;
        
        LineShape * generateBumperRectangle(Rectangle rectangle) const;
        LineShape * generateBumperCircle(Circle circle) const;
        
        MapCatch getCatchFrontRectangle(Rectangle rectangle) const;
        MapCatch getCatchBackRectangle(Rectangle rectangle) const;
        
        Vector getOffsetRectangle(Rectangle rectangle) const;
        
        LinkedList<MapCatch> findCatches(Rectangle rectangle, const Map * p_map) const;
        
        virtual void draw() const;
        
    private:
        Line line;
        Angle normal;
        MapVertex * p_vertexFront = nullptr, * p_vertexBack = nullptr;
    };
}

#endif /* defined(__Nautical__MapEdge__) */

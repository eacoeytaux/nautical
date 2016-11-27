//
//  MapHitbox.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/22/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef MapHitbox_hpp
#define MapHitbox_hpp

#include <memory>

#include "Vector.hpp"
#include "Map.hpp"
#include "MapVertex.hpp"
#include "MapEdge.hpp"

namespace nautical {
    class MapHitbox {
    public:
        virtual ~MapHitbox() { }
        
        Coordinate getCenter() const {
            return center;
        }
        
        virtual MapHitbox & move(Vector vec) {
            center += vec;
            return *this;
        }
        
        virtual std::shared_ptr<Shape> getShape() const = 0; //pointer needs to be deleted after use
        
        std::shared_ptr<const MapElement> getElement() const { return p_element; } //pointer should NOT be deleted
        MapHitbox & setElement(std::shared_ptr<const MapElement> p_element) {
            this->p_element = p_element;
            return *this;
        }
        
        virtual bool adjustVector(Vector & vector) const {
            return adjustVectorAmbigious(p_element, vector);
        }
        
        virtual bool adjustVectorAmbigious(std::shared_ptr<const MapElement> p_element, Vector & vector) const {
            if (p_element) {
                if (p_element->hasTag(MAP_VERTEX_TAG))
                    return adjustVector(std::static_pointer_cast<const MapVertex>(p_element), vector);
                else if (p_element->hasTag(MAP_EDGE_TAG))
                    return adjustVector(std::static_pointer_cast<const MapEdge>(p_element), vector);
            }
            return false;
        }
        
        std::shared_ptr<Shape> createBumper() const {
            return createBumperAmbigious(p_element);
        }
        
        virtual std::shared_ptr<Shape> createBumperAmbigious(std::shared_ptr<const MapElement> p_element) const {
            if (p_element) {
                if (p_element->hasTag(MAP_VERTEX_TAG))
                    return createBumper(std::static_pointer_cast<const MapVertex>(p_element));
                else if (p_element->hasTag(MAP_EDGE_TAG))
                    return createBumper(std::static_pointer_cast<const MapEdge>(p_element));
            }
            return nullptr;
        }
        
        std::vector<MapCatch> findCatches(const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const {
            return findCatchesAmbigious(p_element, p_vertices, p_edges);
        }
        
        virtual std::vector<MapCatch> findCatchesAmbigious(std::shared_ptr<const MapElement> p_element, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const { //TODO vertices and edges should be const...
            if (p_element) {
                if (p_element->hasTag(MAP_VERTEX_TAG))
                    return findCatches(std::static_pointer_cast<const MapVertex>(p_element), p_vertices, p_edges);
                else if (p_element->hasTag(MAP_EDGE_TAG))
                    return findCatches(std::static_pointer_cast<const MapEdge>(p_element), p_vertices, p_edges);
            }
            return std::vector<MapCatch>();
        }
        
        virtual bool adjustVector(std::shared_ptr<const MapVertex> p_vertex, Vector & vector) const = 0;
        virtual std::shared_ptr<Shape> createBumper(std::shared_ptr<const MapVertex> p_vertex) const = 0;
        virtual std::vector<MapCatch> findCatches(std::shared_ptr<const MapVertex> p_vertex, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const = 0;
        
        virtual bool adjustVector(std::shared_ptr<const MapEdge> p_edge, Vector & vector) const = 0;
        virtual std::shared_ptr<Shape> createBumper(std::shared_ptr<const MapEdge> p_edge) const = 0;
        virtual std::vector<MapCatch> findCatches(std::shared_ptr<const MapEdge> p_edge, const std::vector<std::shared_ptr<MapVertex>> & p_vertices, const std::vector<std::shared_ptr<MapEdge>> & p_edges) const = 0;
        
        //virtual MapHitbox * deepCopy() const = 0;
        
    protected:
        Coordinate center;
        std::shared_ptr<const MapElement> p_element = nullptr;
    };
}

#endif /* MapHitbox_hpp */

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
        
        virtual MapHitbox & move(physics::Vector vec) {
            center += vec;
            return *this;
        }
        
        virtual std::shared_ptr<Shape> getShape() const = 0; //pointer needs to be deleted after use
        
        const MapElement * getElement() const { return p_element; } //pointer should NOT be deleted
        MapHitbox & setElement(const MapElement * p_element) {
            this->p_element = p_element;
            return *this;
        }
        
        virtual bool adjustVector(physics::Vector * p_vector) const {
            return adjustVector(p_element, p_vector);
        }
        
        virtual bool adjustVector(const MapElement * p_element, physics::Vector * p_vector) const {
            if (p_element) {
                if (p_element->hasTag(MAP_VERTEX_TAG))
                    return adjustVector(static_cast<const MapVertex*>(p_element), p_vector);
                else if (p_element->hasTag(MAP_EDGE_TAG))
                    return adjustVector(static_cast<const MapEdge*>(p_element), p_vector);
            }
            return false;
        }
        
        std::shared_ptr<Shape> createBumper() const {
            return createBumper(p_element);
        }
        
        virtual std::shared_ptr<Shape> createBumper(const MapElement * p_element) const {
            if (p_element) {
                if (p_element->hasTag(MAP_VERTEX_TAG))
                    return createBumper(static_cast<const MapVertex*>(p_element));
                else if (p_element->hasTag(MAP_EDGE_TAG))
                    return createBumper(static_cast<const MapEdge*>(p_element));
            }
            return nullptr;
        }
        
        std::vector<MapCatch> findCatches(const Map * p_map) const {
            return findCatches(p_element, p_map);
        }
        
        virtual std::vector<MapCatch> findCatches(const MapElement * p_element, const Map * p_map) const {
            if (p_element) {
                if (p_element->hasTag(MAP_VERTEX_TAG))
                    return findCatches(static_cast<const MapVertex*>(p_element), p_map);
                else if (p_element->hasTag(MAP_EDGE_TAG))
                    return findCatches(static_cast<const MapEdge*>(p_element), p_map);
            }
            return std::vector<MapCatch>();
        }
        
        virtual bool adjustVector(const MapVertex * p_vertex, physics::Vector * p_vector) const = 0;
        virtual std::shared_ptr<Shape> createBumper(const MapVertex * p_vertex) const = 0;
        virtual std::vector<MapCatch> findCatches(const MapVertex * p_vertex, const Map * p_map) const = 0;
        
        virtual bool adjustVector(const MapEdge * p_edge, physics::Vector * p_vector) const = 0;
        virtual std::shared_ptr<Shape> createBumper(const MapEdge * p_edge) const = 0;
        virtual std::vector<MapCatch> findCatches(const MapEdge * p_edge, const Map * p_map) const = 0;
        
        virtual MapHitbox * deepCopy() const = 0;
        
    protected:
        Coordinate center;
        const MapElement * p_element = nullptr;
    };
}

#endif /* MapHitbox_hpp */

//
//  MapElement.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/16/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__MapElement__
#define __Nautical__MapElement__

#include "Updatable.hpp"
#include "Drawable.hpp"
#include "Tagable.hpp"

#include "Utility.hpp"
#include "Angle.hpp"
#include "Coordinate.hpp"
#include "Vector.hpp"
#include "Shape.hpp"
#include "Rectangle.hpp"
#include "Circle.hpp"
#include "MapCatch.hpp"

namespace nautical {
    class Map; //forward declaration to avoid circular inclusion
    
    const std::string MAP_ELEMENT_TAG = "map_element";
    
    class MapElement : public Updatable, public Drawable, public Tagable {
        friend class Map;
        
    public:
        MapElement(bool sticky = false) :
        sticky(sticky),
        frictionCoefficient(defaultFrictionCoefficient)
        { appendTag(MAP_ELEMENT_TAG); }
        
        virtual ~MapElement() { }
        
        Map * getParent() const { return p_parent; }
        bool setParent(Map * p_parent) {
            static bool parentSet = false;
            if (parentSet) {
                return false;
            } else {
                this->p_parent = p_parent;
                parentSet = true;
                return true;
            }
        }
        
        bool isSticky() const { return sticky; }
        double getFrictionCoefficient(double value = 1) const { return frictionCoefficient * value; }
        MapElement & setFrictionCoefficient(float frictionCoefficient) {
            this->frictionCoefficient = frictionCoefficient;
            return *this;
        }
        
        static double getDefaultFrictionCoefficient(double value = 1) { return MapElement::defaultFrictionCoefficient * value; }
        static void setDefaultFrictionCoefficient(double frictionCoefficient) { MapElement::defaultFrictionCoefficient = frictionCoefficient; }
        
        virtual void update() { }
        virtual void draw() const = 0; //for debugging
        
    protected:
        Map * p_parent;
        
    private:
        bool sticky; //whether or not gravity affects object while on edge
        double frictionCoefficient;
        static double defaultFrictionCoefficient;
    };
}

#endif /* defined(__Nautical__MapElement__) */

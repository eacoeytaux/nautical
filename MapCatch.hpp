//
//  MapCatch.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/11/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef MapCatch_hpp
#define MapCatch_hpp

#include "Shape.hpp"

namespace nautical {
    class MapElement;
    
    class MapCatch {
    public:
        MapCatch(Coordinate collision, Line line, MapElement * p_element1, MapElement * p_element2);
        virtual ~MapCatch();
        
        Coordinate getCollision() const;
        void setCollision(Coordinate collision);
        Line getLine() const;
        void setLine(Line line);
        bool containsElement(const MapElement * p_element) const;
        const MapElement * getElement(const MapElement * p_other) const;
        const MapElement * getElement1() const;
        void setElement1(const MapElement * p_element);
        const MapElement * getElement2() const;
        void setElement2(const MapElement * p_element);
        
        bool operator==(const MapCatch & mapCatch) const;
        
    private:
        Coordinate collision;
        Line line;
        const MapElement * p_element1 = nullptr,
        * p_element2 = nullptr;
    };
}

#endif /* MapCatch_hpp */

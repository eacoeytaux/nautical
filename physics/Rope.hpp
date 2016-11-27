//
//  Rope.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Rope_hpp
#define Rope_hpp

#include <vector>

#include "Mass.hpp"
#include "Spring.hpp"

namespace nautical {
    class Rope {
    public:
        Rope(Coordinate anchor, double length, double segmentLength = 15);
        ~Rope();
        
        Rope & setAnchor(const Coordinate & anchor);
        
        void update();
        
        void draw() const; //TODO delete
        
    private:
        std::vector<Mass> masses;
        std::vector<Spring> springs;
    };
}

#endif /* Rope_hpp */

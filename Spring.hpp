//
//  Spring.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Spring_hpp
#define Spring_hpp

#include "Mass.hpp"

namespace nautical {
    namespace physics {
        class Spring {
        public:
            Spring(Mass * p_mass1, Mass * p_mass2, double length, double k);
            ~Spring();
            
            const Mass * getMass1();
            Spring & setMass1(Mass * p_mass);
            const Mass * getMass2();
            Spring & setMass2(Mass * p_mass);
            double getK() const;
            Spring & setK(double k);
            double getLength() const;
            Spring & setLength(double length);
            
            void update();
            
        private:
            Mass * p_mass1,
            * p_mass2;
            double length,
            k;
        };
    }
}

#endif /* Spring_hpp */

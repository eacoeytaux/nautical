//
//  Spring.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Spring_hpp
#define Spring_hpp

#include <memory>

#include "Mass.hpp"

namespace nautical {
    class Spring {
    public:
        Spring(std::shared_ptr<Mass> p_mass1, std::shared_ptr<Mass> p_mass2, double length, double k, double damper);
        ~Spring();
        
        std::shared_ptr<const Mass> getMass1() const;
        Spring & setMass1(std::shared_ptr<Mass> p_mass);
        std::shared_ptr<const Mass> getMass2() const;
        Spring & setMass2(std::shared_ptr<Mass> p_mass);
        double getK() const;
        Spring & setK(double k);
        double getLength() const;
        Spring & setLength(double length);
        
        void update();
        
    private:
        std::shared_ptr<Mass> p_mass1,
        p_mass2;
        double length,
        k,
        damper;
    };
}

#endif /* Spring_hpp */

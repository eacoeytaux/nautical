//
//  MaxMinValue.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/19/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__MaxMinValue__
#define __Nautical__MaxMinValue__

#include <cmath>

namespace nautical {
    class MaxValue {
    public:
        MaxValue(double initial = -INFINITY);
        
        double getValue() const;
        bool update(double value);
        
    private:
        double value;
    };
    
    class MinValue {
    public:
        MinValue(double initial = INFINITY);
        
        double getValue() const;
        bool update(double value);
        
    private:
        double value;
    };
}

#endif /* defined(__Nautical__MaxMinValue__) */

//
//  MaxMinValue.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/19/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__MaxMinValue__
#define __Nautical__MaxMinValue__

#define _USE_MATH_DEFINES
#include <cmath>

namespace nautical {
    class MaxValue { //will hold max value of all values passed to update
    public:
        MaxValue(double initial = -INFINITY); //initial value default to to -INFINITY so any value will override value
        
        double getValue() const;
        bool update(double value);
        MaxValue & reset(double initial = -INFINITY);
        
    private:
        double value;
    };
    
    class MinValue { //will hold min value of all values passed to update
    public:
        MinValue(double initial = INFINITY); //initial value default to INFINITY to any value will override value
        
        double getValue() const;
        bool update(double value);
        MinValue & reset(double initial = -INFINITY);
        
    private:
        double value;
    };
}

#endif /* defined(__Nautical__MaxMinValue__) */

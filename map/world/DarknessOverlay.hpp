//
//  DarknessOverlay.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/23/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef DarknessOverlay_hpp
#define DarknessOverlay_hpp

#include "MaxMinValue.hpp"

#include <vector>

#define DARKNESS_LAYERS 3

namespace nautical {
    class Shape;
    
    class DarknessOverlay {
    public:
        static bool isInEffect();
        static void setInEffect(bool b);
        
        static float getPercentage();
        static void setPercentage(float percentage);
        
        static void addShape(Shape * p_shape, int layer);
        static void clearShapes();
        
        static void draw();
        
    private:
        static bool inEffect;
        
        static float percentage;
        static std::vector<Shape*> subtractedShapes[DARKNESS_LAYERS];
        static MinValue lowerBoundX;
        static MinValue lowerBoundY;
        static MaxValue upperBoundX;
        static MaxValue upperBoundY;
    };
}

#endif /* DarknessOverlay_hpp */

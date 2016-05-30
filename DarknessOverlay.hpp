//
//  DarknessOverlay.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/23/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef DarknessOverlay_hpp
#define DarknessOverlay_hpp

#include "Drawable.hpp"

#include "MaxMinValue.hpp"
#include "LinkedList.hpp"

#define DARKNESS_LAYERS 3

namespace nautical {
    class Shape;
    
    class DarknessOverlay : public Drawable {
    public:
        DarknessOverlay();
        DarknessOverlay(const DarknessOverlay & overlay);
        virtual ~DarknessOverlay();
        
        float getPercentage() const;
        DarknessOverlay & setPercentage(float percentage);
        
        DarknessOverlay & addShape(Shape * p_shape, int layer);
        DarknessOverlay & clearShapes();
        
        void draw() const;
        
    private:
        float percentage = 1;
        LinkedList<Shape*> subtractedShapes[DARKNESS_LAYERS];
        MinValue lowerBoundX;
        MinValue lowerBoundY;
        MaxValue upperBoundX;
        MaxValue upperBoundY;
    };
}

#endif /* DarknessOverlay_hpp */

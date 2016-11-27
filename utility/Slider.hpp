//
//  Slider.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/8/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Slider_hpp
#define Slider_hpp

#include "Coordinate.hpp"
#include "Vector.hpp"

namespace nautical {
    class Slider { //TODO add minimum values for updates //TODO derive Updatable?
    public:
        Slider(double currVal, double dVal = 1);
        virtual ~Slider();
        
        double getValue() const;
        double getTargetValue() const;
        double getDValue() const;
        Slider & setValue(double val, bool hardSet = false);
        Slider & setDValue(double dVal); //TODO bounds checking (and constructor)
        
        void update(double dt = 1);
        void updateFull();
        
    private:
        double currVal,
        targetVal,
        dVal;
    };
    
    class Slider2D { //TODO add minimum values for updates //TODO derive Updatable?
    public:
        Slider2D(Coordinate currVal, double dVal = 1);
        virtual ~Slider2D();
        
        Coordinate getValue() const;
        Coordinate getTargetValue() const;
        double getDValue() const;
        Slider2D & setValue(Coordinate val, bool hardSet = false);
        Slider2D & setDValue(double dVal);
        
        void update(double dt = 1);
        void updateFull();
        
    private:
        Coordinate currVal,
        targetVal;
        double dVal;
    };
}

#endif /* Slider_hpp */

//
//  Rope.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/5/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Rope_hpp
#define Rope_hpp

#include "StateMachine.hpp"
#include "Updatable.hpp"
#include "Drawable.hpp"

#include "Coordinate.hpp"
#include "Line.hpp"
#include "Parabola.hpp"
#include "Path.hpp"

namespace climber {
    class Rope : public nautical::StateMachine, public nautical::Updatable, public nautical::Drawable {
    public:
        enum State {
            EXTENDING,
            RETRACTING,
            SET,
            TAUGHT
        };
        
        Rope(nautical::Coordinate * p_origin, nautical::Coordinate * p_head, double length = 0);
        virtual ~Rope();
        
        nautical::Coordinate getOrigin() const;
        nautical::Coordinate getHead() const;
        double getLength() const;
        void setLength(double length);
        
        void update();
        void draw() const;
        
    private:
        double length;
        nautical::Coordinate * p_origin, //origin of Rope
        * p_head;
        nautical::Angle hookAngle;
        
        nautical::Line line;
        nautical::Parabola parabola;
        nautical::Path wave;
    };
}

#endif /* Rope_hpp */

//
//  GrapplingRope.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/5/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef GrapplingRope_hpp
#define GrapplingRope_hpp

#include "WorldObject.hpp"
#include "StateMachine.hpp"

#include "Coordinate.hpp"
#include "Line.hpp"
#include "Parabola.hpp"
#include "Path.hpp"
#include "Vector.hpp"

namespace climber {
    class Player;
    
    const std::string GRAPPLING_ROPE_TAG = "grappling_rope";
    
    class GrapplingRope : public nautical::WorldObject, public nautical::StateMachine { //TODO is rope is taught it should move with player
    public:
        enum State {
            EXTENDING,
            SET,
            RETRACTING
        };
        
        GrapplingRope(nautical::Coordinate origin, double length, nautical::Vector extendVector, double retractSpeed);
        virtual ~GrapplingRope();
        
        nautical::Coordinate getOrigin() const;
        GrapplingRope & setOrigin(nautical::Coordinate origin);
        nautical::Coordinate getHead() const;
        GrapplingRope & setHead(nautical::Coordinate head);
        double getLength() const;
        GrapplingRope & setLength(double length);
        bool isTaught() const;
        
        bool setState(int state);
        
        void update(nautical::World * p_world, double dt);
        void draw() const;
        
    private:
        /*struct Segment { //TODO
            double length;
            virtual void draw() const;
        };
        
        struct LineSegment : public Segment {
            nautical::Line line;
            void draw() const;
        };
        
        struct ParabolaSegment : public Segment {
            nautical::Parabola parabola;
            void draw() const;
        };
        
        struct WaveSegment : public Segment {
            nautical::Path wave;
            void draw() const;
        };
        
        nautical::LinkedList<Segment*> segments;*/
        
        nautical::Coordinate origin, //origin of Rope
        head;
        double length;
        bool taught;
        nautical::Vector extendVector;
        double retractSpeed;
        nautical::Angle hookAngle;
        bool shouldRetract = false, //bool used to note is grapple should retract once done extending
        overrideRetract = false; //bool used to signal when it is actually time to retract rope
        nautical::Line line;
        nautical::Parabola parabola;
        nautical::Path wave;
        
        bool openState(int state);
    };
}

#endif /* GrapplingRope_hpp */

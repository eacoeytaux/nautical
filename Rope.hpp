//
//  Rope.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/5/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Rope_hpp
#define Rope_hpp

#include "WorldObject.hpp"
#include "StateMachine.hpp"

#include "Coordinate.hpp"
#include "Line.hpp"
#include "Parabola.hpp"
#include "Path.hpp"
#include "Vector.hpp"

namespace climber {
    class Player;
    
    const std::string ROPE_TAG = "rope";
    
    class Rope : public nautical::WorldObject, public nautical::StateMachine { //TODO is rope is taught it should move with player
    public:
        enum State {
            EXTENDING,
            SET,
            RETRACTING
        };
        
        Rope(Player * p_parent, nautical::Coordinate origin, double length, nautical::Vector extendVector, double retractSpeed);
        virtual ~Rope();
        
        nautical::Coordinate getOrigin() const;
        Rope & setOrigin(nautical::Coordinate origin);
        nautical::Coordinate getHead() const;
        Rope & setHead(nautical::Coordinate head);
        double getLength() const;
        Rope & setLength(double length);
        bool isTaught() const;
        
        bool setState(int state);
        
        void update();
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
        
        Player * p_parent = nullptr;
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

#endif /* Rope_hpp */

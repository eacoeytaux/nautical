//
//  Mass.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Mass_hpp
#define Mass_hpp

#include "Coordinate.hpp"
#include "Vector.hpp"

namespace nautical {
    namespace physics {
        class Mass {
        public:
            Mass(Coordinate pos, double m = 1.0);
            ~Mass();
            
            double getM() const;
            Mass & setM(double m);
            
            Coordinate getPosition() const;
            Mass & setPosition(const Coordinate & pos);
            virtual Mass & updatePosition();
            
            Vector getVelocity() const;
            Mass & setVelocity(const Vector & vel);
            Mass & addVelocity(const Vector & vel);
            virtual Mass & updateVelocity();
            
            Vector getForce() const;
            Mass & setForce(const Vector & force);
            Mass & addForce(const Vector & force);
            Mass & setAcceleration(const Vector & accel);
            Mass & addAcceleration(const Vector & accel);
            virtual Mass & updateForce();
            
        private:
            double m;
            Coordinate pos;
            Vector vel, force;
        };
    }
}

#endif /* Mass_hpp */

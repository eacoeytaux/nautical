//
//  Mass.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Mass_hpp
#define Mass_hpp

#include <memory>
#include <vector>

#include "Coordinate.hpp"
#include "Vector.hpp"

namespace nautical {
    namespace physics {
        class Mass {
        public:
            struct Accelerator {
                Vector accel;
                virtual Vector accelerate(const Mass & mass) const {
                    return accel;
                }
            };
            
            struct ForceAccelerator : public Accelerator {
                Vector force;
                virtual Vector accelerate(const Mass & mass) const {
                    return force * mass.getM();
                }
            };
            
            Mass(double m, Coordinate pos, Vector vel = Vector());
            ~Mass();
            
            double getM() const;
            Mass & setM(double m);
            
            Coordinate getPosition() const;
            Mass & setPosition(const Coordinate & pos);
            
            Vector getVelocity() const;
            Mass & setVelocity(const Vector & vel);
            Mass & addVelocity(const Vector & vel);
            
            bool isImmobile() const;
            Mass & setImmobile(bool b);
            
            Mass & addAccelerator(std::shared_ptr<Accelerator> accelerator);
            
            void update(double dt = 1);
            
        private:
            double m;
            Coordinate pos;
            Vector vel;
            bool immobile = false;
            std::vector<std::shared_ptr<Accelerator>> accelerators;
            
            std::pair<Vector, Vector> evaluate(const std::pair<Vector, Vector> & derivative, double dt);
        };
    }
}

#endif /* Mass_hpp */

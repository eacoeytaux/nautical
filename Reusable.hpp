//
//  Reusable.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/18/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Reusable_hpp
#define Reusable_hpp

namespace nautical {
    template<typename T>
    class Reusable { //holds onto value so it doesn't need to be recalculated everytime it's accessed
    public:
        Reusable(T value, bool set) : value(value), set(set) { }
        virtual ~Reusable() { }
        
        T getValue() {
            return value;
        }
        
        void setValue(T value) {
            this->value = value;
            set = true;
        }
        
        bool isSet() {
            return set;
        }
        
        void setSet(bool set = false) {
            this->set = set;
        }
        
    private:
        T value;
        bool set;
    };
}

#endif /* Reusable_hpp */

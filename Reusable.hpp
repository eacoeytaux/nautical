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
        
        T getValue() { return value; }
        Reusable & setValue(T value) {
            this->value = value;
            set = true;
            return *this;
        }
        
        bool isSet() { return set; }
        Reusable & setSet(bool set = false) {
            this->set = set;
            return *this;
        }
        
    private:
        T value;
        bool set;
    };
}

#endif /* Reusable_hpp */

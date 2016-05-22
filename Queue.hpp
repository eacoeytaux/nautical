//
//  Queue.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Queue__
#define __Nautical__Queue__

#include "LinkedList.hpp"

namespace nautical {
    template<typename T>
    class Queue : public LinkedList<T> {
    public:
        Queue(bool containsDuplicates = true);
        virtual ~Queue();
        
        bool pop(T * p_element);
        bool peek(T * p_element) const;
    };
    
    /* function definitions - not in seperate file because of template class */
    
    template<typename T>
    Queue<T>::Queue(bool containsDuplicates) { LinkedList<T>::setContainsDuplicates(containsDuplicates); }
    
    template<typename T>
    Queue<T>::~Queue() { }
    
    template<typename T>
    bool Queue<T>::pop(T * p_element) {
        if (LinkedList<T>::getFirstElement(p_element)) {
            LinkedList<T>::remove(*p_element);
            return true;
        } else {
            return false;
        }
    }
    
    template<typename T>
    bool Queue<T>::peek(T * p_element) const {
        return LinkedList<T>::getFirstElement(p_element);
    }
}

#endif /* defined(__Nautical__Queue__) */

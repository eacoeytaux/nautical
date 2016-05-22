//
//  Stack.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Stack__
#define __Nautical__Stack__

#include "LinkedList.hpp"

namespace nautical {
    template<typename T>
    class Stack : public LinkedList<T> {
    public:
        Stack(bool containsDuplicates = true);
        virtual ~Stack();
        
        bool insert(T element);
        
        bool pop(T * p_element);
        bool peek(T * p_element) const;
    };
    
    /* function definitions - not in seperate file because of template class */
    
    template<typename T>
    Stack<T>::Stack(bool containsDuplicates) { Collection<T>::setContainsDuplicates(containsDuplicates); }
    
    template<typename T>
    Stack<T>::~Stack() { }
    
    template<typename T>
    bool Stack<T>::insert(T element) {
        insertHead(element);
    }
    
    template<typename T>
    bool Stack<T>::pop(T * p_element) {
        if (LinkedList<T>::getLastElement(P_element)) {
            LinkedList<T>::remove(*p_element);
            return true;
        } else {
            return false;
        }
    }
    
    template<typename T>
    bool Stack<T>::peek(T * p_element) const {
        return LinkedList<T>::getLastElement(p_element);
    }
}

#endif /* defined(__Nautical__Stack__) */

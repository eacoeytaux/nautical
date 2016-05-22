//
//  Collection.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/20/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Collection__
#define __Nautical__Collection__

#include "Iterator.hpp"

namespace nautical {
    template<typename T>
    class Collection {
    public:
        int size() const;
        bool empty() const;
        bool doesContainDuplicates();
        Collection<T> & setContainsDuplicates(bool containsDuplicates);
        
        virtual bool clear() = 0;
        virtual bool insert(T element) = 0;
        virtual bool remove(T element) = 0;
        virtual bool contains(T element) const = 0;
        
        bool insert(Collection<T> * collection);
        bool remove(Collection<T> * collection);
        
        virtual Iterator<T> * createIterator(bool selfDestruct = true) const = 0; //must return pointer because Iterator is an abstract class
        
    protected:
        bool containsDuplicates = true;
        int sizeVal = 0;
    };
    
    /* function definitions - not in seperate file because of template class */
    
    template<typename T>
    int Collection<T>::size() const {
        return sizeVal;
    }
    
    template<typename T>
    bool Collection<T>::empty() const {
        return sizeVal == 0;
    }
    
    template<typename T>
    bool Collection<T>::doesContainDuplicates() {
        return containsDuplicates;
    }
    
    template<typename T>
    Collection<T> & Collection<T>::setContainsDuplicates(bool containsDuplicates) {
        this->containsDuplicates = containsDuplicates;
        return *this;
    }
    
    template<typename T>
    bool Collection<T>::insert(Collection<T> * collection) {
        bool success = true;
        for (Iterator<T> * iterator = collection->createIterator(); !iterator->complete(); iterator->next()) {
            success &= insert(iterator->current());
        }
        return success;
    }
    
    template<typename T>
    bool Collection<T>::remove(Collection<T> * collection) {
        bool success = true;
        for (Iterator<T> * iterator = collection->createIterator(); !iterator->complete(); iterator->next()) {
            success &= remove(iterator->current());
        }
        return success;
    }
}

#endif /* defined(__Nautical__Collection__) */

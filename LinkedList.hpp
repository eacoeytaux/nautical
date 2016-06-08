//
//  LinkedList.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__LinkedList__
#define __Nautical__LinkedList__

#include "Collection.hpp" //base class

#include "Logger.hpp"

//TODO by adding a tailNode, cycling through entire list to add item would be unneccessary

namespace nautical {
    template<typename T>
    class LinkedList : public Collection<T> {
    public:
        LinkedList(bool containDuplicates = true);
        LinkedList(const LinkedList<T> & list);
        virtual ~LinkedList();
        
        bool clear();
        bool insert(T element);
        bool insertHead(T element);
        bool remove(T element);
        bool contains(T element) const;
        
        bool getElementByIndex(int index, T * p_element) const;
        bool getFirstElement(T * p_element) const;
        bool getLastElement(T * p_element) const;
        
        Iterator<T> * createIterator(bool selfDestruct = true) const;
        
        LinkedList<T> & operator=(LinkedList<T> list);
        LinkedList<T> operator+(const LinkedList<T> & list) const;
        LinkedList<T> & operator+=(const LinkedList<T> & list);
        
    protected:
        struct Node { //Node struct declared here so Iterator can use
            Node(T element) : element(element) { }
            virtual ~Node() { }
            
            T element;
            struct Node * next = nullptr,
            * prev = nullptr;
        };
        
        class IteratorDerived : public Iterator<T> {
            friend class LinkedList<T>;
            
        public:
            Iterator<T> * first();
            Iterator<T> * next();
            bool complete() const;
            T current() const;
            
        private:
            const LinkedList<T> * list = nullptr;
            const Node * currentNode = nullptr;
            
            IteratorDerived(const LinkedList<T> * list, bool selfDestruct = true);
        };
        
        Node * headNode = nullptr;
    };
    
    /* function definitions - not in seperate file because of template class */
    
    template<typename T>
    LinkedList<T>::LinkedList(bool containDuplicates) { Collection<T>::setContainsDuplicates(containDuplicates); }
    
    template<typename T>
    LinkedList<T>::LinkedList(const LinkedList<T> & list) {
        Collection<T>::setContainsDuplicates(list.Collection<T>::containsDuplicates);
        for (Iterator<T> * iterator = list.createIterator(); !iterator->complete(); iterator->next()) {
            insert(iterator->current());
        }
    }
    
    template<typename T>
    LinkedList<T>::~LinkedList() {
        clear();
    }
    
    template<typename T>
    bool LinkedList<T>::clear() {
        Node * tempNode = headNode;
        while (tempNode) {
            Node * nodeToDelete = tempNode;
            tempNode = tempNode->next;
            delete nodeToDelete;
        }
        
        headNode = nullptr;
        
        Collection<T>::sizeVal = 0;
        return true;
    }
    
    template<typename T>
    bool LinkedList<T>::insert(T element) {
        if (!Collection<T>::containsDuplicates)
            if (contains(element))
                return false;
        
        LinkedList<T>::Node * newNode = new Node(element);
        
        if (headNode == nullptr) {
            headNode = newNode;
        } else {
            LinkedList<T>::Node * traversalNode;
            for (traversalNode = headNode; traversalNode->next; traversalNode = traversalNode->next);
            traversalNode->next = newNode;
            newNode->prev = traversalNode;
        }
        
        Collection<T>::sizeVal++;
        return true;
    }
    
    template<typename T>
    bool LinkedList<T>::insertHead(T element) {
        if (!Collection<T>::containsDuplicates)
            if (contains(element))
                return false;
        
        LinkedList<T>::Node * newNode = new Node(element);
        
        if (headNode == nullptr) {
            headNode = newNode;
        } else {
            headNode->prev = newNode;
            newNode->next = headNode;
            headNode = newNode;
        }
        
        Collection<T>::sizeVal++;
        return true;
    }
    
    template<typename T>
    bool LinkedList<T>::remove(T element) {
        if (headNode == nullptr) { //list is empty
            Logger::writeLog(WARNING_MESSAGE, "LinkedList<T>::remove(): attempted to remove element from empty list\n");
            return false;
        }
        
        if (headNode->element == element) {
            Node * toDelete = headNode;
            headNode = headNode->next;
            if (headNode)
                headNode->prev = nullptr;
            delete toDelete;
            
            Collection<T>::sizeVal--;
            return true;
        }
        
        for (Node * tempNode = headNode; tempNode->next; tempNode = tempNode->next) {
            if (tempNode->next->element == element) {
                Node * toDelete = tempNode->next;
                tempNode->next = tempNode->next->next;
                if (tempNode->next)
                    tempNode->next->prev = tempNode;
                delete toDelete;
                
                Collection<T>::sizeVal--;
                return true;
            }
        }
        
        Logger::writeLog(WARNING_MESSAGE, "LinkedList<T>::remove(): attempted to remove element not found in list\n");
        return false; //t not found in list
    }
    
    template<typename T>
    bool LinkedList<T>::contains(T element) const {
        for (Node * tempNode = headNode; tempNode != nullptr; tempNode = tempNode->next) {
            if (tempNode->element == element)
                return true;
        }
        return false;
    }
    
    template<typename T>
    bool LinkedList<T>::getElementByIndex(int index, T * p_element) const {
        if ((index < 0) || (index > Collection<T>::sizeVal - 1)) {
            Logger::writeLog(ERROR_MESSAGE, "LinkedList<T>::getElement(): index out of bounds");
            return false;
        }
        
        Node * tempNode = headNode;
        while (index > 0) {
            tempNode = tempNode->next;
            index--;
        }
        if (p_element)
            *p_element = tempNode->element;
        return true;
    }
    
    template<typename T>
    bool LinkedList<T>::getFirstElement(T * p_element) const {
        if (headNode) {
            if (p_element)
                *p_element = headNode->element;
            return true;
        } else {
            return false;
        }
    }
    
    template<typename T>
    bool LinkedList<T>::getLastElement(T * p_element) const { //TODO make this tailNode when added
        return getElementByIndex(Collection<T>::sizeVal - 1, p_element);
    }
    
    template<typename T>
    Iterator<T> * LinkedList<T>::createIterator(bool selfDestruct) const {
        return new IteratorDerived(this, selfDestruct);
    }
    
    template<typename T>
    LinkedList<T> & LinkedList<T>::operator=(LinkedList<T> list) {
        clear();
        for (Iterator<T> * iterator = list.createIterator(); !iterator->complete(); iterator->next()) {
            insert(iterator->current());
        }
        return *this;
    }
    
    template<typename T>
    LinkedList<T> LinkedList<T>::operator+(const LinkedList<T> & list) const {
        LinkedList<T> ret = *this;
        for (Iterator<T> * iterator = list.createIterator(); !iterator->complete(); iterator->next()) {
            ret.insert(iterator->current());
        }
        return ret;
    }
    
    template<typename T>
    LinkedList<T> & LinkedList<T>::operator+=(const LinkedList<T> & list) {
        for (Iterator<T> * iterator = list.createIterator(); !iterator->complete(); iterator->next()) {
            this->insert(iterator->current());
        }
        return *this;
    }
    
    /* Iterator definitions for LinkedList */
    
    template<typename T>
    LinkedList<T>::IteratorDerived::IteratorDerived(const LinkedList<T> * list, bool selfDestruct) :
    Iterator<T>(selfDestruct), 
    list(list) {
        first();
    }
    
    template<typename T>
    Iterator<T> * LinkedList<T>::IteratorDerived::first() {
        currentNode = list->headNode;
        return this;
    }
    
    template<typename T>
    Iterator<T> * LinkedList<T>::IteratorDerived::next() {
        currentNode = currentNode->next;
        return this;
    }
    
    template<typename T>
    bool LinkedList<T>::IteratorDerived::complete() const {
        if (!currentNode) {
            if (Iterator<T>::shouldSelfDestruct())
                delete this;
            return true;
        } else {
            return false;
        }
    }
    
    template<typename T>
    T LinkedList<T>::IteratorDerived::current() const {
        return currentNode->element;
    }
}

#endif /* defined(__Nautical__LinkedList__) */

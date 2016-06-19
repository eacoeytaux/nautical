//
//  SortedList.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__SortedList__
#define __Nautical__SortedList__

#include "Collection.hpp" //base class

#include "Logger.hpp"
#include "MaxMinValue.hpp"

template<typename T>
double returnInfinity(const T * p_element) {
    return INFINITY;
}

template<typename T>
double returnDouble(const double * p_d) {
    return *p_d;
}

namespace nautical {
    template<typename T>
    class SortedList : public Collection<T> {
    public:
        SortedList(double (*weigh)(const T*) = nullptr, bool containDuplicates = true); //weigh is default set to nullptr but MUST be set to non-nullptr to function
        virtual ~SortedList();
        
        bool clear();
        bool insert(T element);
        bool insert(T element, double negativeSpanVal, double positiveSpanVal);
        bool remove(T element);
        bool contains(T element) const;
        
        bool getElementByIndex(int index, T * p_element);
        bool getElementByWeight(double weight, T * p_element); //returns first node that is equal to or greater than weight
        
        void sort();
        bool updateElement(T element, double negativeSpanVal = 0, double positiveSpanVal = 0); //if weight, replace weigh with new weight
        
        double getSmallestWeight() const;
        double getLargestWeight() const;
        
        Iterator<T> * createIterator(bool shouldSelfDestruct = true) const;
        Iterator<T> * createIterator(double lowerBound, double upperBound, bool selfDestruct = true) const;
        
        SortedList<T> operator+(const SortedList<T> & list) const;
        SortedList<T> & operator+=(const SortedList<T> & list);
        
    protected:
        struct Node {
            Node(T element) : element(element) { }
            virtual ~Node() { }
            
            T element;
            struct Node * next = nullptr,
            * prev = nullptr;
        };
        
        class IteratorDerived : public Iterator<T> {
            friend class SortedList<T>;
            
        public:
            Iterator<T> * first();
            Iterator<T> * next();
            bool complete() const;
            T current() const;
            double currentWeight() const;
            
        private:
            const SortedList<T> * p_list = nullptr;
            double (*weigh)(const T*);
            const Node * currentNode = nullptr;
            bool bound;
            double lowerBound, upperBound;
            
            IteratorDerived(const SortedList<T> * p_list, double (*weigh)(const T*), bool selfDestruct = true);
            IteratorDerived(const SortedList<T> * p_list, double (*weigh)(const T*), double lowerBound, double upperBound, bool selfDestruct = true);
        };
        
        double (*weigh)(const T*);
        Node * headNode = nullptr, * tailNode = nullptr;
        MaxValue negativeSpan = MaxValue(0), positiveSpan = MaxValue(0);
        
        const Node * getNodeByWeight(double weight) const;
        int floatNode(Node * node);
        int sinkNode(Node * node);
    };
    
    template<typename T>
    SortedList<T>::SortedList(double (*weigh)(const T*), bool containDuplicates) :
    weigh(weigh) {
        Collection<T>::setContainsDuplicates(containDuplicates);
    }
    
    template<typename T>
    SortedList<T>::~SortedList() {
        clear();
    }
    
    template<typename T>
    bool SortedList<T>::clear() {
        Node * traversalNode = headNode;
        while (traversalNode) {
            Node * nodeToDelete = traversalNode;
            traversalNode = traversalNode->next;
            delete nodeToDelete;
        }
        headNode = tailNode = nullptr;
        
        Collection<T>::sizeVal = 0;
        return true;
    }
    
    template<typename T>
    bool SortedList<T>::insert(T element) {
        return insert(element, 0, 0);
    }
    
    template<typename T>
    bool SortedList<T>::insert(T element, double negativeSpanVal, double positiveSpanVal) {
        if (!Collection<T>::containsDuplicates)
            if (contains(element))
                return false;
        
        Node * node = new Node(element);
        
        if (headNode == nullptr) {
            headNode = node;
            tailNode = node;
        } else {
            //insert at back
            tailNode->next = node;
            node->prev = tailNode;
            tailNode = node;
            floatNode(node);
            
            //insert at front
            //headNode->prev = node;
            //newNode->next = node;
            //headNode = node;
            //sinkNode(node);
        }
        
        negativeSpan.update(fabs(negativeSpanVal));
        positiveSpan.update(fabs(positiveSpanVal));
        
        Collection<T>::sizeVal++;
        return true;
    }
    
    template<typename T>
    bool SortedList<T>::remove(T element) {
        if (Collection<T>::sizeVal == 0) {
            Logger::writeLog(WARNING_MESSAGE, "SortedCollection<T>::remove(): attempted to remove element from empty list\n");
            return false;
        }
        
        for (Node * traversalNode = headNode; traversalNode; traversalNode = traversalNode->next) {
            if (traversalNode->element == element) { //element to remove found
                Node * node = traversalNode;
                
                if (node->prev == nullptr) { //node is head
                    if (node->next == nullptr) { //node is only node
                        headNode = tailNode = nullptr;
                    } else {
                        node->next->prev = nullptr;
                        headNode = node->next;
                    }
                } else if (node->next == nullptr) { //node is tail
                    node->prev->next = nullptr;
                    tailNode = node->prev;
                } else {
                    Node * tempNode = node->prev;
                    node->next->prev = node->prev;
                    tempNode->next = node->next;
                }
                
                delete node;
                Collection<T>::sizeVal--;
                
                return true;
            }
        }
        Logger::writeLog(WARNING_MESSAGE, "SortedCollection<T>::remove(): element to remove could not be found in list\n");
        
        return false;
    }
    
    template<typename T>
    bool SortedList<T>::contains(T element) const {
        for (Node * traversalNode = headNode; traversalNode; traversalNode = traversalNode->next) {
            if (traversalNode->element == element)
                return true;
        }
        return false;
    }
    
    template<typename T>
    bool SortedList<T>::getElementByIndex(int index, T * p_element) {
        //TODO boundary checking
        
        for (Node * traversalNode = headNode; traversalNode; traversalNode = traversalNode->next) {
            if (index-- == 0) {
                if (p_element)
                    *p_element = traversalNode->element;
                return true;
            }
        }
        
        return false;
    }
    
    template<typename T>
    bool SortedList<T>::getElementByWeight(double weight, T * p_element) {
        Node * node = getNodeByWeight(weight);
        
        if (node) {
            if (p_element)
                *p_element = node->element;
            return true;
        } else {
            return false;
        }
    }
    
    template<typename T>
    void SortedList<T>::sort() {
        int skips = 0;
        for (Node * traversalNode; traversalNode; traversalNode = traversalNode->next) {
            if (skips-- > 0)
                continue;
            
            skips = floatNode(traversalNode);
        }
    }
    
    template<typename T>
    bool SortedList<T>::updateElement(T element, double negativeSpanVal, double positiveSpanVal) {
        for (Node * traversalNode = headNode; traversalNode; traversalNode = traversalNode->next) {
            if (traversalNode->element == element) {
                if (weigh)
                    traversalNode->weigh = weigh;
                
                negativeSpan.update(negativeSpanVal);
                positiveSpan.update(positiveSpanVal);
                
                floatNode(fabs(traversalNode));
                sinkNode(fabs(traversalNode));
                return true;
            }
        }
        
        return false;
    }
    
    template<typename T>
    double SortedList<T>::getSmallestWeight() const {
        if (headNode)
            return *(headNode->weight);
        else
            return 0;
    }
    
    template<typename T>
    double SortedList<T>::getLargestWeight() const {
        if (tailNode)
            return *(tailNode->weight);
        else
            return 0;
    }
    
    template<typename T>
    const typename SortedList<T>::Node * SortedList<T>::getNodeByWeight(double weight) const {
        for (Node * traversalNode = headNode; traversalNode; traversalNode = traversalNode->next) {
            if (weigh(&(traversalNode->element)) >= weight)
                return traversalNode;
        }
        
        return nullptr;
    }
    
    template<typename T>
    int SortedList<T>::floatNode(Node * node) {
        int floats = 0;
        
        while ((node->prev) && (weigh(&(node->element)) < weigh(&(node->prev->element)))) { //once node encounters smaller node it stops - this depends on all smaller nodes being sorted
            Node * tempNode = node->prev;
            
            tempNode->next = node->next;
            
            node->prev = tempNode->prev;
            node->next = tempNode;
            
            tempNode->prev = node;
            
            if (node->prev) node->prev->next = node;
            else headNode = node;
            
            if (tempNode->next) tempNode->next->prev = tempNode;
            else tailNode = tempNode;
            
            floats++;
        }
        
        return floats;
    }
    
    template<typename T>
    int SortedList<T>::sinkNode(Node * node) {
        int sinks = 0;
        
        while ((node->next) && (node->next->weight < node->weight)) { //once node encounters larger node it stops - this depends on all larger nodes being sorted
            Node * tempNode = node->next;
            
            tempNode->prev = node->prev;
            
            node->next = tempNode->next;
            node->prev = tempNode;
            
            tempNode->next = node;
            
            if (node->next) node->next->prev = node;
            else tailNode = node;
            
            if (tempNode->prev) tempNode->prev->next = tempNode;
            else headNode = tempNode;
            
            sinks++;
        }
        
        return sinks;
    }
    
    template<typename T>
    Iterator<T> * SortedList<T>::createIterator(bool selfDestruct) const {
        return new IteratorDerived(this, weigh, selfDestruct);
    }
    
    template<typename T>
    Iterator<T> * SortedList<T>::createIterator(double lowerBound, double upperBound, bool selfDestruct) const {
        return new IteratorDerived(this, weigh, lowerBound - negativeSpan.getValue(), upperBound + negativeSpan.getValue(), selfDestruct);
    }
    
    template<typename T>
    SortedList<T> SortedList<T>::operator+(const SortedList<T> & list) const {
        SortedList<T> ret = *this;
        for (Iterator<T> * iterator = list.createIterator(); !iterator->complete(); iterator->next()) {
            ret.insert(iterator->current(), iterator->currentWeight());
        }
        return ret;
    }
    
    template<typename T>
    SortedList<T> & SortedList<T>::operator+=(const SortedList<T> & list) {
        for (Iterator<T> * iterator = list.createIterator(); !iterator->complete(); iterator->next()) {
            this->insert(iterator->current(), iterator->currentWeight());
        }
        return *this;
    }
    
    /* Iterator definitions for SortedList */
    
    template<typename T>
    SortedList<T>::IteratorDerived::IteratorDerived(const SortedList<T> * p_list, double (*weigh)(const T*), bool selfDestruct) :
    Iterator<T>(selfDestruct),
    p_list(p_list),
    weigh(weigh),
    bound(false) {
        first();
    }
    
    template<typename T>
    SortedList<T>::IteratorDerived::IteratorDerived(const SortedList<T> * p_list, double (*weigh)(const T*), double lowerBound, double upperBound, bool selfDestruct) :
    Iterator<T>(selfDestruct),
    p_list(p_list),
    weigh(weigh),
    bound(true),
    lowerBound(lowerBound),
    upperBound(upperBound) {
        first();
    }
    
    template<typename T>
    Iterator<T> * SortedList<T>::IteratorDerived::first() {
        currentNode = bound ? p_list->getNodeByWeight(lowerBound) : p_list->headNode;
        return this;
    }
    
    template<typename T>
    Iterator<T> * SortedList<T>::IteratorDerived::next() {
        currentNode = currentNode->next;
        return this;
    }
    
    template<typename T>
    bool SortedList<T>::IteratorDerived::complete() const {
        if (!currentNode || (bound && (weigh(&(currentNode->element)) > upperBound))) {
            if (Iterator<T>::shouldSelfDestruct())
                delete this;
            return true;
        } else {
            return false;
        }
    }
    
    template<typename T>
    T SortedList<T>::IteratorDerived::current() const {
        return currentNode->element;
    }
    
    template<typename T>
    double SortedList<T>::IteratorDerived::currentWeight() const {
        return *(currentNode->p_weight);
    }
}

#endif /* defined(__Nautical__SortedList__) */

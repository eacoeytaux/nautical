//
//  Tagable.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/24/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef Tagable_hpp
#define Tagable_hpp

#include <string>

#include "Utility.hpp"
#include "LinkedList.hpp"

namespace nautical {
    const std::string HEADER_TAG = "nautical";
    
    class Tagable { //base class for anything with a tag //TODO move this to a .cpp file
    public:
        Tagable() {
            static int id = 0;
            this->id = id++;
            appendTag(HEADER_TAG);
        }
        
        virtual ~Tagable() { }
        
        int getID() const { return id; }
        
        std::string getTag(bool fullPath = false) const {
            if (fullPath) {
                std::string tag = "";
                tags.getLastElement(&tag);
                return tag;
            }
            
            Iterator<std::string> * iterator = tags.createIterator();
            std::string tag = iterator->current();
            for (iterator->next(); !iterator->complete(); iterator->next()) {
                tag.append("::");
                tag.append(iterator->current());
            }
            return tag;
        }
        
        bool hasTag(std::string tag) const {
            for(Iterator<std::string> * iterator = tags.createIterator(); !iterator->complete(); iterator->next()) {
                if (stringEqual(tag, iterator->current())) {
                    return true;
                }
            }
            return false;
        }
        
        Tagable & operator=(Tagable other) {
            id = other.id;
            tags = other.tags;
            return *this;
        }
        
    protected:
        Tagable & appendTag(std::string tag) {
            tags.insert(tag);
            return *this;
        }
        
    private:
        int id;
        LinkedList<std::string> tags;
    };
}

#endif /* Tagable_hpp */

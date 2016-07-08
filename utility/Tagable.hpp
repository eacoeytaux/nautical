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
#include <vector>

#include "Utility.hpp"

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
        
        int getID() const {
            return id;
        }
        
        std::string getTag(bool fullPath = false) const {
            if (!fullPath)
                return tags.back();
            
            std::vector<const std::string>::iterator it = tags.begin();
            std::string tag = *it;
            for (it++; it != tags.end(); it++) {
                tag.append("::");
                tag.append(*it);
            }
            return tag;
        }
        
        bool hasTag(std::string tag) const {
            for(std::vector<const std::string>::iterator it = tags.begin(); it != tags.end(); it++) {
                if (stringEqual(tag, *it)) {
                    return true;
                }
            }
            return false;
        }
        
        Tagable & operator=(const Tagable & other) {
            id = other.id;
            tags = other.tags;
            return *this;
        }
        
    protected:
        void appendTag(std::string tag) {
            tags.push_back(tag);
        }
        
    private:
        int id;
        std::vector<std::string> tags;
    };
}

#endif /* Tagable_hpp */

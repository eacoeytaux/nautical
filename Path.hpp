//
//  Path.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/3/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Path_hpp
#define Path_hpp

#include "Drawable.hpp"

#include "Coordinate.hpp"
#include "Line.hpp"
#include "Vector.hpp"

namespace nautical {
    class Path : public Drawable {
    public:
        Path(Coordinate origin = Coordinate()) : origin(origin) { }
        
        virtual ~Path() { }
        
        int getCount() {
            return path.size();
        }
        
        bool addVector(Vector vector) {
            return path.insert(vector);
        }
        
        bool clear() {
            return path.clear();
        }
        
        Coordinate getEndPoint() {
            Coordinate endPoint = origin;
            for (Iterator<Vector> * iterator = path.createIterator(); !iterator->complete(); iterator->next())
                endPoint += iterator->current();
            
            return endPoint;
        }
        
        void draw() const {
            Coordinate lastCoor = origin;
            for (Iterator<Vector> * iterator = path.createIterator(); !iterator->complete(); iterator->next()) {
                Coordinate coor = lastCoor + iterator->current();
                GraphicsManager::drawLine(Line(lastCoor, coor), getColor());
                lastCoor = coor;
            }
        }
        
    private:
        Coordinate origin;
        LinkedList<Vector> path;
    };
}

#endif /* Path_hpp */

//
//  Drawable.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/19/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Drawable__
#define __Nautical__Drawable__

#include "GraphicsManager.hpp"
#include "Color.hpp"

namespace nautical {
    class Drawable { //base class for anything than can be drawn
    public:
        Color getColor() const { return color; }
        
        const Drawable & setColor(Color color) const {
            this->color = color;
            return *this;
        }
        
        int getTimestamp() const { return timestamp; }
        
        void draw(int timestamp) const {
            if (this->timestamp != timestamp) {
                this->timestamp = timestamp;
                draw();
            }
        }
        
        virtual void draw(Color color) const {
            this->color = color;
            draw();
        }
        
        virtual void draw() const = 0;
        
    private:
        mutable Color color = DEFAULT_COLOR;
        mutable int timestamp = -1;
    };
}

#endif /* defined(__Nautical__Drawable__) */

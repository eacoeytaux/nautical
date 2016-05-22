//
//  Color.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Color__
#define __Nautical__Color__

#include "Logger.hpp"

namespace nautical {
    class Color {
    public:
        Color(unsigned char R = 255, unsigned char G = 255, unsigned char B = 255, unsigned char A = 255);
        
        unsigned char getR() const;
        Color & setR(unsigned char R);
        unsigned char getG() const;
        Color & setG(unsigned char G);
        unsigned char getB() const;
        Color & setB(unsigned char B);
        unsigned char getA() const;
        Color & setA(unsigned char A);
        
    private:
        unsigned char R; //red value
        unsigned char G; //green value
        unsigned char B; //blue value
        unsigned char A; //alpha value
    };
    
    const Color WHITE(255, 255, 255);
    const Color BLACK(0, 0, 0);
    const Color RED(255, 0, 0);
    const Color GREEN(0, 255, 0);
    const Color BLUE(0, 0, 255);
    const Color CYAN(0, 255, 255);
    const Color MAGENTA(255, 0, 255);
    const Color YELLOW(255, 255, 0);
    
    static const Color DEFAULT_COLOR = WHITE;
}

#endif /* defined(__Nautical__Color__) */

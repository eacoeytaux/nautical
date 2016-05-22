//
//  Color.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Color.hpp"

using namespace nautical;

Color::Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
    setR(R);
    setG(G);
    setB(B);
    setA(A);
}

unsigned char Color::getR() const {
    return R;
}

Color & Color::setR(unsigned char R) {
    this->R = R;
    return *this;
}

unsigned char Color::getG() const {
    return G;
}

Color & Color::setG(unsigned char G) {
    this->G = G;
    return *this;
}

unsigned char Color::getB() const {
    return B;
}

Color & Color::setB(unsigned char B) {
    this->B = B;
    return *this;
}

unsigned char Color::getA() const {
    return A;
}

Color & Color::setA(unsigned char A) {
    this->A = A;
    return *this;
}

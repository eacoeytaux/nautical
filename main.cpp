//
//  main.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifdef __APPLE__
#include "SDL2/SDL.h"
#endif
#ifdef _WIN32
#include <SDL.h>
#endif

#include "GameManager.hpp"

int main(int argc, char * argv[]) {
    nautical::GameManager::run();
    return 0;
}

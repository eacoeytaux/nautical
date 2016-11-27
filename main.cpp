//
//  main.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Logger.hpp"
#include "GameManager.hpp"

int main(int argc, char * argv[]) {
    nautical::Logger::startup();
    nautical::GameManager::run();
    nautical::Logger::shutdown();
    return 0;
}

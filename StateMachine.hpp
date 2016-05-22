//
//  StateMachine.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/5/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef StateMachine_hpp
#define StateMachine_hpp

#include "Logger.hpp"

namespace nautical {
    class StateMachine {
    public:
        StateMachine(int initState) : currentState(initState) { }
        virtual ~StateMachine() { }
        
        int getState() const { return currentState; }
        
        StateMachine & setState(int state) {
            if (!closeState(state))
                Logger::writeLog(WARNING, "StateMachine::setState(): could not close current state");
            else if (!openState(state))
                Logger::writeLog(WARNING, "StateMachine::setState(): could not open next state");
            else
                currentState = state;
            
            return *this;
        }
        
    protected:
        virtual bool closeState(int state) { return true; }
        virtual bool openState(int state) { return true; }
        
    private:
        int currentState;
    };
}

#endif /* StateMachine_hpp */

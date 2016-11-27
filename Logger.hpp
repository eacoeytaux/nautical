//
//  Logger.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Logger__
#define __Nautical__Logger__

namespace nautical { //TODO close logs
    enum MESSAGE_TYPE {
        INFO,
        MESSAGE,
        ERROR
    };
    
    class Logger {
    public:
        static bool startup();
        static bool shutdown();
        
        static void setFlush(bool flush = true);
        static void writeLog(MESSAGE_TYPE type, const char * entry, ...);
        
    private:
        static bool init,
        flush; //whether or not log is flushed to file after each log
    };
}

#endif /* defined(__Nautical__Logger__) */

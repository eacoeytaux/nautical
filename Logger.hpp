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
        PLAIN_MESSAGE,
        WARNING_MESSAGE,
        ERROR_MESSAGE
    };
    
    class Logger {
    public:
        static void setFlush(bool flush = true);
        static void writeLog(MESSAGE_TYPE type, const char * entry, ...);
        
    private:
        static bool flush; //whether or not log is flushed to file after each log
    };
}

#endif /* defined(__Nautical__Logger__) */

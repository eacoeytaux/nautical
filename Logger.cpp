//
//  Logger.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Logger.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <string>

using namespace nautical;

bool Logger::flush = true;

void Logger::setFlush(bool flush) {
    Logger::flush = flush;
}

void Logger::writeLog(MESSAGE_TYPE type, const char * entry, ...) {
    static FILE * logFile = fopen("nautical.log", "w+");
    static FILE * logWarningsFile = fopen("nautical-warnings.log", "w+");
    static FILE * logErrorsFile = fopen("nautical-errors.log", "w+");
    
    va_list args;
    va_list argsWarning;
    va_list argsError;
    va_start(args, entry);
    va_start(argsWarning, entry);
    va_start(argsError, entry);
    
    switch (type) {
        case WARNING_MESSAGE: {
            fprintf(logWarningsFile, "WARNING: ");
            vfprintf(logWarningsFile, entry, argsWarning);
            fprintf(logWarningsFile, "\n");
            if (flush)
                fflush(logWarningsFile);
            
            fprintf(logFile, "WARNING: ");
            break;
        } case ERROR_MESSAGE: {
            fprintf(logErrorsFile, "ERROR: ");
            vfprintf(logErrorsFile, entry, argsError);
            fprintf(logErrorsFile, "\n");
            if (flush)
                fflush(logErrorsFile);
            
            fprintf(logFile, "ERROR: ");
            break;
        } default: {
            break;
        }
    }
    
    vfprintf(logFile, entry, args);
    fprintf(logFile, "\n");
    if (flush)
        fflush(logFile);
    
    va_end(args);
    va_end(argsWarning);
    va_end(argsError);

}

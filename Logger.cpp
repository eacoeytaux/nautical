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

bool Logger::init = false;

bool Logger::flush = true;

FILE * logFile;
FILE * logWarningsFile;
FILE * logErrorsFile;

bool Logger::startup() {
    if (init)
        return init;
    
    //open all log files
    logFile = fopen("nautical.log", "w+");
    logWarningsFile = fopen("nautical-warnings.log", "w+");
    logErrorsFile = fopen("nautical-errors.log", "w+");
    
    return (init = true); //set init to true
}

bool Logger::shutdown() {
    if (!init)
        return !init;
    
    //close all log files
    fclose(logFile);
    fclose(logWarningsFile);
    fclose(logErrorsFile);
    
    return !(init = false);
}

void Logger::setFlush(bool flush) {
    Logger::flush = flush;
}

void Logger::writeLog(MESSAGE_TYPE type, const char * entry, ...) {
    if (!init)
        return;
    
    //extract string from arguments
    va_list args;
    va_list argsWarning;
    va_list argsError;
    va_start(args, entry);
    va_start(argsWarning, entry);
    va_start(argsError, entry);
    
    switch (type) {
        case MESSAGE: {
            fprintf(logWarningsFile, "WARNING: ");
            vfprintf(logWarningsFile, entry, argsWarning);
            fprintf(logWarningsFile, "\n");
            if (flush)
                fflush(logWarningsFile);
            
            fprintf(logFile, "WARNING: ");
            break;
        }
        case ERROR: {
            fprintf(logErrorsFile, "ERROR: ");
            vfprintf(logErrorsFile, entry, argsError);
            fprintf(logErrorsFile, "\n");
            if (flush)
                fflush(logErrorsFile);
            
            fprintf(logFile, "ERROR: ");
            break;
        }
        default:
            break;
    }
    
    //print to main log no matter what message type
    vfprintf(logFile, entry, args);
    fprintf(logFile, "\n");
    if (flush)
        fflush(logFile);
    
    va_end(args);
    va_end(argsWarning);
    va_end(argsError);
}

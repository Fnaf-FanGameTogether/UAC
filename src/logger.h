
#ifndef LOGGER_H
#define LOGGER_H //LOGGER_G OOPS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>



/// NEEDS CLEANUP AND FIXING/TESTING

// #define str char* // It's just a prank bro

enum TypeOfLog {
    NORMAL = 0, // Normal log, like to say what files it compiles
    WARNING = 1, // A warning
    ERROR = 2, // An error like, files that doesnt exist.
    FATAL_ERR = 3 // Error that closes the program inmediately because of how bad it is
};

typedef enum TypeOfLog logtype_t;

FILE *fileOpened;

//header

uint8_t init_logging();
void log_to_file(char* message, logtype_t type, ...);
void clear_log();
void stop_logging();


uint8_t is_logging() {
    if (fileOpened == NULL){
        return 0;
    }
    return 1;
}



//functions

uint8_t init_logging(){
    if (fileOpened != NULL){
        return 2;
    }
    fileOpened = fopen("./compilerLog.log", "w");
    if (fileOpened == NULL){
        // log_to_file("The log file diesnt exists!", FATAL_ERR) ; //okay okay, this is a joke too
        return 1;
    }
    return 0;
}

void log_to_file(char* message, logtype_t type, ...){
    va_list argptr;
    va_start(argptr, format);
    vfprintf(fileOpened, "[%d]",type, message, argptr);
    va_end(argptr);
}



// I'm sorry
void clear_file(){
    // If file open
    freopen("compilerLog.log", "w",fileOpened);
    if (is_logging() == 1){
        fileOpened = freopen("compilerLog.log", "a", fileOpened);
    }
    else{
        stop_logging();
    }
    
}

void stop_logging(){
    fclose(fileOpened);
    fileOpened = NULL;
}

#endif

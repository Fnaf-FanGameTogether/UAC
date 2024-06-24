#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef LOGGER_H
#define LOGGER_H //LOGGER_G OOPS


/// NEEDS CLEANUP AND FIXING/TESTING

// #define str char* // It's just a prank bro

enum TypeOfLog{
    NORMAL = 0, // Normal log, like to say what files it compiles
    WARNING = 1, // A warning
    ERROR = 2, // An error like, files that doesnt exist.
    FATAL_ERR = 3 // Error that closes the program inmediately because of how bad it is
};

typedef enum TypeOfLog logtype_t;

FILE *fileOpened;

//header

uint8_t init_logging();
void log_to_file(char* message, logtype_t type);
void clear_log();
void stop_logging();
uint8_t is_logging() {
    if (fileOpened == NULL){
        return 0;
    }
    return 1;}



//functions

uint8_t init_logging(){
    if (fileOpened != NULL){
        return 2;
    }
    fileOpened = fopen("./compilerLog.log", "w");
    if (fileOpened == NULL){
        log_to_file("The log file diesnt exists!", FATAL_ERR) ; //okay okay, this is a joke too
        return 1;
    }
    return 0;
}

void log_to_file(char* message, logtype_t type){
    fprintf(fileOpened, "[%d]",type, message);
}

#endif
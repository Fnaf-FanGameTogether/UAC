// #pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>


/// NEEDS CLEANUP AND FIXING/TESTING

enum TypeOfLog {
    NORMAL = 0, // Normal log, like to say what files it compiles
    WARNING = 1, // A warning
    ERROR = 2, // An error like, files that doesnt exist.
    FATAL_ERR = 3 // Error that closes the program inmediately because of how bad it is (short: we fucked it up)
};

struct LogInfo {
    FILE *file;

    // stuff we will prob use
};

typedef enum TypeOfLog logtype_t;
typedef struct LogInfo loginfo_t;

FILE *fileOpened;

//header

loginfo_t* start_log_file(char* filename);
void log_to_file(loginfo_t* info, logtype_t type, /*char* message,*/ int count, ...);
void clear_log(loginfo_t* info);
void end_log_file(loginfo_t* info);
uint8_t is_logging(loginfo_t* info);
void stop_logging(loginfo_t* info);


//functions

loginfo_t* start_log_file(char* filename){
    loginfo_t* info = (loginfo_t*)malloc(sizeof(loginfo_t));

    info->file = fopen(filename, "a");
    if (info->file == NULL){
        return NULL;
    }

    return info;
}
void end_log_file(loginfo_t* info){
    if (is_logging(info)){
        fclose(info->file);
        info->file = NULL;
    }
    free(info);
}

void stop_logging(loginfo_t* info) {} 

uint8_t is_logging(loginfo_t* info){
    if (info->file == NULL){
        return 0;
    }
    return 1;
}

void log_to_file(loginfo_t* info, logtype_t type, /*char* message,*/ int count, ...){ // char* message is commented because I'm still thinking on using it
    va_list argptr;
    va_start(argptr, count); // C23 removes the use of int count
    char* EndMSG = "[%d] ";
    vfprintf(info->file, EndMSG, /*message,*/ argptr);

    va_end(argptr);
}


// I'm sorry
void clear_file(loginfo_t* info){
    // If file is open as w, it deletes its content, maybe I'm being too bold here
    freopen("compilerLog.log", "w",info->file);
    if (is_logging(info) == 1){
        info->file = freopen("compilerLog.log", "a", info->file);
    }
    else{
        stop_logging();
    }
    
}
#endif

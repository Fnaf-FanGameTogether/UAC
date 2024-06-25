
#ifndef LOGGER_H
#define LOGGER_H //LOGGER_G OOPS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>



/// NEEDS CLEANUP AND FIXING/TESTING

// #define str char* // It's just a prank bro

enum TypeOfLog {
    DEBUG = 0,  // you forgot debug
    NORMAL = 1, // Normal log, like to say what files it compiles
    WARNING = 2, // A warning
    ERROR = 3, // An error like, files that doesnt exist.
    FATAL_ERR = 4 // Error that closes the program inmediately because of how bad it is
};

enum LogInfoState {
    LOGGING_STATE_DISABLED = 1,
    LOGGING_STATE_ENABLED  = 0,
    LOGGING_ACCEPT_ALL     = 2, // log every message, no matter of the level
    LOGGING_DONT_PRINT_LEVEL = 4,
    LOGGING_DONT_PRINT_NAME  = 8
};

typedef enum LogInfoState logstate_t;

typedef enum TypeOfLog logtype_t;

struct LogInfo {
    FILE *file;

    // stuff we will prob use
    logstate_t state;
    logtype_t  min;

    char* name;
    uint16_t namesz;
};

typedef struct LogInfo loginfo_t;


FILE *fileOpened;

//header

loginfo_t* start_log_file(char* filename);


void log_to_file(loginfo_t* info, logtype_t type, char* message, ...);
/*
 you motherfucker, tf you where thinking for `int count`
 compiler complains 'can't cast from char* to int'
 because it expects your int even though you're only using it as a pointer for va_args
 leave it this way until you get whatever arg order you want with this syntax

 use helper functions or smth idk
*/
// void log_to_file(loginfo_t* info, logtype_t type, /*char* message,*/ int count, ...);

void clear_log(loginfo_t* info);
void stop_logging(loginfo_t* info);
uint8_t is_logging(loginfo_t* info);

loginfo_t* logfile_from_file(FILE* fd, uint8_t state);
void set_logger_name(loginfo_t* info, char* name);
void set_logger_min(loginfo_t* info, logtype_t tp);


//functions

void stop_logging(loginfo_t* info)
{
    // this motherfucker's code didnt even compile because this wasnt defined
    info->state = info->state | LOGGING_STATE_DISABLED;
    return;
}

void set_logger_name(loginfo_t* info, char* name)
{
    info->name = name;
    info->namesz = strlen(name); // cache name size
}

loginfo_t* start_log_file(char* filename){
    loginfo_t* info = (loginfo_t*)malloc(sizeof(loginfo_t));

    if(info == NULL){
        return NULL;
    }

    info->file = fopen(filename, "a");
    if (info->file == NULL){
        // log_to_file("The log file diesnt exists!", FATAL_ERR) ; //okay okay, this is a joke too
        return NULL;
    }
    info->state = LOGGING_STATE_ENABLED;

    info->name = NULL;
    info->namesz = 0;
    info->min = WARNING;

    return info;
}

loginfo_t* logfile_from_file(FILE* fd, uint8_t state)
{
    if(fd == stdin)
    {
        // not supported, don't even bother
        return NULL;
    }
    if(state == 0xFF)
    {
        // not specified
        // default'd to 0
        state = LOGGING_STATE_ENABLED;
    }
    loginfo_t* info = (loginfo_t*)malloc(sizeof(loginfo_t));

    if(info == NULL){
        return NULL;
    }
    
    info->file = fd;
    info->state = state;
    info->name = NULL;
    info->namesz = 0;
    info->min = WARNING;

    return info;
}


void set_logger_min(loginfo_t* info, logtype_t tp)
{
    info->min = tp;
}

uint8_t is_logging(loginfo_t* info){
    if (info->file == NULL || info->state & LOGGING_STATE_DISABLED){
        return 0;
    }
    return 1;
}


void log_logger_header(loginfo_t* info, logtype_t type)
{
    if((info->state & LOGGING_DONT_PRINT_NAME) && (info->state & LOGGING_DONT_PRINT_LEVEL)){
        return;
    }
    // print start of the header
    fprintf(info->file, "[ ");

    if(info->state & LOGGING_DONT_PRINT_NAME){
        // since both would've returned, this is print only type
        fprintf(info->file,"%d ]:", type);
        return;
    }
    if (info->state & LOGGING_DONT_PRINT_LEVEL)
    {
        // since both would've returned, this is print only name
        if(info->name == NULL)
        {
            // whatever
            // since we printed '[' we shall at least close it
            fprintf(info->file, "]:");
            return;
        }
        fprintf(info->file, "%s ]:", info->name);
        return;
    }
    // by here print both
    if(info->name != NULL){
        //
        fprintf(info->file, "%d %s ]:", type, info->name);
        return;
    }
    fprintf(info->file, "%d ]:", type);
}


void log_to_file(loginfo_t* info, logtype_t type, char* message,...){
    if(type < info->min && !(info->state & LOGGING_ACCEPT_ALL)){
        // skip unaccepted loggings
        return;
    }

    if(!is_logging(info)){
        // disable logging for non-logging loggers
        return;
    }

    va_list argptr;
    va_start(argptr, message);

    log_logger_header(info, type);

    // print or not
    
    vfprintf(info->file, message, argptr);

    va_end(argptr);
}


// I'm sorry <- you better be, tf is this
void clear_file(loginfo_t* info){
    // If file open
    freopen("compilerLog.log", "w",info->file);
    if (is_logging(info) == 1){
        info->file = freopen("compilerLog.log", "a", info->file);
    }
    else{
        stop_logging(info);
    }
    
}


void destroy_logger(loginfo_t* info)
{
    if(info->file != NULL)
    {
        if(info->file == stdout || info->file == stderr)
        {
            goto memory_deallocation;
        }
        fclose(info->file);
    }
memory_deallocation:
    if(info->name != NULL)
    {
        free(info->name);
    }
    free(info);
}

#endif

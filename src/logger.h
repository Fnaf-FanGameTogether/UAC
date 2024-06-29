
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
    LOGGING_STATE_DISABLED = 1, // The logger is disabled
    LOGGING_STATE_ENABLED  = 0, // The logger is enabled
    LOGGING_ACCEPT_ALL     = 2, // log every message, no matter of the level
    LOGGING_DONT_PRINT_LEVEL = 4, //Doesnt print the level of the msg
    LOGGING_DONT_PRINT_NAME  = 8  //Doesnt print the name of the logger
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


//FILE *fileOpened; // Why is this here???
loginfo_t* default_logger; // GG

//header

loginfo_t* start_log_file(char* filename);

void wlog(loginfo_t* info, logtype_t type, char* message, ...);

void clear_log(loginfo_t* info);
void stop_logging(loginfo_t* info);
uint8_t is_logging(loginfo_t* info);

loginfo_t* logfile_from_file(FILE* fd, uint8_t state);
void set_logger_name(loginfo_t* info, char* name);
void set_logger_min(loginfo_t* info, logtype_t tp);
loginfo_t* create_default_logger();
loginfo_t* get_default_logger();
void destroy_default_logger();
void check_logger(loginfo_t** info);

//functions

void check_logger(loginfo_t** info){
    if (*info == NULL){
        // TWO GET DEFAULT CALLS, AND THIS HAS INCONSISTENT IDENT FOR SOME REASON (at least my version, didnt notice it on github)
        /*
        if (get_default_logger() == NULL){
            create_default_logger();
        }
        *info = get_default_logger();
        */
        *info = get_default_logger(); //This had truble compiling last time
        if(*info == NULL)
        {
            *info = create_default_logger();
        }
    }

}

void stop_logging(loginfo_t* info)
{
    // this motherfucker's code didnt even compile because this wasnt defined <- Hey! In my defense you were doing the improvements, so i said. nah, let him do it.
    info->state = info->state | LOGGING_STATE_DISABLED;
    return;
}

void set_logger_name(loginfo_t* info, char* name)
{
    check_logger(&info);
    if (info-> name){
        free(info->name);
    }
    info->name = name;
    info->namesz = strlen(name); // cache name size
}

loginfo_t* start_log_file(char* filename){
    loginfo_t* info = (loginfo_t*)malloc(sizeof(loginfo_t));

    if(info == NULL){
        return NULL;
    }

    info->file = fopen(filename, "w");
    if (info->file == NULL){
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
        printf("The logger couldnt be created");
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
    check_logger(&info);
    info->min = tp;
}

uint8_t is_logging(loginfo_t* info){
    check_logger(&info);
    if (info->file == NULL || info->state & LOGGING_STATE_DISABLED){
        return 0;
    }
    return 1;
}


void log_logger_header(loginfo_t* info, logtype_t type)
{
    check_logger(&info);
    if((info->state & LOGGING_DONT_PRINT_NAME) && (info->state & LOGGING_DONT_PRINT_LEVEL)){
        return;
    }
    // print start of the header
    fprintf(info->file, "[ ");

    if(info->state & LOGGING_DONT_PRINT_NAME){
        // since both would've returned, this is print only type
        fprintf(info->file,"%d ]: ", type);
        return;
    }
    if (info->state & LOGGING_DONT_PRINT_LEVEL)
    {
        // since both would've returned, this is print only name
        if(info->name == NULL)
        {
            // whatever
            // since we printed '[' we shall at least close it
            fprintf(info->file, "]: ");
            return;
        }
        fprintf(info->file, "%s ]: ", info->name);
        return;
    }
    // by here print both
    if(info->name != NULL){
        //
        fprintf(info->file, "%d %s ]: ", type, info->name);
        return;
    }
    fprintf(info->file, "%d ]: ", type);
}


void wlog(loginfo_t* info, logtype_t type, char* message,...){
    check_logger(&info);
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


// I'm sorry <- you better be, tf is this <- The only way my tired brain could clear files without deletion at 2 AM, NVM doesnt work with how its structured now
void clear_file(loginfo_t* info){
    check_logger(&info);

    // Fuck it, we ball
    // Fuck it, we ball
    // Fuck it, we ball
    // Fuck it, we ball
    return;
}


void destroy_logger(loginfo_t* info)
{
    if (info == NULL){
        destroy_default_logger();
        return;
    }
    if(info->file != NULL)
    {
        if(info->file == stdout || info->file == stderr)
        {
            goto memory_deallocation; // What the fuck is this?
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

loginfo_t* create_default_logger(){
    default_logger = logfile_from_file(stdout, 0xff);
    return default_logger;
}
loginfo_t* get_default_logger(){
    return default_logger;
}
void destroy_default_logger(){
    destroy_logger(default_logger);
    default_logger = NULL;
}

#endif

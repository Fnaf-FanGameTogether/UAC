#ifndef _GLOBAL_PARSING_TYPES_H_
#define _GLOBAL_PARSING_TYPES_H_


// a bunch of imports in case we use'em
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

// here there are global data definitions for all argparsing logic
// have in mind the argparser is a parser of it's own
// and all the information about what it has to do shall be efficiently stored

// when a phandle_type(handle) returns UNREACHABLE, means handle is an invalid handle
#define UNREACHABLE 0

typedef void* pdhandle_t; // Parsed Data Handle t

typedef enum {
    VERSION_PRINTING = 0,
    ENVIRON_MANAGEMENT = 1,
    COMPILING = 2,
    LINKING = 3,
    ASSEMBLING = 4,
    // Debugs an specific part of UAC
    DEBUGGING = 5,
    // compiler error checking
    // this may some compiler mode that sets the compiler to a 'watch out' state in case something happens
    ERROR_CHECKING = 6
} actions_t;

// Global Parsing Info struct
struct gpi_s {
    // Says the type(actions) from enum actions_t
    actions_t  act;

    // type is `act`, but for here on now please use pdhandle_type
    pdhandle_t action_info;
    // Separated for easier access
    pdhandle_t verbose_descriptor;

    // make a copy the arguments here
    uint32_t argc; 
    char**   argv;
}; 

typedef struct gpi_s gpi_t;

struct version_verbose_descriptor
{
    // other fields to be added

    // may be specified
    char* logger_name;
};

typedef struct version_verbose_descriptor vv_descr_t;


typedef enum {
    TOKENIZER = 0,
    LINKER = 1,
    COMPILER = 2,
    ASSEMBLER = 4, // Mischevious >:3
    NONE = 0xFF
} debug_t;


// The action infos

struct compilation_debug_action_info {
    // name is self explanatory
    uint8_t dummy; // argument for the compiler not to complain
};

struct tokenizer_debug_info {
    uint8_t action;
};
typedef struct tokenizer_debug_info tokenizer_debug_info_t;
typedef struct compilation_debug_action_info compiler_debug_action_info_t;

struct debugging_action_info {
    debug_t debug;
    pdhandle_t info;
};
typedef struct debugging_action_info debug_action_info_t;


// Function declarations
uint16_t pdhandle_type(pdhandle_t handle);
gpi_t*   create_gpi();
void init_gpi_args(gpi_t* gpi, uint32_t argc, char** argv);
void get_gpi_action(gpi_t* gpi);
void destroy_gpi(gpi_t* gpi);


// Function code
gpi_t* create_gpi()
{
    // this struct must be populated afterwards anyway so just this'll do
    return (gpi_t*)malloc(sizeof(gpi_t));
}

uint16_t pdhandle_type(pdhandle_t handle)
{
    if(handle == NULL){
        return UNREACHABLE;
    }
    return ((uint16_t*)handle)[0]; // first 2 bytes refer to this, everything else (handle[1:]) is the struct
                                   // what struct is there depends on both the phandle_type and the context the handle is
}

void init_gpi_args(
    gpi_t* gpi, 
    uint32_t argc, 
    char** argv
)
{
    // gpi == NULL is imposible, check the main function
    gpi->argc = argc;
    gpi->argv = argv;
    return;
}

void get_gpi_action(gpi_t* gpi){
    if(gpi->argc < 2){
        gpi->act = COMPILING; // default
        return;
    }

    if (!mstrcmp(gpi->argv[1], "-d")){
        // printf("DEBUG MODE BITCH\n"); // Bucking feach
        gpi->act = DEBUGGING;
        return;
    }
    gpi->act = COMPILING;
    return;
}

void destroy_gpi(gpi_t* gpi)
{
    // todo: more logic for handle info

    // you probably don't wanna try deallocate argv
    free(gpi);
}

#endif
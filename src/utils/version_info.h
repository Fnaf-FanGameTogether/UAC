#ifndef __VERSION_INFO
#define __VERSION_INFO

#include "macros_config.h"
#include "utils/length.h"
#include <stdio.h>
#include <stdint.h>

struct builtin_platform_s {
    uint8_t bIs32BitCursor;
    uint8_t DebugEnabled;
};

typedef struct builtin_platform_s builtin_platform_t;

// function declarations
builtin_platform_t* _init_builtin_platform_object();
void populate_bplatform_info(builtin_platform_t* plf);
void fprint_builtin_platform_info(FILE* fd);

// functions code
builtin_platform_t* _init_builtin_platform_object()
{
    builtin_platform_t* plf = (builtin_platform_t*)malloc(sizeof(builtin_platform_t));

    // todo: some more initialization if needed

    return plf;
}

void populate_bplatform_info(builtin_platform_t* plf)
{
    if(plf == NULL) {return;}
    plf->bIs32BitCursor = _is32bit_enabled();
    #ifdef IS_DEBUG_MODE
    plf->DebugEnabled = 1;
    #else
    plf->DebugEnabled = 0;
    #endif
}


void fprint_builtin_platform_info(FILE* fd)
{
    builtin_platform_t* plf = _init_builtin_platform_object(); // manuel, rename _init_object to _init_builtin_platform_memory_space or smth like that
    populate_bplatform_info(plf);

    loginfo_t* logger = logfile_from_file(fd, LOGGING_ACCEPT_ALL | LOGGING_DONT_PRINT_LEVEL);

    set_logger_name(logger, "{ version-info-descriptor }");

    // printout // CHANGE IN THE FUTURE ADDING MORE INFO
    // fprintf(fd,"32 Bit cursor enabled: %d\n", (uint32_t)plf->bIs32BitCursor); // TODO change this thing's format to print directly the byte and don't need to cast (or zero extend)
    // fprintf(fd, "Debug Build: %d\n", (uint32_t)plf->DebugEnabled);

    log_to_file(logger,NORMAL,"32 Bit cursor enabled: %d\n", (uint32_t)plf->bIs32BitCursor); // TODO change this thing's format to print directly the byte and don't need to cast (or zero extend)
    log_to_file(logger,NORMAL, "Debug Build: %d\n", (uint32_t)plf->DebugEnabled);
    

    // shutdown
    stop_logging(logger); // maybe in the future we'll add more stuff so that this is recomended
    destroy_logger(logger);

    return;
}






#endif

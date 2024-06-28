#include <stdio.h>
#include "./macros_config.h"
#include "logger.h"

#include "argparsing/argparsing.h"
#include "compiler/compiler/tokenizer/tokenizer.h"

// this has to be imported last for all configuration loading to have finished
#include "utils/version_info.h"

int main(int argc, char** argv)
{
    gpi_t* args = create_gpi();
    if(args == NULL)
    {
        // just fail, if can't allocate gpi_t*, nothing to do here no more
        return 1;
    }

    init_gpi_args(args, argc, argv);

    fprint_builtin_platform_info(stdout);


    loginfo_t* logger = NULL;

    check_logger(&logger); // automatically initialize it

    logger->state |= LOGGING_ACCEPT_ALL;

    log_to_file(logger, NORMAL, "arg count: %d\n", argc);

    destroy_default_logger();
    destroy_gpi(args);
    return 0;
}


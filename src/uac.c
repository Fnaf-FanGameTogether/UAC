#include <stdio.h>
#include "./macros_config.h"
#include "logger.h"

#include "argparsing/argparsing.h"
#include "compiler/compiler/tokenizer/tokenizer.h"


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





    destroy_gpi(args);
    return 0;
}


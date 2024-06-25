#include <stdio.h>
#include "./macros_config.h"
#include "compiler/compiler/tokenizer/tokenizer.h"
#include "logger.h"
#include "utils/version_info.h"

int main(int argc, char** argv)
{
    fprint_builtin_platform_info(stdout);
    return 0;
}


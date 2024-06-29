#ifndef _TOKENIZER_DEBUG_H_
#define _TOKENIZER_DEBUG_H_

#include <stdio.h>
#include <string.h>

#include "compiler/compiler/tokenizer/tokenizer.h"

// func declarations
tokenizer_reader_t* readline();

const char* DEFAULT_FILENAME = "0";

tokenizer_reader_t* tokenizer_debug_readline_catastrophy()
{
    uint16_t l = strlen(DEFAULT_FILENAME);
    const char* cmd = "python ./scripts/tokenizer_debug.py ";
    uint16_t ol = strlen(cmd);
    char* cdm = (char*)malloc(l + ol);
    if(cdm == NULL)
    {
        return NULL;
    }
    strcpy(cdm, cmd);
    strcpy(cdm + ol, DEFAULT_FILENAME);


    if(system(cdm))
    {
        // wrong arguments or python inexistent
        return NULL;
    }

    // TODO: since we have to return an open handle from the file, 
    // we can't erase it yet, so we gonna leave that temp file there
    // cmd = "python ./scripts/tokenizer_debug.py -d";
    // ol = strlen(cmd);
    // free(cdm);
    // cdm = (char*)malloc(l + ol);

    FILE* prompt = fopen(DEFAULT_FILENAME, "r");

    if(prompt == NULL)
    {
        return NULL;
    }

    return tokenizer_reader_from_file(prompt);
}



#endif
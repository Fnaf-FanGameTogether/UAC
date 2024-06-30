#ifndef _MACROS_CONFIG_H_
#define _MACROS_CONFIG_H_

#include <stdint.h>
#include <string.h>

// this is a configuration file for the build

/* this sets all cursor to use 32 bit length instead of 64 bit length for file cursors, and for other stuff*/
// flag
// #define __USE32BIT_LENGTH

/* this is the number of tokens to allocate in a single tokens_chunk, the rest is linked-listed */
// uint8_t
// #define TOKENS_CHUNK_SIZE ~~

/* this says if the compiler is in debug mode when started */
// flag
// #define IS_DEBUG_MODE


// this is just for Visual Studio to stop complaining. //GG Rip bozo, mine just works
#ifndef NULL
#define NULL (void*)0
#endif






// Better
uint8_t mstrcmp(char* a, char* b)
{
    while(*a || *b)
    {
        if(*a != *b){
            return 1;
        }
        a++;
        b++;
    }
    return *a != *b;
}

#endif
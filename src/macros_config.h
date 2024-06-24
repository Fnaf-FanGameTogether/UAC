#ifndef _MACROS_CONFIG_H_
#define _MACROS_CONFIG_H_


/* this sets all cursor to use 32 bit length instead of 64 bit length for file cursors, and for other stuff*/
// #define __USE32BIT_LENGTH

/* this is the number of tokens to allocate in a single tokens_chunk, the rest is linked-listed */
// #define TOKENS_CHUNK_SIZE 16 


// this is just for Visual Studio to stop complaining.
#ifndef NULL
#define NULL (void*)0
#endif

#endif
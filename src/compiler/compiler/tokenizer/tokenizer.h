#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "./token.h"
#include "utils/length.h"


#ifndef TOKENS_CHUNK_SIZE
# define TOKENS_CHUNK_SIZE 16
#endif


struct tokens_chunk_s
{
    // tokenize()
    // .push_token(tokentype,...)
    //  ^^^ <- populate(tok_chunk->toks[tok_chunk->cwt])
    token_t* toks;
    uint8_t cwt;
    struct tokens_chunk_s* next;
};


typedef struct tokens_chunk_s tokens_chunk_t;

struct tokenarr_s {
    tokens_chunk_t* head;
    tokens_chunk_t* end;
    tokens_chunk_t* curr;
    length_t cc_i;
};

typedef struct tokenarr_s tokenarr_t;


// function declarations
tokens_chunk_t* create_tokens_chunk();
void destroy_tokens_chunk(tokens_chunk_t* cht);
void _write_token(tokens_chunk_t* cht, toktype_t type, char* val,uint16_t size, pos_t* pos, uint8_t* state);
void _delete_token(tokens_chunk_t* cht);

// functions code
tokens_chunk_t* create_tokens_chunk()
{
    tokens_chunk_t* chunk = (tokens_chunk_t*)malloc(sizeof(tokens_chunk_t));
    if(chunk == NULL){ return NULL; }

    chunk->toks = (token_t*)malloc(sizeof(token_t) * TOKENS_CHUNK_SIZE);
    if(chunk->toks == NULL) {
        free(chunk);
        return NULL;
    }
    for(uint8_t i=0; i < TOKENS_CHUNK_SIZE;i++)
    {
        create_unready_token(chunks->toks + i);
    }
    
    chunk->cwt = 0; // Current Working Token

    chunk->next = NULL;

    return chunk;
}

void destroy_tokens_chunk(tokens_chunk_t* cht)
{
    for(uint8_t i=0; i < TOKENS_CHUNK_SIZE; i++)
    {
        // token_t *tok = chunks->toks + i 
        destroy_token(chunks->toks + i);
    }

    free(chunk); // do not free next chunk, leave that for the upper destroying function.
}

void _write_token(tokens_chunk_t* cht, toktype_t type, char* val,uint16_t size, pos_t* pos, uint8_t* state)
{
    // check for space left, if none, raise a flag and exit
    if(cht->cwt >= TOKENS_CHUNK_SIZE - 1)
    {
        // need to append new chunk
        *state = 1;
        return;
    }
    _write_token_info(cht->toks + cht->cwt, val, size, type, pos);

    // add to Current Working Token
    cht->cwt++;
}

void _delete_token(tokens_chunk_t* cht)
{
    if (cht->cwt == 0)
    {
        return;
    }
    cht->cwt--;

    free((cht->toks + cht->cwt)->val);
    free((cht->toks + cht->cwt)->pos);
    (cht->toks+cht->cwt)->size = 0;
    (cht->toks+cht->cwt)->type = 0xff;

    return;
}

#endif
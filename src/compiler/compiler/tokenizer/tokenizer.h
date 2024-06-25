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
    tokens_chunk_t* end; // append to last one

    length_t cc_i;
};

typedef struct tokenarr_s tokenarr_t;

struct tokenizer_reader_s {
	 uint8_t type;
	 FILE* fh; // TODO: pipelines and stuff like that

	 uint8_t eof; // flag raised by read_char() to mark EOF
};

typedef struct tokenizer_reader_s tokenizer_reader_t;

struct tokenizer_s {
	 tokenarr_t* t_arr;
	 pos_t* curr_pos;

	 tokenizer_reader_t* reader;
};


// function declarations
tokens_chunk_t* create_tokens_chunk();
void destroy_tokens_chunk(tokens_chunk_t* cht);
void _write_token(tokens_chunk_t* cht, toktype_t type, char* val,uint16_t size, pos_t* pos, uint8_t* state);
void _delete_token(tokens_chunk_t* cht);
void _create_tokarr(tokenarr_t** arr);
void _destroy_tokarr(tokenarr_t* arr);

char read_char(tokenizer_reader_t* reader);
uint8_t eof(tokenizer_reader_t* reader);

// this is important
uint8_t push_token(tokenarr_t* arr, toktype_t type, char* val,uint16_t size, pos_t* pos);



// functions code


char read_char(tokenizer_reader_t* reader, uint8_t* valid)
{
	*valid = 0; // set success as true
	// TODO: more logic to check if we're reading the correct type of input
	if(reader == NULL || reader->fh == NULL)
	{
	// womp womp
	*valid = 1;
	return "\00";
	}
	char bf;
	// most probably
	if(fread(&bf, 1,1,reader->fh)){
       reader->eof = 1;
       *valid = 2;
       return '\00';
	}
	return bf;
}

uint8_t eof(tokenizer_reader_t* reader){
	return reader->eof;
}

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
        create_unready_token(chunk->toks + i);
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
        destroy_token(cht->toks + i);
    }

    free(cht); // do not free next chunk, leave that for the upper destroying function.
}

void _write_token(tokens_chunk_t* cht, toktype_t type, char* val,uint16_t size, pos_t* pos, uint8_t* state)
{
    // check for space left, if none, raise a flag and exit
    // succeed by defult
    *state = 0;

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
    // hopefully not used
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


void _create_tokarr(tokenarr_t** arr)
{
    tokenarr_t* karr = (tokenarr_t*)malloc(sizeof(tokenarr_t));
    if(karr == NULL)
    {
        return;
    }
    *arr = karr;

    karr->cc_i = 1; // head

    karr->head = karr->end = create_tokens_chunk();

    return;
}

void _destroy_tokarr(tokenarr_t* arr)
{
    tokens_chunk_t* curr= arr->head;
    uint8_t sb=0;
    while(!sb)
    {
        // can't say curr->next because destroy_tokens_chunk deallocates that pointer as well

        if(curr->next == NULL){
            sb = 1;
        }

        destroy_tokens_chunk(curr);

        if (!sb){
            curr = curr->next;
        }
    }

    free(arr);

    return;
}

uint8_t push_token(tokenarr_t* arr, toktype_t type, char* val,uint16_t size, pos_t* pos)
{
    // Wow.
    // WHY??? 
    /* 
     if(!push_token(...))
     {
        // failed
     } else {
        // succeded
     }
    */
    uint8_t state;
    if(arr == NULL)
    {
        // whatever
        return 0;
    }
    tokens_chunk_t* end = arr->end;
    _write_token(end, type, val, size, pos, &state);
    if(state)
    {
        // failed, append a new chunk
        tokens_chunk_t* appended = create_tokens_chunk();
        if(appended == NULL)
        {
            return 0;
        }
        end->next = appended;
        arr->cc_i++;
        _write_token(appended, type, val, size, pos, &state);
        if(state){
            // we give up
            return 0;
        }
    }
    return 1;
}




#endif

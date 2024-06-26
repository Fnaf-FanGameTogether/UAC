#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <stdio.h>

#include "./token.h"
#include "logger.h"
#include "utils/length.h"




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
	uint8_t type; // type = 1 <-> stdin
	FILE* fh; // TODO: pipelines and stuff like that

	uint8_t eof; // flag raised by read_char() to mark EOF
};

typedef struct tokenizer_reader_s tokenizer_reader_t;

struct tokenizer_s {
	tokenarr_t* t_arr;
	pos_t* curr_pos;

	tokenizer_reader_t* reader;


    char*      name;
    loginfo_t* logger;
};

typedef struct tokenizer_s tokenizer_t;

// static data declarations
char NULL_TERMINATOR = '\00';


// function declarations
tokens_chunk_t* create_tokens_chunk();
void destroy_tokens_chunk(tokens_chunk_t* cht);
void _write_token(tokens_chunk_t* cht, toktype_t type, char* val,uint16_t size, pos_t* pos, uint8_t* state); // alloc
void _delete_token(tokens_chunk_t* cht); // dealloc
void _create_tokarr(tokenarr_t** arr); // alloc
void _destroy_tokarr(tokenarr_t* arr); // dealloc

tokenizer_t* create_tokenizer(char* name, tokenizer_reader_t* reader); // alloc

char read_char(tokenizer_reader_t* reader, uint8_t* valid);
uint8_t eof(tokenizer_reader_t* reader);

// this is important
uint8_t push_token(tokenarr_t* arr, toktype_t type, char* val,uint16_t size, pos_t* pos);
tokenizer_reader_t* tokenizer_reader_from_file(FILE* fd);
void close_reader(tokenizer_reader_t* reader);

/*
 read `toks` tokens from the tokenizer and add them to it's tokenarr

 returns: the amount of tokens read

 if(fetch_token(tokenizer, toks) != toks) {
    // EOF found
    ...
 }

*/

uint16_t fetch_token(tokenizer_t* tokenizer, uint16_t toks);


// functions code
void close_reader(tokenizer_reader_t* reader)
{
    if(reader == NULL)
    {
        return;
    }
    if(reader->fh == NULL)
    {
        return;
    }
    fclose(reader->fh);
    free(reader);
}

tokenizer_reader_t* tokenizer_reader_from_file(FILE* fd)
{
    tokenizer_reader_t* reader= (tokenizer_reader_t*)malloc(sizeof(tokenizer_reader_t));

    if(reader == NULL){ return NULL; }

    reader->eof = 0;
    reader->type = 0;
    reader->fh = fd;

    return reader;
}

tokenizer_t* create_tokenizer(char* name, tokenizer_reader_t* reader)
{
    tokenizer_t* tok = (tokenizer_t*)malloc(sizeof(tokenizer_t));

    if(tok == NULL)
    {
        return NULL;
    }
    tok->name = name;
    // as long as the tokenizer is not complete, leave the logger either unset

    tok->logger = NULL;
    // or create it with the stdout
    // tok->logger = logfile_from_file(stdout, 0xff);
    // set_logger_name(tok->logger, tok->name);


    tok->curr_pos = create_pos();
    tok->reader = reader;
    _create_tokarr(&tok->t_arr);

    return tok;
}


uint16_t fetch_token(tokenizer_t* tokenizer, uint16_t toks)
{
    // TODO
    return 0;
}

char read_char(tokenizer_reader_t* reader, uint8_t* valid)
{
	*valid = 0; // set success as true
	// TODO: more logic to check if we're reading the correct type of input
    if(reader == NULL)
    {
        *valid = 1;
        return NULL_TERMINATOR;
    }
	if(reader->fh == NULL)
	{
	    // womp womp
	    *valid = 1;
	    return NULL_TERMINATOR;
	}
	char bf;
	// most probably
	if(fread(&bf, 1,1, reader->fh)){
       reader->eof = 1;
       *valid = 2;
       return NULL_TERMINATOR;
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
    tokens_chunk_t* curr = arr->head;
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

    void fetch_token(){
        ...
        if(push_token(...))
        {
            ...
            return;
        }
        error(...);
        return;
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

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdint.h>
#include <stdlib.h>

#include "utils/length.h"



// enum TokenType {
    // EOF = 0,
    // EOS = 1, // End Of Statement
    // IDENTIFIER = 2,
    // SYMBOL = 3,
    // INTEGER_LITERAL = 4,
    // STRING_LITERAL = 5,
    // TOKEN_NOT_READY = 0xff
// };

struct pos_s {
    length_t cursor;  // file's cursor
    length_t line;    // current line
    length_t in_line; // in-line position
};

typedef struct pos_s pos_t;


typedef uint8_t toktype_t;
// #define toktype_t uint8_t

struct token_s {
    pos_t* pos;
    toktype_t type;

    uint16_t size;
    char*    val;
};

typedef struct token_s token_t;

// function declarations

pos_t* create_pos(void);
void destroy_pos(pos_t* pos);

pos_t* copy_pos(pos_t* og);

token_t* create_token(char* val, uint16_t size, toktype_t type, pos_t* pos);
void destroy_token(token_t* tok);
void create_unready_token(token_t* tok);

void _write_token_info(token_t* tok, char* val, uint16_t size, toktype_t type, pos_t* pos);


// function code


void _write_token_info(token_t* tok, char* val, uint16_t size, toktype_t type, pos_t* pos)
{
    tok->val = val;
    tok->size =  size;
    tok->type = type;
    tok->pos = copy_pos(pos);
    if(tok->pos == NULL)
    {
        // bad luck, but whatever
    }

}

pos_t* copy_pos(pos_t* og)
{
    pos_t* new = create_pos();
    if (new == NULL){
        return NULL;
    }
    // memcpy(new, og, sizeof(pos_t));
    
    new->cursor  = og->cursor;
    new->line    = og->line;
    new->in_line = og->in_line;

    return new;
}

void create_unready_token(token_t* tok)
{

    // token_t* tok = (token_t*)malloc(sizeof(token_t));
    
    if(tok == NULL) {
        return;
    }

    tok->type = 0xff;

    return;
}


token_t* create_token(char* val, uint16_t size, toktype_t type, pos_t* pos)
{
    pos_t* new_pos = copy_pos(pos);
    while (new_pos == NULL)
    {
        return NULL;
    }

    token_t* tok = (token_t*)malloc(sizeof(token_t));
    
    if(tok == NULL) {
        return NULL;
    }

    tok->pos = new_pos;
    tok->size = size;
    tok->val = val;
    tok->type = type;

    return tok;
}

void destroy_token(token_t* tok)
{  
    if(tok->type==0xff)
    {
        free(tok);
        return;
    }

    destroy_pos(tok->pos);

    if(tok->val!=NULL){
        free(tok->val);
    }
    free(tok);
}

pos_t* create_pos()
{
    return (pos_t*)malloc(sizeof(pos_t));
}

void destroy_pos(pos_t* pos)
{
    free(pos);
}

#endif
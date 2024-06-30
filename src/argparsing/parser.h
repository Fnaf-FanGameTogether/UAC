#ifndef _PARSER_H_
#define _PARSER_H_

// #include <stdint.h>
#include "argparsing/global_parsing_types.h"

// we may use this one in the future
// "not now, not now" <- this has already been imported so it doesnt really matter rn
#include "logger.h"


// func declarations
void parse_debug_mode(gpi_t* gpi);
debug_action_info_t* create_debug();
void parse_tokenizer_args(gpi_t* gpi);
void parse_compiler_args(gpi_t* gpi);

tokenizer_debug_info_t* create_tokenizer_debug_info();
compiler_debug_action_info_t* create_compiler_debug_info();


// func code


void parse_tokenizer_args(gpi_t* gpi)
{
	debug_action_info_t* ai = (debug_action_info_t*)gpi->action_info;
	tokenizer_debug_info_t* info = create_tokenizer_debug_info();
	ai->info = info;

	if(gpi->argc < 4){ //TODO: NEEDS TO BE CHANGED BADLY... 
		return;
	}

	if(!mstrcmp(gpi->argv[3], "-readlines"))
	{
		// TODO: mroe
		info->action = 1; // 1 <-> READLINES
		return;
	}

}

void parse_compiler_args(gpi_t* gpi)
{
	debug_action_info_t* ai = (debug_action_info_t*)gpi->action_info;
	compiler_debug_action_info_t* info = create_compiler_debug_info();
	ai->info = info;

	// MORE
	wlog(NULL, NORMAL, "Compilation argument parser\n");
	return;

}

tokenizer_debug_info_t* create_tokenizer_debug_info()
{
	tokenizer_debug_info_t* res = (tokenizer_debug_info_t*)malloc(sizeof(tokenizer_debug_info_t));

	return res;
}

compiler_debug_action_info_t* create_compiler_debug_info(){
	// "te caiste? :c"
	compiler_debug_action_info_t* cai = (compiler_debug_action_info_t*)malloc(sizeof(compiler_debug_action_info_t));
	return cai;
}

// XD
void parse_debug_mode(gpi_t* gpi)
{
	debug_action_info_t* info = create_debug();
	gpi->action_info = info;

	if(gpi->argc < 3){
		// no extra arguments, populate with default
		info->debug = NONE;
		return;
	}
	
	if (!mstrcmp(gpi->argv[2], "tokenizer") || !mstrcmp(gpi->argv[2], "tok")){
		info->debug = TOKENIZER;
		parse_tokenizer_args(gpi);
	}
	else if (!mstrcmp(gpi->argv[2], "compiler") || !mstrcmp(gpi->argv[2], "cmp")){
		info->debug = COMPILER;
		parse_compiler_args(gpi);
	}
	else if (!mstrcmp(gpi->argv[2], "linker") || !mstrcmp(gpi->argv[2], "ld")){
		info->debug = LINKER;
	}
	else if (!mstrcmp(gpi->argv[2], "assembler") || !mstrcmp(gpi->argv[2], "ass")){
		info->debug = ASSEMBLER;
	}
	else{
		wlog(NULL,NORMAL,"WHY??!!!\n");
	}

	return;
}

debug_action_info_t* create_debug(){
	debug_action_info_t* info = (debug_action_info_t*)malloc(sizeof(debug_action_info_t));
	return info;
}

// thank you so much for working out this memory deallocation functions we forgot to even conceive

void free_debug_tokenizer_info(tokenizer_debug_info_t* tdi)
{
	// tokenizer debug info

	free(tdi); // posibly in the future we'll add more stuff here
}

void free_debug_compiler_info(compiler_debug_action_info_t* cdi)
{
	// posibly add more freeing as things get more complicated, but for now
	free(cdi);
}


void destroy_debug_action_info(debug_action_info_t* di)
{
	// di <-> Debug Info


	switch(di->debug)
	{
		case TOKENIZER:
			free_debug_tokenizer_info(di->info);
			break;
		case COMPILER:
			free_debug_compiler_info(di->info);
			break;
		// this ones we havent even declared it's existence outside parse_debug_mode and enums
		case LINKER:
		case ASSEMBLER:
			break;
		default:
			// whatever
			break;
	}
	// maybe more logic
	free(di);
}

#endif

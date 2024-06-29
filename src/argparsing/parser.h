#ifndef _PARSER_H_
#define _PARSER_H_
#include "argparsing/global_parsing_types.h"
#include "logger.h"

// func declarations
void parse_debug_mode(gpi_t* gpi);
debug_action_info_t* create_debug();
void parse_tokenizer_args(gpi_t* gpi);

tokenizer_debug_info_t* create_tokenizer_debug_info();


// func code


void parse_tokenizer_args(gpi_t* gpi)
{
	debug_action_info_t* ai = (debug_action_info_t*)gpi->action_info;
	tokenizer_debug_info_t* info = create_tokenizer_debug_info();
	ai->info = info;

	if(gpi->argc < 4){
		return;
	}

	if(!mstrcmp(gpi->argv[3], "-readlines"))
	{

		// TODO: mroe
		info->action = 1; // 1 <-> READLINES
		return;
	}

}

tokenizer_debug_info_t* create_tokenizer_debug_info()
{
	tokenizer_debug_info_t* res = (tokenizer_debug_info_t*)malloc(sizeof(tokenizer_debug_info_t));

	return res;
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
	
	if (mstrcmp(gpi->argv[2], "tokenizer") || mstrcmp(gpi->argv[2], "tok")){
		info->debug = TOKENIZER;
		parse_tokenizer_args(gpi);
	}
	else if (mstrcmp(gpi->argv[2], "compiler") || mstrcmp(gpi->argv[2], "cmp")){
		info->debug = COMPILER;
	}
	else if (mstrcmp(gpi->argv[2], "linker") || mstrcmp(gpi->argv[2], "ld")){
		info->debug = LINKER;
	}
	else if (mstrcmp(gpi->argv[2], "assembler") || mstrcmp(gpi->argv[2], "ass")){
		info->debug = ASSEMBLER;
	}
	else{
		wlog(NULL,NORMAL,"WHY??!!!");
	}

	return;
}

debug_action_info_t* create_debug(){
	debug_action_info_t* info = (debug_action_info_t*)malloc(sizeof(debug_action_info_t));
	return info;
}

#endif

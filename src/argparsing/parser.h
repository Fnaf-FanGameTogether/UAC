#ifndef _PARSER_H_
#define _PARSER_H_
#include "argparsing/global_parsing_types.h"

// func declarations
void parse_debug_mode(gpi_t* gpi);
debug_action_info_t* create_debug();

// func code

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
	
	if (strcmp(gpi->argv[2], "tokenizer") || strcmp(gpi->argv[2], "tok")){
		info->debug = TOKENIZER;
	}
	else if (strcmp(gpi->argv[2], "compiler") || strcmp(gpi->argv[2], "cmp")){
		info->debug = COMPILER;
	}
	else if (strcmp(gpi->argv[2], "linker") || strcmp(gpi->argv[2], "ld")){
		info->debug = LINKER;
	}
	else if (strcmp(gpi->argv[2], "assembler") || strcmp(gpi->argv[2], "ass")){
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

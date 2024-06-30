#ifndef _PARSER_H_
#define _PARSER_H_

// #include <stdint.h>
#include "argparsing/global_parsing_types.h"

// we may use this one in the future
// "not now, not now" <- this has already been imported so it doesnt really matter rn
#include "logger.h"


// func declarations
debug_action_info_t* create_debug();

tokenizer_debug_info_t* create_tokenizer_debug_info();				//Alloc
compiler_debug_action_info_t* create_compiler_debug_info();			//Alloc
linking_debug_action_info_t* create_linker_debug_info();			//Alloc
assembler_debug_action_info_t* create_assembler_debug_info();		//Alloc


void parse_debug_mode(gpi_t* gpi);

void parse_tokenizer_args(gpi_t* gpi);
void parse_compiler_args(gpi_t* gpi);
void parse_linker_args(gpi_t* gpi);
void parse_assembler_args(gpi_t* gpi);


void destroy_debug_action_info(debug_action_info_t* di);			//Dealloc

void free_debug_tokenizer_info(tokenizer_debug_info_t* tdi);		//Dealloc
void free_debug_compiler_info(compiler_debug_action_info_t* cdi);	//Dealloc
void free_debug_linker_info(linking_debug_action_info_t* ldi);		//Dealloc
void free_debug_assembler_info(assembler_debug_action_info_t* adi);	//Dealloc


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

	// TODO: moeor
	wlog(NULL, NORMAL, "Compilation argument parser\n");
	return;

}

void parse_linker_args(gpi_t* gpi){
	debug_action_info_t* ai = (debug_action_info_t*)gpi->action_info;
	linking_debug_action_info_t* info = create_linker_debug_info();
	ai->info = info;

	// TODO: do morr
	wlog(NULL, NORMAL, "Linker argument parser\n");
	return;
}

void parse_assembler_args(gpi_t* gpi){
	debug_action_info_t* ai = (debug_action_info_t*)gpi->action_info;
	assembler_debug_action_info_t* info = create_assembler_debug_info();
	ai->info = info;

	// TODO: do morer
	wlog(NULL, NORMAL, "Assembler argument parser\n");
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

linking_debug_action_info_t* create_linker_debug_info(){
	linking_debug_action_info_t* ldai = (linking_debug_action_info_t*)malloc(sizeof(linking_debug_action_info_t));
	return ldai;
}

assembler_debug_action_info_t* create_assembler_debug_info(){
	linking_debug_action_info_t* adai = (linking_debug_action_info_t*)malloc(sizeof(linking_debug_action_info_t)); // I dunno why when I wrote "adai", my mind said "iwatodai"
	return adai;
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
		parse_linker_args(gpi);
	}
	else if (!mstrcmp(gpi->argv[2], "assembler") || !mstrcmp(gpi->argv[2], "ass")){
		info->debug = ASSEMBLER;
		parse_assembler_args(gpi);
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

void free_debug_linker_info(linking_debug_action_info_t* ldi){
	free(ldi);
}

void free_debug_assembler_info(assembler_debug_action_info_t* adi){
	free(adi);
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
		// this ones we havent even declared it's existence outside parse_debug_mode and enums <- Done on my phone :>
		case LINKER:
			free_debug_linker_info(di->info);
			break;
		case ASSEMBLER:
			free_debug_assembler_info(di->info);
			break;
		default:
			// whatever
			break;
	}
	// maybe more logic
	free(di);
}

#endif

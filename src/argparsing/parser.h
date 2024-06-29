#ifndef _PARSER_H_
#define _PARSER_H_
#include "argparsing/global_parsing_types.h"

// func declarations
void parse_debug_mode(gpi_t* gpi);
debug_info_t* create_debug();

// func code

void parse_debug_mode(gpi_t* gpi)
{
   gpi->action_info = create_debug();
	 if(gpi->argc < 3){
			// no extra arguments, populate with default
			return; // todo
	 }

	 // ...

	 return;
}

#endif

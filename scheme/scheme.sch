
uac/
    fed (File Encoder/Decoder)/
    	python (source -> source in utf-8)/
		object-file encoder/decoder/ (.obj, .lib)
		label-file encoder/decoder
		binary-file encoder/decoder

	package unit/
		// to add architectures
		Architecture packages
		// to add file-object formats
		Object file packages
		// to add libraries
		libraries packages
		// to add languajes
		compiler packages
	

-> C/C++ ;       eng <-
  .c/.cpp        .e


						               ------ <- add libraries and dependencies
source.e -> compiler ->  source.asm -> linker -> dumper
                         (usl)                   ^^^^^^ <- add architectures

src/
	entry-point: uac.c
	(C)    argparsing/*
	(C)    packaging/*

	[either]
	(C/py) encoders/*
	(C/py) decoders/*
	[or]
	(C/py) fileman/*

	// package managing
	(C/py) environ/*
	// random stuff
	(C)    utils/*
	// compiler
	(C)    compiler/
	//     assembler
	(C)    compiler/asembler/*
	//     linker
	(C)    compiler/linker/*
	//     dumper
	(C)    compiler/dumper/*
	// compiler
	(C)    compiler/compiler/*
	// tokenizer
	(C)    compiler/compiler/tokenizer/*
	// parser
	(C)    compiler/compiler/parser/*
	// macros
	(C)    compiler/macros/*


uac soruce.e -o main.exe
uac {
	Source file: {
		Language: eng (bc .e)
		Language compilers: ['./eng']
		chosen: eng.dll
	}
	
	fetching methods from eng.dll -> AST -> uac -> .asm
	passing to utf-8

}




# How it should work

- tokenize stuff into tokens of type Eof, Eos (End of Statement (Semicolon)), Symbol (any symbol), identifier, int literal and
  string literal (add as needed, maybe a MACROS token for #)


## how the tokenizer works

no idea, hasnt been implemented yet
what has been implemented

### tokens and how they're structured

the token struct is done and ready to use;
the tokenizer allocates tokens in chunks of TOKENS_CHUNK_SIZE, 16 by default
and then the tokenizer keeps them in a linked list of these chunks, which are only allocated when more tokens than
allocated are ready

population and destruction of these structs is implemented

## Warning

We're both implementing in Windows so Linux support is gonna take some time and, moreover, MacOs support may not even come (I don't wanna mess with MacOs)


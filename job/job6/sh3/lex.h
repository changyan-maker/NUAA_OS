#ifndef _LEX_H
#define _LEX_H

#include "parse.h"

extern int token_is(tree_t *this, char *token);
extern int token_is_word(tree_t *this);

extern int lex_is_end();
extern void lex_init(char *line);
extern void lex_destroy();
extern tree_t *lex_get_token();
extern void lex_unget_token(tree_t *token);
extern void lex_dump();
extern void lex_test();

#endif

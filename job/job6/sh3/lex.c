#include "root.h"
#include "parse.h"
#include "lex.h"
#include <ctype.h>

static char *buffer;
static char *cursor;
static char *bound;
static tree_t *first_token;

void lex_init(char *line)
{
    buffer = strdup(line);
    cursor = buffer;
    bound = buffer + strlen(line);
    first_token = NULL;
}

void lex_destroy()
{
    free(buffer);
}

tree_t *token_new(char *start, char *end)
{
    int size = end - start;
    char *token = malloc(size + 1);
    memcpy(token, start, size);
    token[size] = 0;

    tree_t *this = tree_new(TREE_TOKEN);
    this->token = token;
    return this;
}

int token_is(tree_t *this, char *token)
{
    assert(this->type == TREE_TOKEN);
    return strcmp(this->token, token) == 0;
}

static int is_operator(char c)
{
    static char *operators = "|<>&";
    return strchr(operators, c) != NULL;
}

static int is_word(char c)
{
    return !isspace(c) && !is_operator(c);
}

int token_is_word(tree_t *this)
{
    char c = this->token[0];
    return is_word(c);
}

void lex_skip_space()
{
    while ((cursor < bound) && isspace(*cursor))
        cursor++;
}

void lex_skip_word()
{
    while ((cursor < bound) && is_word(*cursor))
        cursor++;
}

int lex_is_end()
{
    lex_skip_space();
    return cursor == bound;
}

tree_t *lex_get_token()
{
    if (first_token) {
        tree_t *token = first_token;
        first_token = NULL;
        return token;
    }

    lex_skip_space();
    if (cursor == bound)
        return NULL;

    char *saved_cursor = cursor;
    if (is_operator(*cursor)) {
        cursor++;
        return token_new(saved_cursor, cursor);
    }
 
    lex_skip_word();
    return token_new(saved_cursor, cursor);
}

void lex_unget_token(tree_t *token)
{
    assert(token != NULL);
    assert(first_token == NULL);
    first_token = token;
}

void lex_dump()
{
    tree_t *tree;

    while (tree = lex_get_token())
        printf("[%s]\n", tree->token);
}

void lex_test()
{
    lex_init("cat <input | sort| cat > output");
    lex_dump();
    lex_destroy();
}

#include "root.h"
#include "parse.h"
#include "lex.h"

void logger(char *text)
{
#ifdef notyet
    puts(text);
#endif
}

tree_t *tree_new(int type)
{
    tree_t *this;

    this = malloc(sizeof(tree_t));
    this->type = type;
    this->token = NULL;
    vector_init(&this->child_vector);
    return this;
}

void indent(int level, char *text)
{
    int i;
    for (i = 0; i < level; i++)
        printf("  ");
    puts(text);
}

void tree_dump(tree_t *this, int level)
{
    switch (this->type) {
        case TREE_ASYNC:
            indent(level, "async");
            break;

        case TREE_PIPE:
            indent(level, "pipe");
            break;

        case TREE_REDIRICT:
            indent(level, "redirect");
            break;

        case TREE_BASIC:
            indent(level, "basic");
            break;

        case TREE_TOKEN:
			indent(level, this->token);
            break;
    }

    int i;
    tree_t *child;
    vector_each (&this->child_vector, i, child)
        tree_dump(child, level + 1);
}

tree_t *tree_get_child(tree_t *this, int index)
{
    tree_t *child = vector_get(&this->child_vector, index);
    return child;
}

void tree_append_child(tree_t *this, tree_t *child)
{
    vector_push_back(&this->child_vector, child);
}

tree_t *parse_basic_tree()
{
    tree_t *token;
    tree_t *basic_tree;

    logger("parse basic tree");
    basic_tree = tree_new(TREE_BASIC);
    while ((token = lex_get_token()) && token_is_word(token))
        tree_append_child(basic_tree, token);
    if (token) 
        lex_unget_token(token);
    return basic_tree;
}

tree_t *parse_redirect_tree()
{
    tree_t *token;
    tree_t *basic_tree; 
    tree_t *redirect_tree;

    logger("parse redirect tree");
    basic_tree = parse_basic_tree();
    token = lex_get_token();
    if (!token)
        return basic_tree;

    if (token_is(token, ">") || token_is(token, "<")) {
        redirect_tree = tree_new(TREE_REDIRICT);
        tree_append_child(redirect_tree, basic_tree);
        tree_append_child(redirect_tree, token);
        token = lex_get_token(); 
        assert(token);
        tree_append_child(redirect_tree, token);
        return redirect_tree;
    }

    lex_unget_token(token);
    return basic_tree;
}

tree_t *parse_pipe_tree()
{
    tree_t *token;
    tree_t *redirect_tree;
    tree_t *right_tree;
    tree_t *pipe_tree;

    logger("parse pipe tree");
    redirect_tree = parse_redirect_tree();

    token = lex_get_token();
    if (token == NULL)
        return redirect_tree;
    if (!token_is(token, "|")) {
        lex_unget_token(token);
        return redirect_tree;
    }

    right_tree = parse_pipe_tree();
    pipe_tree = tree_new(TREE_PIPE);
    tree_append_child(pipe_tree, redirect_tree);
    tree_append_child(pipe_tree, right_tree);
    return pipe_tree;
}

tree_t *parse_async_tree()
{
    tree_t *token;
    tree_t *async_tree;
    tree_t *pipe_tree;
    
    logger("parse async tree");
    pipe_tree = parse_pipe_tree();

    token = lex_get_token();
    if (token == NULL)
        return pipe_tree;
    if (!token_is(token, "&"))
        assert(0);

    async_tree = tree_new(TREE_ASYNC);
    tree_append_child(async_tree, pipe_tree);
    return async_tree;
}

/**
 * async_tree 
 *  : pipe_tree '&'
 *  ;
 *
 * pipe_tree 
 *  : redirect_tree 
 *  | redirect_tree '|' pipe_tree
 *  ;
 *
 * redirect_tree 
 *  : simple_tree
 *  | redirect_tree '>' token
 *  | redirect_tree '<' token
 *  ;
 *
 * simple_tree
 *  : token token ... token
 *  ;
 */
tree_t *parse_tree()
{
    if (lex_is_end())
        return NULL;
    return parse_async_tree();
}

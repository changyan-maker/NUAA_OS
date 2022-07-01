#ifndef _PARSE_H
#define _PARSE_H

#include "vector.h"

enum {
    TREE_ASYNC,     // cmd &
    TREE_PIPE,      // cmdA | cmdB
    TREE_REDIRICT,  // cmd >output <input
    TREE_BASIC,     // cmd arg1 arg2
    TREE_TOKEN,     // leaf 
};

typedef struct {
    int type;
    char *token;            // TREE_TOKEN
    vector_t child_vector;  // other tree
} tree_t;

extern tree_t *tree_new(int type);
extern tree_t *tree_get_child(tree_t *this, int index);
extern void tree_append_child(tree_t *this, tree_t *child);
extern tree_t *parse_tree();
extern void tree_dump(tree_t *this, int level);

#endif

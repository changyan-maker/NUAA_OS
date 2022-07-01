#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

enum {
    TREE_PIPE,         // cmdA | cmdB
    TREE_REDIRECT,     // cmd >output <input
    TREE_BASIC,        // cmd arg1 arg2
};

struct tree {
    int type;
};

#define MAX_ARG 8

struct basic_tree {
    int type;
    int argc;
    char *argv[MAX_ARG];
};

struct redirect_tree {
    int type;
    char *io;           // "<" or ">"
    char *file;
    struct tree *child;
};

struct pipe_tree {
    int type;
    struct tree *left;
    struct tree *right;
};

// cat </etc/passwd 
struct tree *build_cat()
{
    struct basic_tree *cat = malloc(sizeof(struct basic_tree));
    cat->type = TREE_BASIC;
    cat->argc = 1;
    cat->argv[0] = "cat";
    cat->argv[1] = NULL;

    struct redirect_tree *in = malloc(sizeof(struct redirect_tree));
    in->type = TREE_REDIRECT;
    in->io = "<";
    in->file = "/etc/passwd";
    in->child = (struct tree *)cat;
    return (struct tree *)in;
}

// wc -l >log
struct tree *build_wc()
{
    struct basic_tree *wc = malloc(sizeof(struct basic_tree));
    wc->type = TREE_BASIC;
    wc->argc = 2;
    wc->argv[0] = "wc";
    wc->argv[1] = "-l";
    wc->argv[2] = NULL;

    struct redirect_tree *out = malloc(sizeof(struct redirect_tree));
    out->type = TREE_REDIRECT;
    out->io = ">";
    out->file = "log";
    out->child = (struct tree *) wc;
    return (struct tree *)out;
}

struct tree *build_pipe()
{
    struct tree *cat = build_cat();
    struct tree *wc = build_wc();

    struct pipe_tree *pipe = malloc(sizeof(struct pipe_tree));
    pipe->type = TREE_PIPE;
    pipe->left = cat;
    pipe->right = wc;
    return (struct tree *)pipe;
}

void indent(int level)
{
    int i;
    for (i = 0; i < level; i++)
        printf("  ");
}

void tree_dump(struct tree *tree, int level);

void basic_tree_dump(struct basic_tree *tree,  int level)
{
    indent(level);
    printf("basic\n");

    indent(level + 1);
    int i;
    for (i = 0; i < tree->argc; i++)
        printf("%s ", tree->argv[i]);
    printf("\n");
}

void redirect_tree_dump(struct redirect_tree *tree, int level)
{
    indent(level);
    printf("redirect\n");

    tree_dump(tree->child, level + 1);

    indent(level + 1);
    printf("%s\n", tree->io);

    indent(level + 1);
    printf("%s\n", tree->file);
}

void pipe_tree_dump(struct pipe_tree *tree, int level)
{
    indent(level);
    printf("pipe\n");

    tree_dump(tree->left, level + 1);
    tree_dump(tree->right, level + 1);
}

void tree_dump(struct tree *tree, int level)
{
    switch (tree->type) {
        case TREE_BASIC:
            basic_tree_dump((struct basic_tree *) tree, level);
            break;

        case TREE_REDIRECT:
            redirect_tree_dump((struct redirect_tree *) tree, level);
            break;

        case TREE_PIPE:
            pipe_tree_dump((struct pipe_tree *) tree, level);
            break;
    
	}
}

//Todo
void basic_tree_exec(struct basic_tree *tree)
{

}

void redirect_tree_exec(struct redirect_tree *tree)
{
		
}

void pipe_tree_exec(struct pipe_tree *tree)
{	

}

void tree_exec(struct tree *tree)
{

}

int main()
{
    struct tree *pipe = build_pipe();
    tree_dump(pipe, 0);
    tree_exec(pipe);
    return 0;
}

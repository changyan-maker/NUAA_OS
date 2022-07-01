#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parse.h"
#include "exec.h"

int verbose = 0;

void execute_line(char *line)
{
    tree_t *tree;
    lex_init(line);
    tree = parse_tree();
    if (verbose)
        tree_dump(tree, 0);
    if (tree != NULL) 
        tree_execute_wrapper(tree), tree_dump(tree, 0);
    lex_destroy();
}

void read_line(char *line, int size)
{
    int count;

    count = read(0, line, size);
    if (count == 0)
        exit(EXIT_SUCCESS);
    assert(count > 0);
    if ((count > 0) && (line[count - 1] == '\n'))
        line[count - 1] = 0;
    else
        line[count] = 0;
}

void read_and_execute()
{
    char line[128];

    write(1, "# ", 2);
    read_line(line, sizeof(line));
    execute_line(line);
}

void test()
{
    execute_line("cat /etc/passwd | sort | grep root >log");
}

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "-v") == 0) 
        verbose = 1;
    while (1)
        read_and_execute();
    return 0;
}

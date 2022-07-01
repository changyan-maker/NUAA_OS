#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define CONSUMERS 2
#define ITEMS 10

pid_t pid[2];

void delay()
{
    int i;
    for (i = 0; i < 87654321; i++)
        ;
}

//Todo
void consume(int fd){

}

void produce(int fd)
{

}

int main()
{
   
    delay();
    printf("Produce END\n");
    return 0;
}

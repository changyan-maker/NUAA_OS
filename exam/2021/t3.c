#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

struct node {
	char data;
	struct node *next;
};

pthread_t workers[7];

struct node *node_create(char data){
	struct node *node = malloc(sizeof(struct node));
	node->data = data;
	node->next = NULL;
	return node;
}

void *node_print(void *arg){
	struct node *node = (struct node *)arg;
	printf("%c\n",node->data);
}

void list_visit(struct node *list){
	struct node *node = list;
	int i=0;
	while(node){
		pthread_create(&workers[i], NULL, node_print, node);
		i++;
		node = node->next;
	}
}

void list_print(struct node *list){
	struct node *node = list;
	while(node){
		printf("%c\n", node->data);
		node = node->next;
	}
}

void wait_sub_threads(){
	for(int i=0;i<7;i++)
		pthread_join(workers[i], NULL);
}

int main(){
	struct node *list = NULL;
	for(char data = 'g'; data >= 'a';data--){
		struct node *node = node_create(data);
		node->next = list;
		list = node;
	}

	list_visit(list);
	wait_sub_threads();
	puts("END");
	return 0;
}

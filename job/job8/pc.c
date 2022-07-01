#include<stdio.h>
#include<pthread.h>

#define CAPACITY 4
#define ITEM_COUNT (CAPACITY*2)

int buffer1[CAPACITY];
int buffer2[CAPACITY];
int in[2],out[2];

pthread_mutex_t mutex[2];
pthread_cond_t wait_empty_buffer[2];
pthread_cond_t wait_full_buffer[2];

int buffer_is_empty(int i){
	return (in[i]==out[i]);
}

int buffer_is_full(int i){
	return (((in[i]+1)% CAPACITY) == out[i]);
}

void put_item(int i,int item){
	pthread_mutex_lock(&mutex[i]);
	while(buffer_is_full(i))
		pthread_cond_wait(&wait_empty_buffer[i], &mutex[i]);

	if(i==0){				//put item
		buffer1[in[i]] = item;	
	}
	else
		buffer2[in[i]] = item;
	in[i] = (in[i]+1)%CAPACITY;

	pthread_cond_signal(&wait_full_buffer[i]);
	pthread_mutex_unlock(&mutex[i]);
}

int get_item(int i){	
	int item;
	pthread_mutex_lock(&mutex[i]);	 	//let the mutex and cond in the get_item
	while(buffer_is_empty(i))		//prevent some errors
		pthread_cond_wait(&wait_full_buffer[i], &mutex[i]);

	if(i==0)
		item = buffer1[out[i]];		//get item
	else
		item = buffer2[out[i]];

	out[i] = (out[i]+1)% CAPACITY;

	pthread_cond_signal(&wait_empty_buffer[i]);
	pthread_mutex_unlock(&mutex[i]);	
	
	return item;
}

void *produce(){
	int item = 'a';
	for(int i=0;i<ITEM_COUNT;i++){
		printf("%c\n",item);
		put_item(0,item);
		item++;
	}
}

void *compute(){
	int item;
	for(int i=0;i<ITEM_COUNT;i++){
		item = get_item(0);
		printf("    %c:",item);
		item = item - 32;
		put_item(1,item);
		printf("%c\n",item);
	}
}

void *consume(){
	int item;
	for(int i=0;i<ITEM_COUNT;i++){
		item = get_item(1);
		printf("         %c\n",item);
	}
}


int main(){
	pthread_t producer,computer,consumer;

	int i;
	for(i=0;i<2;i++){			//init
		pthread_mutex_init(&mutex[i], NULL);
		pthread_cond_init(&wait_empty_buffer[i], NULL);
		pthread_cond_init(&wait_full_buffer[i], NULL);
	}
	
	//create the sub threads
	pthread_create(&producer, NULL, produce, NULL);		
	pthread_create(&computer, NULL, compute, NULL);
	pthread_create(&consumer, NULL, consume, NULL);

	//wait the sub threads finished
	pthread_join(producer, NULL);
	pthread_join(computer, NULL);
	pthread_join(consumer, NULL);

	return 0;
}

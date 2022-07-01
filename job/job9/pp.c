#include<stdio.h>
#include<pthread.h>

#define CAPACITY 4
#define ITEM_COUNT (CAPACITY*2)

int in[2],out[2];
int buffer[2][CAPACITY];

typedef struct{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

void sema_init(sema_t* sema, int value){
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	while(sema->value<=0)
		pthread_cond_wait(&sema->cond, &sema->mutex);
	sema->value--;
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	sema->value++;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex_sema[2];
sema_t full_buffer[2];
sema_t empty_buffer[2];

int buffer_is_empty(int i){
	return (in[i]==out[i]);
}

int buffer_is_full(int i){
	return ((in[i]+1)%CAPACITY == out[i]);
}

void put_item(int i,int item){
	sema_wait(&mutex_sema[i]);
	sema_wait(&empty_buffer[i]);

	buffer[i][in[i]] = item;
	in[i] = (in[i]+1) % CAPACITY;

	sema_signal(&full_buffer[i]);
	sema_signal(&mutex_sema[i]);
}

int get_item(int i){
	int item;
	sema_wait(&mutex_sema[i]);
	sema_wait(&full_buffer[i]);

	item = buffer[i][out[i]];
	out[i] = (out[i]+1)% CAPACITY;

	sema_signal(&empty_buffer[i]);
	sema_signal(&mutex_sema[i]);

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
		put_item(1, item);
		printf("%c\n", item);
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
	for(i=0;i<2;i++){
		sema_t *sema;
		sema = &mutex_sema[i];
		sema_init(sema,1);
		sema = &empty_buffer[i];
		sema_init(sema, CAPACITY-1);
		sema = &full_buffer[i];
		sema_init(sema, 0);
	}

	pthread_create(&producer, NULL, produce, NULL);
	pthread_create(&computer, NULL, compute, NULL);
	pthread_create(&consumer, NULL, consume, NULL);

	pthread_join(producer, NULL);
	pthread_join(computer, NULL);
	pthread_join(consumer, NULL);

	return 0;
}

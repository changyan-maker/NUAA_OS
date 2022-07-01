#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

#define CAPACITY 4
int buffer1[CAPACITY];
int in1;
int out1;
int buffer2[CAPACITY];
int in2;
int out2;

int buffer1_is_empty(){
	return in1 == out1;
}

int buffer1_is_full(){
	return (in1 + 1) % CAPACITY == out1;
}

int buffer2_is_empty(){
	return in2 == out2;
}

int buffer2_is_full(){
	return (in2 + 1) % CAPACITY == out2;
}

int get_item1(){
	int item1;
	item1 = buffer1[out1];
	out1 = (out1 + 1) % CAPACITY;
	return item1;
}

int get_item2(){
	int item2;
	item2 = buffer2[out2];
	out2 = (out2 + 1) % CAPACITY;
	return item2;
}

void put_item1(int item1){
	buffer1[in1] = item1;
	in1 = (in1 + 1)% CAPACITY;
}

void put_item2(int item2){
	buffer2[in2] = item2;
	in2 = (in2 + 1)% CAPACITY;
}

typedef struct {
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sema, int value){
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	while(sema->value <= 0)
		pthread_cond_wait(&sema->cond, &sema->mutex);
	sema->value --;
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	++ sema->value;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex1_sema;
sema_t empty_buffer1_sema;
sema_t full_buffer1_sema;
sema_t mutex2_sema;
sema_t empty_buffer2_sema;
sema_t full_buffer2_sema;

#define ITEM_COUNT (CAPACITY*2)

void *consume(void *arg){
	int i;
	int item2;
	for(i=0;i<ITEM_COUNT;i++){
		sema_wait(&full_buffer2_sema);
		sema_wait(&mutex2_sema);

		item2 = get_item2();
		printf("        %c\n", item2);

		sema_signal(&mutex2_sema);
		sema_signal(&empty_buffer2_sema);
	}
	return NULL;
}

void *compute(){
	int i;
	int item1;
	int item2;

	for(i=0;i<ITEM_COUNT;i++){
		//read
		sema_wait(&full_buffer1_sema);
		sema_wait(&mutex1_sema);

		item1 = get_item1();
		item2 = item1 - 32;
		printf("    %c:%c\n",item1,item2);

		sema_signal(&mutex1_sema);
		sema_signal(&empty_buffer1_sema);

		//write
		sema_wait(&empty_buffer2_sema);
		sema_wait(&mutex2_sema);

		
		put_item2(item2);
		
		sema_signal(&mutex2_sema);
		sema_signal(&full_buffer2_sema);
	}
	return NULL;
}

void *produce(){
	int i;
	int item1;

	for(i=0;i<ITEM_COUNT;i++){
		sema_wait(&empty_buffer1_sema);
		sema_wait(&mutex1_sema);

		item1 = i + 'a';
		put_item1(item1);
		printf("%c\n", item1);

		sema_signal(&mutex1_sema);
		sema_signal(&full_buffer1_sema);
	}
	return NULL;
}

int main(){
	pthread_t consumer_tid;
	pthread_t computer_tid;

	sema_init(&mutex1_sema,1);
	sema_init(&empty_buffer1_sema, CAPACITY-1);
	sema_init(&full_buffer1_sema, 0);

	sema_init(&mutex2_sema, 1);
	sema_init(&empty_buffer2_sema, CAPACITY-1);
	sema_init(&full_buffer2_sema, 0);

	pthread_create(&computer_tid, NULL, compute, NULL);
	pthread_create(&consumer_tid, NULL, consume, NULL);
	produce();
	pthread_join(consumer_tid, NULL);
	pthread_join(computer_tid, NULL);
	return 0;
}

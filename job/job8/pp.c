#include<stdio.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t wait_ping,wait_pong;

int state = 1;

void *ping(){
	int count=0;
	while(count++ < 20){
		pthread_mutex_lock(&mutex);
		while(state!=1)
			pthread_cond_wait(&wait_ping, &mutex);
		printf("ping\n");
		state = 0;
		pthread_cond_signal(&wait_pong);
		pthread_mutex_unlock(&mutex);
	}
}

void *pong(){
	int count=0;
	while(count++ < 20){
		pthread_mutex_lock(&mutex);
		while(state!=0)	
			pthread_cond_wait(&wait_pong, &mutex);
		printf("pong\n");
		state = 1;
		pthread_cond_signal(&wait_ping);
		pthread_mutex_unlock(&mutex);
	}
}

int main(){
	pthread_t ping_tid,pong_tid;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&wait_ping, NULL);
	pthread_cond_init(&wait_pong, NULL);

	pthread_create(&ping_tid, NULL, ping, NULL);
	pthread_create(&pong_tid, NULL, pong, NULL);

	pthread_join(ping_tid, NULL);
	pthread_join(pong_tid, NULL);

	return 0;
}

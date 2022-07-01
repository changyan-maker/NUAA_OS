#include<stdio.h>
#include<pthread.h>

typedef struct{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

sema_t ping_sema;
sema_t pong_sema;

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
	sema->value++;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

void *ping(){
	int count=0;
	while(count++<10){
		sema_wait(&ping_sema);
		printf("ping\n");
		sema_signal(&pong_sema);
	}
}

void *pong(){
	int count = 0;
	while(count++ < 10){
		sema_wait(&pong_sema);
		printf("pong\n");
		sema_signal(&ping_sema);
	}
}

int main(){
	pthread_t ping_tid,pong_tid;
	sema_init(&ping_sema, 1);
	sema_init(&pong_sema, 0);

	pthread_create(&ping_tid, NULL, ping, NULL);
	pthread_create(&pong_tid, NULL, pong, NULL);

	pthread_join(ping_tid, NULL);
	pthread_join(pong_tid, NULL);

	return 0;
}

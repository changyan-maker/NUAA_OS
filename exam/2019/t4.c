#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<dirent.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t is_dad, is_son, is_mom, is_dau;

int state = 1;

void *dad(){
	int num = 0;
	while(num++ < 10){
		pthread_mutex_lock(&mutex);
		while(state!=1)
			pthread_cond_wait(&is_dad, &mutex);

		printf("put apple\n");
		state = 2;

		pthread_cond_signal(&is_son);
		pthread_mutex_unlock(&mutex);
	}
}

void *son(){
	int num = 0;
	while(num++ < 10){
		pthread_mutex_lock(&mutex);
		while(state!=2)
			pthread_cond_wait(&is_son, &mutex);

		printf("get apple\n");
		state = 3;

		pthread_cond_signal(&is_mom);
		pthread_mutex_unlock(&mutex);
	}
}


void *mom(){
	int num = 0;
	while(num++ < 10){
		pthread_mutex_lock(&mutex);
		while(state!=3)
			pthread_cond_wait(&is_mom, &mutex);

		printf("put orange\n");
		state = 4;

		pthread_cond_signal(&is_dau);
		pthread_mutex_unlock(&mutex);
	}
}


void *dau(){
	int num = 0;
	while(num++ < 10){
		pthread_mutex_lock(&mutex);
		while(state!=4)
			pthread_cond_wait(&is_dau, &mutex);

		printf("get orange\n");
		state = 1;

		pthread_cond_signal(&is_dad);
		pthread_mutex_unlock(&mutex);
	}
}


int main(){
	pthread_t dad_tid,son_tid,mom_tid,dau_tid;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&is_dad, NULL);
	pthread_cond_init(&is_son, NULL);	
	pthread_cond_init(&is_mom, NULL);	
	pthread_cond_init(&is_dau, NULL);

	pthread_create(&dad_tid, NULL, dad, NULL);
	pthread_create(&son_tid, NULL, son, NULL);
	pthread_create(&mom_tid, NULL, mom, NULL);	
	pthread_create(&dau_tid, NULL, dau, NULL);

	pthread_join(dad_tid, NULL);	
	pthread_join(son_tid, NULL);	
	pthread_join(mom_tid, NULL);	
	pthread_join(dau_tid, NULL);

	return 0;
}

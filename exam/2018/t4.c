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

int state = 1;

pthread_mutex_t mutex;
pthread_cond_t is_t1, is_t2, is_t3, is_t4;


void *T1_entry(void *arg){
	pthread_mutex_lock(&mutex);
	while(state != 1)
		pthread_cond_wait(&is_t1,&mutex);
	
	sleep(2);
	puts("T1");
	state = 2;

	pthread_cond_signal(&is_t2);
	pthread_cond_signal(&is_t3);
	pthread_mutex_unlock(&mutex);
}
int finish = 0;

void *T2_entry(void *arg){
	pthread_mutex_lock(&mutex);
	while(state != 2)
		pthread_cond_wait(&is_t2, &mutex);

	sleep(1);
	puts("T2");
	if(finish == 0){
		state = 3;
		finish = 2;
		pthread_cond_signal(&is_t3);
	}
	else{
		state = 4;
		pthread_cond_signal(&is_t4);
	}
	
	pthread_mutex_unlock(&mutex);
}

void *T3_entry(void *arg){
	pthread_mutex_lock(&mutex);
	while(state!=2 && state != 3)
		pthread_cond_wait(&is_t3, &mutex);

	sleep(1);
	puts("T3");

	if(finish == 0){
		state = 2;
		finish = 3;
		pthread_cond_signal(&is_t2);
	}
	else{
		state = 4;
		pthread_cond_signal(&is_t4);
	}
	pthread_mutex_unlock(&mutex);
}

void *T4_entry(void *arg){
	pthread_mutex_lock(&mutex);
	while(state != 4)
		pthread_cond_wait(&is_t4, &mutex);
	puts("T4");	

	pthread_mutex_unlock(&mutex);
}

int main(){
	pthread_t t1_tid,t2_tid,t3_tid,t4_tid;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&is_t1, NULL);	
	pthread_cond_init(&is_t2, NULL);
	pthread_cond_init(&is_t3, NULL);
	pthread_cond_init(&is_t4, NULL);

	pthread_create(&t1_tid, NULL, T1_entry, NULL);	
	pthread_create(&t2_tid, NULL, T2_entry, NULL);	
	pthread_create(&t3_tid, NULL, T3_entry, NULL);
	pthread_create(&t4_tid, NULL, T4_entry, NULL);

	pthread_join(t1_tid, NULL);	
	pthread_join(t2_tid, NULL);	
	pthread_join(t3_tid, NULL);
	pthread_join(t4_tid, NULL);

	return 0;
}

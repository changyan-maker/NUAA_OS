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

#define n 2

typedef struct{
	int start;
	int end;
}param;

typedef struct{
	int res;
}result;

void *compute(void *arg){
	param *par = (param *)arg;
	int res=0;
	for(int i=par->start;i<=par->end;i++)
		res += i;
	result *result;
	result = malloc(sizeof(result));
	result->res = res;
	return result;
}

int main(){
	pthread_t workers[n];

	int i;
	param pars[i];
	for(i=0;i<n;i++){
		param *par;
		par = &pars[i];
		par->start = i*50 + 1;
		par->end = (i+1)*50;
		pthread_create(&workers[i], NULL, compute, par);
	}
	int res = 0;
	result *result[n];
	for(i=0;i<n;i++)
		pthread_join(workers[i],(void **)&result[i]);

	for(i=0;i<n;i++)
		res += result[i]->res;
	printf("%d\n",res);
	return 0;
}

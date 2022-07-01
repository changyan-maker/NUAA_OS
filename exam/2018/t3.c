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

#define n 10

typedef struct{
	int start;
	int end;
}param;

typedef struct{
	int res;
}result;

void *compute(void *arg){
	param *par = (param *)arg;
	int sum = 0;
	for(int i=par->start;i<=par->end;i++)
		sum += i;

	result *res;
	res = malloc(sizeof(param));
	res->res = sum;
	return res;
}

int main(){
	pthread_t computers[n];
	
	int i;
	param pars[n];
	for(i=0;i<n;i++){
		param *par;
		par = &pars[i];
		par->start = (i*10)+1;
		par->end = (i+1)*10;
		pthread_create(&computers[i], NULL, compute, par);
	}

	result *res[n];
	for(i=0;i<n;i++){
		pthread_join(computers[i], (void **)&res[i]);
	}

	int ans = 0;
	for(i=0;i<n;i++)
		ans += res[i]->res;

	printf("%d\n",ans);
	return 0;
}

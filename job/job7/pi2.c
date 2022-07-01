#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
int N = 4;
int count = 1e7;

typedef struct{		//return the result
	double sum;
}result;

typedef struct{		//connect main_thread and child_thread
	int start;
	int end;
}param;

void *compute(void *arg){
	double sum=0;
	param *par;
	par = (param *)arg;
	for(int i=par->start;i<par->end;i++){
		if(i&1)		//sub
			sum -= (1.0/(i*2+1));
		else		//add
			sum += (1.0/(i*2+1));
	}
	result *res;		// return the result
	res = malloc(sizeof(result));
	res->sum = sum;
	return res;
}

int main(){
	pthread_t computers[N];
	int i;
	param pars[N];
	for(i=0;i<N;i++){		// create the child threads
		param *par;
		par = &pars[i];
		par->start = (count/N) * i;
		par->end = (count/N) * (i+1);
		pthread_create(&computers[i], NULL, compute, par);
	}
	result *res[N];
	for(i=0;i<N;i++)		// wait the child threads
		pthread_join(computers[i], (void**)&res[i]);
	double sum = 0;
	for(i=0;i<N;i++)
		sum += res[i]->sum;		// add the result
	printf("%.7lf\n",sum*4);
	return 0;
}

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int count = 1e7;

typedef struct{
	double sum;
}result;

void *compute(){
	double sum = 0;
	for(int i=count/2;i<count;i++){
		if(i&1)		//sub
			sum -= (1.0/(i*2+1));
		else 		//sub
			sum += (1.0/(i*2+1));
	}
	result *res;
	res = malloc(sizeof(result));
	res->sum = sum;
	return res;
}

int main(){
	pthread_t compute_tid;
	pthread_create(&compute_tid, NULL, compute, NULL);
	double sum=0;
	for(int i=0;i<count/2;i++){
		if(i&1)		//sub
			sum -= (1.0/(i*2+1));
		else		//add
			sum += (1.0/(i*2+1));
	}
	result *res;
	pthread_join(compute_tid, (void **)&res);	//wait compute finished
	sum = (sum + res->sum)*4;
	printf("%.7lf\n",sum);
	return 0;
}

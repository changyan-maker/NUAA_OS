#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int array[] = {5, 9, 1, 3, 6, 7, 2, 10, 4, 8};
#define N 2
#define num 10
int temp[num];

struct param {
	int *array;
	int start;
	int end;
};

struct result {
	int *array;
};

void *selection_sort(void *arg){
	struct param *param;
	struct result *result;
	int sum = 0;
	param = (struct param *)arg;
	int i,j,min,temp;
	for(i=param->start;i<param->end-1;i++){	
		min = i;
		for(j=i+1;j<param->end;j++){
			if(param->array[j]<param->array[min])
				min = j;
		}
		if(min!=i){
			temp = param->array[min];
			param->array[min] = param->array[i];
			param->array[i] = temp;
		}
	}
	result = malloc(sizeof(struct result));
	result->array = param->array;
	return result;
}

void merge_sort(int a[], int l, int r){
	int mid;
	if( l >= r)	return;
	mid = (l+r)/2;
	merge_sort(a,l,mid);
	merge_sort(a,mid+1,r);
	int i = l, j = mid + 1;
	int k = 0;
	while(i<=mid && j<=r) {
		if(a[i]<=a[j])
			temp[k++] = a[i++];
		else
			temp[k++] = a[j++];
	}
	while(i<=mid) temp[k++] = a[i++];
	while(j<=r) temp[k++] = a[j++];
	for(i=l,j=0;i<=r;i++,j++)
		a[i] = temp[j];
}

int main(){
	pthread_t workers[N];
	struct param params[N];
	for(int i=0;i<N;i++){
		struct param *param;
		param  = &params[i];
		param->array = array;
		param->start = i * (num/N);
	        param->end = (i+1) * (num/N);
		pthread_create(&workers[i], NULL, selection_sort, param);	
	}
	for(int i=0;i<N;i++){
		struct result *result;
		pthread_join(workers[i], (void **)&result);
		free(result);
	}
	// finish selection_sort
	merge_sort(array,0,num-1);
	for(int i=0;i<num;i++)
		printf("%d ",array[i]);
	printf("\n");
	return 0;
}

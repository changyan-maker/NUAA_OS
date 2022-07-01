#include<stdio.h>
#include<pthread.h>

int array[] = {5, 9, 1, 3, 6, 7, 2, 10, 4, 8};
#define N 2
#define num 10
int temp[num];

typedef struct{
	int *array;
	int start;
	int end;
}param;

void *select_sort(void *arg){
	param *par;
	int sum = 0;
	par = (param *)arg;
	int i,j,min,temp;
	for(i=par->start;i<par->end-1;i++){	
		min = i;
		for(j=i+1;j<par->end;j++){
			if(par->array[j]<par->array[min])
				min = j;
		}
		if(min!=i){
			temp = par->array[min];
			par->array[min] = par->array[i];
			par->array[i] = temp;
		}
	}
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
	int i;
	param params[N];
	for(i=0;i<N;i++){
		param *par;
		par = &params[i];
		par->array = array;
		par->start = (num/2)*i;
		par->end = (num/2)*(i+1);
		pthread_create(&workers[i], NULL, select_sort, par);
	}

	for(i=0;i<N;i++)
		pthread_join(workers[i], NULL);

	merge_sort(array, 0, num-1);

	for(i=0;i<num;i++)
		printf("%d ",array[i]);
	printf("\n");
	return 0;
}

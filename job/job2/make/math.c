#include<stdio.h>

extern int max(int a,int b);
extern int min(int a,int b);

int main()
{
	printf("min = %d\n", min(1,2));
	printf("max = %d\n", max(1,2));
	return 0;
}

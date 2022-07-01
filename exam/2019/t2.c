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

# define n 10
int main(){
	int pid[n];
	int fd[2];
	pipe(fd);
	int res = 0;
	for(int i=0;i<n;i++){
		pid[i] = fork();
		if(pid[i] == 0){
			dup2(fd[1],1);
			close(fd[0]);
			close(fd[1]);
			int sum = 0;
			for(int j=i*10+1;j<=(i+1)*10;j++)
				sum += j;
			write(1, &sum, sizeof(sum));
			exit(0);
		}
		else{
			wait(NULL);
			dup2(fd[0],0);
			int ans = 0;
			read(0,&ans,sizeof(ans));
			res += ans;
		}
	
	}
	printf("%d\n",res);
	return 0;
}

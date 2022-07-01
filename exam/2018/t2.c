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

int main(){
	int pid[2];
	int fd[2];
	pipe(fd);
	int result = 0;
	for(int i=0;i<2;i++){
		pid[i] = fork();
		if(pid[i] == 0){
			dup2(fd[1],1);
			close(fd[0]);
			close(fd[1]);
			int sum = 0;
			for(int j=i*50+1;j<=(i+1)*50;j++){
				sum += j;
			}
			write(1, &sum, sizeof(sum));
			exit(0);
		}
		else{
			wait(NULL);
			dup2(fd[0],0);
			int res = 0;
			read(0, &res, sizeof(res));
			result += res;
		}
	}
	printf("%d\n",result);
	return 0;
}

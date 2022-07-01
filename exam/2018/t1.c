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
	int pid;
	int fd[2];
	pipe(fd);
	pid = fork();
	if(pid == 0){
		dup2(fd[1],1);
		close(fd[0]);
		close(fd[1]);
		execlp("echo","echo","hello world", NULL);
		exit(0);
	}
	else{
		wait(NULL);
		dup2(fd[0],0);
		close(fd[0]);
		close(fd[1]);
		char buf[100];
		read(0, buf, sizeof(buf));
		printf("%s\n",buf);
	}
	return 0;
}

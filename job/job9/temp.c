#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc, char *argv[]){
	int pid;
	int fd[2];
	pipe(fd);
	pid = fork();
	if(pid==0){
		close(fd[0]);
		dup2(fd[1],1);
		close(fd[1]);
		execlp("cat","cat","/etc/passwd", NULL);
		exit(0);
	}
	else{
		wait(NULL);
		dup2(fd[0],0);
		close(fd[0]);
		close(fd[1]);
		execlp("wc","wc","-l",NULL);
	}
	return 0;
}

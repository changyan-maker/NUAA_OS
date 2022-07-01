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
#include<ctype.h>

int main(){
	int pid;
	int fd[2];
	pipe(fd);
	pid = fork();
	if(pid==0){
		sleep(1);
		dup2(fd[0],0);
		close(fd[1]);
		close(fd[0]);
		int fd1 = open("result.txt", O_CREAT|O_WRONLY, 0666);
		dup2(fd1, 1);
		close(fd1);
		execlp("wc","wc","-l", NULL);
	}
	else{
		dup2(fd[1],1);
		close(fd[0]);
		close(fd[1]);
		int fd2 = open("/etc/passwd", O_RDONLY);
		dup2(fd2,0);
		close(fd2);
		execlp("cat","cat", NULL);
	}
	return 0;
}

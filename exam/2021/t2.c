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

int main(int argc, char *argv[]){
	char target[128];
	char path[128];
	strcpy(target, argv[1]);
	strcpy(path, argv[2]);
	int count;
	int fd;

	fd = open(path, O_RDONLY);
	char buf[512];
	count = read(fd,buf,sizeof(buf));
	while(count){
		buf[count] = 0;

		char *word = strtok(buf, "\n");
		char *argv[100];
		int num = 0;
		while(word != NULL){
			argv[num++] = word;
			word = strtok(NULL, "\n");
		}
		
		for(int i=0;i<num;i++){
			if(strstr(argv[i],target))
				printf("%s\n", argv[i]);
		}
		count = read(fd, buf, sizeof(buf));
	}
	close(fd);

	return 0;
}

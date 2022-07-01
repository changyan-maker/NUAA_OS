#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<stdlib.h>

void mysys(char *command)
{
	int pid;
	pid = fork();
	if(pid==0){
		int i=0;
		char a[5][20];
		int num = 0;
		int j=0;
		while(command[i]!='\0')
		{
			if(command[i]!=' '){
				a[num][j++] = command[i];
			}		
			else{
				a[num++][j++] = '\0';
				j=0;
			}
			i++;
		}
		a[num][j++] = '\0';
		int error;

		char *argv[5];
		for(i=0;i<=num;i++){
			argv[i] = (char *)malloc(sizeof(char)*20);
			strcpy(argv[i],a[i]);
		}
		argv[i] = NULL;
		error = execvp(argv[0],argv);
		if(error<0)
			perror("execv");
	}
	wait(NULL);
}

int main()
{
	printf("--------------------------------------------------\n");
	mysys("echo HELLO WORLD");	
	printf("--------------------------------------------------\n");
	mysys("ls /");
	printf("--------------------------------------------------\n");
	return 0;
}

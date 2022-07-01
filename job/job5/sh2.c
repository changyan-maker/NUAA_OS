#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>

void child(char *argv[]){
	int num=0;
	int i = 0;
	while(argv[i]){
		i++;
		num++;
	}

	char str1[80];
	char str2[80];
	if(argv[1]!=NULL && argv[1][0] == '<'){
		memcpy(str1, argv[1]+1,sizeof(argv[1]-1));
		int fd1 = open(str1,O_CREAT|O_RDWR,0666);
		dup2(fd1,0);		//input from fd1;
		close(fd1);
		argv[1] = NULL;		//for cat
	}
	if(argv[2]!= NULL && argv[2][0] ==  '>'){
		int fd2;
		if(argv[2][1]=='>'){		// >>
			memcpy(str2,argv[2]+2,sizeof(argv[2]-2));
			fd2 = open(str2,O_CREAT|O_RDWR|O_APPEND,0666);		
		}
		else{				// >	
			memcpy(str2,argv[2]+1,sizeof(argv[2]-1));
			fd2 = open(str2,O_CREAT|O_RDWR|O_TRUNC,0666);
		}
		dup2(fd2,1);	//output into fd2;
		close(fd2);
		argv[2] = NULL;		//for echo
	}

	int error = execvp(argv[0],argv);
	if(error<0)
		perror("execv");
}

int main()
{
	while(1){
		printf("%s","> ");
		char str[105];
		fgets(str,100,stdin);
		int i=0;
		while(str[i]!='\n')
			i++;
		str[i] = '\0';
		
		i=0;
		char a[5][20];
		int num = 0;
		int j=0;
		while(str[i]!='\0')
		{
			if(str[i]!=' '){
				a[num][j++] = str[i];
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

		if(strcmp(argv[0],"cd")==0){
			chdir(argv[1]);
		}
		else if(strcmp(argv[0],"exit")==0){
			exit(0);
		}
		else if(strcmp(argv[0],"pwd")==0){
			char work_dir[80];
			getcwd(work_dir,sizeof(work_dir));
			printf("%s\n",work_dir);
		}
		else{
			int pid;
			pid = fork();
			if(pid==0){
				child(argv);
			}
			wait(NULL);
		}
	}
	return 0;
}	

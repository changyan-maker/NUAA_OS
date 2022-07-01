#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<stdlib.h>

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
				error = execvp(argv[0],argv);
				if(error<0)
					perror("execv");
			}
			wait(NULL);
		}
	}
	return 0;
}	

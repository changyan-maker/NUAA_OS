#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

void myls(int argc, char *argv[]){
	DIR *dir;
	struct dirent *dirp;

	if(argc >= 2)
		dir = opendir(argv[1]);
	else
		dir = opendir(getcwd(NULL,0));

	if(dir == NULL){
		perror("opendir");
		exit(0);
	}
	while((dirp = readdir(dir))!=NULL){
		if(strcmp(dirp->d_name,".")==0 || strcmp(dirp->d_name,"..")==0)
			continue;
		printf("%s\n", dirp->d_name);
	}
	closedir(dir);
}

int main(int argc, char *argv[]){
	myls(argc,argv);
	return 0;
}

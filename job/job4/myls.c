#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
	DIR *dir;
	if(argc >= 2)
		dir = opendir(argv[1]);
	else
		dir = opendir(getcwd(NULL,0));
	while(1){
		struct dirent *de = readdir(dir);
		if(de == NULL)	break;
		if(strcmp(de->d_name,"..")!=0 && strcmp(de->d_name,".")!=0){
					
			printf("%s\n",de->d_name);

		}
	}
	closedir(dir);
	return 0;
}

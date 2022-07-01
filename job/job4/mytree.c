#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

void mytree(int k_num, char *work_dir){
	char k[80] = "";
	for(int i=0;i<k_num;i++)
		strcat(k," ");
	DIR *dir = opendir(work_dir);
	if(dir == NULL){
		perror("opendir");
		exit(0);
	}
	while(1){
		struct dirent *de = readdir(dir);
		if(de == NULL)	break;
		if(strcmp(de->d_name,"..")!=0 && strcmp(de->d_name,".")!=0){
			printf("%s",k);
			printf("%s\n", de->d_name);
			if(de->d_type == DT_DIR){
				strcat(work_dir,"/");
				strcat(work_dir,de->d_name);
				mytree(k_num+1,work_dir);
			}
		}
	}
	closedir(dir);
}


int main(int argc, char *argv[]){
	if(argc >= 2){	
		printf("%s\n",argv[1]);
		mytree(1, argv[1]);
	}
	else{
		printf("%s\n",getcwd(NULL,0));
		mytree(1, getcwd(NULL,0));
	}
	return 0;
}

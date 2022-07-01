#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>

#define WORKER_NUMBER 4
#define CAPACITY 4

typedef struct{
	int is_end;
	char path[128];
	char string[128];
}task;

task buffer[CAPACITY];
int in;
int out;


int buffer_is_empty(){
	return in==out;
}

int buffer_is_full(){
	return (in+1)%CAPACITY == out;
}

pthread_mutex_t mutex;
pthread_cond_t wait_empty_buffer;
pthread_cond_t wait_full_buffer;

task get_task()
{
	task item;
//	task *buffer;
//	buffer = &buffer[out];
	pthread_mutex_lock(&mutex);
	while(buffer_is_empty())
		pthread_cond_wait(&wait_full_buffer, &mutex);

	item.is_end = buffer[out].is_end;
	strcpy(item.path,buffer[out].path);
	strcpy(item.string, buffer[out].string);
	out = (out + 1) % CAPACITY;

	pthread_cond_signal(&wait_empty_buffer);
	pthread_mutex_unlock(&mutex);
	
	return item;
}

void put_task(task *item){
//	task *buffer;
//	buffer = &buffer[in];
	pthread_mutex_lock(&mutex);
	while(buffer_is_full())
		pthread_cond_wait(&wait_empty_buffer, &mutex);

	buffer[in].is_end = item->is_end;
	strcpy(buffer[in].path, item->path);
	strcpy(buffer[in].string, item->string);
	in = (in + 1) % CAPACITY;

	pthread_cond_signal(&wait_full_buffer);
	pthread_mutex_unlock(&mutex);
}

void find_file(char *path, char *target){
	FILE *file = fopen(path, "r");

	char line[256];
	while(fgets(line, sizeof(line), file)){
		if(strstr(line, target))
			printf("%s: %s",path, line);
	}
	fclose(file);
}

void *worker_entry(){
	while(1){
		task t;
		t = get_task();
		if(t.is_end)
			break;
		find_file(t.path, t.string);
	}
	return NULL;
}
void find_dir(char *path, char *target){
	DIR *dir = opendir(path);
	struct dirent *entry;
	while(entry = readdir(dir)){
		char name[256];
		strcpy(name, path);
		if(strcmp(entry->d_name, ".") == 0)
			continue;
		if(strcmp(entry->d_name, "..") == 0)
			continue;
		if(entry->d_type == DT_DIR){
			strcat(name,"/");
			strcat(name,entry->d_name);
		//	printf("%s\n", name);
			find_dir(name, target);
		}
		if(entry->d_type == DT_REG){
			strcat(name,"/");
			strcat(name,entry->d_name);
			task item;
			item.is_end = 0;
			strcpy(item.path, name);
			strcpy(item.string, target);
			put_task(&item);
	//		printf("111%s\n",name);
				
		//	find_file(name, target);
		//	printf("file  %s\n", entry->d_name);
		}
	}
	closedir(dir);
}

int main(int argc, char *argv[]){
	if(argc != 3){
		puts("Usage: sfind file string");
		return 0;
	}


	char *path = argv[1];
	char *string = argv[2];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&wait_empty_buffer, NULL);
	pthread_cond_init(&wait_full_buffer, NULL);


	struct stat info;
	stat(path, &info);

	pthread_t workers[WORKER_NUMBER];
	for(int i=0;i<WORKER_NUMBER;i++){
		pthread_create(&workers[i], NULL, worker_entry, NULL);
	}


	if(S_ISDIR(info.st_mode)){
		find_dir(path, string);
		for(int i=0;i<WORKER_NUMBER;i++){
			task item;
			item.is_end = 1;
			put_task(&item);
		}
	}
	else
		find_file(path, string);
	for(int i=0;i<WORKER_NUMBER;i++){
		pthread_join(workers[i], NULL);
	}
	return 0;
}

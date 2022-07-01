#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>

enum Type{
	TREE_BACK,
	TREE_PIPE,
	TREE_REDIRICT,
	TREE_BASIC,
	TREE_TOKEN,
};

#define MAX_CHILD 10
struct tree_t{
	int type;
	char *token;
	int child_count;
	struct tree_t *child_vector[MAX_CHILD];
};

int split_word(char *line, char *separator, char *word_array[]){
	int word_count = 0;
	char *word;
	word = strtok(line, separator);
	while(word != NULL){
		word_array[word_count] = word;
		word_count++;
		word = strtok(NULL, separator);
	}
	return word_count;
}


//void parse_command(char *line, struct Command *command){
void parse_command(char *line, struct tree_t *tree){
	int wordc;
	char *wordv[10];
	wordc = split_word(line, " ", wordv);
	struct tree_t *trees[15];
	tree->type = TREE_BASIC;
	tree->child_count = wordc;
	for(int i=0;i<wordc;i++){
		char *word = wordv[i];
		trees[i]->type = TREE_TOKEN;
		trees[i]->token = word;
		if(word[0] == '&')
			tree->type = TREE_BACK;
		else if(word[0] == '|')
			tree->type = TREE_PIPE;
		else if(word[0] == '>' || word[0] == '<')
			tree->type = TREE_REDIRICT;
		tree->child_vector[i] = trees[i];
	}
}
	//	for(int i=0;i<wordc;i++){
	//	char *word = wordv[i];
	//	
	//	if(word[0] == '>'){
	//		if(word[1] == '>'){		// >>
	//			command->append = word + 2;
	//		}
	//		else{				// >
	//			command->output = word + 1;
	//		}
	//	}
	//	if(word[0] == '<'){	// <
	//		command->input = word + 1;
	//	}
//	}
//	command->argc = wordc;
//	for(int i=0;i<wordc;i++)
//		command->argv[i] = wordv[i];
//	command->argv[wordc] = NULL;
//}

//void child(struct Command *command){
//			int pid = fork();
//			if(pid == 0){
//				if(command->input != NULL){
//					int fd = open(command->input,O_CREAT|O_RDWR,0666);
//					dup2(fd,0);
//					close(fd);
//					command->argv[1] = NULL;
//				}
//				if(command->output!=NULL){
//					int fd = open(command->output,O_CREAT|O_RDWR|O_TRUNC,0666);
//					dup2(fd,1);
//					close(fd);
//					command->argv[2] = NULL;
//				}
//				if(command->append != NULL){
//					int fd = open(command->append,O_CREAT|O_RDWR|O_APPEND,0666);
//					dup2(fd,1);
//					close(fd);
//					command->argv[2] = NULL;
//				}
//				int error = execvp(command->argv[0],command->argv);
//				if(error<0)
//					perror("execv");
//			}
//			wait(NULL);
	
//}

int main()
{
	while(1){
		printf("%s", "> ");
		char line[105];
		fgets(line,100,stdin);
		int i = 0;
		while(line[i]!='\n')
			i++;
		line[i] = '\0';
		struct tree_t tree;
		parse_command(line, &tree);
		printf("%d\n",tree.type);
		//	struct Command command;
	//	command.input = NULL;
	//	command.output = NULL;
	//	command.append = NULL;
//		struct tree_t tree;
//	        parse_command(line, &tree);	
//		if(strcmp(command.argv[0], "cd") == 0){
//			chdir(command.argv[1]);
//		}
//		else if(strcmp(command.argv[0], "exit") == 0){
//			exit(0);
//		}
//		else if(strcmp(command.argv[0], "pwd") == 0){
////			char work_dir[80];
//			getcwd(work_dir,sizeof(work_dir));
//			printf("%s\n",work_dir);
//		}
//		else{
//			child(&command);
//		}
	}
	return 0;
}

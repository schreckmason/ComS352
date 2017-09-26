//##################################################################//
//##################################################################//
//								INCLUDES							//
//##################################################################// 
//##################################################################//
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//##################################################################//
//##################################################################// 
//							DEFINE / GLOBALS						//
//##################################################################//
//##################################################################//
#define MAX_LINE 80 /*max length input buffer*/


//##################################################################//
//##################################################################//
//							PROTOTYPES								//
//##################################################################//
//##################################################################//
char *get_input(void);
char **parse_input(char *line);
int str_find(char *container, char to_find, size_t size);

//##################################################################//
//##################################################################//
//								MAIN								//
//##################################################################//
//##################################################################//
int main(void)
{
	char *args[MAX_LINE/2+1]; /* cli arguments */
	int should_run = 1; /*flag to determin when to exit program */
	int should_wait = 0; /*flag to detmine when to have the parent process wait for the child process*/
	char *exit = "exit";
	char **tokenized_line;
	while(should_run){
		printf("osh>");
		fflush(stdout);
		/* Obtain the line of input from keyboard*/
		char *line = NULL;
		line = get_input();
		//printf("typed %s\n", line);/*for debugging*/
		
		/*Sanitize input of newline character for string comparison*/
		line[ strcspn(line, "\r\n") ] = 0;	
		int comparison = strcmp(line,exit);
		if(comparison == 0){
			should_run=0;
		}else{
			should_run=1;
		}
		
		/* Parse the input from keyboard into tokens then 'save' to args array */
		tokenized_line = parse_input(line);
		for(int i=0;i<(MAX_LINE/2+1);i++){
			if(tokenized_line[i] != NULL){
//				printf("i:%d = %s\n",i,args[i]);/*for debugging */
				if(strcmp(tokenized_line[i], "&")==0){
					should_wait = 1;
					continue;
				}
				args[i] = tokenized_line[i];
			}else{
				//continue;
				break;
			}
		}
/*FORK CHILD PROCESS*/
		pid_t pid = fork();
		if(pid==0){
			printf("entered child process\n");/*for debugging*/
			execvp(args[0], args);
			//need to flush the output here so that way we won't get what looks like a hung terminal
			//printf("child finished\n");/*for debugging*/
		}else{
			if(should_wait){
				wait(NULL);
			}
			printf("parent finished\n");/*for debugging*/
		}
		//TODO: clear out args here
	}
	return 0;
}

char *get_input(void){
	char *line = NULL;
	size_t bufsize = 32;
	line = malloc(sizeof(char) * MAX_LINE);
	getline(&line, &bufsize, stdin);
	return line;	
}

char **parse_input(char *line){
	char * whitespace = " \t\r\n\a";
	int bufsize = MAX_LINE;
	int position = 0;	
	char **arguments = malloc(sizeof(char *) * bufsize);
	char *string_token;
	
	string_token = strtok(line, whitespace);
	while(string_token != NULL){
		arguments[position] = string_token; 
		position++;

		if(position>=bufsize){
			bufsize+=MAX_LINE;
			arguments = realloc(arguments, bufsize*sizeof(char *));
		}
		
		string_token = strtok(NULL, whitespace);
	}	
	arguments[position] = NULL;
	return arguments;
}

int str_find(char *container, char to_find, size_t size){
	for(size_t i=0;i<size;i++){
		if(container[i] == to_find){
			return 0;
		}
	}
	return 1;
}

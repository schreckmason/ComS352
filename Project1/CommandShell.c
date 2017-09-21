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

//##################################################################//
//##################################################################//
//								MAIN								//
//##################################################################//
//##################################################################//
int main(void)
{
//	char *args[MAX_LINE/2+1]; /* cli arguments */
	int should_run = 1; /*flag to determin when to exit program */
	while(should_run){
		printf("osh>");
		
		char *input = NULL;
		input = get_input();
		printf("typed %s\n", input);

		/*FORK CHILD PROCESS*/
		int pid = fork();
		if(pid==0){
			//the child will execute second unless wait is specified
			//printf("hello world\n");
			
		}else{
			//Unless wait is specified this will happen first
			//wait(NULL); /*toggled if '&' is found in args*/
			printf("fuggit\n");
		}
		fflush(stdout);
		should_run=0;/*currently for testing, only set when 'exit' is entered*/

		/**
		* After reading user input, the steps are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke execvp()
		* (3) if comamnd included &, parent will invoke wait()
		*/
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




#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 /*max length input buffer*/

int main(void)
{
	char *args[MAX_LINE/2+1]; /* cli arguments */
	int should_run = 1; /*flag to determin when to exit program */
	while(should_run){
		printf("osh>");
		fflush(stdout);

		/**
		* After reading user input, the steps are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke execvp()
		* (3) if comamnd included &, parent will invoke wait()
		*/
	}
	return 0;
}

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
#include <ctype.h>

//##################################################################//
//##################################################################// 
//							DEFINE / GLOBALS						//
//##################################################################//
//##################################################################//
#define MAX_LINE 80 /*max length input buffer*/


//##################################################################//
//##################################################################//
//					QUEUE FUNCTIONALITY								//
//##################################################################//
//##################################################################//
struct QNode {
	int id;
	char *key;
	struct QNode *next;
	struct QNode *prev;
};

struct Queue {
	struct QNode *front, *rear;
	int size;
};

struct Queue *createQueue(){
	struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
	q->front = q->rear = NULL;
	q->size = 0;
	return q;
};

struct QNode *newNode(int i, char *k)
{
	struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
	temp->id = i;
	temp->key = k;
	temp->prev = NULL;
	temp->next = NULL;
	return temp;
};

void enqueue(struct Queue *q, int i, char *k){
	struct QNode *temp= newNode(i, k);
	if(q->rear == NULL){
		//now the queue contains one node which is both front and rear
		q->front = q->rear = temp;
		return;
	}
	q->rear->next = temp; /*update rear->next reference to new node*/
	temp->prev = q->rear; /*set temp previous reference to current rear*/
	q->rear = temp; /*set teh new end of the queue to the new node*/
	q->size++;
}

struct QNode *dequeue(struct Queue *q){
	if(q->front == NULL){
		return NULL;
	}
	struct QNode *temp = q->front;
	q->front = q->front->next;
	if(q->front==NULL){
		q->rear = NULL;
	}
	q->size--;
	return temp;
	
}

struct QNode *pop(struct Queue *q){
	if(q->rear == NULL){
		return NULL;
	}
	struct QNode *temp = q->rear;
	q->rear = q->rear->prev;
	if(q->rear == NULL){
		q->front = NULL;
	}
	q->size--;
	return temp;
}
struct QNode *peek(struct Queue *q){
	if(q->rear == NULL){
		return NULL;
	}
	struct QNode *temp = q->rear;
	return temp;
}
//##################################################################//
//##################################################################//
//							PROTOTYPES								//
//##################################################################//
//##################################################################//
char *get_input(void);
char **parse_input(char *line);
char **history_parse(char *line);
void empty_args(char **arguments);

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
	int run_count = 0;
	char *exit_string = "exit";
	char **tokenized_line;
	char **tokenized_history;
	int status;
	struct Queue *args_q = createQueue();

	while(should_run){
		printf("osh>");
		fflush(stdout);
		/* Obtain the line of input from keyboard*/
		char *line = NULL;
		line = get_input();
		
		/*Sanitize input of newline character for string comparison*/
		line[ strcspn(line, "\r\n") ] = 0;

		int exit_comparison = strcmp(line,exit_string);
		if(exit_comparison == 0){
			should_run=0;
			exit(0);
		}else{
			should_run=1;
		}


		int hist_comparison = strcmp(line, "history");
		struct Queue *history_q = createQueue();		
		if(hist_comparison == 0){
			for(int i=0;i<10;i++){
				struct QNode *n = pop(args_q);
				if(n!=NULL){
					printf("%d -- %s\n", n->id, n->key);
					enqueue(history_q, n->id, n->key);
				}
			}
			for(int i=0;i<10;i++){
				struct QNode *h = pop(history_q);
				if(h!=NULL){
					enqueue(args_q, h->id, h->key);
				}
			}
		}else{
			enqueue(args_q, run_count, line);/*Store the command in the argument queue*/
		}

		pid_t pid = fork();
		if(pid==0){
			if(should_run==0){ exit(0); }
				
			char *placeholder = line, *p = placeholder;
			int id = -1;
			while(*p){
				if(isdigit(*p)){
					 id = strtol(p, &p, 10);
				}else{
					p++;
				}
			}

			if(id != -1){
				//note i was getting bugs using args_q->size
				for(int i=0;i<10;i++){
					struct QNode *n = pop(args_q);
					if(id == n->id){
						//found, parse the cmd
						tokenized_line = parse_input(n->key);
						for(int j=0;j<(MAX_LINE/2+1);j++){
							if(tokenized_line[j]!=NULL){
								args[j] = tokenized_line[j];
							}else{
								break;
							}
						}
						execvp(args[0],args);
					}else{
						continue;
					}
				}
			}

			if(strcmp(line,"!!")==0){
				struct QNode *latest = pop(args_q);
				latest=pop(args_q);
				if(latest!=NULL){
					tokenized_line = parse_input(latest->key);
					for(int i=0;i<(MAX_LINE/2+1); i++){
						if(tokenized_line[i]!=NULL){
							args[i] = tokenized_line[i];
						}else{
							break;
						}
					}
					execvp(args[0], args);
				}else{
					printf("no command in history\n");
				}
			}
			if(strcmp(line,"history")!=0){
				//If not history do the normal operation and tokenize the line using regular spaces as well other whitespace delimiters
				tokenized_line = parse_input(line);
				for(int i=0;i<(MAX_LINE/2+1);i++){
					if(tokenized_line[i] != NULL){
						if(strcmp(tokenized_line[i], "&")==0){
							should_wait = 1;
							continue;
						}
						args[i] = tokenized_line[i];
					}else{
						break;
					}
				}
				execvp(args[0], args);
			}else if(strcmp(line, "history")==0){
				//If history no execution of a command takes place, only parsing the input using a separate set of delimiters
				tokenized_history = history_parse(line);
				for(int i=0;i<(MAX_LINE/2+1);i++){
					if(tokenized_history[i]!=NULL){
						args[i] = tokenized_history[i];
					}else{
						break;
					}
				}
			}
		}else{
			if(should_wait){
				wait(NULL);
			}else{
				waitpid(pid, &status, WUNTRACED);
			}
		}
		//CLEAR OUT ARGS SO MULTIPLE 'ENTER' KEYS WON'T REPEAT PREVIOUS COMMAND
		empty_args(args);
		run_count++;
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

char **history_parse(char *line){
	char *whitespace = "\r\n\a";
	int bufsize = MAX_LINE;
	int position = 0;
	char **arguments = malloc(sizeof(char *) * bufsize);
	char *string_token;
	string_token = strtok(line, whitespace);
	while(string_token != NULL){
		arguments[position] = string_token;
		position++;
		
		if(position>=bufsize){
			bufsize+= MAX_LINE;
			arguments = realloc(arguments, bufsize * sizeof(char *));
		}
		string_token = strtok(NULL, whitespace);
	}
	arguments[position] = NULL;
	return arguments;
}

void empty_args(char **arguments){
	for(int i=0;i<(MAX_LINE/2+1);i++){
		arguments[i]= NULL;
	}
}

//#include <stdio.h>
//#include <stdlib.h>

struct QNode
{
	int id;
	int size;
	char *key;
	struct QNode *next;
	struct QNode *prev;

};

struct Queue
{
	struct QNode *front, *rear;
};

//struct Queue *createQueue()
struct Queue *createQueue()
{
	struct Queue * q = (struct Queue*)malloc(sizeof(struct Queue));
	q->front = q->rear = NULL;
	return q;
};

struct QNode *newNode(int i, char *k){
	struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
	temp->id = i;
	temp->key = k;
	temp->prev = NULL;
	temp->next = NULL;
	return temp;
};

void enQueue(struct Queue *q, int i, char *k){
	struct QNode = newNode(i, k);
	if(q->rear == NULL){
		//now the queue contains one node which is both the front and the rear
		q->front = q->rear = temp;
		return;
	}

	//If the queue is not empty
	q->rear->next = temp; /*update rear->next reference to the new node*/
	temp->prev = q->rear; /*set temp previous reference to current rear */
	q->rear = temp; /*set the new end of the queue tot he new node */
}

struct QNode *dequeue(struct Queue *q){
	if(q->front == NULL){
		return NULL;
	}
	struct QNode *temp = q->front;
	q->front = q->front->next;
	
	if(q->front == NULL){
		q->rear = NULL;
	}
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
	return temp;
}

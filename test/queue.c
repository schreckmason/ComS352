#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct QNode{
	int id; char *key;
	struct QNode *next;
	struct QNode *prev;
};

struct Queue{
	struct QNode *front, *rear;
};

// A utility function to create an empty queue
struct Queue *createQueue()
{
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

struct QNode *newNode(int i, char* k){
	struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
	temp->id = i;
    temp->key = k;
    temp->next = NULL;
	temp->prev = NULL;
    return temp;
};

// The function to add a key k to q
void enQueue(struct Queue *q, int i, char *k)
{
    // Create a new LL node
    struct QNode *temp = newNode(i,k);
 
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
       q->front = q->rear = temp;
       return;
    }
 
    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
	temp->prev = q->rear;
    q->rear = temp;

}
 
// Function to remove a key from given queue q
struct QNode *deQueue(struct Queue *q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
       return NULL;
 
    // Store previous front and move front one node ahead
    struct QNode *temp = q->front;
    q->front = q->front->next;
 
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
       q->rear = NULL;
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

int main()
{
    struct Queue *q = createQueue();
    enQueue(q,1, "hello");
    enQueue(q,2, "ls -al");
    //struct QNode *n = deQueue(q);
	struct QNode *n = pop(q);
    if (n != NULL)
      printf("Dequeued item is, %d --  %s\n",n->id, n->key);
	struct QNode *n2 = pop(q);
	printf("popped %d -- %s\n", n2->id, n2->key);
	struct QNode *n3 = pop(q);
	if(n3!=NULL){
		printf("popped %d -- %s\n", n3->id, n3->key);
	}
    return 0;
}

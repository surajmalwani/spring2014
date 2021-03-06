#include<stdio.h>
#include<stdlib.h>
#include<gtthread_types.h>
#include<gtthread_queue.h>


/**
 * Initialize the Queue
 */
void initQueue(gtthread_queue* queue)
{
    queue->front = queue->rear = NULL;
}

/**
 * Add Thread to Queue
 */
void addThreadToQueue(gtthread_queue* queue, gtthread_t* thread)
{
    gtthread_node *temp;

    temp = (gtthread_node*)malloc(sizeof(gtthread_node)); 
    temp->thread = thread;
    temp->link = NULL;

    if(queue->front == NULL)
    {
        queue->rear = queue->front = temp;
        return;
    }

    queue->rear->link = temp;
    queue->rear = queue->rear->link;
}

/**
 * Remove Thread from front of Queue
 */
gtthread_t* removeThreadFromQueue(gtthread_queue* queue)
{
    gtthread_node *temp;
    gtthread_t* thread;    

    if(queue->front == NULL)
    {
      printf("\nQueue is empty.");
      return NULL;
    }

    thread = queue->front->thread;
    temp = queue->front;

    queue->front = queue->front->link;
    
    free(temp);

    return thread;
}


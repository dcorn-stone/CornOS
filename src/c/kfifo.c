#include <kfifo.h>

#define QUEUE_SIZE      10


/*  queue:
 *  
 *
 */
typedef struct {
        int items[QUEUE_SIZE];
        int front;
        int rear;
} queue;

void init_queue(queue *q)
{
        q->front = -1;
        q->rear = 0;
}

int isEmpty(queue *q)
{
        return q->front == -1;
}

int isFull(queue *q)
{
        return (q->rear + 1) % QUEUE_SIZE == q->front;
}

int enqueue(queue *q, int value)
{
        if(isFull(q)){

                return 0;

        } else {

                if (q->front == -1)
                        q->front = 0;

                q->rear = (q->rear + 1) % QUEUE_SIZE;
                q->items[q->rear] = value;

                return 1;
        }
}

void dequeue(queue *q)
{
        if(isEmpty(q))
                return;
        
        q->front = (q->front + 1) % QUEUE_SIZE;
}





#include "kfifo.h"

#define QUEUE_SIZE      10

/* Struct for 8bit element queue */
typedef struct {
        uint8_t items[QUEUE_SIZE];
        int front;
        int rear;
} queue_t;


/* Queue manipulation functions */
void init_queue(queue_t *q)
{
        q->front = 0;
        q->rear = 0;
}

int isEmpty(queue_t *q)
{
        return q->front == q->rear;
}

int isFull(queue_t *q)
{
        return (q->rear + 1) % QUEUE_SIZE == q->front;
}

int enqueue(queue_t *q, int value)
{
        if(isFull(q)){

                return 0;

        } else {

                q->items[q->rear] = value;
                q->rear = (q->rear + 1) % QUEUE_SIZE;

                return 1;
        }
}

void dequeue(queue_t *q)
{
        if(isEmpty(q))
                return;
        
        q->front = (q->front + 1) % QUEUE_SIZE;
}





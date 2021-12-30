#ifndef GALACTIC_EMPIRE_PI_QUEUE_H
#define GALACTIC_EMPIRE_PI_QUEUE_H

typedef struct {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;

/**
 * Queue is full when size becomes equal to the capacity.
 */
int isFull(Queue* queue);

/**
 * Queue is empty when size is 0.
 */
int isEmpty(Queue* queue);

/**
 * Adds an item to the queue, it changes rear and size.
 */
void enqueue(Queue* queue, int item);

/**
 * Removes item from queue, changes front and size.
 */
int dequeue(Queue* queue);

/**
 * Gets front of queue.
 */
int front(Queue* queue);

/**
 * Gets rear of queue.
 */
int rear(Queue* queue);

#endif
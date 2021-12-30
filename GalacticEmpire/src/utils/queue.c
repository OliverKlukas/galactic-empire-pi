#include <limits.h>
#include "queue.h"


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/


/**
 * Queue is full when size becomes equal to the capacity.
 *
 * <p>Copied from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
 *
 * @param queue
 * @return
 */
int isFull(Queue *queue) {
    return (queue->size == queue->capacity);
}

/**
 * Queue is empty when size is 0.
 *
 * <p>Copied from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
 *
 * @param queue
 * @return
 */
int isEmpty(Queue *queue) {
    return (queue->size == 0);
}

/**
 * Adds an item to the queue, it changes rear and size.
 *
 * <p>Copied from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
 *
 * @param queue
 * @param item
 */
void enqueue(Queue *queue, int item) {
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

/**
 * Removes item from queue, changes front and size.
 *
 * <p>Copied from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
 *
 * @param queue
 * @return
 */
int dequeue(Queue *queue) {
    int item;
    if (isEmpty(queue))
        return INT_MIN;
    item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

/**
 * Gets front of queue.
 *
 * <p>Copied from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
 *
 * @param queue
 * @return
 */
int front(Queue *queue) {
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

/**
 * Gets rear of queue.
 *
 * <p>Copied from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
 *
 * @param queue
 * @return
 */
int rear(Queue *queue) {
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

typedef struct {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;
 
// struct Queue* createQueue(unsigned capacity);

int isFull(Queue* queue);

int isEmpty(Queue* queue);

void enqueue(Queue* queue, int item);

int dequeue(Queue* queue);

int front(Queue* queue);

int rear(Queue* queue);
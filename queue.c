#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "queue.h"


typedef struct node {
    void* data;
    long index;
    struct node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    atomic_size_t items;
    atomic_ulong nextOperation;
    atomic_ulong totalOperations;
    atomic_ulong visited_items;
    mtx_t queue_lock;
    cnd_t cond;
} Queue;

Queue queue;

void initQueue(void) {
    queue.items = 0;
    queue.head = NULL;
    queue.tail = NULL;
    mtx_init(&queue.queue_lock, mtx_plain);
    cnd_init(&queue.cond);
    queue.totalOperations = 0;
    queue.nextOperation = 0;
    queue.nextOperation = 0;
    queue.visited_items = 0;

}

void destroyQueue(void) {
    mtx_lock(&queue.queue_lock);
    while (queue.head != NULL) {
        Node* temp = queue.head;
        queue.head = queue.head->next;
        free(temp);
    }
    mtx_unlock(&queue.queue_lock);
    mtx_destroy(&queue.queue_lock);
    cnd_destroy(&queue.cond);
}

void enqueue(void* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    mtx_lock(&queue.queue_lock);

    if (queue.tail == NULL) {
        queue.head = queue.tail = new_node;
    } else {
        queue.tail->next = new_node;
        queue.tail = new_node;
    }
    queue.items++;
    cnd_broadcast(&queue.cond); 
    mtx_unlock(&queue.queue_lock);
}

void* dequeue(void) {
    long myTurn ;
    mtx_lock(&queue.queue_lock);
    myTurn = queue.totalOperations++;
    while (queue.items == 0 || myTurn != queue.nextOperation ) {
        cnd_wait(&queue.cond, &queue.queue_lock);
    }
    Node* to_remove = queue.head;
    void* data = to_remove->data;
    queue.head = queue.head->next;
    if (queue.head == NULL) {
        queue.tail = NULL;
    }
    free(to_remove);
    queue.items--;
    queue.nextOperation++;
    queue.visited_items++;
    cnd_broadcast(&queue.cond); 
    mtx_unlock(&queue.queue_lock);
    return data;
}

bool tryDequeue(void** data) {
    mtx_lock(&queue.queue_lock);
    if (queue.items == 0) {
        mtx_unlock(&queue.queue_lock);
        return false;
    }
    Node* to_remove = queue.head;
    *data = to_remove->data;
    queue.head = queue.head->next;
    if (queue.head == NULL) {
        queue.tail = NULL;
    }
    free(to_remove);
    queue.items--;
    queue.visited_items++;
    cnd_broadcast(&queue.cond); 
    mtx_unlock(&queue.queue_lock);    
    return true;
}

size_t size(void) {
    mtx_lock(&queue.queue_lock);
    size_t current_size = queue.items;
    mtx_unlock(&queue.queue_lock);
    return current_size;
}


size_t waiting(void) {
    return queue.totalOperations - queue.nextOperation;
}

size_t visited(void) {
    return queue.visited_items;
}

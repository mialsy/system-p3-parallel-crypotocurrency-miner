/**
 * @file queue.c
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "logger.h"

struct qnode
{
    uint64_t data;
    struct qnode *next;
};

struct queue
{
    struct qnode *head, *tail;
    size_t size;
};

int queue_size(struct queue *q) {
    return q->size;
}

struct queue *queue_init()
{
    struct queue *q = malloc(sizeof(struct queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

int queue_offer(struct queue *q, uint64_t data)
{
    struct qnode *new_node = malloc(sizeof(struct qnode));
    if (new_node == NULL)
    {

        free(new_node);
        return -1;
    }

    new_node->data = data;

    if (q->head == NULL)
    {
        q->head = new_node;
        q->tail = new_node;
        q->size = 1;
    }
    else
    {
        q->tail->next = new_node;
        q->tail = new_node;
        q->size += 1;
    }
    return 0;
}

uint64_t queue_poll(struct queue *q)
{
    if (q->size == 0)
    {
        return -1;
    }

    struct qnode *old_head = q->head;
    int val = old_head->data;
    q->head = q->head->next;

    q->size -= 1;

    if (q->size == 0)
    {
        q->head = NULL;
        q->tail = NULL;
    }
    free(old_head);
    return val;
}

void queue_print(struct queue *q)
{
    struct qnode *current = q->head;
    while (current != NULL)
    {
        printf("%lu -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void queue_destory(struct queue *q)
{
    while (queue_size(q) > 0) {
        queue_poll(q);
    }
    free(q);
}



// void main()
// {
//     struct queue* q1 = queue_init();

//     queue_offer(q1, 1);
//     queue_offer(q1, 2);
//     queue_offer(q1, 3);
//     queue_offer(q1, 4);
//     queue_offer(q1, 5);

//     int count = queue_size(q1);
//     printf("size: %d\n", count);

//     while (queue_size(q1) > 0)
//     {
//         int num = queue_poll(q1);
//         printf("poll: %d\n", num);
//     }
    
//     queue_print(q1);
//     queue_destory(q1);
// }
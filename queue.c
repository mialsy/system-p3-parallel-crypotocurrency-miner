/**
 * @file queue.c
 * 
 * @brief a linkedlist implmentation of queue
 * 
 * Queue is a first in first out structure.
 * In this implemenetaion, only uint64_t data can be offered to the queue,
 * the data is offered to the tail every time,
 * and can be retrived from the head of the linked list.
 * 
 * Note that as this queue stores the starting nonce of the mining range,
 * all data is supposed to be positive uint64_t,
 * thus zero value would be returned to indicate polling failure. 
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "logger.h"

/**
 * @brief linked list node for the queue
 * 
 */
struct qnode
{
    uint64_t data; /**< the data stored in the queue */
    struct qnode *next; /**< the next node in the queue */
}; 

struct queue
{
    struct qnode *head, *tail; /**< the head and tail nodes of the queue */
    size_t size; /**< the size of the queue */
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
        // malloc failed
        free(new_node);
        return -1;
    }

    new_node->data = data;

    if (q->head == NULL)
    {
        // handle cases that queue is empty
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
        // cannot poll, return zero as data should be positive 
        return 0;
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
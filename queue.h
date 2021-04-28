#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <sys/types.h>

struct queue;

struct queue *queue_init();
int queue_offer(struct queue *q, int data);
int queue_poll(struct queue *q);
void queue_print(struct queue *q);
void queue_destory(struct queue *q);
int queue_size(struct queue *q);

#endif
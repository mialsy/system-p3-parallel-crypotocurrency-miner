
/**
 * @file queue.h
 * 
 * @brief the header file for queue.c
 *
 **/
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdint.h>

/**
 * @brief a struct of the queue
 * 
 */
struct queue;

/**
 * @brief initialize the queue structure
 * 
 * @return struct queue* the pointer to the initialized queue structure
 */
struct queue *queue_init();

/**
 * @brief offer an element to the input queue
 * 
 * @param q the queue 
 * @param data the data to of the element to be added to the queue
 * @return int zero on success, non-zero on failure
 */
int queue_offer(struct queue *q, int data);

/**
 * @brief poll one element from the queue
 * 
 * @param q the queue to be poll from
 * @return uint64_t of the data of the element polled from the queue
 */
uint64_t queue_poll(struct queue *q);

/**
 * @brief print the queue from head to tail
 * 
 * @param q the queue to print
 */
void queue_print(struct queue *q);

/**
 * @brief clean up the space allocated to the input queue
 * 
 * @param q the pointer to the input queue
 */
void queue_destory(struct queue *q);

/**
 * @brief get the size of the input queue
 * 
 * @param q the input queue
 * @return int size of the queue
 */
int queue_size(struct queue *q);

#endif
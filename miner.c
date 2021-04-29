/**
 * @file mine.c
 *
 * Parallelizes the hash inversion technique used by cryptocurrencies such as
 * bitcoin.
 *
 * Input:    Number of threads, block difficulty, and block contents (string)
 * Output:   Hash inversion solution (nonce) and timing statistics.
 *
 * Compile:  (run make)
 *           When your code is ready for performance testing, you can add the
 *           -O3 flag to enable all compiler optimizations.
 *
 * Run:      ./miner 4 24 'Hello CS 521!!!'
 *
 *   Number of threads: 4
 *     Difficulty Mask: 00000000000000000000000011111111
 *          Block data: [Hello CS 521!!!]
 *
 *   ----------- Starting up miner threads!  -----------
 *
 *   Solution found by thread 3:
 *   Nonce: 10211906
 *   Hash: 0000001209850F7AB3EC055248EE4F1B032D39D0
 *   10221196 hashes in 0.26s (39312292.30 hashes/sec)
 */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "logger.h"
#include "queue.h"
#include "sha1.h"

#define RANGE 10000

unsigned long long total_inversions;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condc = PTHREAD_COND_INITIALIZER;
pthread_cond_t condp = PTHREAD_COND_INITIALIZER;

struct queue *task_queue;
uint64_t found_nounce = 0;
long found_rank = -1;
uint8_t digest[SHA1_HASH_SIZE];
char *bitcoin_block_data = "";
uint32_t difficulty_mask = 0;

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void print_binary32(uint32_t num) {
    int i;
    for (i = 31; i >= 0; --i) {
        uint32_t position = (unsigned int) 1 << i;
        printf("%c", ((num & position) == position) ? '1' : '0');
    }
    puts("");
}

uint64_t mine(char *data_block, uint32_t difficulty_mask,
        uint64_t nonce_start, uint64_t nonce_end,
        uint8_t digest[SHA1_HASH_SIZE]) {

    for (uint64_t nonce = nonce_start; nonce < nonce_end; nonce++) {
        /* A 64-bit unsigned number can be up to 20 characters  when printed: */
        size_t buf_sz = sizeof(char) * (strlen(data_block) + 20 + 1);
        char *buf = malloc(buf_sz);

        /* Create a new string by concatenating the block and nonce string.
         * For example, if we have 'Hello World!' and '10', the new string
         * is: 'Hello World!10' */
        snprintf(buf, buf_sz, "%s%lu", data_block, nonce);

        /* Hash the combined string */
        sha1sum(digest, (uint8_t *) buf, strlen(buf));
        free(buf);
        total_inversions++;

        /* Get the first 32 bits of the hash */
        uint32_t hash_front = 0;
        hash_front |= digest[0] << 24;
        hash_front |= digest[1] << 16;
        hash_front |= digest[2] << 8;
        hash_front |= digest[3];

        /* Check to see if we've found a solution to our block */
        if ((hash_front & difficulty_mask) == hash_front) {
            return nonce;
        }
    }

    return 0;
}

void *worker_thread(void *ptr)
{
    long rank = (long)ptr;
    uint64_t start = 0;

    while (true)
    {
        pthread_mutex_lock(&mutex);
        if (found_nounce != 0) {
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&condp);
            break;
        }

        while (queue_size(task_queue) == 0)
        {
            pthread_cond_wait(&condc, &mutex);
        }
        start = queue_poll(task_queue);

        pthread_mutex_unlock(&mutex);

        /* Mine the block. */
        uint64_t end = start + RANGE - 1;
        end = end < start ? UINT64_MAX: end;

        LOG("mine range [%ld-%ld]\n", start, end);

        uint64_t nonce = mine(
            bitcoin_block_data,
            difficulty_mask,
            start, end,
            digest);

        LOG("nounce %ld\n", nonce);

        pthread_mutex_lock(&mutex);
        if (nonce != 0)
        {
            if (found_nounce == 0) {
                found_nounce = nonce;
                found_rank = rank;
            } 
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&condp);
            pthread_cond_broadcast(&condc);
            break;
        }

        /* Wake up the producer */
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condp);
    }
    return 0;
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: %s threads difficulty 'block data (string)'\n", argv[0]);
        return EXIT_FAILURE;
    }

    // TODO refactor to strcol
    int num_threads = atoi(argv[1]); 
    printf("Number of threads: %d\n", num_threads);

    // TODO refactor to strcol
    int desired_difficulty = atoi(argv[2]);
    LOG("difficulty: %d\n", desired_difficulty);

    // set difficulty
    for (int i = 0; i < 32 - desired_difficulty; i++) {
        difficulty_mask = difficulty_mask | 1 << i;
    }

    printf("  Difficulty Mask: ");
    print_binary32(difficulty_mask);

    /* We use the input string passed in (argv[3]) as our block data. In a
     * complete bitcoin miner implementation, the block data would be composed
     * of bitcoin transactions. */
    char *bitcoin_block_data = argv[3];
    printf("       Block data: [%s]\n", bitcoin_block_data);

    printf("\n----------- Starting up miner threads!  -----------\n\n");

    double start_time = get_time();

    /* init task queue */
    task_queue = queue_init();

    pthread_t *workers = malloc(num_threads * sizeof(pthread_t));
    for (int i = 0; i < num_threads; i++) {
            pthread_create(
            &workers[i],
            NULL,
            worker_thread,
            ((void *) (unsigned long) i));
    }

    /* create worker threads */    
    uint64_t i;
    for (i = 1; i < UINT64_MAX; i += RANGE) {
        pthread_mutex_lock(&mutex);
        while (queue_size(task_queue) == num_threads && found_nounce == 0)
        {
            pthread_cond_wait(&condp, &mutex);
        }

        if (found_nounce != 0)
        {
            printf("%ld found %ld\n", found_rank, found_nounce);
            pthread_mutex_unlock(&mutex);
            break;
        }
        /* Time to produce a new value: */
        LOG("produce %ld\n", i);
        queue_offer(task_queue, i);
        /* Wake up consumer */
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&mutex);
    }

    double end_time = get_time();

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(workers[i], NULL);
    }
    free(workers);
    queue_destory(task_queue);

    if (found_nounce == 0) {
        printf("No solution found!\n");
        return 1;
    }

    /* When printed in hex, a SHA-1 checksum will be 40 characters. */
    char solution_hash[41];
    sha1tostring(solution_hash, digest);

    printf("Solution found by thread %ld:\n", found_rank);
    printf("Nonce: %lu\n", found_nounce);
    printf(" Hash: %s\n", solution_hash);

    double total_time = end_time - start_time;
    printf("%llu hashes in %.2fs (%.2f hashes/sec)\n",
            total_inversions, total_time, total_inversions / total_time);

    return 0;
}

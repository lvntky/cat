/**
 * @file thread_pool.h
 * @brief Thread Pool Manager/Executor
 * @author Levent Kaya
 * @date 2024-04-01
 * @version 1.0
 * @license GPLv3.0
 * 
 * Thread Pool Manager Util
 * For CAT library
 */

#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#define THREAD_POOL_MIN_LIMIT 2
#define THREAD_POOL_MAX_LIMIT 200

#include "common.h"

/**
 * @brief 
 * The main task 
 */
typedef struct thread_pool_task {
	void (*function)(void *);
	void *argument;
} thread_pool_task_t;

typedef struct thread_pool {
	pthread_t *threads;
	int number_of_threads;
	thread_pool_task_t *task_queue;
	int queue_size;
	int queue_capacity;
	pthread_mutex_t queue_mutex;
	pthread_cond_t queue_not_empty;
} thread_pool_t;

/**
 * @brief thread poool initializer
 * 
 * @param number_of_threads 
 * @return thread_pool_t* 
 */
thread_pool_t *thread_pool_init(int number_of_threads);

/**
 * @brief Add the task to the task queue or directly assign it to a worker thread
 * Ensure thread safety when accessing shared data structures
 * 
 * @param pool ist the related pool to assign task
 * @param task the function that needs to be executed by pool workers
 * @param arg arguments of the given task
 */
void thread_pool_submit(thread_pool_t *pool, void (*task)(void *), void *arg);

/**
 * @brief Retrieve tasks from the task queue or directly from the pool's task list
 * Execute the task
 * Handle task completion or errors
 * 
 * @param arg 
 * @return void* 
 */
void *thread_pool_worker_thread(void *arg);

/**
 * @brief Handle task completion, e.g., notify the caller or invoke callback functions
 * 
 */
void thread_pool_task_completed();

/**
 * @brief Resize the thread pool by creating or terminating worker threads as needed
 * 
 * @param pool 
 * @param new_thread_count 
 */
void thread_pool_resize(thread_pool_t *pool, int new_thread_count);

/**
 * @brief Clean up resources, join worker threads, and free memory
 * 
 * @param pool 
 */
void thread_pool_destroy(thread_pool_t *pool);

/**
 * @brief wait function(blocker) for pool
 * 
 * @param pool 
 * 
 * Wait all the work(both queue and active) is done.  
 */
void thread_pool_wait(thread_pool_t *pool);

int thread_pool_get_size(thread_pool_t *pool);

#endif //__THREAD_POOL_H__
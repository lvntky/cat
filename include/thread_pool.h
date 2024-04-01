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

#include "common.h"

typedef struct thread_pool
{
    pthread_t *threads;
    int number_of_threads;
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
 * @param pool 
 * @param task 
 * @param arg 
 */
void thread_pool_submit(thread_pool_t *pool, void (*task)(void *), void* arg);

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

#endif //__THREAD_POOL_H__
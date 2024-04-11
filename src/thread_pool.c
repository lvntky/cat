#include "thread_pool.h"

int thread_pool_limit_handler(int *number_of_threads)
{
	if (number_of_threads == NULL) {
		return EXIT_FAILURE;
	}

	if (*number_of_threads < THREAD_POOL_MIN_LIMIT) {
		*number_of_threads = THREAD_POOL_MIN_LIMIT;
	} else if (*number_of_threads > THREAD_POOL_MAX_LIMIT) {
		*number_of_threads = THREAD_POOL_MAX_LIMIT;
	}

	return EXIT_SUCCESS;
}

thread_pool_t *thread_pool_init(int number_of_threads, int queue_capacity)
{
	if (thread_pool_limit_handler(&number_of_threads) != EXIT_SUCCESS) {
		fprintf(stderr, "Error: Invalid number of threads.\n");
		return NULL;
	}

	thread_pool_t *thread_pool = malloc(sizeof(thread_pool_t));
	if (thread_pool == NULL) {
		fprintf(stderr,
			"Error: Memory allocation failed for thread pool.\n");
		return NULL;
	}

	thread_pool->number_of_threads = number_of_threads;

	thread_pool->threads = malloc(number_of_threads * sizeof(pthread_t));
	if (thread_pool->threads == NULL) {
		fprintf(stderr,
			"Error: Memory allocation failed for threads.\n");
		free(thread_pool);
		return NULL;
	}
	if (queue_capacity == NULL) {
		thread_pool->queue_capacity =
			THREAD_POOL_INITIAL_QUEUE_CAPACITY;
	} else {
		thread_pool->queue_capacity = queue_capacity;
	}

	thread_pool->task_queue = malloc(thread_pool->queue_capacity *
					 sizeof(thread_pool_task_t));
	if (thread_pool->task_queue == NULL) {
		fprintf(stderr,
			"Error: Memory allocation failed for task queue.\n");
		free(thread_pool->threads);
		free(thread_pool);
		return NULL;
	}
	thread_pool->queue_size = 0;

	// Initialize the queue mutex and condition variable
	pthread_mutex_init(&thread_pool->queue_mutex, NULL);
	pthread_cond_init(&thread_pool->queue_not_empty, NULL);

	// Create worker threads
	for (int i = 0; i < number_of_threads; i++) {
		int result = pthread_create(&thread_pool->threads[i], NULL,
					    thread_pool_worker_thread,
					    (void *)thread_pool);
		if (result != 0) {
			fprintf(stderr, "Error: Failed to create thread %d.\n",
				i);
			// Clean up resources and return NULL
			for (int j = 0; j < i; j++) {
				pthread_cancel(thread_pool->threads[j]);
			}
			free(thread_pool->task_queue);
			free(thread_pool->threads);
			free(thread_pool);
			return NULL;
		}
	}

	return thread_pool;
}

void thread_pool_destroy(thread_pool_t *pool)
{
	if (pool != NULL) {
		// Cancel all worker threads
		for (int i = 0; i < pool->number_of_threads; ++i) {
			pthread_cancel(pool->threads[i]);
		}

		// Join all worker threads
		for (int i = 0; i < pool->number_of_threads; ++i) {
			pthread_join(pool->threads[i], NULL);
		}

		// Free memory and destroy mutex and condition variable
		free(pool->task_queue);
		free(pool->threads);
		pthread_mutex_destroy(&pool->queue_mutex);
		pthread_cond_destroy(&pool->queue_not_empty);
		free(pool);
	}
}

void thread_pool_submit(thread_pool_t *pool, void (*task_function)(void *),
			void *task_argument)
{
	if (pool == NULL) {
		fprintf(stderr, "Error: Thread pool is NULL\n");
		return;
	}

	// Lock the queue mutex to ensure thread safety
	pthread_mutex_lock(&pool->queue_mutex);

	// Check if the task queue is full
	if (pool->queue_size >= pool->queue_capacity) {
		fprintf(stderr, "Error: Task queue is full\n");
		pthread_mutex_unlock(&pool->queue_mutex);
		return;
	}

	// Add the task to the task queue
	int index = (pool->queue_size++) % pool->queue_capacity;
	pool->task_queue[index].function = task_function;
	pool->task_queue[index].argument = task_argument;

	// Signal the worker threads that the queue is not empty
	pthread_cond_signal(&pool->queue_not_empty);

	// Unlock the queue mutex
	pthread_mutex_unlock(&pool->queue_mutex);
}

void *thread_pool_worker_thread(void *arg)
{
	// Cast the argument to the thread pool structure
	thread_pool_t *pool = (thread_pool_t *)arg;

	// Worker thread main loop
	while (1) {
		// Lock the queue mutex to ensure thread safety
		pthread_mutex_lock(&pool->queue_mutex);

		// Wait until there's a task in the queue
		while (pool->queue_size == 0) {
			pthread_cond_wait(&pool->queue_not_empty,
					  &pool->queue_mutex);
		}

		// Retrieve a task from the task queue
		thread_pool_task_t task = pool->task_queue[--pool->queue_size];

		// Unlock the queue mutex
		pthread_mutex_unlock(&pool->queue_mutex);

		// Execute the retrieved task
		task.function(task.argument);

		// For illustration purposes, here we just print a message
		printf("Worker thread %ld: Executed task.\n", pthread_self());
	}

	// The worker thread will never reach here, as it's designed to run indefinitely
	// However, a return statement is added to avoid compiler warnings
	return NULL;
}

void thread_pool_task_completed()
{
	// TODO: implement a notifyer
}

void thread_pool_resize(thread_pool_t *pool, int new_thread_count)
{
	// Check and handle thread limit
	if (thread_pool_limit_handler(&new_thread_count) != EXIT_SUCCESS) {
		fprintf(stderr, "Error: Invalid number of threads.\n");
		return;
	}

	// Lock the queue mutex to ensure thread safety
	pthread_mutex_lock(&pool->queue_mutex);

	// Check if resizing is needed
	if (new_thread_count == pool->number_of_threads) {
		// No need to resize
		pthread_mutex_unlock(&pool->queue_mutex);
		return;
	}

	// Resize the thread pool
	if (new_thread_count > pool->number_of_threads) {
		// Increase the number of threads
		int thread_difference =
			new_thread_count - pool->number_of_threads;
		for (int i = 0; i < thread_difference; ++i) {
			int result = pthread_create(
				&pool->threads[pool->number_of_threads + i],
				NULL, thread_pool_worker_thread, (void *)pool);
			if (result != 0) {
				fprintf(stderr,
					"Error: Failed to create thread %d.\n",
					i);
				// Rollback the creation of threads and return
				for (int j = 0; j < i; ++j) {
					pthread_cancel(
						pool->threads
							[pool->number_of_threads +
							 j]);
				}
				pthread_mutex_unlock(&pool->queue_mutex);
				return;
			}
		}
	} else {
		// Decrease the number of threads
		int thread_difference =
			pool->number_of_threads - new_thread_count;
		for (int i = 0; i < thread_difference; ++i) {
			pthread_cancel(
				pool->threads[pool->number_of_threads - 1 - i]);
		}
	}

	// Update the number of threads
	pool->number_of_threads = new_thread_count;

	// Unlock the queue mutex
	pthread_mutex_unlock(&pool->queue_mutex);
}

void thread_pool_wait(thread_pool_t *pool)
{
	// Lock the queue mutex to ensure thread safety
	pthread_mutex_lock(&pool->queue_mutex);

	// Wait until the task queue is empty
	while (pool->queue_size > 0) {
		pthread_cond_wait(&pool->queue_not_empty, &pool->queue_mutex);
	}

	// Unlock the queue mutex
	pthread_mutex_unlock(&pool->queue_mutex);
}

int thread_pool_get_size(thread_pool_t *pool)
{
	return pool->number_of_threads;
}
#include <cat/thread_pool.h>

int main()
{
	thread_pool_t *pool = thread_pool_init(8);

	printf("number of threads: %d\n", pool->number_of_threads);
	printf("number of threads with library function: %d\n",
	       thread_pool_get_size(pool));

	return 0;
}
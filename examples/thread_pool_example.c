#include <cat/thread_pool.h>

int main()
{
	thread_pool_t *pool = thread_pool_init(8);

	printf("number of threads: %d\n", pool->number_of_threads);

	return 0;
}
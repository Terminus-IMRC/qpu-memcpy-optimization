/*
 * Copyright (c) 2016 Sugizaki Yukimasa
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include <string.h>
#include <errno.h>
#include <omp.h>
#include <vc4vec.h>

void cpu_memcpy_3_init()
{
}

void cpu_memcpy_3_finalize()
{
}

int cpu_memcpy_3(unsigned *dest_cpu, unsigned dest_gpu, unsigned *src_cpu, unsigned src_gpu, size_t n)
{
	if (n % sizeof(unsigned) != 0) {
		errno = EINVAL;
		return -1;
	}

	(void) dest_gpu; (void) src_gpu;
#pragma omp parallel
	{
		int thread_num, num_threads;
		size_t offset, len;

		thread_num = omp_get_thread_num();
		num_threads = omp_get_num_threads();

		offset = (n / num_threads) * thread_num;
		len = n / num_threads;
		if (thread_num == num_threads - 1)
			len = n - (n / num_threads) * (num_threads - 1);

		memcpy(dest_cpu + offset, src_cpu + offset, len);
	}

	return 0;
}

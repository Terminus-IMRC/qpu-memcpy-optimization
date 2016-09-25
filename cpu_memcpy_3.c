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

int cpu_memcpy_3(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n)
{
	if (n % sizeof(unsigned) != 0) {
		errno = EINVAL;
		return -1;
	}

#pragma omp parallel
	{
		int thread_num, num_threads;
		size_t offset, len;
		uint8_t *out = dest->cpu_addr, *in = src->cpu_addr;

		thread_num = omp_get_thread_num();
		num_threads = omp_get_num_threads();

		offset = (n / num_threads) * thread_num;
		len = n / num_threads;
		if (thread_num == num_threads - 1)
			len = n - (n / num_threads) * (num_threads - 1);

		memcpy(out + offset, in + offset, len);
	}

	return 0;
}

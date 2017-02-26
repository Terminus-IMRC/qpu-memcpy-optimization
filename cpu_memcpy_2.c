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
#include <vc4vec.h>

void cpu_memcpy_2_init()
{
}

void cpu_memcpy_2_finalize()
{
}

int cpu_memcpy_2(unsigned *dest_cpu, unsigned dest_gpu, unsigned *src_cpu, unsigned src_gpu, size_t n)
{
	int i, len;

	if (n % sizeof(unsigned) != 0) {
		errno = EINVAL;
		return -1;
	}

	len = n / sizeof(unsigned);

	(void) dest_gpu; (void) src_gpu;
#pragma omp parallel for
	for (i = 0; i < len; i ++)
		dest_cpu[i] = src_cpu[i];

	return 0;
}

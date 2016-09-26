/*
 * Copyright (c) 2016 Sugizaki Yukimasa
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <vc4vec.h>
#include "qpu_memcpy.h"

static const int mem_size = 1024 * 1024 * 30;

static void rand_mem(struct vc4vec_mem *mem, size_t n)
{
	unsigned *p = mem->cpu_addr;
	size_t i, len = n / sizeof(*p);

	for (i = 0; i < len; i ++)
		p[i] = random();
}

int main()
{
	struct vc4vec_mem dest, src;
	float time, flops;

	vc4vec_init();
	atexit(vc4vec_finalize);
	vc4vec_mem_alloc(&dest, mem_size);
	vc4vec_mem_alloc(&src, mem_size);
	qpu_memcpy_init();
	{
		struct timeval st;
		gettimeofday(&st, NULL);
		srandom(st.tv_sec ^ st.tv_usec);
	}


	rand_mem(&src, mem_size);
	qpu_memcpy_launch(&time, &flops, cpu_memcpy_1, &dest, &src, mem_size);
	printf("cpu_memcpy_1: %g [s], %g [flop/s]\n", time, flops);

	rand_mem(&src, mem_size);
	qpu_memcpy_launch(&time, &flops, cpu_memcpy_2, &dest, &src, mem_size);
	printf("cpu_memcpy_2: %g [s], %g [flop/s]\n", time, flops);

	rand_mem(&src, mem_size);
	qpu_memcpy_launch(&time, &flops, cpu_memcpy_3, &dest, &src, mem_size);
	printf("cpu_memcpy_3: %g [s], %g [flop/s]\n", time, flops);

	rand_mem(&src, mem_size);
	qpu_memcpy_launch(&time, &flops, qpu_memcpy_1, &dest, &src, mem_size);
	printf("qpu_memcpy_1: %g [s], %g [flop/s]\n", time, flops);

	rand_mem(&src, mem_size);
	qpu_memcpy_launch(&time, &flops, qpu_memcpy_2, &dest, &src, mem_size);
	printf("qpu_memcpy_2: %g [s], %g [flop/s]\n", time, flops);


	qpu_memcpy_finalize();
	vc4vec_mem_free(&src);
	vc4vec_mem_free(&dest);
	/* vc4vec_finalize was registered to atexit, so it will be called after this return. */
	return 0;
}

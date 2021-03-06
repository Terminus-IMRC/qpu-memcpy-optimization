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

static void rand_mem(unsigned *p, size_t n)
{
	size_t i;
	const size_t len = n / sizeof(*p);

#pragma omp parallel for private(i) firstprivate(p, len)
	for (i = 0; i < len; i ++)
		p[i] = random();
}

int main()
{
	struct vc4vec_mem dest, src;
	unsigned *dest_cpu, *src_cpu;
	unsigned dest_gpu, src_gpu;
	float time, Bps;

	vc4vec_init();
	atexit(vc4vec_finalize);
	vc4vec_mem_alloc(&dest, mem_size);
	vc4vec_mem_alloc(&src, mem_size);
	dest_cpu = dest.cpu_addr;
	dest_gpu = dest.gpu_addr;
	src_cpu = src.cpu_addr;
	src_gpu = src.gpu_addr;
	qpu_memcpy_init();
	{
		struct timeval st;
		gettimeofday(&st, NULL);
		srandom(st.tv_sec ^ st.tv_usec);
	}


	rand_mem(src_cpu, mem_size);
	qpu_memcpy_launch(&time, &Bps, cpu_memcpy_1, dest_cpu, dest_gpu, src_cpu, src_gpu, mem_size);
	printf("cpu_memcpy_1: %g [s], %g [B/s]\n", time, Bps);

	rand_mem(src_cpu, mem_size);
	qpu_memcpy_launch(&time, &Bps, cpu_memcpy_2, dest_cpu, dest_gpu, src_cpu, src_gpu, mem_size);
	printf("cpu_memcpy_2: %g [s], %g [B/s]\n", time, Bps);

	rand_mem(src_cpu, mem_size);
	qpu_memcpy_launch(&time, &Bps, cpu_memcpy_3, dest_cpu, dest_gpu, src_cpu, src_gpu, mem_size);
	printf("cpu_memcpy_3: %g [s], %g [B/s]\n", time, Bps);

	rand_mem(src_cpu, mem_size);
	qpu_memcpy_launch(&time, &Bps, qpu_memcpy_1, dest_cpu, dest_gpu, src_cpu, src_gpu, mem_size);
	printf("qpu_memcpy_1: %g [s], %g [B/s]\n", time, Bps);

	rand_mem(src_cpu, mem_size);
	qpu_memcpy_launch(&time, &Bps, qpu_memcpy_2, dest_cpu, dest_gpu, src_cpu, src_gpu, mem_size);
	printf("qpu_memcpy_2: %g [s], %g [B/s]\n", time, Bps);

	rand_mem(src_cpu, mem_size);
	qpu_memcpy_launch(&time, &Bps, qpu_memcpy_3, dest_cpu, dest_gpu, src_cpu, src_gpu, mem_size);
	printf("qpu_memcpy_3: %g [s], %g [B/s]\n", time, Bps);


	qpu_memcpy_finalize();
	vc4vec_mem_free(&src);
	vc4vec_mem_free(&dest);
	/* vc4vec_finalize was registered to atexit, so it will be called after this return. */
	return 0;
}

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
#include <stdint.h>
#include <sys/time.h>
#include <errno.h>
#include "qpu_memcpy.h"

struct vc4vec_mem mem_unif;

void qpu_memcpy_init()
{
	vc4vec_mem_alloc(&mem_unif, 1024 * (32 / 8));
	cpu_memcpy_1_init();
	cpu_memcpy_2_init();
	cpu_memcpy_3_init();
	qpu_memcpy_1_init();
}

void qpu_memcpy_finalize()
{
	qpu_memcpy_1_finalize();
	cpu_memcpy_3_finalize();
	cpu_memcpy_2_finalize();
	cpu_memcpy_1_finalize();
	vc4vec_mem_free(&mem_unif);
}

void qpu_memcpy_launch(float *time, float *flops, int (*qpu_memcpy_n)(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n), struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n)
{
	struct timeval start, end;
	int reti;

	gettimeofday(&start, NULL);
	reti = qpu_memcpy_n(dest, src, n);
	gettimeofday(&end, NULL);

	if (reti == -1) {
		fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (memcmp(dest->cpu_addr, src->cpu_addr, n) != 0) {
		fprintf(stderr, "%s:%d: error: dest and src differ\n", __FILE__, __LINE__);
		{
			size_t i;
			unsigned *out = dest->cpu_addr, *in = src->cpu_addr;

			for (i = 0; i < n / sizeof(*out); i ++) {
				if (out[i] != in[i]) {
					fprintf(stderr, "%s:%d: The differ is at i=%d\n", __FILE__, __LINE__, i);
					break;
				}
			}
		}
		exit(EXIT_FAILURE);
	}

	*time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;
	*flops = n / *time;
}

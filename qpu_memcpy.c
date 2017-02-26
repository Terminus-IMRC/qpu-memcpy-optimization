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

static struct vc4vec_mem mem_unif;
unsigned *mem_unif_cpu, mem_unif_gpu;

void qpu_memcpy_init()
{
	vc4vec_mem_alloc(&mem_unif, 1024 * (32 / 8));
    mem_unif_cpu = mem_unif.cpu_addr;
    mem_unif_gpu = mem_unif.gpu_addr;
	cpu_memcpy_1_init();
	cpu_memcpy_2_init();
	cpu_memcpy_3_init();
	qpu_memcpy_1_init();
	qpu_memcpy_2_init();
	qpu_memcpy_3_init();
}

void qpu_memcpy_finalize()
{
	qpu_memcpy_3_finalize();
	qpu_memcpy_2_finalize();
	qpu_memcpy_1_finalize();
	cpu_memcpy_3_finalize();
	cpu_memcpy_2_finalize();
	cpu_memcpy_1_finalize();
	vc4vec_mem_free(&mem_unif);
}

void qpu_memcpy_launch(float *time, float *Bps, int (*qpu_memcpy_n)(unsigned *dest_cpu, unsigned dest_gpu, unsigned *src_cpu, unsigned src_gpu, size_t n), unsigned *dest_cpu, unsigned dest_gpu, unsigned *src_cpu, unsigned src_gpu, size_t n)
{
	struct timeval start, end;
	int reti;

	gettimeofday(&start, NULL);
	reti = qpu_memcpy_n(dest_cpu, dest_gpu, src_cpu, src_gpu, n);
	gettimeofday(&end, NULL);

	if (reti == -1) {
		fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (memcmp(dest_cpu, src_cpu, n) != 0) {
		fprintf(stderr, "%s:%d: error: dest and src differ\n", __FILE__, __LINE__);
		{
			size_t i;

			for (i = 0; i < n / sizeof(*dest_cpu); i ++) {
				if (dest_cpu[i] != src_cpu[i]) {
					fprintf(stderr, "%s:%d: The differ is at i=%d\n", __FILE__, __LINE__, i);
					break;
				}
			}
		}
		exit(EXIT_FAILURE);
	}

	*time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;
	*Bps = n / *time;
}

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

void cpu_memcpy_1_init()
{
}

void cpu_memcpy_1_finalize()
{
}

int cpu_memcpy_1(unsigned *dest_cpu, unsigned dest_gpu, unsigned *src_cpu, unsigned src_gpu, size_t n)
{
	(void) dest_gpu; (void) src_gpu;
	memcpy(dest_cpu, src_cpu, n);
	return 0;
}

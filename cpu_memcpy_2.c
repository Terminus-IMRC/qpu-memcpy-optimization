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

int cpu_memcpy_2(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n)
{
	int i, len;
	unsigned *out, *in;

	if (n % sizeof(unsigned) != 0) {
		errno = EINVAL;
		return -1;
	}

	out = dest->cpu_addr;
	in = src->cpu_addr;
	len = n / sizeof(unsigned);

#pragma omp parallel for
	for (i = 0; i < len; i ++)
		out[i] = in[i];

	return 0;
}

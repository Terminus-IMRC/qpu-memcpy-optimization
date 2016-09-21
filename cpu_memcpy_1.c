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

int cpu_memcpy_1(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n)
{
	memcpy(dest->cpu_addr, src->cpu_addr, n);
	return 0;
}

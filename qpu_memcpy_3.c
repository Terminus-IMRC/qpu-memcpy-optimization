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
#include "qpu_memcpy.h"

static unsigned code[] = {
#include "qpu_memcpy_3.qhex"
};
static size_t code_size = sizeof(code);
static struct vc4vec_mem mem_code;

void qpu_memcpy_3_init()
{
	vc4vec_mem_alloc(&mem_code, code_size);
	memcpy(mem_code.cpu_addr, code, code_size);
}

void qpu_memcpy_3_finalize()
{
	vc4vec_mem_free(&mem_code);
}

int qpu_memcpy_3(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n)
{
	unsigned *p;

	/* 1024 = 16 * 16 * (32 / 8) */

	if ((n < 8192) || (n % 4096 != 0)) {
		errno = EINVAL;
		return -1;
	}

	p = mem_unif.cpu_addr;
	*p++ = dest->gpu_addr;
	*p++ = src->gpu_addr;
	*p++ = n / 4096 - 1;

	launch_qpu_job_mailbox(1, 0, QPU_MEMCPY_TIMEOUT, mem_unif.gpu_addr, mem_code.gpu_addr);

	return 0;
}

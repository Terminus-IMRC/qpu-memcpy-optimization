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
#include "qpu_memcpy_2.qhex"
};
static size_t code_size = sizeof(code);
static struct vc4vec_mem mem_code;
static unsigned *mem_code_cpu, mem_code_gpu;

void qpu_memcpy_2_init()
{
	vc4vec_mem_alloc(&mem_code, code_size);
	mem_code_cpu = mem_code.cpu_addr;
	mem_code_gpu = mem_code.gpu_addr;
	memcpy(mem_code_cpu, code, code_size);
}

void qpu_memcpy_2_finalize()
{
	vc4vec_mem_free(&mem_code);
}

int qpu_memcpy_2(unsigned *dest_cpu, unsigned dest_gpu, unsigned *src_cpu, unsigned src_gpu, size_t n)
{
	unsigned *p;

	/* 1024 = 16 * 16 * (32 / 8) */

	if ((n < 4096) || (n % 2048 != 0)) {
		errno = EINVAL;
		return -1;
	}

	(void) dest_cpu; (void) src_cpu;
	p = mem_unif_cpu;
	*p++ = dest_gpu;
	*p++ = src_gpu;
	*p++ = n / 2048 - 1;

	launch_qpu_job_mailbox(1, 0, QPU_MEMCPY_TIMEOUT, mem_unif_gpu, mem_code_gpu);

	return 0;
}

/*
 * Copyright (c) 2016 Sugizaki Yukimasa
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#ifndef QPU_MEMCPY_H
#define QPU_MEMCPY_H

#include <string.h>
#include <vc4vec.h>

	void qpu_memcpy_init();
	void qpu_memcpy_finalize();

	void qpu_memcpy_launch(float *time, float *Bps, int (*qpu_memcpy_n)(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n), struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n);

	void cpu_memcpy_1_init();
	void cpu_memcpy_1_finalize();
	int cpu_memcpy_1(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n);

	void cpu_memcpy_2_init();
	void cpu_memcpy_2_finalize();
	int cpu_memcpy_2(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n);

	void cpu_memcpy_3_init();
	void cpu_memcpy_3_finalize();
	int cpu_memcpy_3(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n);

	void qpu_memcpy_1_init();
	void qpu_memcpy_1_finalize();
	int qpu_memcpy_1(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n);

	void qpu_memcpy_2_init();
	void qpu_memcpy_2_finalize();
	int qpu_memcpy_2(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n);

	void qpu_memcpy_3_init();
	void qpu_memcpy_3_finalize();
	int qpu_memcpy_3(struct vc4vec_mem *dest, struct vc4vec_mem *src, size_t n);

	extern struct vc4vec_mem mem_unif;

#define QPU_MEMCPY_TIMEOUT 100e3

#endif /* QPU_MEMCPY_H */

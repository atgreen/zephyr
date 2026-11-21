/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_MOXIE_EXCEPTION_H_
#define ZEPHYR_INCLUDE_ARCH_MOXIE_EXCEPTION_H_

#ifndef _ASMLANGUAGE
#include <zephyr/types.h>
#include <zephyr/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

/* What the exception entry pushes on the interrupted stack. */
struct arch_esf {
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r12;
	uint32_t r13;
	uint32_t r6;	/* callee-saved, but the entry code borrows it */
	uint32_t sr;	/* status: interrupt enable and condition codes */
	uint32_t fp;
	uint32_t pc;
};

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_MOXIE_EXCEPTION_H_ */

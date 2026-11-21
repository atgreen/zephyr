/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_MOXIE_THREAD_H_
#define ZEPHYR_INCLUDE_ARCH_MOXIE_THREAD_H_

#ifndef _ASMLANGUAGE
#include <zephyr/types.h>

/*
 * $sp is not here: "ret" reloads it from $fp, so the frame pointer is
 * all a parked thread needs to find its stack again.
 */
struct _callee_saved {
	uint32_t fp;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
};
typedef struct _callee_saved _callee_saved_t;

struct _thread_arch {
};

typedef struct _thread_arch _thread_arch_t;

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_MOXIE_THREAD_H_ */

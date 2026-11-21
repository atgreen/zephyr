/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_ARCH_MOXIE_INCLUDE_OFFSETS_SHORT_ARCH_H_
#define ZEPHYR_ARCH_MOXIE_INCLUDE_OFFSETS_SHORT_ARCH_H_

#include <zephyr/offsets.h>

#define _thread_offset_to_fp \
	(___thread_t_callee_saved_OFFSET + ___callee_saved_t_fp_OFFSET)

#define _thread_offset_to_r6 \
	(___thread_t_callee_saved_OFFSET + ___callee_saved_t_r6_OFFSET)

#define _thread_offset_to_r7 \
	(___thread_t_callee_saved_OFFSET + ___callee_saved_t_r7_OFFSET)

#define _thread_offset_to_r8 \
	(___thread_t_callee_saved_OFFSET + ___callee_saved_t_r8_OFFSET)

#define _thread_offset_to_r9 \
	(___thread_t_callee_saved_OFFSET + ___callee_saved_t_r9_OFFSET)

#define _thread_offset_to_r10 \
	(___thread_t_callee_saved_OFFSET + ___callee_saved_t_r10_OFFSET)

#define _thread_offset_to_r11 \
	(___thread_t_callee_saved_OFFSET + ___callee_saved_t_r11_OFFSET)

#endif /* ZEPHYR_ARCH_MOXIE_INCLUDE_OFFSETS_SHORT_ARCH_H_ */

/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/moxie/moxie_sr.h>
#include <offsets_short.h>

GTEXT(z_moxie_switch)
GTEXT(z_moxie_thread_start)
GTEXT(z_thread_entry)

/*
 * void z_moxie_switch(struct k_thread *new, struct k_thread *old)
 *
 * The call that got us here left a frame at $fp holding the caller's
 * $fp and the return address, and "ret" needs nothing else.  So a
 * thread is suspended by saving $fp and the callee-saved registers,
 * and resumed by loading them and returning.
 */
SECTION_FUNC(TEXT, z_moxie_switch)
	sto.l	_thread_offset_to_fp($r1), $fp
	sto.l	_thread_offset_to_r6($r1), $r6
	sto.l	_thread_offset_to_r7($r1), $r7
	sto.l	_thread_offset_to_r8($r1), $r8
	sto.l	_thread_offset_to_r9($r1), $r9
	sto.l	_thread_offset_to_r10($r1), $r10
	sto.l	_thread_offset_to_r11($r1), $r11

	/* The old thread is now safe to resume; say so. */
	sto.l	___thread_t_switch_handle_OFFSET($r1), $r1

	ldo.l	$fp, _thread_offset_to_fp($r0)
	ldo.l	$r6, _thread_offset_to_r6($r0)
	ldo.l	$r7, _thread_offset_to_r7($r0)
	ldo.l	$r8, _thread_offset_to_r8($r0)
	ldo.l	$r9, _thread_offset_to_r9($r0)
	ldo.l	$r10, _thread_offset_to_r10($r0)
	ldo.l	$r11, _thread_offset_to_r11($r0)
	ret

/*
 * Where a new thread lands the first time it is switched to.
 * arch_new_thread() parked the entry point and its arguments in
 * callee-saved registers.
 */
SECTION_FUNC(TEXT, z_moxie_thread_start)
	mov	$r0, $r6
	mov	$r1, $r7
	mov	$r2, $r8
	mov	$r3, $r9
	ldi.l	$r4, MOXIE_STATUS_IE
	ssr	$r4, MOXIE_SR_STATUS
	xor	$fp, $fp
	jsra	z_thread_entry
1:	jmpa	1b

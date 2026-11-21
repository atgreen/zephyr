/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/moxie/moxie_sr.h>

GTEXT(__start)
GTEXT(z_prep_c)
GTEXT(z_moxie_exception_entry)
GDATA(z_interrupt_stacks)

/*
 * The core starts executing at the base of memory with interrupts
 * disabled.  Borrow the interrupt stack until the kernel has a thread
 * to run, tell the core where exceptions go, and head for C.
 */
SECTION_FUNC(reset, __start)
	/* Leave the argument area the ABI expects every caller to provide. */
	ldi.l	$sp, z_interrupt_stacks + CONFIG_ISR_STACK_SIZE - 24
	xor	$fp, $fp
	ldi.l	$r0, z_moxie_exception_entry
	ssr	$r0, MOXIE_SR_HANDLER
	jsra	z_prep_c
1:	jmpa	1b

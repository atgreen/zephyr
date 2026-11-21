/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/moxie/moxie_sr.h>
#include <offsets_short.h>

#define ESF(field)	__struct_arch_esf_##field##_OFFSET
#define KERNEL(field)	_kernel + _kernel_offset_to_##field

/*
 * The ABI has every caller provide room at the bottom of its frame for
 * the callee to store the six argument registers, and compiled code
 * does use it.
 */
#define ARG_AREA	24

GTEXT(z_moxie_exception_entry)
GTEXT(z_moxie_handle_irqs)
GTEXT(z_moxie_exception)
GTEXT(z_moxie_switch)
GTEXT(z_get_next_switch_handle)
GDATA(_kernel)

/*
 * Every exception lands here with interrupts disabled.  The core has
 * copied the status (interrupt enable and condition codes) into
 * MOXIE_SR_SAVED_STATUS and the address to resume at into
 * MOXIE_SR_RESUME; everything else still belongs to whatever was
 * interrupted.
 *
 * Save the caller-saved registers in an exception frame on the
 * interrupted stack.  The callee-saved ones are preserved by the C
 * code we call, or saved by z_moxie_switch() if we end up switching
 * threads.  The exception is $r6, which we need for ourselves to
 * remember where the frame is.
 */
SECTION_FUNC(TEXT, z_moxie_exception_entry)
	dec	$sp, __struct_arch_esf_SIZEOF
	sto.l	ESF(r0)($sp), $r0
	sto.l	ESF(r1)($sp), $r1
	sto.l	ESF(r2)($sp), $r2
	sto.l	ESF(r3)($sp), $r3
	sto.l	ESF(r4)($sp), $r4
	sto.l	ESF(r5)($sp), $r5
	sto.l	ESF(r12)($sp), $r12
	sto.l	ESF(r13)($sp), $r13
	sto.l	ESF(r6)($sp), $r6
	sto.l	ESF(fp)($sp), $fp
	gsr	$r0, MOXIE_SR_RESUME
	sto.l	ESF(pc)($sp), $r0
	gsr	$r0, MOXIE_SR_SAVED_STATUS
	sto.l	ESF(sr)($sp), $r0
	mov	$r6, $sp

	/*
	 * Interrupts, and the software interrupt irq_offload() uses, run
	 * as interrupts.  Anything else is a fault in whatever was
	 * running and is dealt with in that context, so that a faulting
	 * thread can be aborted like any other.
	 */
	gsr	$r1, MOXIE_SR_CAUSE
	ldi.l	$r2, MOXIE_CAUSE_IRQ
	cmp	$r1, $r2
	beq	0f
	ldi.l	$r2, MOXIE_CAUSE_SWI
	cmp	$r1, $r2
	beq	0f
	dec	$sp, ARG_AREA
	mov	$r0, $r6
	jsra	z_moxie_exception
	jmpa	5f
0:
	/* The outermost interrupt moves to the interrupt stack. */
	lda.l	$r0, KERNEL(nested)
	inc	$r0, 1
	sta.l	KERNEL(nested), $r0
	ldi.l	$r1, 1
	cmp	$r0, $r1
	bne	1f
	lda.l	$sp, KERNEL(irq_stack)
1:
	dec	$sp, ARG_AREA

	gsr	$r1, MOXIE_SR_CAUSE
	ldi.l	$r2, MOXIE_CAUSE_IRQ
	cmp	$r1, $r2
	bne	2f
	jsra	z_moxie_handle_irqs
	jmpa	3f
2:
	/* z_moxie_exception(esf, cause) */
	mov	$r0, $r6
	jsra	z_moxie_exception
3:
	lda.l	$r0, KERNEL(nested)
	dec	$r0, 1
	sta.l	KERNEL(nested), $r0
	xor	$r1, $r1
	cmp	$r0, $r1
	bne	5f

	/*
	 * We are about to return to a thread.  If the scheduler wants a
	 * different one, switch to it; we get back here when the
	 * interrupted thread is next scheduled.
	 */
	mov	$sp, $r6
	dec	$sp, ARG_AREA + 4
	lda.l	$r1, KERNEL(current)
	sto.l	ARG_AREA($sp), $r1
	xor	$r0, $r0
	jsra	z_get_next_switch_handle
	ldo.l	$r1, ARG_AREA($sp)
	xor	$r2, $r2
	cmp	$r0, $r2
	beq	5f
	jsra	z_moxie_switch
5:
	/*
	 * Put the resume address and status back where eret looks for
	 * them, restore what we saved, and go.  eret restores the
	 * condition codes and the interrupt enable as it jumps.
	 */
	mov	$sp, $r6
	ldo.l	$r0, ESF(pc)($sp)
	ssr	$r0, MOXIE_SR_RESUME
	ldo.l	$r0, ESF(sr)($sp)
	ssr	$r0, MOXIE_SR_SAVED_STATUS
	ldo.l	$r0, ESF(r0)($sp)
	ldo.l	$r1, ESF(r1)($sp)
	ldo.l	$r2, ESF(r2)($sp)
	ldo.l	$r3, ESF(r3)($sp)
	ldo.l	$r4, ESF(r4)($sp)
	ldo.l	$r5, ESF(r5)($sp)
	ldo.l	$r12, ESF(r12)($sp)
	ldo.l	$r13, ESF(r13)($sp)
	ldo.l	$r6, ESF(r6)($sp)
	ldo.l	$fp, ESF(fp)($sp)
	inc	$sp, __struct_arch_esf_SIZEOF
	eret

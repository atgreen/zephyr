/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/arch/moxie/moxie_sr.h>
#include <zephyr/arch/exception.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(os, CONFIG_KERNEL_LOG_LEVEL);

FUNC_NORETURN void z_moxie_fatal_error(unsigned int reason,
				       const struct arch_esf *esf)
{
#ifdef CONFIG_EXCEPTION_DEBUG
	if (esf != NULL) {
		EXCEPTION_DUMP("  pc: 0x%08x  sr: 0x%08x  fp: 0x%08x",
			       esf->pc, esf->sr, esf->fp);
		EXCEPTION_DUMP("  r0: 0x%08x  r1: 0x%08x  r2: 0x%08x  r3: 0x%08x",
			       esf->r0, esf->r1, esf->r2, esf->r3);
		EXCEPTION_DUMP("  r4: 0x%08x  r5: 0x%08x  r6: 0x%08x",
			       esf->r4, esf->r5, esf->r6);
		EXCEPTION_DUMP(" r12: 0x%08x r13: 0x%08x", esf->r12, esf->r13);
	}
#endif /* CONFIG_EXCEPTION_DEBUG */

	z_fatal_error(reason, esf);
	CODE_UNREACHABLE;
}

static const char *cause_str(unsigned int cause)
{
	switch (cause) {
	case MOXIE_CAUSE_IRQ:
		return "interrupt";
	case MOXIE_CAUSE_SWI:
		return "software interrupt";
	case MOXIE_CAUSE_BRK:
		return "breakpoint";
	default:
		return "unknown";
	}
}

/* Called from the exception entry for anything that is not an interrupt. */
void z_moxie_exception(struct arch_esf *esf, unsigned int cause)
{
#ifdef CONFIG_IRQ_OFFLOAD
	extern void z_moxie_irq_offload_handler(void);

	if (cause == MOXIE_CAUSE_SWI) {
		z_moxie_irq_offload_handler();
		return;
	}
#endif
	EXCEPTION_DUMP("exception cause %u (%s)", cause, cause_str(cause));
	z_moxie_fatal_error(K_ERR_CPU_EXCEPTION, esf);
}

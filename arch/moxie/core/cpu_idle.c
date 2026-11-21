/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/irq.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/arch/moxie/moxie_sr.h>

/*
 * wfi stops the core until an interrupt is requested, and it wakes
 * for a request even while interrupts are disabled.  So waiting first
 * and enabling afterwards cannot miss one.
 */
static ALWAYS_INLINE void moxie_wait(unsigned int key)
{
	__asm__ volatile ("wfi");
	irq_unlock(key);
}

#ifndef CONFIG_ARCH_HAS_CUSTOM_CPU_IDLE
void arch_cpu_idle(void)
{
	sys_trace_idle();
	moxie_wait(MOXIE_STATUS_IE);
}
#endif

#ifndef CONFIG_ARCH_HAS_CUSTOM_CPU_ATOMIC_IDLE
void arch_cpu_atomic_idle(unsigned int key)
{
	sys_trace_idle();
	moxie_wait(key);
}
#endif

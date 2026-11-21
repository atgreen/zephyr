/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_MOXIE_IRQ_H_
#define ZEPHYR_INCLUDE_ARCH_MOXIE_IRQ_H_

#include <zephyr/arch/moxie/moxie_sr.h>

#ifndef _ASMLANGUAGE
#include <zephyr/types.h>
#include <zephyr/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

void arch_irq_enable(unsigned int irq);
void arch_irq_disable(unsigned int irq);
int arch_irq_is_enabled(unsigned int irq);

/*
 * Writing the status register sets the condition codes as well as the
 * interrupt enable.  Each of these is one asm statement so that the
 * compiler cannot schedule a compare between the read and the write.
 */

static ALWAYS_INLINE unsigned int arch_irq_lock(void)
{
	unsigned int status;
	unsigned int masked;

	__asm__ volatile ("gsr %0, 0\n\t"
			  "mov %1, %0\n\t"
			  "and %1, %2\n\t"
			  "ssr %1, 0"
			  : "=&r" (status), "=&r" (masked)
			  : "r" (~MOXIE_STATUS_IE));

	return status & MOXIE_STATUS_IE;
}

static ALWAYS_INLINE void arch_irq_unlock(unsigned int key)
{
	unsigned int status;

	if (key == 0U) {
		return;
	}

	__asm__ volatile ("gsr %0, 0\n\t"
			  "or %0, %1\n\t"
			  "ssr %0, 0"
			  : "=&r" (status)
			  : "r" (MOXIE_STATUS_IE));
}

static ALWAYS_INLINE bool arch_irq_unlocked(unsigned int key)
{
	return key != 0U;
}

static ALWAYS_INLINE bool arch_cpu_irqs_are_enabled(void)
{
	unsigned int status;

	__asm__ volatile ("gsr %0, 0" : "=r" (status));

	return (status & MOXIE_STATUS_IE) != 0U;
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_MOXIE_IRQ_H_ */

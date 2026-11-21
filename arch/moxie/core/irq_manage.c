/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/sys/sys_io.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/logging/log.h>
#include <kernel_arch_func.h>
#include <ksched.h>

LOG_MODULE_DECLARE(os, CONFIG_KERNEL_LOG_LEVEL);

/*
 * The core has a single level-sensitive interrupt input.  A small
 * interrupt controller in front of it says which sources are pending
 * and lets each be masked.
 */
#define PIC_BASE	DT_REG_ADDR(DT_NODELABEL(pic))
#define PIC_PENDING	(PIC_BASE + 0)
#define PIC_ENABLE	(PIC_BASE + 4)

FUNC_NORETURN void z_irq_spurious(const void *unused)
{
	ARG_UNUSED(unused);

	LOG_ERR("Spurious interrupt detected!");
	z_moxie_fatal_error(K_ERR_SPURIOUS_IRQ, NULL);
}

void arch_irq_enable(unsigned int irq)
{
	unsigned int key = irq_lock();

	sys_write32(sys_read32(PIC_ENABLE) | BIT(irq), PIC_ENABLE);
	irq_unlock(key);
}

void arch_irq_disable(unsigned int irq)
{
	unsigned int key = irq_lock();

	sys_write32(sys_read32(PIC_ENABLE) & ~BIT(irq), PIC_ENABLE);
	irq_unlock(key);
}

int arch_irq_is_enabled(unsigned int irq)
{
	return (sys_read32(PIC_ENABLE) & BIT(irq)) != 0;
}

/* Called from the exception entry, on the interrupt stack. */
void z_moxie_handle_irqs(void)
{
	uint32_t pending;

	while ((pending = sys_read32(PIC_PENDING) & sys_read32(PIC_ENABLE)) != 0) {
		const unsigned int irq = find_lsb_set(pending) - 1;
		const struct _isr_table_entry *const ite = &_sw_isr_table[irq];

		if (IS_ENABLED(CONFIG_TRACING_ISR)) {
			sys_trace_isr_enter();
		}

		ite->isr(ite->arg);

		if (IS_ENABLED(CONFIG_TRACING_ISR)) {
			sys_trace_isr_exit();
		}
	}
}

#ifdef CONFIG_DYNAMIC_INTERRUPTS
int arch_irq_connect_dynamic(unsigned int irq, unsigned int priority,
			     void (*routine)(const void *parameter),
			     const void *parameter, uint32_t flags)
{
	ARG_UNUSED(flags);
	ARG_UNUSED(priority);

	z_isr_install(irq, routine, parameter);
	return irq;
}
#endif /* CONFIG_DYNAMIC_INTERRUPTS */

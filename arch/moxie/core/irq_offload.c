/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/irq_offload.h>

static irq_offload_routine_t offload_routine;
static const void *offload_param;

/* Runs on the interrupt stack, called from the swi exception path. */
void z_moxie_irq_offload_handler(void)
{
	offload_routine(offload_param);
}

void arch_irq_offload(irq_offload_routine_t routine, const void *parameter)
{
	unsigned int key = irq_lock();

	offload_routine = routine;
	offload_param = parameter;
	__asm__ volatile ("swi 0");
	irq_unlock(key);
}

void arch_irq_offload_init(void)
{
}

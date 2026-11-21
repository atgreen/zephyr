/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT moxie_sim_timer

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/irq.h>
#include <zephyr/drivers/timer/system_timer.h>
#include <zephyr/sys/sys_io.h>

#define TIMER_BASE	DT_INST_REG_ADDR(0)
#define TIMER_CTRL	(TIMER_BASE + 0x0)	/* write 1 to run */
#define TIMER_PERIOD	(TIMER_BASE + 0x4)	/* clocks between interrupts */
#define TIMER_ACK	(TIMER_BASE + 0x8)	/* write to drop the interrupt */
#define TIMER_COUNT	(TIMER_BASE + 0xc)	/* free-running clock count */

#define CYCLES_PER_TICK	(sys_clock_hw_cycles_per_sec() / CONFIG_SYS_CLOCK_TICKS_PER_SEC)

static void moxie_sim_timer_isr(const void *arg)
{
	ARG_UNUSED(arg);

	sys_write32(1, TIMER_ACK);
	sys_clock_announce(1);
}

uint32_t sys_clock_elapsed(void)
{
	return 0;
}

uint32_t sys_clock_cycle_get_32(void)
{
	return sys_read32(TIMER_COUNT);
}

static int moxie_sim_timer_init(void)
{
	IRQ_CONNECT(DT_INST_IRQN(0), 0, moxie_sim_timer_isr, NULL, 0);

	sys_write32(CYCLES_PER_TICK, TIMER_PERIOD);
	sys_write32(1, TIMER_CTRL);
	irq_enable(DT_INST_IRQN(0));

	return 0;
}

SYS_INIT(moxie_sim_timer_init, PRE_KERNEL_2, CONFIG_SYSTEM_CLOCK_INIT_PRIORITY);

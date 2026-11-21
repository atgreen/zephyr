/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT moxie_sim_uart

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/sys_io.h>

struct uart_moxie_sim_config {
	mem_addr_t base;
};

static int uart_moxie_sim_poll_in(const struct device *dev, unsigned char *c)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(c);

	return -1;
}

static void uart_moxie_sim_poll_out(const struct device *dev, unsigned char c)
{
	const struct uart_moxie_sim_config *config = dev->config;

	sys_write16(c, config->base);
}

static DEVICE_API(uart, uart_moxie_sim_api) = {
	.poll_in = uart_moxie_sim_poll_in,
	.poll_out = uart_moxie_sim_poll_out,
};

#define UART_MOXIE_SIM_INIT(n)							\
	static const struct uart_moxie_sim_config uart_moxie_sim_config_##n = {	\
		.base = DT_INST_REG_ADDR(n),					\
	};									\
	DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL,				\
			      &uart_moxie_sim_config_##n, PRE_KERNEL_1,		\
			      CONFIG_SERIAL_INIT_PRIORITY, &uart_moxie_sim_api);

DT_INST_FOREACH_STATUS_OKAY(UART_MOXIE_SIM_INIT)

/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel_internal.h>
#include <zephyr/platform/hooks.h>
#include <zephyr/arch/common/init.h>
#include <zephyr/arch/common/xip.h>

/**
 * @brief Prepare to and run C code
 *
 * This routine prepares for the execution of and runs C code.
 */
void z_prep_c(void)
{
	soc_prep_hook();

	arch_bss_zero();
	arch_data_copy();
	z_cstart();
	CODE_UNREACHABLE;
}

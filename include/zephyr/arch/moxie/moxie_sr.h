/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_MOXIE_MOXIE_SR_H_
#define ZEPHYR_INCLUDE_ARCH_MOXIE_MOXIE_SR_H_

/* Special registers, read with gsr and written with ssr. */
#define MOXIE_SR_STATUS		0 /* interrupt enable and condition codes */
#define MOXIE_SR_HANDLER	1 /* exception handler address */
#define MOXIE_SR_CAUSE		2 /* why the last exception was taken */
#define MOXIE_SR_SWI		3 /* number given to the last swi */
#define MOXIE_SR_SAVED_STATUS	4 /* status when the exception was taken */
#define MOXIE_SR_RESUME		5 /* where to resume after the exception */

#define MOXIE_STATUS_IE		0x1

#define MOXIE_CAUSE_IRQ		2
#define MOXIE_CAUSE_SWI		3
#define MOXIE_CAUSE_BRK		5

#endif /* ZEPHYR_INCLUDE_ARCH_MOXIE_MOXIE_SR_H_ */

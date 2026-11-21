/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel_arch_data.h>
#include <gen_offset.h>
#include <kernel_offsets.h>

GEN_OFFSET_SYM(_callee_saved_t, fp);
GEN_OFFSET_SYM(_callee_saved_t, r6);
GEN_OFFSET_SYM(_callee_saved_t, r7);
GEN_OFFSET_SYM(_callee_saved_t, r8);
GEN_OFFSET_SYM(_callee_saved_t, r9);
GEN_OFFSET_SYM(_callee_saved_t, r10);
GEN_OFFSET_SYM(_callee_saved_t, r11);

GEN_OFFSET_STRUCT(arch_esf, r0);
GEN_OFFSET_STRUCT(arch_esf, r1);
GEN_OFFSET_STRUCT(arch_esf, r2);
GEN_OFFSET_STRUCT(arch_esf, r3);
GEN_OFFSET_STRUCT(arch_esf, r4);
GEN_OFFSET_STRUCT(arch_esf, r5);
GEN_OFFSET_STRUCT(arch_esf, r12);
GEN_OFFSET_STRUCT(arch_esf, r13);
GEN_OFFSET_STRUCT(arch_esf, r6);
GEN_OFFSET_STRUCT(arch_esf, sr);
GEN_OFFSET_STRUCT(arch_esf, fp);
GEN_OFFSET_STRUCT(arch_esf, pc);
GEN_OFFSET_STRUCT(arch_esf, static_chain);

GEN_ABSOLUTE_SYM(__struct_arch_esf_SIZEOF, sizeof(struct arch_esf));

GEN_ABS_SYM_END

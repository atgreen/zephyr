# Copyright (c) 2026 Anthony Green
# SPDX-License-Identifier: Apache-2.0

zephyr_ld_options(-Wl,--no-warn-rwx-segments)

set(SOC_LINKER_SCRIPT ${ZEPHYR_BASE}/include/zephyr/arch/moxie/linker.ld CACHE INTERNAL "")

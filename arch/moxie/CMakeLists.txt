# Copyright (c) 2026 Anthony Green
# SPDX-License-Identifier: Apache-2.0

set_property(GLOBAL PROPERTY PROPERTY_OUTPUT_FORMAT "elf32-bigmoxie")

add_subdirectory(core)

zephyr_include_directories(include)

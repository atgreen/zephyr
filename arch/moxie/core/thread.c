/*
 * Copyright (c) 2026 Anthony Green
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <ksched.h>

/*
 * A thread that is not running is parked inside z_moxie_switch(), and
 * comes back to life when that function executes "ret".  ret takes
 * everything it needs from the frame at $fp:
 *
 *   $fp + 0   caller's $fp
 *   $fp + 4   return address
 *   $fp + 8   static chain slot
 *
 * So a new thread just needs one of those frames, returning into
 * z_moxie_thread_start, which moves the entry point and its arguments
 * out of the callee-saved registers they were parked in.
 */
struct init_frame {
	uint32_t fp;
	uint32_t return_address;
	uint32_t static_chain;
};

void arch_new_thread(struct k_thread *thread, k_thread_stack_t *stack,
		     char *stack_ptr, k_thread_entry_t entry,
		     void *p1, void *p2, void *p3)
{
	extern void z_moxie_thread_start(void);

	struct init_frame *frame = (struct init_frame *)Z_STACK_PTR_ALIGN(
		Z_STACK_PTR_TO_FRAME(struct init_frame, stack_ptr));

	ARG_UNUSED(stack);

	frame->fp = 0;
	frame->return_address = (uint32_t)z_moxie_thread_start;
	frame->static_chain = 0;

	thread->callee_saved.fp = (uint32_t)frame;
	thread->callee_saved.r6 = (uint32_t)entry;
	thread->callee_saved.r7 = (uint32_t)p1;
	thread->callee_saved.r8 = (uint32_t)p2;
	thread->callee_saved.r9 = (uint32_t)p3;
	thread->callee_saved.r10 = 0;
	thread->callee_saved.r11 = 0;

	/* our switch handle is the thread pointer itself */
	thread->switch_handle = thread;
}

int arch_coprocessors_disable(struct k_thread *thread)
{
	ARG_UNUSED(thread);

	return -ENOTSUP;
}

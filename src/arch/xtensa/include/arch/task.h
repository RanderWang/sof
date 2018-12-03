/*
 * Copyright (c) 2017, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the Intel Corporation nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
 *
 */

/**
 * \file arch/xtensa/include/arch/task.h
 * \brief Arch task header file
 * \authors Liam Girdwood <liam.r.girdwood@linux.intel.com>
 */

#ifndef __ARCH_TASK_H_
#define __ARCH_TASK_H_

#include <sof/schedule.h>
#include <sof/interrupt.h>
#include <platform/platform.h>
#include <sof/debug.h>
#include <sof/alloc.h>

#define IRQ_TASK_LOW	0
#define IRQ_TASK_HIGH	1
#define IRQ_TASK_MED	2

/** \brief IRQ task data. */
struct irq_task {
	spinlock_t lock;	/**< lock */
	struct list_item list;	/**< list of tasks */
	uint32_t irq;		/**< IRQ level */
};

/**
 * \brief Returns IRQ task data.
 * \return Pointer to pointer of IRQ task data.
 */
struct irq_task **task_irq_get(int level);

/**
 * \brief Retrieves task IRQ level.
 * \param[in,out] task Task data.
 * \return IRQ level.
 */

extern int task_irq[];

static inline uint32_t task_get_irq(struct task *task)
{
	uint32_t irq;

	switch (task->priority) {
	case TASK_PRI_MED + 1 ... TASK_PRI_LOW:
		irq = PLATFORM_IRQ_TASK_LOW;
		break;
	case TASK_PRI_HIGH ... TASK_PRI_MED - 1:
		irq = PLATFORM_IRQ_TASK_HIGH;
		break;
	case TASK_PRI_MED:
	default:
		irq = PLATFORM_IRQ_TASK_MED;
		break;
	}

	return irq;
}

/**
 * \brief Adds task to the list per IRQ level.
 * \param[in,out] task Task data.
 */
static inline void task_set_data(struct task *task)
{
	struct list_item *dst = NULL;
	struct irq_task *irq_task;
	int level;
	uint32_t flags;

	switch (task->priority) {
	case TASK_PRI_MED + 1 ... TASK_PRI_LOW:
		level = IRQ_TASK_LOW;
		break;
	case TASK_PRI_HIGH ... TASK_PRI_MED - 1:
		level = IRQ_TASK_HIGH;
		break;
	case TASK_PRI_MED:
	default:
		if (PLATFORM_IRQ_TASK_LEVEL > 2)
			level = IRQ_TASK_MED;
		else
			level = IRQ_TASK_LOW;
		break;
	}

	irq_task = *task_irq_get(level);
	dst = &irq_task->list;
	spin_lock_irq(&irq_task->lock, flags);
	list_item_append(&task->irq_list, dst);
	spin_unlock_irq(&irq_task->lock, flags);
}

/**
 * \brief Interrupt handler for the IRQ task.
 * \param[in,out] arg IRQ task data.
 */
static void _irq_task(void *arg)
{
	struct irq_task *irq_task = *(struct irq_task **)arg;
	struct list_item *tlist;
	struct list_item *clist;
	struct task *task;
	uint32_t flags;

	spin_lock_irq(&irq_task->lock, flags);

	interrupt_clear(irq_task->irq);

	list_for_item_safe(clist, tlist, &irq_task->list) {

		task = container_of(clist, struct task, irq_list);
		list_item_del(clist);

		spin_unlock_irq(&irq_task->lock, flags);

		if (task->func && task->state == TASK_STATE_RUNNING)
			task->func(task->data);

		schedule_task_complete(task);
		spin_lock_irq(&irq_task->lock, flags);
	}

	spin_unlock_irq(&irq_task->lock, flags);
}

/**
 * \brief Runs task.
 * \param[in,out] task Task data.
 */
static inline void arch_run_task(struct task *task)
{
	uint32_t irq;

	task_set_data(task);
	irq = task_get_irq(task);
	interrupt_set(irq);
}

/**
 * \brief Allocates IRQ tasks.
 */
static inline int arch_allocate_tasks(void)
{
	int i;
	for (i = 0; i < PLATFORM_IRQ_TASK_LEVEL; i++) {
		struct irq_task **task = task_irq_get(i);
		*task = rzalloc(RZONE_SYS, SOF_MEM_CAPS_RAM, sizeof(**task));

		list_init(&((*task)->list));
		spinlock_init(&((*task)->lock));
		(*task)->irq = task_irq[i];
	}

	return 0;
}

/**
 * \brief Frees IRQ tasks.
 */
static inline void arch_free_tasks(void)
{
	uint32_t flags;
	int i;
/* TODO: do not want to free the tasks, just the entire heap */

	/* free IRQ low task */
	for(i = 0; i < PLATFORM_IRQ_TASK_LEVEL; i++) {
		struct irq_task **task = task_irq_get(i);

		spin_lock_irq(&(*task)->lock, flags);
		interrupt_disable(task_irq[i]);
		interrupt_unregister(task_irq[i]);
		list_item_del(&(*task)->list);
		spin_unlock_irq(&(*task)->lock, flags);
	}
}

/**
 * \brief Assigns IRQ tasks to interrupts.
 */
static inline int arch_assign_tasks(void)
{
	int i;
	for (i = 0; i < PLATFORM_IRQ_TASK_LEVEL; i++) {
		interrupt_register(task_irq[i], IRQ_AUTO_UNMASK, _irq_task,
				   task_irq_get(i));
		interrupt_enable(task_irq[i]);
	}

	return 0;
}

#endif

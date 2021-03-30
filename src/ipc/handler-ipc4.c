// SPDX-License-Identifier: BSD-3-Clause
//
// Copyright(c) 2021 Intel Corporation. All rights reserved.
//
// Author: Rander Wang <rander.wang@linux.intel.com>
/*
 * IPC (InterProcessor Communication) provides a method of two way
 * communication between the host processor and the DSP. The IPC used here
 * utilises a shared mailbox and door bell between the host and DSP.
 *
 */

#include <sof/audio/buffer.h>
#include <sof/audio/component_ext.h>
#include <sof/audio/pipeline.h>
#include <sof/common.h>
#include <sof/debug/gdb/gdb.h>
#include <sof/debug/panic.h>
#include <sof/drivers/idc.h>
#include <sof/drivers/interrupt.h>
#include <sof/drivers/ipc.h>
#include <sof/drivers/timer.h>
#include <sof/lib/alloc.h>
#include <sof/lib/clk.h>
#include <sof/lib/cache.h>
#include <sof/lib/cpu.h>
#include <sof/lib/dai.h>
#include <sof/lib/dma.h>
#include <sof/lib/mailbox.h>
#include <sof/lib/memory.h>
#include <sof/lib/mm_heap.h>
#include <sof/lib/pm_runtime.h>
#include <sof/list.h>
#include <sof/math/numbers.h>
#include <sof/platform.h>
#include <sof/schedule/schedule.h>
#include <sof/schedule/task.h>
#include <sof/spinlock.h>
#include <sof/string.h>
#include <sof/trace/dma-trace.h>
#include <sof/trace/trace.h>
#include <ipc/control.h>
#include <ipc/dai.h>
#include <ipc/debug.h>
#include <ipc/header2.h>
#include <ipc/pm.h>
#include <ipc/stream.h>
#include <ipc/topology.h>
#include <ipc/trace.h>
#include <user/trace.h>
#include <ipc/probe.h>
#include <sof/probe/probe.h>

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/*
 * Global IPC Operations.
 */
static int sof_ipc4_create_pipeline(uint32_t dr, uint32_t dd)
{
	tr_err(&ipc_tr, "ipc4 create pipeline %x:%x", dr, dd);

	return 0;
}

static int sof_ipc4_delete_pipeline(uint32_t dr, uint32_t dd)
{
	tr_err(&ipc_tr, "ipc4 delete pipeline %x:%x", dr, dd);

	return 0;
}

static int sof_ipc4_set_pipeline_state(uint32_t dr, uint32_t dd)
{
	tr_err(&ipc_tr, "ipc4 set pipeline state %x:%x", dr, dd);

	return 0;
}

int sof_ipc4_process_glb_message(uint32_t dr, uint32_t dd)
{
	uint32_t type;
	int ret;

	type = (dr & SOF_IPC4_MSG_TYPE_MASK) >> SOF_IPC4_MSG_TYPE_SHIFT;
	switch (type) {
	case SOF_IPC4_GLB_BOOT_CONFIG:
	case SOF_IPC4_GLB_ROM_CONTROL:
	case SOF_IPC4_GLB_IPCGATEWAY_CMD:
	case SOF_IPC4_GLB_START_RTOS_EDF_TASK:
	case SOF_IPC4_GLB_STOP_RTOS_EDF_TASK:
	case SOF_IPC4_GLB_PERF_MEASUREMENTS_CMD:
	case SOF_IPC4_GLB_CHAIN_DMA:
	case SOF_IPC4_GLB_LOAD_MULTIPLE_MODULES:
	case SOF_IPC4_GLB_UNLOAD_MULTIPLE_MODULES:
		tr_err(&ipc_tr, "unsupported ipc message type %d", type);
		break;

	/* pipeline settings */
	case SOF_IPC4_GLB_CREATE_PIPELINE:
		ret = sof_ipc4_create_pipeline(dr, dd);
		break;
	case SOF_IPC4_GLB_DELETE_PIPELINE:
		ret = sof_ipc4_delete_pipeline(dr, dd);
		break;
	case SOF_IPC4_GLB_SET_PIPELINE_STATE:
		ret = sof_ipc4_set_pipeline_state(dr, dd);
		break;

	case SOF_IPC4_GLB_GET_PIPELINE_STATE:
	case SOF_IPC4_GLB_GET_PIPELINE_CONTEXT_SIZE:
	case SOF_IPC4_GLB_SAVE_PIPELINE:
	case SOF_IPC4_GLB_RESTORE_PIPELINE:
		tr_err(&ipc_tr, "unsupported ipc message type %d", type);
		break;

	/* Loads library (using Code Load or HD/A Host Output DMA) */
	case SOF_IPC4_GLB_LOAD_LIBRARY:
	case SOF_IPC4_GLB_INTERNAL_MESSAGE:
		tr_err(&ipc_tr, "unsupported ipc message type %d", type);
		break;

	/* Notification (FW to SW driver) */
	case SOF_IPC4_GLB_NOTIFICATION:
		tr_err(&ipc_tr, "unsupported ipc message type %d", type);
		break;

	default:
		tr_err(&ipc_tr, "unsupported ipc message type %d", type);
		ret = -EINVAL;
		break;
	}

	return ret;
}

/*
 * Module IPC perations.
 */

static int sof_ipc4_init_module(uint32_t dd, uint32_t dr)
{
	return 0;
}

static int sof_ipc4_bind_module(uint32_t dd, uint32_t dr)
{
	return 0;
}

static int sof_ipc4_unbind_module(uint32_t dd, uint32_t dr)
{
	return 0;
}

static int sof_ipc4_set_large_config_module(uint32_t dd, uint32_t dr)
{
	return 0;
}

int sof_ipc4_process_module_message(uint32_t dd, uint32_t dr)
{
	uint32_t type;
	int ret;

	type = (dd & SOF_IPC4_MSG_TYPE_MASK) >> SOF_IPC4_MSG_TYPE_SHIFT;
	switch (type) {
	case SOF_MOD_INIT_INSTANCE:
		ret = sof_ipc4_init_module(dd, dr);
		break;
	case SOF_MOD_CONFIG_GET:
		break;
	case SOF_MOD_CONFIG_SET:
		break;
	case SOF_MOD_LARGE_CONFIG_GET:
		break;
	case SOF_MOD_LARGE_CONFIG_SET:
		ret = sof_ipc4_set_large_config_module(dd, dr);
		break;
	case SOF_MOD_BIND:
		ret = sof_ipc4_bind_module(dd, dr);
		break;
	case SOF_MOD_UNBIND:
		ret = sof_ipc4_unbind_module(dd, dr);
		break;
	case SOF_MOD_SET_DX:
		break;
	case SOF_MOD_SET_D0IX:
		break;
	case SOF_MOD_ENTER_MODULE_RESTORE:
		break;
	case SOF_MOD_EXIT_MODULE_RESTORE:
		break;
	case SOF_MOD_DELETE_INSTANCE:
		break;
	default:
		break;
	}

	return ret;
}

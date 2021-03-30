/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-3-Clause) */
/*
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * Copyright(c) 2020 Intel Corporation. All rights reserved.
 */

#ifndef __INCLUDE_SOUND_SOF_HEADER2_H__
#define __INCLUDE_SOUND_SOF_HEADER2_H__


/** \addtogroup sof_uapi uAPI
 *  SOF uAPI specification.
 *  @{
 */

/*
 * IPC4 messages have two 32 bit identifier made up as follows :-
 *
 * header - msg type, msg id, msg direction ...
 * extension - extra params such as msg data size in mailbox
 *
 * These are sent at the start of the IPC message in the mailbox. Messages
 * should not be sent in the doorbell (special exceptions for firmware).
 */

enum sof_ipc4_msg_target {
	FW_GLB_MSG = 0,
	MODULE_MSG = 1
};

/* Message direction */
enum sof_ipc4_msg_dir {
	MSG_REQUEST = 0,
	MSG_REPLY = 1,
};

enum sof_ipc4_global_msg {
	SOF_IPC4_GLB_BOOT_CONFIG = 0,
	SOF_IPC4_GLB_ROM_CONTROL = 1,
	SOF_IPC4_GLB_IPCGATEWAY_CMD = 2,

	SOF_IPC4_GLB_START_RTOS_EDF_TASK = 3,
	SOF_IPC4_GLB_STOP_RTOS_EDF_TASK = 4,

	SOF_IPC4_GLB_PERF_MEASUREMENTS_CMD = 13,
	SOF_IPC4_GLB_CHAIN_DMA = 14,

	SOF_IPC4_GLB_LOAD_MULTIPLE_MODULES = 15,
	SOF_IPC4_GLB_UNLOAD_MULTIPLE_MODULES = 16,

	SOF_IPC4_GLB_CREATE_PIPELINE = 17,
	SOF_IPC4_GLB_DELETE_PIPELINE = 18,
	SOF_IPC4_GLB_SET_PIPELINE_STATE = 19,
	SOF_IPC4_GLB_GET_PIPELINE_STATE = 20,
	SOF_IPC4_GLB_GET_PIPELINE_CONTEXT_SIZE = 21,
	SOF_IPC4_GLB_SAVE_PIPELINE = 22,
	SOF_IPC4_GLB_RESTORE_PIPELINE = 23,

	SOF_IPC4_GLB_LOAD_LIBRARY = 24,
	SOF_IPC4_GLB_INTERNAL_MESSAGE = 26,

	SOF_IPC4_GLB_NOTIFICATION = 27,
	SOF_IPC4_GLB_MAX_IXC_MESSAGE_TYPE = 31
};

enum sof_ipc4_pipeline_state {
	SOF_IPC4_PIPE_UNUSED = 0,
	SOF_IPC4_PIPE_INVALID = 1,
	SOF_IPC4_PIPE_RESET = 2,
	SOF_IPC4_PIPE_PAUSED = 3,
	SOF_IPC4_PIPE_RUNNING = 4,
	SOF_IPC4_PIPE_EOS = 5
};

/* global common ipc msg */
#define SOF_IPC4_MSG_TARGET_SHIFT	30
#define SOF_IPC4_MSG_TARGET_MASK	BIT(30)
#define SOF_IPC4_MSG_TARGET(x)	((x) << SOF_IPC4_MSG_TARGET_SHIFT)

#define SOF_IPC4_MSG_DIR_SHIFT	29
#define SOF_IPC4_MSG_DIR_MASK	BIT(29)
#define SOF_IPC4_MSG_DIR(x)	((x) << SOF_IPC4_MSG_DIR_SHIFT)

#define SOF_IPC4_MSG_TYPE_SHIFT		24
#define SOF_IPC4_MSG_TYPE_MASK		0x1F000000
#define SOF_IPC4_MSG_TYPE(x)	((x) << SOF_IPC4_MSG_TYPE_SHIFT)

/* pipeline creation ipc msg */
#define SOF_IPC4_GLB_PIPE_INSTANCE_SHIFT	16
#define SOF_IPC4_GLB_PIPE_INSTANCE_MASK	0xFF0000
#define SOF_IPC4_GLB_PIPE_INSTANCE_ID(x)	((x) << SOF_IPC4_GLB_PIPE_INSTANCE_SHIFT)

#define SOF_IPC4_GLB_PIPE_PRIORITY_SHIFT	11
#define SOF_IPC4_GLB_PIPE_PRIORITY_MASK	0xF800
#define SOF_IPC4_GLB_PIPE_PRIORITY(x)	((x) << SOF_IPC4_GLB_PIPE_PRIORITY_SHIFT)

#define SOF_IPC4_GLB_PIPE_MEM_SIZE_SHIFT	0
#define SOF_IPC4_GLB_PIPE_MEM_SIZE_MASK	0x7FF
#define SOF_IPC4_GLB_PIPE_MEM_SIZE(x)	((x) << SOF_IPC4_GLB_PIPE_MEM_SIZE_SHIFT)

#define SOF_IPC4_GLB_PIPE_EXT_LP_SHIFT	0
#define SOF_IPC4_GLB_PIPE_EXT_LP_MASK	BIT(0)
#define SOF_IPC4_GLB_PIPE_EXT_LP(x)	((x) << SOF_IPC4_GLB_PIPE_EXT_LP_SHIFT)

/* pipeline set state ipc msg */
#define SOF_IPC4_GLB_PIPE_STATE_TYPE_SHIFT	24
#define SOF_IPC4_GLB_PIPE_STATE_TYPE_MASK	0x1F000000
#define SOF_IPC4_GLB_PIPE_STATE_TYPE(x)	((x) << SOF_IPC4_GLB_PIPE_STATE_TYPE_SHIFT)

#define SOF_IPC4_GLB_PIPE_STATE_ID_SHIFT	16
#define SOF_IPC4_GLB_PIPE_STATE_ID_MASK	0xFF0000
#define SOF_IPC4_GLB_PIPE_STATE_ID(x)	((x) << SOF_IPC4_GLB_PIPE_STATE_ID_SHIFT)

#define SOF_IPC4_GLB_PIPE_STATE_SHIFT	0
#define SOF_IPC4_GLB_PIPE_STATE_MASK	0xFFFF
#define SOF_IPC4_GLB_PIPE_STATE(x) ((x) << SOF_IPC4_GLB_PIPE_STATE_SHIFT)

/* ipc4 notification msg */

enum sof_ipc4_notification_type {
	SOF_IPC4_NOTIFY_PHRASE_DETECTED = 4,
	SOF_IPC4_NOTIFY_RESOURCE_EVENT = 5,
	SOF_IPC4_NOTIFY_LOG_BUFFER_STATUS = 6,
	SOF_IPC4_NOTIFY_TIMESTAMP_CAPTURED = 7,
	SOF_IPC4_NOTIFY_FW_READY = 8,
	SOF_IPC4_FW_AUD_CLASS_RESULT = 9,
	SOF_IPC4_EXCEPTION_CAUGHT = 10,
	SOF_IPC4_MODULE_NOTIFICATION = 12,
	SOF_IPC4_UAOL_RSVD_ = 13,
	SOF_IPC4_PROBE_DATA_AVAILABLE = 14,
	SOF_IPC4_WATCHDOG_TIMEOUT = 15,
	SOF_IPC4_MANAGEMENT_SERVICE = 16,
};

#define SOF_IPC4_GLB_NOTIFY_DIR_MASK	BIT(29)
#define SOF_IPC4_REPLY_STATUS_MASK	0xFFFFFF

#define SOF_IPC4_GLB_NOTIFY_TYPE_SHIFT	16
#define SOF_IPC4_GLB_NOTIFY_TYPE_MASK	0xFF
#define SOF_IPC4_GLB_NOTIFY_TYPE(x)		(((x) >> SOF_IPC4_GLB_NOTIFY_TYPE_SHIFT) \
						& SOF_IPC4_GLB_NOTIFY_TYPE_MASK)

#define SOF_IPC4_GLB_NOTIFY_MSG_TYPE_SHIFT	24
#define SOF_IPC4_GLB_NOTIFY_MSG_TYPE_MASK	0x1F
#define SOF_IPC4_GLB_NOTIFY_MSG_TYPE(x)	(((x) >> SOF_IPC4_GLB_NOTIFY_MSG_TYPE_SHIFT)	\
						& SOF_IPC4_GLB_NOTIFY_MSG_TYPE_MASK)

enum sof_ipc4_module_type {
	SOF_MOD_INIT_INSTANCE		 = 0,
	SOF_MOD_CONFIG_GET	 = 1,
	SOF_MOD_CONFIG_SET	 = 2,
	SOF_MOD_LARGE_CONFIG_GET	 = 3,
	SOF_MOD_LARGE_CONFIG_SET	 = 4,
	SOF_MOD_BIND				 = 5,
	SOF_MOD_UNBIND				 = 6,
	SOF_MOD_SET_DX				 = 7,
	SOF_MOD_SET_D0IX			 = 8,
	SOF_MOD_ENTER_MODULE_RESTORE = 9,
	SOF_MOD_EXIT_MODULE_RESTORE  = 10,
	SOF_MOD_DELETE_INSTANCE	= 11,
};

/* common module ipc msg */
#define SOF_IPC4_MOD_INSTANCE_SHIFT	16
#define SOF_IPC4_MOD_INSTANCE_MASK	0xFF0000
#define SOF_IPC4_MOD_INSTANCE(x)	((x) << SOF_IPC4_MOD_INSTANCE_SHIFT)

#define SOF_IPC4_MOD_ID_SHIFT	0
#define SOF_IPC4_MOD_ID_MASK	0xFFFF
#define SOF_IPC4_MOD_ID(x)	((x) << SOF_IPC4_MOD_ID_SHIFT)

/* init module ipc msg */
#define SOF_IPC4_MOD_EXT_PARAM_SIZE_SHIFT	0
#define SOF_IPC4_MOD_EXT_PARAM_SIZE_MASK	0xFFFF
#define SOF_IPC4_MOD_EXT_PARAM_SIZE(x)	((x) << SOF_IPC4_MOD_EXT_PARAM_SIZE_SHIFT)

#define SOF_IPC4_MOD_EXT_PPL_ID_SHIFT	16
#define SOF_IPC4_MOD_EXT_PPL_ID_MASK	0xFF0000
#define SOF_IPC4_MOD_EXT_PPL_ID(x)	((x) << SOF_IPC4_MOD_EXT_PPL_ID_SHIFT)

#define SOF_IPC4_MOD_EXT_CORE_ID_SHIFT	24
#define SOF_IPC4_MOD_EXT_CORE_ID_MASK	0xF000000
#define SOF_IPC4_MOD_EXT_CORE_ID(x)	((x) << SOF_IPC4_MOD_EXT_CORE_ID_SHIFT)

#define SOF_IPC4_MOD_EXT_DOMAIN_SHIFT	28
#define SOF_IPC4_MOD_EXT_DOMAIN_MASK	BIT(28)
#define SOF_IPC4_MOD_EXT_DOMAIN(x)	((x) << SOF_IPC4_MOD_EXT_DOMAIN_SHIFT)

/*  bind/unbind module ipc msg */
#define SOF_IPC4_MOD_EXT_DST_MOD_ID_SHIFT	0
#define SOF_IPC4_MOD_EXT_DST_MOD_ID_MASK	0xFFFF
#define SOF_IPC4_MOD_EXT_DST_MOD_ID(x)	((x) << SOF_IPC4_MOD_EXT_DST_MOD_ID_SHIFT)

#define SOF_IPC4_MOD_EXT_DST_MOD_INSTANCE_SHIFT	16
#define SOF_IPC4_MOD_EXT_DST_MOD_INSTANCE_MASK	0xFF0000
#define SOF_IPC4_MOD_EXT_DST_MOD_INSTANCE(x)	((x) << SOF_IPC4_MOD_EXT_DST_MOD_INSTANCE_SHIFT)

#define SOF_IPC4_MOD_EXT_DST_MOD_QUEUE_ID_SHIFT	24
#define SOF_IPC4_MOD_EXT_DST_MOD_QUEUE_ID_MASK	0x7000000
#define SOF_IPC4_MOD_EXT_DST_MOD_QUEUE_ID(x)	((x) << SOF_IPC4_MOD_EXT_DST_MOD_QUEUE_ID_SHIFT)

#define SOF_IPC4_MOD_EXT_SRC_MOD_QUEUE_ID_SHIFT	27
#define SOF_IPC4_MOD_EXT_SRC_MOD_QUEUE_ID_MASK	0x38000000
#define SOF_IPC4_MOD_EXT_SRC_MOD_QUEUE_ID(x)	((x) << SOF_IPC4_MOD_EXT_SRC_MOD_QUEUE_ID_SHIFT)

#define MOD_ENABLE_LOG	6
#define MOD_SYSTEM_TIME	20

/* set module large config */
#define SOF_IPC4_MOD_EXT_MSG_SIZE_SHIFT	0
#define SOF_IPC4_MOD_EXT_MSG_SIZE_MASK	0xFFFFF
#define SOF_IPC4_MOD_EXT_MSG_SIZE(x)	((x) << SOF_IPC4_MOD_EXT_MSG_SIZE_SHIFT)

#define SOF_IPC4_MOD_EXT_MSG_PARAM_ID_SHIFT	20
#define SOF_IPC4_MOD_EXT_MSG_PARAM_ID_MASK	0xFF00000
#define SOF_IPC4_MOD_EXT_MSG_PARAM_ID(x)	((x) << SOF_IPC4_MOD_EXT_MSG_PARAM_ID_SHIFT)

#define SOF_IPC4_MOD_EXT_MSG_LAST_BLOCK_SHIFT	28
#define SOF_IPC4_MOD_EXT_MSG_LAST_BLOCK_MASK	BIT(28)
#define SOF_IPC4_MOD_EXT_MSG_LAST_BLOCK(x)	((x) << SOF_IPC4_MOD_EXT_MSG_LAST_BLOCK_SHIFT)

#define SOF_IPC4_MOD_EXT_MSG_FIRST_BLOCK_SHIFT	29
#define SOF_IPC4_MOD_EXT_MSG_FIRST_BLOCK_MASK	BIT(29)
#define SOF_IPC4_MOD_EXT_MSG_FIRST_BLOCK(x)	((x) << SOF_IPC4_MOD_EXT_MSG_FIRST_BLOCK_SHIFT)

#endif

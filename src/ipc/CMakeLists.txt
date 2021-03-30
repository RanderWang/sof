# SPDX-License-Identifier: BSD-3-Clause

add_local_sources(sof
	ipc.c
	handler.c
)

if (CONFIG_CAVS_IPC4)
	add_local_sources(sof
		handler-ipc4.c)
endif()

if (CONFIG_TRACE)
	add_local_sources(sof
		dma-copy.c)
endif()

if (CONFIG_LIBRARY)
	return()
endif()

if (CONFIG_HOST_PTABLE)
	add_local_sources(sof
		ipc-host-ptable.c)
endif()

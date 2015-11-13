/*
 * Copyright (c) 2011
 *  Freescale Semiconductor Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Freescale Semiconductor Inc nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */


/******************************************************************************

 @File          bsc913x_heterogeneous_debug_print.h

 @Description   Debug Print Tracing header file.

*//***************************************************************************/

#ifndef __BSC913X_HETEROGENEOUS_DEBUG_PRINT_H
#define __BSC913X_HETEROGENEOUS_DEBUG_PRINT_H

#include "bsc913x_heterogeneous_common.h"

_OS_BEGIN_EXTERN_C
#include "bsc913x_heterogeneous_mem.h"

#define NUM_OF_DBGP_TABLES_PER_SC           2
/**< The number of tables used by debug print for each core */

#define NUM_OF_DBGP_SC_CORES                1
/**<  The number of SC cores supported by debug print */


typedef struct {
	uint64_t 	system_clock;
	uint64_t 	DSP_clock;
} debug_print_clocks_t;

/**************************************************************************//**
 @Description   SmartDSP Debug print structure

*//***************************************************************************/

typedef struct {

	void                *buffer_location;
	/**< Pointer to the base address of the SC VTB */
	uint32_t 	            segment_size;
	/**< Size of each segment in VTB */
	uint32_t 	            num_of_segments;
	/**< Number of VTB segments */
	os_het_tracker_t        tracker;
	/**< Tracker for segment number; SC client is the producer and
		PA engine is the consumer */
	debug_print_clocks_t    sample_clock;
	/**< Clock synchronization */
	uint32_t 	            overflow;
	/**< Overflow indicator */
	void	 	            *segment_info;
	/**< segmet information */
	uint32_t 	            reserved[4];

} os_het_debug_print_sc_t;


typedef struct {

    os_het_mem_t            pa_debug_print_shared;
    /**< PA Debug Print shared memory region */
    os_het_mem_t            sc_debug_print_shared;
    /**< SC Debug Print shared memory region */
    uint32_t                sc_array_size;
    /**< Number of entries in sc_debug_print[];
	Should be NUM_OF_DBGP_TABLES_PER_SC*NUM_OF_DBGP_SC_CORES */
    os_het_debug_print_sc_t (*sc_debug_print)[];
   /**< SC debug print main structure, size of */
} os_het_debug_print_t;


_OS_END_EXTERN_C

#endif /*__BSC913X_HETEROGENEOUS_DEBUG_PRINT_H*/

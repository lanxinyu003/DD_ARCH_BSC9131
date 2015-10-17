/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_memmanager.c,v 1.33 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_memmanager.c,v $
 $Revision: 1.33 $
******************************************************************************/

/******************************************************************************
 
 @File          os_memmanager.c

 @Description   OS Memory Manager functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"


extern uint16_t         g_total_mem_parts;
extern os_mem_part_t    **g_mem_part_list;


/*****************************************************************************/
os_mem_part_t * osMemPartCreate(uint32_t        block_size, // __attribute__((noalias)),
                                uint32_t        num_of_blocks,
                                uint8_t         *data_address,
                                uint32_t        alignment,
                                uint16_t        buffer_offset,
                                os_mem_part_t   *mem_part
#if (OS_MULTICORE == 1)
                                ,bool shared
#endif
                                )
{
    uint32_t    i;
    uint8_t     j, idx = 0xff;
    uint8_t     **mem_part_array_addr; /* Compiler optimization */


 
    /* align addresses as needed */
    data_address = (uint8_t *)ALIGN_ADDRESS(data_address, alignment);
    mem_part = (os_mem_part_t *)ALIGN_ADDRESS(mem_part, ALIGNED_4_BYTES);
 
    /* initialize partition manager and data blocks */
    osSpinLockInitialize(&mem_part->guard);
#if (OS_MULTICORE == 1)
    /* disable guard on local partition */
    mem_part->shared = shared;
    if(!shared)
        osSpinLockDisable(&mem_part->guard);
#endif
    mem_part->curr_index = (int32_t)(num_of_blocks - 1);
    mem_part->block_size = block_size - buffer_offset;
    mem_part->buffer_offset = buffer_offset;
    mem_part->array_addr = (volatile uint8_t **)
                           ((uint32_t)mem_part + sizeof(os_mem_part_t));
    mem_part_array_addr = (uint8_t **)mem_part->array_addr;
 
    OS_ASSERT_COMPILER_COND(num_of_blocks > 0);
    for (i=0; i < num_of_blocks; i++)
    {
        mem_part_array_addr[i] = data_address + buffer_offset;
        data_address += ALIGN_SIZE(block_size, alignment);
    }
 
    osHwiSwiftDisable();
 
    OS_ASSERT_COMPILER_COND(g_total_mem_parts > 0);
    /* store partition in the list of active partitions */
    for (j=0; j < g_total_mem_parts; j++)
    {
        if (g_mem_part_list[j] == mem_part)
        {
            idx = j;
            break;
        }
        if (g_mem_part_list[j] == NULL)
        {
            idx = j;
        }
    }
 
    if (idx < g_total_mem_parts)
        g_mem_part_list[idx] = mem_part;
 
    osHwiSwiftEnable();
 
    return mem_part;
}




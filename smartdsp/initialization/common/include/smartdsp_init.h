/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/06/30 10:47:12 $
 $Id: smartdsp_init.h,v 1.39 2013/06/30 10:47:12 b42006 Exp $
 $Source: /cvsdata/SmartDSP/initialization/common/include/smartdsp_init.h,v $
 $Revision: 1.39 $
******************************************************************************/

/**************************************************************************//**
 
 @File          smartdsp_init.h


 @Description   SmartDSP OS Initialization header.

*//***************************************************************************/

#ifndef __SMARTDSP_INIT_H
#define __SMARTDSP_INIT_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#define OS_LOCAL_HEAP_MNGMNT_DEFAULT_SIZE 1000

#if (OS_MULTICORE == 1)
#define OS_SHARED_HEAP_MNGMNT_DEFAULT_SIZE 1000
#else
#define OS_SHARED_HEAP_MNGMNT_DEFAULT_SIZE 0
#endif

/**************************************************************************//**
 @Group         os_init_id   SmartDSP OS Initialization

 @Description   These functions are called via osInitialize()
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osInitializeKernel

 @Description   Initializes the OS kernel.

                This function initializes various parameters of the OS,
                using the given configuration parameters. In addition,
                it aligns the stack as necessary.
 
 @Param         system_clock        - The system clock in MHz.
 @Param         num_of_cores        - Number of active cores (multicore only).
 @Param         max_num_of_cores    - Maximum number of cores (multicore only).
 @Param         id_of_master_core   - ID of the master core (multicore only).

 @Return        OS_SUCCESS

 @Cautions      Must be called before osStart().
 
*//***************************************************************************/
#if (OS_MULTICORE == 1)
os_status   osInitializeKernel(uint32_t system_clock,
                               uint8_t  num_of_cores,
                               uint8_t  max_num_of_cores,
                               uint8_t  id_of_master_core);
#else
os_status   osInitializeKernel(uint32_t system_clock);
#endif // OS_MULTICORE


/**************************************************************************//**
 @Function      osActivate

 @Description   Starts the OS operation.

                This function receives a pointer to a background task that
                is ready to run and switches into that task. If the background
                task runs infinitely, osActivate() never returns.
 
 @Param         background_task - The background task to run.

 @Retval        OS_FAIL - The background task has returned;

 @Cautions      osInitialize() must be called before osActivate();
                Don't use temporary variables here, since this function does
                not return, and its stack will not be cleared.
 
*//***************************************************************************/
os_status   osActivate(os_background_task_function background_task);


/**************************************************************************//**
 @Function      osArchInitialize

 @Description   Architecture-specific general initialization.

                This function should be implemented per architecture.
 
 @Return        OS_SUCCESS on success; OS_FAIL otherwise.

*//***************************************************************************/
os_status   osArchInitialize();


/**************************************************************************//**
 @Function      osArchDevicesInitialize

 @Description   Architecture-specific devices initialization.

                This function should be implemented per architecture.
 
 @Return        OS_SUCCESS on success; OS_FAIL otherwise.

*//***************************************************************************/
os_status   osArchDevicesInitialize();

/**************************************************************************//**
    Heap description
*//***************************************************************************/
typedef struct
{
    volatile uint32_t*  mem_start;
    volatile uint32_t   mem_size;
    volatile uint32_t   mem_type;
 
    volatile void*  busy_list_head;
    volatile void*  free_list_head;
} os_mem_heap_t;

/**************************************************************************//**
 @Function      osMemInitialize

 @Description   Initializes the memory management module.
 
 @Param         os_mem_heap_local   - Pointer to list of local heaps.
 @Param         num_of_local_heaps  - Number of local heaps.
 @Param         os_mem_heap_shared  - Pointer to list of shared heaps (multicore only).
 @Param         num_of_shared_heaps - Number of shared heaps (multicore only).
 
 @Retval        OS_SUCCESS          - Memory management module was
                                      initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status osMemInitialize(os_mem_heap_t os_mem_heap_local[], int num_of_local_heaps
#if (OS_MULTICORE == 1)
		, os_mem_heap_t os_mem_heap_shared[], int  num_of_shared_heaps
#endif
		);

/**************************************************************************//**
 @Function      osMemLocalInitialize

 @Description   Initializes the local memory management module.
 
 @Param         total_mem_parts     - Number of memory partitions in the system.
 @Param         local_mngmnt_space  - Pointer to preallocated management memory space for local heaps.
 @param         local_mngmnt_size   - Amount of memory space preallocated for local memory internal management (Bytes).

 @Retval        OS_SUCCESS          - Memory management module was
                                      initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status osMemLocalInitialize(uint16_t  total_mem_parts,
                          uint8_t * local_mngmnt_space,
                          uint32_t local_mngmnt_size);

#if (OS_MULTICORE == 1)
/**************************************************************************//**
 @Function      osMemSharedInitialize

 @Description   Initializes the shared memory management module.
 
 @Param         shared_mngmnt_space - Pointer to preallocated management memory space for shared heaps.
 @param         shared_mngmnt_size  - Amount of memory space preallocated for shared memory internal management (Bytes).
 
 @Retval        OS_SUCCESS          - Memory management module was
                                      initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status osMemSharedInitialize(uint8_t * shared_mngmnt_space,
                            uint32_t shared_mngmnt_size);
#endif

/**************************************************************************//**
 @Function      osHwiInitialize

 @Description   Initializes the hardware interrupts module.

 @Param         int_vec_add - Base address of interrupts vector.
 
 @Return        OS_SUCCESS on success; OS_FAIL otherwise.

 @Cautions      Must be called before calling osStart() and before creating
                a hardware interrupt.

*//****************************************************************************/
//os_status   osHwiInitialize(void *int_vec_add);
os_status   osHwiInitialize(void);

/**************************************************************************//**
 @Function      osTickInitialize

 @Description   Initializes the OS Tick module.
 
 @Param         tick_parameter  - Tick value (architecture-specific), must be
                                  greater than zero.

 @Return        OS_SUCCESS

 @Cautions      The tick value parameter MUST be greater than zero !

*//***************************************************************************/
os_status   osTickInitialize(uint32_t tick_parameter);


/**************************************************************************//**
 @Function      osTickStart

 @Description   Starts the OS tick functionality.
 
 @Param         priority - OS Tick priority

 @Return        OS_SUCCESS on success of osTickSetup(); OS_FAIL otherwise.

*//***************************************************************************/
os_status   osTickStart(os_swi_priority priority);


/**************************************************************************//**
 @Function      osSwiInitialize

 @Description   Initializes the software interrupts module.
 
 @Param         total_swi - Number of software interrupts in the system.

 @Retval        OS_SUCCESS          - Software interrupts module was
                                      initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status   osSwiInitialize(uint16_t total_swi);


/**************************************************************************//**
 @Function      osTimerInitialize

 @Description   Initializes the software timers module.
 
 @Param         total_timers - Number of software timers in the system.

 @Retval        OS_SUCCESS          - Software timers module was initialized
                                      successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status   osTimerInitialize(uint16_t total_timers);


/**************************************************************************//**
 @Function      osHwTimerInitialize

 @Description   Initializes the hardware timers module.
 
 @Return        OS_SUCCESS
 
*//***************************************************************************/
os_status   osHwTimerInitialize();


/**************************************************************************//**
 @Function      osMessageInitialize

 @Description   Initializes the intercore messages module.
 
 @Param         total_messages - Number of intercore messages in the system.

 @Retval        OS_SUCCESS              - The intercore messages module was
                                          initialized successfully.
 @Retval        OS_ERR_NO_MEMORY        - Not enough memory; operation failed.
 @Retval        OS_ERR_MSG_NUM_LIMIT    - Requested number of messages is too
                                          high (unsupported); operation failed.

*//***************************************************************************/
os_status   osMessageInitialize(uint16_t total_messages);

/**************************************************************************//**
 @Function      osMultiCoreSyncInitialize

 @Description   Initializes the multicore synchronization module.
 
 @Retval        OS_SUCCESS          - The multicore synchronization module was
                                      initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - For master core only: not enough memory;
                                      operation failed. other cores will enter
                                      a busy wait state until the master core
                                      is successful.
 
*//***************************************************************************/
os_status   osMultiCoreSyncInitialize();

/**************************************************************************//**
 @Function      sysInit

 @Description   Initializes the multicore synchronization module.
 
 @Retval        OS_SUCCESS          - The multicore synchronization module was
                                      initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - For master core only: not enough memory;
                                      operation failed. other cores will enter
                                      a busy wait state until the master core
                                      is successful.
 
*//***************************************************************************/
os_status   sysInit();

/**************************************************************************//**
 @Function      osQueueInitialize

 @Description   Initializes the OS Queues.
 
 @Param         total_queues    - Number of queues in the system.
 @Param         total_squeues   - Number of shared queues in the system
                                  (multicore only).

 @Retval        OS_SUCCESS          - Queues were initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
#if (OS_MULTICORE == 1)
os_status   osQueueInitialize(uint16_t total_queues, uint16_t total_squeues);
#else
os_status   osQueueInitialize(uint16_t total_queues);
#endif // OS_MULTICORE


/**************************************************************************//**
 @Function      osFrameInitialize

 @Description   Initializes the OS Frames.
 
 @Return        OS_SUCCESS
 
*//***************************************************************************/
os_status   osFrameInitialize();


/**************************************************************************//**
 @Function      osCioInitialize

 @Description   Initializes the CIO layer.
 
 @Param         total_cio_devices - Number of CIO devices in the system.

 @Retval        OS_SUCCESS          - CIO was initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status   osCioInitialize(uint16_t total_cio_devices);


/**************************************************************************//**
 @Function      osBioInitialize

 @Description   Initializes the BIO layer.
 
 @Param         total_bio_devices - Number of BIO devices in the system.

 @Retval        OS_SUCCESS          - BIO was initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status   osBioInitialize(uint16_t total_bio_devices);


/**************************************************************************//**
 @Function      osSioInitialize

 @Description   Initializes the SIO layer.
 
 @Param         total_sio_devices - Number of SIO devices in the system.

 @Retval        OS_SUCCESS          - SIO was initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status   osSioInitialize(uint16_t total_sio_devices);


/**************************************************************************//**
 @Function      osCopInitialize

 @Description   Initializes the COP layer.
 
 @Param         total_cop_devices - Number of COP devices in the system.

 @Retval        OS_SUCCESS          - COP was initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory; operation failed.
 
*//***************************************************************************/
os_status   osCopInitialize(uint16_t total_cop_devices);


/**************************************************************************//**
 @Function      osDmaInitialize

 @Description   Initializes the DMA controller.

 @Param         dma_config - Architecture-specific DMA configuration structure.

 @Return        OS_SUCCESS on success; OS_FAIL otherwise.

*//***************************************************************************/
os_status   osDmaInitialize(arch_specific *dma_config);
 
/**************************************************************************//**
 @Function      osHwWatchdogsInit

 @Description
 
 @Retval        OS_SUCCESS - the watchdog SWI and timer were initialized successfully.
 @Retval        OS_FAIL - the OS failed to initialize the watchdog SWI and timer.

*//***************************************************************************/
os_status osHwWatchdogsInit(void);


/**************************************************************************//**
 @Function      osEventsInitialize

 @Description   Stores max number of event types. Currently only queues
                and counting semaphores are supported.
 
 
 @Param         max_event_queue - Maximal number of event queues.
 @Param         max_event_semaphore - Maximal number of event semaphores.

 @Return        OS_SUCCESS      - Events initializized.
 @Return        OS_FAIL         - Events not initializized.

*//***************************************************************************/
os_status  osEventsInitialize(int max_event_queue, int max_event_semaphore);


/**************************************************************************//**
 @Function      osTasksInitialize

 @Description   Allocates a scheduler and then allocate tasks and events
                objects and insert them in it.
                Background function is defined in smartdsp_init.c, before
                osInitialize. It can be chosen to be wait command or idle
                count or user defined. In osStart, background task is
                created and activated.
 
 
 @Param         max_task - Maximal number of tasks.

 @Return        OS_SUCCESS      - Tasks initializized.
 @Return        OS_FAIL         - Tasks not initializized.

*//***************************************************************************/
os_status  osTasksInitialize(int max_task);


/**************************************************************************//**
 @Function      osKernelAwarenessInitialize

 @Description   Overrides the default stack locaiton and size
                         for kernel awareness. Assumes contiguous address
                         space shared between OS_NUM_OF_CORES. If
                         no multicore support, all stack size goes to single core.
                         Will configure virtual and physical address to the same
                         value (in platforms where applicable).
 
 @Param         base_addr - Base address of shared stack size.
 @Param         stack_size - Size of shared stack size.
 @Param         num_cores - Number of cores (if have multicore support)
 
 @Return        OS_SUCCESS      - Kernel awareness stack set.
 @Return        OS_FAIL         - Kernel awareness stack not set.

*//***************************************************************************/
#if (OS_MULTICORE == 1)
os_status osLogInitialize(uint8_t *base_addr, uint32_t stack_size, uint32_t num_cores);
#else
os_status osLogInitialize(uint8_t *base_addr, uint32_t stack_size);
#endif

/** @} */ // end of os_init_id

_OS_END_EXTERN_C

#endif // __SMARTDSP_INIT_H

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/01/21 06:47:28 $
 $Id: os_config.h,v 1.10 2013/01/21 06:47:28 b08551 Exp $
 $Source: /cvsdata/SmartDSP/demos/starcore/psc9x3x/ipc_integration/os_config.h,v $
 $Revision: 1.10 $
******************************************************************************/

/******************************************************************************

 @File          os_config.h

 @Description   OS Configuration definitions.

                This file was automatically generated using the
                SmartDSP OS Configuration Tool.

 @Cautions      None.

*//***************************************************************************/

#ifndef __OS_CONFIG_H
#define __OS_CONFIG_H

#include "smartdsp_os_device.h"

extern volatile uint32_t KernelAwareness_b;
extern volatile uint32_t KernelAwareness_size;
#ifndef PSC9X3X_SC_ONLY_BSP
/* Base address of the heterogeneous BSP control segment from l3k  */
extern uint32_t _SHARED_CTRL_b;
#endif

#define ON      1
#define OFF     0

/* OS Power architecture simulation */
#define OS_MIMIC_PA_ARCH                    OFF

/* OS General Configuration **************************************************/
#define OS_HEAP_SIZE                        0x30000  /**< Heap size */
#define OS_STACK_SIZE                       0x1000   /**< OS (system) stack size */

#define OS_HEAP_NONCACHEABLE_SIZE           0x2000   /**< Local non-cacheable heap, must be power 2 and >= 256 */
#if (OS_MULTICORE == ON)
#define OS_SHARED_MEM_SIZE                  0x4000   /**< Shared Memory Size */
#endif

#define OS_BACKGROUND_STACK_SIZE            0x1000


/*#ifdef PSC9132
#define OS_HEAP_M3_SIZE                     0x100
#define OS_HEAP_M3_NOCACHEABLE_SIZE         0x100
#if (OS_MULTICORE == ON)
#define OS_SHARED_MEM_M3_SIZE_CACHEABLE     0x100
#endif
#endif  PSC9132 */

#define OS_LOCAL_HEAP_MNGMNT_SIZE           3000
#if (OS_MULTICORE == ON)
#define OS_SHARED_HEAP_MNGMNT_SIZE          5000
#endif

#define SOC_IPC                             ON

//#ifndef PSC9132
/* PA shares it's DDR1 only with PSC9131, PSC9132 has DDR0 */
#define OS_HEAP_DDR1_SIZE                   0x2380
#define OS_HEAP_DDR1_NOCACHEABLE_SIZE       0x100000
//#if (OS_MULTICORE == ON)
//#define OS_SHARED_MEM_DDR1_SIZE             0x3000
//#define OS_SHARED_MEM_DDR1_SIZE_CACHEABLE   0x2400
//#endif
//#endif /* PSC9132 */

#define OS_TICK                             ON                   /**< Tick Functionality */
#define OS_TICK_PRIORITY                    OS_SWI_PRIORITY0     /**< Tick Priority */
#define OS_TICK_PARAMETER                   SOC_TICK_001MS       /**< Tick Parameter */

#define OS_CLKIN                            133.3                /**< (MHz) */
#define OS_SYSTEM_CLOCK                     OS_CLKIN             /**< provided for backward compatibility */

#define OS_HW_TIMERS                        ON  /**< Hardware Timers */

#define OS_TOTAL_NUM_OF_MEM_PARTS           5   /**< Memory Partitions */

#define OS_TOTAL_NUM_OF_FRAME_POOLS         0   /**< Frame Pools Number */

#define OS_TOTAL_NUM_OF_SWI                 10  /**< Software Interrupts Number */
#define OS_TOTAL_NUM_OF_SW_TIMERS           3   /**< Software Timers Number */

#define OS_TOTAL_NUM_OF_EVENT_QUEUES        10  /**< Event Queues Number */
#define OS_TOTAL_NUM_OF_QUEUES              (OS_TOTAL_NUM_OF_EVENT_QUEUES)  /**< Queues Number */

#define OS_TOTAL_NUM_OF_EVENT_SEMAPHORES    10  /**< Event Semaphores Number */

#define OS_TOTAL_NUM_OF_TASKS               20  /**< Tasks Number */

#if (OS_MULTICORE == ON)
#define OS_TOTAL_NUM_OF_SHARED_QUEUES       (OS_TOTAL_NUM_OF_QUEUES)    /**< Shared Queues Number */
#endif

#define OS_TOTAL_NUM_OF_MESSAGE_QUEUES      0

#define OS_WATCHDOGS                        OFF
#define OS_WATCHDOG_TIMEOUT                 0x0400
#define OS_WATCHDOG_PRIORITY                OS_SWI_PRIORITY15

#ifdef USER_WATCHDOG
    #define OS_WATCHDOG_SUPPORT             OS_WDT_USER_HANDLING
#else
    #define OS_WATCHDOG_SUPPORT             OS_WDT_AUTO_HANDLING
    #define OS_WATCHDOG_BEHAVIOR            DSP_WATCHDOG_DEFAULT
    #define OS_WATCHDOG_HANDLER             appWdtHandler
#endif


/* MULTICORE *****************************************************************/

/* OS_MULTICORE should be defined as 0 or 1 by the compiler */

#define OS_MULTICORE_SYNCHRONIZATION        ON  /**< Multi Core Synchronization */

#define OS_TOTAL_NUM_OF_INTERCORE_MESSAGES  1   /**< Intercore Messages Number  */

#ifdef MULTIMODE
#define OS_NUM_OF_CORES                     SOC_MAX_NUM_OF_CORES   /**< Cores Number   */
#else //single mode
#define OS_NUM_OF_CORES                     1   /**< Cores Number   */
#endif
#define OS_MAX_NUM_OF_CORES                 SOC_MAX_NUM_OF_CORES    /**< Max Cores Number */
#define OS_MASTER_CORE                      0                       /**< Master Core ID */


/* Kernel Awareness************************************************************/

#define USER_KERNEL_AWARENESS_STACK         OFF

#define KERNEL_AWARENESS_ADDR               &KernelAwareness_b

#define KERNEL_AWARENESS_STACK_SIZE         (uint32_t)&KernelAwareness_size


/* Architecture Configuration ************************************************/
#define MAX_NUM_OF_DATA_CONTEXTS            10
#define MMU_DATA_CONTEXT_NUM                MAX_NUM_OF_DATA_CONTEXTS

#define MAX_NUM_OF_PROG_CONTEXTS            10
#define MMU_PROG_CONTEXT_NUM                MAX_NUM_OF_PROG_CONTEXTS

#define DCACHE_ENABLE                       ON
#define ICACHE_ENABLE                       ON
#define L2CACHE_ENABLE                      ON
#define OS_L2_CACHE_SIZE                    ((uint32_t)&_L2_cache_size)
#define SOC_HET_CTRL_BASE                   ((uint32_t)&_SHARED_CTRL_b)     /**< Heterogeneous shared control base address */


/* Drivers no HAL ************************************************************/
#define PSC9X3X_DMA                         OFF

/* CIO Devices ***************************************************************/


/* BIO Devices ***************************************************************/


/* SIO Devices ***************************************************************/
#define TDM0                                OFF
#define TDM1                                OFF

/* COP Devices ***************************************************************/
#define MAPLE                               OFF
#define MAPLE_TVPE                          OFF


/* Total number of devices */

#define OS_TOTAL_NUM_OF_CIO_DEVICES         0
#define OS_TOTAL_NUM_OF_BIO_DEVICES         0
#define OS_TOTAL_NUM_OF_SIO_DEVICES         0
#define OS_TOTAL_NUM_OF_COP_DEVICES         0


#endif // __OS_CONFIG_H

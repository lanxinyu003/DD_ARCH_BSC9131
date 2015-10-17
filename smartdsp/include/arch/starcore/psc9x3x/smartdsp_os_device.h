/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/05/15 10:41:51 $
 $Id: smartdsp_os_device.h,v 1.29 2014/05/15 10:41:51 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/smartdsp_os_device.h,v $
 $Revision: 1.29 $
******************************************************************************/

/**************************************************************************//**

 @File          psc9x3x/smartdsp_os_device.h

 @Description   PSC9X3X-specific definitions and structures.

 @Cautions      Do not change this file in any way (unless specified otherwise in the file.
                It is used for creating the memory map and by the drivers.
                The application may refer to these defines as well

*//***************************************************************************/

#ifndef __SMARTDSP_OS_DEVICE_H
#define __SMARTDSP_OS_DEVICE_H

/**************************************************************************//**
 @Group         sdos_dev_id  SmartDsp OS Devices
 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         sdos_dev_9x3x_id  PSC9x3x SmartDsp OS Devices
 @{
*//***************************************************************************/

#define OS_MEM_RESERVED(start_addr,next_addr) volatile uint8_t reserved ## start_addr [next_addr - start_addr]
/**< Reserved uint8_t space generation; Useful for memory maps */

/******************************************************************************/
/* Devices families definition                                                */
/******************************************************************************/
#if (!defined(PSC9132) && !defined(PSC9131))
#error "Must define one of PSC913X devices!"
#endif


#ifndef SC3X50
#error "Must define SC3X50!"
#endif

/******************************************************************************/
/* PLATFORM INFORMATION                                                       */
/******************************************************************************/
#define OS_BUILD_COMPILER           "MW StarCore " __VERSION__
/**< Compiler version */
#define OS_BUILD_PLATFORM           "PSC9X3X"
/**< Build platform */


/*************************************************************************//**
 @Collection   Tick Configuration
 
               Values for a 1ms and 10ms clock are listed below.
 @{
*//***************************************************************************/
#define INTERRUPTS_PER_SEC_1000     1000
#define INTERRUPTS_PER_SEC_100      100

#define SOC_TICK_001MS          INTERRUPTS_PER_SEC_1000
#define SOC_TICK_010MS          INTERRUPTS_PER_SEC_100
#define SOC_TICK_DEFAULT        SOC_TICK_010MS

/* @} */

#if defined(PSC9131)
#define SOC_MAX_NUM_OF_CORES    1
#define PSC9131_FAMILY
#elif defined(PSC9132)
#define SOC_MAX_NUM_OF_CORES    2
#ifndef PSC9132_FAMILY
#define PSC9132_FAMILY
#endif /* ifdef PSC9132_FAMILY */
#else
#error "Choose an architecture within the PSC913X DSP"
#endif

#define SOC_MAX_NUM_OF_MESSAGES     2
/**< There are 2 interrupt line from each core to each other for point to point messages - DO NOT CHANGE THIS VALUE! */
#define  OS_SOC_MAX_NUM_OF_CORES SOC_MAX_NUM_OF_CORES

/*************************************************************************//**
 @Collection   Core IDs
 @{
*//***************************************************************************/
#define CORE0_ID                    0x0000U
#define CORE1_ID                    0x0001U
#define ANY_CORE_ID                 0xFFFEU
#define EXTERNAL_MASTER_ID          0xFFFFU

/* @} */

/******************************************************************************/
/* Memory Map Configuration                                                   */
/******************************************************************************/
#define SOC_DSP_PLAT_DEFAULT        0xFFF00000 /**< The SC3850 internal address space */
#define SOC_PA_CCSR_DEFAULT         0xFEF00000 /**< CCSR PA peripheral base address */
#define SOC_DSP_CCSR_DEFAULT        0xFFF10000 /**< CCSR DSP peripheral base address */
#define SOC_DSP_MAPLE_DEFAULT       0xFFF1C000 /**< Maple SBUS base */
#define OS_KA_LOG_BASE_DEFAULT      0x48000000 /**< KA Log base address */

#if !defined(FALSE)
#define FALSE                           (0)
#endif
#if !defined(TRUE)
#define TRUE                            (1)
#endif

/******************************************************************************/
/* (Extended) Core Specific Defines                                           */
/******************************************************************************/
#define DSP_PRIVATE_M2_BASE             0xB0000000

/*************************************************************************//**
 @Collection   EPIC Defines
 @{
*//***************************************************************************/
#define NUMBER_OF_IPL_REGISTERS             64
#define NUMBER_OF_EPIC_REGS                 8

/* @} */

/*************************************************************************//**
 @Collection   MMU Defines
 @{
*//***************************************************************************/
#define NUMBER_OF_MMU_PROG_MATT             12
#define NUMBER_OF_MMU_DATA_MATT             20
#define MMU_DEFAULT_VBR                     MMU_DATA_BURST_SIZE_4
#define SUPPORTS_MMU_FSM                    TRUE
#define SUPPORTS_MMU_NEXT_LINE_PREFETCH     TRUE

/* @} */

/*************************************************************************//**
 @Collection   Core Private L2 Cache
 @{
*//***************************************************************************/
#define PRIVATE_L2_CACHE                    TRUE
#define NUMBER_L2_CACHE_PARTITIONS          8
#define NUMBER_L2_CACHE_PF_CHANNELS         4

/* @} */

/******************************************************************************/
/* Enabled Peripherals                                                        */
/******************************************************************************/

#define MAPLE_IN_PLATFORM                   TRUE
/**< Boolean - Is MAPLE be present on device.
     Should be set to FALSE if using an MAPLE off the SoC */

#if defined(PSC9132)
#define CPRI_IN_PLATFORM                    TRUE
#elif defined(PSC9131)
#define CPRI_IN_PLATFORM                    FALSE
#endif
/**< CPRI in platform */

#define SEC31_IN_PLATFORM                   FALSE   /**< SEC31 in platform */
#define eMSG_IN_PLATFORM                    FALSE   /**< eMSG in platform */
#define LYNX0_IN_PLATFORM                   FALSE   /**< LYNX defines */
#define LYNX1_IN_PLATFORM                   FALSE   /**< LYNX defines */
#define PEX_IN_PLATFORM                     FALSE   /**< PEX in platform */
#define CLASS1_IN_PLATFORM                  FALSE   /**< CLASS1 in platform */
#define TDM_IN_PLATFORM                     TRUE    /**< TDM in platform */
#ifdef PSC9131
#define GPIO_IN_PLATFORM                    TRUE   /**< GPIO in platform */
#elif defined(PSC9132)
#define GPIO_IN_PLATFORM                    FALSE   /**< GPIO in platform */
#endif //PSC9131

/******************************************************************************/
/* Peripherals Defines                                                                  */
/******************************************************************************/
#if defined(PSC9132)
#define NUM_OF_HW_WATCHDOGS                 2   /**< Number of watchdog timers */
#elif defined(PSC9131)
#define NUM_OF_HW_WATCHDOGS                 1   /**< Number of watchdog timers */
#endif

#define WATCHDOGS_NO_RESET                  TRUE
/**< Indicated that the WDT can't reset the SoC */
#define DSP_WATCHDOG_DEFAULT                GENERATE_NMI
/**< Default behavior of WDT is to generate an interrupt */

#define NUM_OF_HW_TIMER_32b_MODULES         2   /**< Number of timer 32b modules */
#define NUM_OF_HW_TIMERS_32b_PER_MODULE     4   /**< Number of 32b timer per modules */
#define NUM_OF_HW_TIMERS_32b                (NUM_OF_HW_TIMER_32b_MODULES * NUM_OF_HW_TIMERS_32b_PER_MODULE)
/**< Total number of 32b timers */

#define TIMER32_MODULE_RESERVED   0xF0

/* TDM Defines */
#define MAX_NUM_OF_TDMS                     2   /**< Maximal Number of TDM ports, should be the same for 9131 and 9132 */
#if defined(PSC9132)
#define NUMBER_OF_TDMS                      2   /**< Real Number of TDM ports */
#elif defined(PSC9131)
#define NUMBER_OF_TDMS                      1   /**< Real Number of TDM ports */
#endif

#define NUMBER_OF_DMA_CHANNELS              16  /**< Number of system DMA channels */

/* Hardware Semaphore defines */
#define NUMBER_OF_HW_SEMAPHORES             8    /**< Number of HW semaphores */

/* CLASS Defines */
#define MAX_NUMBER_OF_CLASS_REGISTERS       16   /**< The number of class registers */
#define MAX_NUMBER_OF_CLASS_ERR_WINDOWS     4    /**< The number of class error windows */

/* DDRC Defines */
#define MAX_NUM_OF_DDRC                     1    /**< Number of DDR controllers */
#define NUM_DDRC_CHIP_SELECT                4    /**< Number of DDR controller chip selects */

/* ACU Defines */
#define NUM_CCSR_ACC_WINDOWS                16  /**< The number of CCSR Access Windows at ACU unit */

/* eSPI Defines */
#ifdef PSC9131
#define NUMBER_OF_SPI_UNITS                 4  /**< The number of eSPI units */
#elif defined(PSC9132)
#define NUMBER_OF_SPI_UNITS                 2  /**< The number of eSPI units */
#endif //PSC9131

/*************************************************************************//**
 @Collection   MAPLE Defines
 @{
*//***************************************************************************/
/** Sets whether accesses to MAPLE are performed by OS functions or application provided functions */
#if (MAPLE_IN_PLATFORM == TRUE)
#define MAPLE_DIRECT_ACCESSES               TRUE
#else
#define MAPLE_DIRECT_ACCESSES               FALSE
#endif

#define MAPLE_RESET_FEATURE                 TRUE

#ifdef PSC9131
#define MAPLE_B2F
#elif defined(PSC9132)
#define MAPLE_B2P
#endif


#ifdef MAPLE_B2P
#define MAPLE_MAX_NUM_PE_BD_RINGS_LOW_PRIORITY      4   /**< Maximal number of low BD rings in B2F and B2P */
#define MAPLE_MAX_NUM_PE_BD_RINGS_HIGH_PRIORITY     4   /**< Maximal number of high BD rings in B2F and B2P */
#elif defined(MAPLE_B2F)
#define MAPLE_MAX_NUM_PE_BD_RINGS_LOW_PRIORITY      2   /**< Maximal number of low BD rings in B2F and B2P */
#define MAPLE_MAX_NUM_PE_BD_RINGS_HIGH_PRIORITY     2   /**< Maximal number of high BD rings in B2F and B2P */
#endif

#define MAPLE_PE_MAX_NUM_BD_RINGS   \
                (MAPLE_MAX_NUM_PE_BD_RINGS_LOW_PRIORITY + MAPLE_MAX_NUM_PE_BD_RINGS_HIGH_PRIORITY)
#define MAPLE_NUM_TVPE_PUNCTURING_VECTORS           10
#define MAPLE_NUM_INT                               32

#define MAX_NUM_MAPLE                               1   /**< Number of MAPLE driver can handle */
#define MAX_NUM_MAPLE_TVPE                          1   /**< Number of (e)TVPE in MAPLE */
#define MAX_NUM_MAPLE_CRCPE                         1   /**< Number of CRCPE in MAPLE */
#define MAX_NUM_MAPLE_FTPE                          1   /**< Number of FTPE physical elements inside MAPLE */
#define MAX_NUM_MAPLE_DEPE                          1   /**< Number of DEPE in MAPLE */
#define MAX_NUM_MAPLE_PUFFT                         1   /**< Number of PUFFT in MAPLE */
#define MAX_NUM_MAPLE_PUPE                          1   /**< Number of PUPE in MAPLE */
#define MAX_NUM_MAPLE_PDPE                          1   /**< Number of PDPE in MAPLE */
#define MAX_NUM_MAPLE_PUSCH                         1   /**< Number of PUSCH EDF in MAPLE */
#define MAX_NUM_MAPLE_PDSCH                         1   /**< Number of PDSCH EDF in MAPLE */
#ifdef MAPLE_B2P
#define MAX_NUM_MAPLE_EQPE                          1   /**< Number of EQPE in MAPLE */
#define MAX_NUM_MAPLE_CONVPE                        1   /**< Number of CONPE in MAPLE */
#elif defined(MAPLE_B2F)
#define MAX_NUM_MAPLE_EQPE                          0   /**< Number of EQPE in MAPLE */
#define MAX_NUM_MAPLE_CONVPE                        0   /**< Number of CONPE in MAPLE */
#endif

#define NUM_MAPLE_PE    \
        (MAX_NUM_MAPLE_TVPE + MAX_NUM_MAPLE_FTPE + MAX_NUM_MAPLE_CRCPE + MAX_NUM_MAPLE_DEPE + \
         MAX_NUM_MAPLE_EQPE  + MAX_NUM_MAPLE_CONVPE + MAX_NUM_MAPLE_PUFFT + MAX_NUM_MAPLE_PDPE + \
         MAX_NUM_MAPLE_PUSCH + MAX_NUM_MAPLE_PDSCH)
/**< Number of Processing Elements in MAPLE */

#define NUM_MAPLE_PE_BD_RING  \
         (MAPLE_PE_MAX_NUM_BD_RINGS * NUM_MAPLE_PE)

#define NUM_VITERBI_PUNC_VECTORS                    10  /**< TVPE number of viterbi puncturing vectors */
#define NUM_VITERBI_POLYNOMS                        4   /**< TVPE number of viterbi polynoms */
#define NUM_VITERBI_POLYNOM_SETS                    3   /**< TVPE number of viterbi polynoms sets */

/* @} */

#define MAX_NUM_OF_VIRTS 16     /**< The number of available virtual interrupts */

/******************************************************************************/
/* Misc                                                                       */
/******************************************************************************/
/**************************************************************************//**
 @Description    CLEAR_LEVEL_INTERRUPT is the mechanism used to clear level interrutps at their source
                 (not EPIC) where the driver doesn't know how to detect false positives on interrupt assertions.
                 This is not used by all drivers - ONLY where the condition above is fullfilled

     CLEAR_LEVEL_INTERRUPT_ULTRA_SAFE - Safest mode for clearing interrupts, highest overhead.
                                        Generates 2 writes to the interrupt status register.
                                        The first clears the interrupt and the second stalls the
                                        core (syncio) until the interrupt is actually cleard.
     CLEAR_LEVEL_INTERRUPT_SAFE       - Safe mode, medium overhead. Generates a single write to the
                                        interrupt status register. Clear is with parallel syncio and
                                        therefore stalls the core until sampled in the SkyBlue system
                                        (albeit not necessariy at the register itself).
     None of the above                - Minimal overhead, no assurance. Generates a single, non blocking,
                                        write to the interrupt status register. Assuming low congestion in
                                        the system and a long enough ISR (generally the case) - should be used.
                                        Default configuration!
*//***************************************************************************/
#ifdef CLEAR_LEVEL_INTERRUPT_ULTRA_SAFE
#define CLEAR_LEVEL_INTERRUPT(ADDR,DATA) \
    { WRITE_UINT32(ADDR,DATA); WRITE_SYNCIO_UINT32(ADDR,DATA);}
#elif defined(CLEAR_LEVEL_INTERRUPT_SAFE)
#define CLEAR_LEVEL_INTERRUPT(ADDR,DATA) \
    { WRITE_SYNCIO_UINT32(ADDR,DATA);}
#else
#define CLEAR_LEVEL_INTERRUPT(ADDR,DATA) \
    { WRITE_UINT32(ADDR,DATA); }
#endif

/** @} */ // 9x3x devices
/** @} */ // sdos devices

/******************************************************************************/
/* All include header files specific to platform                              */
/******************************************************************************/
#include "os_datatypes.h"
#include "os_accessors.h"
#include "psc9x3x_hwi.h"
#include "psc9x3x_init.h"
#include "psc9x3x_clocks.h"
#include "sc3x00_mmu.h"



#endif // __SMARTDSP_OS_DEVICE_H

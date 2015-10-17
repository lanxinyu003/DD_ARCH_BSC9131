/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/10/27 19:02:26 $
 $Id: sc3x00_mmu.h,v 1.5 2011/10/27 19:02:26 dovl Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/core_family/sc3X00/sc3x00_mmu.h,v $
 $Revision: 1.5 $
******************************************************************************/

/**************************************************************************//**
 
 @File          sc3x00_mmu.h

 @Cautions      Attributes are expected in the format below, thus some of the
                bitmasks and bits are shifted.
 
                31   28  24       16        0
                +----+----+--------+--------+
                |xSDC|    |  xSDB  |  xSDA  |
                +----+----+--------+--------+
 
*//***************************************************************************/

#ifndef __SC3X00_MMU_H
#define __SC3X00_MMU_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         mem_mmu_sc3x00_id SC3x00 MMU API

 @Description   SC3x00 specific Program and Data MMU API
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Collection   MMU Segment Register Masks
 @{
*//***************************************************************************/
#define MMU_PSDA_ATTR_MASK                             0x000000FF   /**< Data Segment Descriptor Registers A */
#define MMU_PSDB_ATTR_MASK                             0x00FF0000   /**< Data Segment Descriptor Registers B */
#define MMU_PSDC_ATTR_MASK                             0xC0000000   /**< Data Segment Descriptor Registers C */

#define MMU_DSDA_ATTR_MASK                             0x000000FF   /**< Program Segment Descriptor Registers A */
#define MMU_DSDB_ATTR_MASK                             0x00FF0000   /**< Program Segment Descriptor Registers A */
#define MMU_DSDC_ATTR_MASK                             0xF0000000   /**< Program Segment Descriptor Registers A */
 
/* @} */

/**************************************************************************//**
 @Collection   MMU Program Register Bits
 
                These bits are used for setting the attributes of
                Program MMU segments

 @{
*//***************************************************************************/
#define MMU_PROG_DEF_SYSTEM                            0x00000010
/**< System/Shared task Virtual Program Memory. Use to make Segment shared for all PID */
#define MMU_PROG_DEF_CACHEABLE                         0x00000008
/**< Instruction Cacheability. If this bit is set, the segment is cacheable in the ICache */
#define MMU_PROG_DEF_XPERM_USER                        0x00000004
/**< Program Access Permission in user level */
#define MMU_PROG_DEF_XPERM_SUPER                       0x00000002
/**< Program Access Permission in supervisor Level */

#define MMU_PROG_L2_CACHEABLE                          0x00080000
/**< Program L2 Cache Policy - cacheable */
#define MMU_PROG_L2_NONCACHEABLE                       0x00000000
/**< Program L2 Cache Policy - non cacheable */
#define MMU_PROG_PREFETCH_ENABLE                       0x00040000
/**< Program Prefetch Line Enable */
#define MMU_PROG_BURST_SIZE_4                          0x00020000
/**< Program Burst Size - 4 VBR. Defines the burst sizes in the region to a cacheable segment */
#define MMU_PROG_BURST_SIZE_2                          0x00010000
/**< Program Burst Size - 2 VBR. Defines the burst sizes in the region to a cacheable segment */
#define MMU_PROG_BURST_SIZE_1                          0x00000000
/**< Program Burst Size - 1 VBR. Defines the burst sizes in the region to a cacheable segment */
#define MMU_PROG_NEXT_LINE_PREFETCH                    0x40000000
/**< Program Next Line Prefetch */

/* @} */

/**************************************************************************//**
 @Collection    MMU Data Register Bits
 
                These bits are used for setting the attributes of
                Data MMU segments
@{
*//***************************************************************************/
#define MMU_DATA_DEF_SYSTEM                            0x00000020
/**< System/Shared task Virtual Data Memory. Use to make Segment shared for all DID */
#define MMU_DATA_DEF_RPERM_USER                        0x00000010
/**< Data Read Access Permission in user level */
#define MMU_DATA_DEF_WPERM_USER                        0x00000008
/**< Data Write Access Permission in user level */
#define MMU_DATA_DEF_RPERM_SUPER                       0x00000004
/**< Data Read Access Permission in supervisor Level */
#define MMU_DATA_DEF_WPERM_SUPER                       0x00000002
/**< Data Write Access Permission in supervisor Level */

#define MMU_DATA_CACHEABLE_WRITETHROUGH                0x00000000
/**< Data Write Policy - Cacheable write-through */
#define MMU_DATA_NONCACHEABLE_WRITETHROUGH             0x00800000
/**< Data Write Policy - Noncacheable write-through */
#define MMU_DATA_CACHEABLE_WRITEBACK                   0x00400000
/**< Data Write Policy - Cacheable write-back */
#define MMU_DATA_L2_ADAPTIVE_WRITE                     0x00180000
/**< Data L2 Cache Policy - Adaptive write */
#define MMU_DATA_L2_NONCACHEABLE                       0x00100000
/**< Data L2 Cache Policy - Noncacheable */
#define MMU_DATA_L2_CACHEABLE_WRITEBACK                0x00080000
/**< Data L2 Cache Policy - Cacheable write-back */
#define MMU_DATA_L2_CACHEABLE_WRITETHROUGH             0x00000000
/**< Data L2 Cache Policy - Cacheable write-through */
#define MMU_DATA_PREFETCH_ENABLE                       0x00040000
/**< Data Prefetch Line Enable */
#define MMU_DATA_BURST_SIZE_4                          0x00020000
/**< Data Burst Size - 4 VBR. Defines the burst sizes in the region to a cacheable segment */
#define MMU_DATA_BURST_SIZE_2                          0x00010000
/**< Data Burst Size - 4 VBR. Defines the burst sizes in the region to a cacheable segment */
#define MMU_DATA_BURST_SIZE_1                          0x00000000
/**< Data Burst Size - 4 VBR. Defines the burst sizes in the region to a cacheable segment */


#define MMU_FLEX_SEGMENT_MODEL                        0x80000000
/**< Data/Program Segment Programming Model - will be set by the startup hooks! */
#define MMU_MATT_ENABLE                                0x00000001
/**< Segment Descriptor Enable */
#define MMU_MATT_VIRT_ADDRESS_MASK                     0xFFFFFF80
/**< Segment Virtual Address Mask */
#define MMU_MATT_ADDRESS_MASK                          0xFFFFFF00
/**< Segment Physical Address Mask */


#define M_CR_PMPE           0x00008000     /**< Program MATT Programming Error */
#define M_CR_DMPE           0x00004000     /**< Data MATT Programming Error */
#define M_CR_CCCE           0x00002000     /**< Cancel Cache Commands Enabled */
#define M_CR_CCEE           0x00001000     /**< Cache Commands Error Enabled */
#define M_CR_OCEE           0x00000800     /**< OCE Enabled */
#define M_CR_ABTF           0x00000400     /**< Automatic Branch Target Buffer Flush */
#define M_CR_PAB            0x00000100     /**< Peripherals Access Bus - read only! */
#define M_CR_CVAE           0x00000080     /**< Capture Violation Address Enable */
#define M_CR_DNAMEE         0x00000040     /**< Data Non-aligned Memory Exception Enable */
#define M_CR_WSBEE          0x00000020     /**< Write to Same Memory Byte Exception Enable */
#define M_CR_DPUE           0x00000010     /**< Debug and Profiling Unit Enable */
#define M_CR_MPE            0x00000008     /**< Protection bit */
#define M_CR_ATE            0x00000004     /**< Address Translation Enable */
#define M_CR_EDCEE          0x00000002     /**< Error Detection Code Exception Enable */
#define M_CR_CMIR           0x00000001     /**< Clear MMU Interrupt Request */

#define M_DSR_DCCV          0x00100000     /**< Data Cache Command Violation */
#define M_DSR_DAVW          0x000C0000     /**< Data Access Violation Width */
#define M_DSR_DAVD          0x00020000     /**< Data Access Violation Direction */
#define M_DSR_DPL           0x00010000     /**< Data Privilege Level */
#define M_DSR_DSPE          0x00000080     /**< Data Segment Programming Error */
#define M_DSR_DMSD          0x00000040     /**< Data Multiple Segment Descriptor Hit */
#define M_DSR_DNME          0x00000020     /**< Data Non-mapped Memory Access Error */
#define M_DSR_DPV           0x00000010     /**< Data Privilege Violation */
#define M_DSR_DNAE          0x00000008     /**< Data Non-aligned Access Error */
#define M_DSR_WSBE          0x00000004     /**< Write to Same Byte Exception */
#define M_DSR_DSM           0x00000002     /**< Data Segment Miss */
#define M_DSR_DEDC          0x00000001     /**< Data EDC */

#define M_PSR_PPL           0x00010000     /**< Program Privilege Level */
#define M_PSR_PSPE          0x00000040     /**< Program Segment Programming Error */
#define M_PSR_PMSD          0x00000020     /**< Program Multiple Segment Descriptor Hit */
#define M_PSR_PNME          0x00000010     /**< Program Non-mapped Memory Access Error */
#define M_PSR_PPV           0x00000008     /**< Program Privilege Violation */
#define M_PSR_PNAE          0x00000004     /**< Program Non-aligned Access Error */
#define M_PSR_PSM           0x00000002     /**< Program Segment Miss */
#define M_PSR_PEDC          0x00000001     /**< Program EDC */

#define M_DQSR_DSPE         0x00040000     /**< Data Segment Programming Error */
#define M_DQSR_DMSD         0x00020000     /**< Data Multiple Segment Descriptor Hit */
#define M_DQSR_DSM          0x00010000     /**< Data Query Status Register Miss */
#define M_DQSR_SDHI         0x0000001F     /**< Segment Descriptor Hit Index  */


#define MMU_SHARED_PID  0       /**<  PID/DID reserved (by hardware) for shared MATT */
#define MMU_SYSTEM_PID  1       /**<  PID/DID reserved (by software) for OS */

/* @} */

/**************************************************************************//**
 @Collection   MMU defines to be deprecated
 @{
*//***************************************************************************/
#define MMU_NEXT_LINE_PREFETCH  MMU_PROG_NEXT_LINE_PREFETCH
/**< Replaced by MMU_PROG_NEXT_LINE_PREFETCH */
#define MMU_MEMORY_PROTECTION_ENABLE                   0x00000008
/**< Same as M_CR_MPE */
#define MMU_MEMORY_TRANSLATION_ENABLE                  0x00000004
/**< Same as M_CR_ATE */
#define MMU_CAPTURE_ENABLE                             0x00000080
/**< Same as M_CR_CVAE */

/* @} */


/**************************************************************************//**
 @Function      osMmuProgVirtToPhysManual

 @Description   Translate program virtual address to physical by manual inspection

 @Param[in]     virt_addr -   virtual address
 @Param[out]    phys_addr -  physical address
 
 @Cautions      This is an extremely slow implementation and shouldn't be used in runtime
 
 @Return        OS_SUCCESS if segment exists

*//***************************************************************************/
os_status osMmuProgVirtToPhysManual(void const* virt_addr,void ** phys_addr);

/**************************************************************************//**
 @Function      osMmuDataVirtToPhysManual

 @Description   Translate data virtual address to physical by manual inspection

 @Param[in]     virt_addr -   virtual address
 @Param[out]    phys_addr -  physical address
 
 @Cautions      This is an extremely slow implementation and shouldn't be used in runtime
 
 @Return        OS_SUCCESS if segment exists
*//***************************************************************************/
os_status osMmuDataVirtToPhysManual(void const* virt_addr,void ** phys_addr);

/**************************************************************************//**
 @Function      osMmuDataPhysToVirtManual

 @Description   Translate data physical address to virtual by manual inspection

 @Param[in]     phys_addr -   physical address
 @Param[out]    virt_addr -  virtual address
 
 @Cautions      This is an extremely slow implementation and shouldn't be used in runtime
 
 @Return        OS_SUCCESS if segment exists
*//***************************************************************************/
os_status osMmuDataPhysToVirtManual(void const* phys_addr,void ** virt_addr);

/** @} */ // mem_mmu_sc3x00_id

_OS_END_EXTERN_C

#endif // __SC3X00_MMU_H


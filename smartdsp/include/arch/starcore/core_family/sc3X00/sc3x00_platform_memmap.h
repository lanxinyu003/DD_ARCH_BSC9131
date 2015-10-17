/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_platform_memmap.h,v 1.8 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/core_family/sc3X00/sc3x00_platform_memmap.h,v $
 $Revision: 1.8 $
******************************************************************************/

/**************************************************************************//**
 
 @File          sc3x00_platform_memmap.h

 @Description   DSP Subsystem memory map.
 
 @Cautions      Not all of the structures are valid for all of the devices.
 
*//***************************************************************************/
#ifndef __SC3X00_PLATFORM_MEMMAP_H
#define __SC3X00_PLATFORM_MEMMAP_H

#include "os_datatypes.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C


/**************************************************************************//**
 @Group         dsp_mem_map_id  DSP Platform Memory Maps
 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         dsp_map_sc3x00_id  SC3X00 Memory Maps
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Description   Extended Programmable Interrupt Controller (EPIC) Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t    p_ipl[NUMBER_OF_IPL_REGISTERS];    /**< EPIC Interrupt Priority Level Registers */
    volatile uint32_t    p_elr[NUMBER_OF_EPIC_REGS];        /**< EPIC Edge/Level Trigger Registers */
    volatile uint32_t    p_ipr[NUMBER_OF_EPIC_REGS];        /**< EPIC Interrupt Pending Registers */
    volatile uint32_t    p_endis[NUMBER_OF_EPIC_REGS];      /**< EPIC Enable/Disable Interrupts Registers */
    volatile uint32_t    p_sr;                              /**< EPIC Status Register */
    volatile uint32_t    p_de;                              /**< EPIC Double-edge Interrupt Index Register */
#ifdef SC3X50
    volatile uint32_t    p_di;                              /**< EPIC disable interrupts register */
#else
    volatile uint8_t     reserved0[sizeof(uint32_t)];
#endif
    volatile uint8_t     reserved1[0x294];
} dsp_plat_epic_map_t;


/**************************************************************************//**
 @Description   L1 Data Cache (DCache) Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t    dc_cr0;                /**< Data Cache Control Register 0 */
    volatile uint32_t    dc_cr1;                /**< Data Cache Control Register 1 */
    volatile uint16_t    dc_cr2;                /**< Data Cache Control Register 2 */
    volatile uint8_t     reserved0[0x16];
    volatile uint32_t    dc_lrm;                /**< Data Cache LRM State Register */
    volatile uint32_t    dc_et;                 /**< Data Cache Extended Tag State Register */
    volatile uint32_t    dc_vd;                 /**< Data Cache Valid Dirty State Register */
    volatile uint32_t    dc_dbg_data;           /**< Data Cache Debug Data Register */
    volatile uint32_t    dc_dbg_acs;            /**< Data Cache Debug Access Register */
    volatile uint8_t     reserved1[0x3CC];
} dsp_plat_dcache_map_t;

/**************************************************************************//**
 @Description   L1 Instruction Cache (ICache) Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t    ic_cr0;                /**< Instruction Cache Control Register 0 */
    volatile uint32_t    ic_cr1;                /**< Instruction Cache Control Register 1 */
    volatile uint16_t    ic_cr2;                /**< Instruction Cache Control Register 2 */
    volatile uint8_t     reserved0[0x16];
    volatile uint32_t    ic_lrm;                /**< Instruction Cache LRM Status Register */
    volatile uint32_t    ic_et;                 /**< Instruction Cache Extended TAG State Register */
    volatile uint32_t    ic_v;                  /**< Instruction Cache Valid State Register */
    volatile uint32_t    ic_dbg_data;           /**< Instruction Cache Debug Data Register */
    volatile uint32_t    ic_dbg_acs;            /**< Instruction Cache Debug Access Register */
    volatile uint8_t     reserved1[0x3CC];
} dsp_plat_icache_map_t;
 
/**************************************************************************//**
 @Description   Memory Management Unit (MMU) Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t    m_cr;                  /**< MMU Control Register */
    volatile uint32_t    m_dsr;                 /**< Data Status Register */
    volatile uint32_t    m_dva;                 /**< Data Violation Access Register */
    volatile uint32_t    m_psr;                 /**< Program Status Register */
    volatile uint32_t    m_pva;                 /**< Program Violation Address Register */
    volatile uint8_t     reserved0[0x06028 - 0x06014];
    volatile uint32_t    m_pir;                 /**< Platform Information Register */
    volatile uint8_t     reserved1[0x06030 - 0x0602C];
    volatile uint16_t    m_gpr0;                /**< General Purpose Register 0 */
    volatile uint8_t     reserved2[0x06038 - 0x06034];
    volatile uint32_t    m_pmesr;               /**< Platform Master Error Status Register */
    volatile uint32_t    m_pvar;                /**< Peripheral Violation Address Register */
    volatile uint32_t    m_psesr;               /**< Platform Slave Error Status Register */
    volatile uint8_t     reserved3[0x06100 - 0x06044];
    /* Data Control and ID Registers */
    volatile uint32_t    m_dsdcr;               /**< Data Segment Descriptor Control Register */
    volatile uint32_t    m_cdid;                /**< Current Data ID Register */
    volatile uint32_t    m_dqsr;                /**< Data Query Status Register */
    volatile uint32_t    m_dqpr;                /**< Data Query Physical Register */
    volatile uint8_t     reserved4[0x07000 - 0x06110];
    /* Program Control and ID Registers */
    volatile uint32_t    m_psdcr;               /**< Program Segment Descriptor Control Register */
    volatile uint32_t    m_cpid;                /**< Current Program ID Register */
    volatile uint32_t    m_pqsr;                /**< Program Query Status Register */
    volatile uint32_t    m_pqpr;                /**< Program Query Physical Register */
    volatile uint8_t     reserved5[0x08000 - 0x07010];

    struct
    {
        volatile uint32_t m_psda;               /**< Program Segment Descriptor Registers A */
        volatile uint32_t m_psdb;               /**< Program Segment Descriptor Registers B */
        volatile uint32_t m_psdc;               /**< Program Segment Descriptor Registers C */
        volatile uint8_t  reserved6[0x04];
    } m_psd[NUMBER_OF_MMU_PROG_MATT];
    /**< Program MATT */

    volatile uint8_t     reserved6[0x09000 - 0x080C0];

    struct
    {
        volatile uint32_t m_dsda;               /**< Data Segment Descriptor Registers A */
        volatile uint32_t m_dsdb;               /**< Data Segment Descriptor Registers B */
        volatile uint32_t m_dsdc;               /**< Data Segment Descriptor Registers C */
        volatile uint8_t  reserved8[0x04];
    }m_dsd[NUMBER_OF_MMU_DATA_MATT];
    /**< Data MATT */

    volatile uint8_t reserved9[0x0A000 - 0x09140];

} dsp_plat_mmu_map_t;


/**************************************************************************//**
 @Description   DPU Counter Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t    dp_cc;                 /**< DPU Counter Control Register */
    volatile uint32_t    dp_cv;                 /**< DPU Counter Value Register */
} dsp_plat_dpu_counter_t;


/**************************************************************************//**
 @Description   Debug and Profiling Unit (DPU) Registers.
*//***************************************************************************/
typedef struct
{
    /* General Registers */
    volatile uint32_t    dp_cr;                 /**< DPU Control Register */
    volatile uint32_t    dp_sr;                 /**< DPU Status Register */
    volatile uint32_t    dp_mr;                 /**< DPU Monitor Register */
    volatile uint32_t    dp_rpid;               /**< DPU PID Detection Reference Value Register */
    volatile uint32_t    dp_rdid;               /**< DPU DID Detection Reference Value Register */
    volatile uint8_t     reserved0[0x0A020 - 0x0A014];
 
    /* General Counters */
    volatile uint32_t    dp_tac;                /**< DPU Counter Triad A Control Register */
    volatile uint32_t    dp_tbc;                /**< DPU Counter Triad B Control Register */
    volatile uint8_t     reserved1[0x0A02C - 0x0A028];
 
    dsp_plat_dpu_counter_t dp_ca[0x03];          /**< DPU Counters A0, A1, A2 Registers */
    volatile uint8_t      reserved2[0x0A054 - 0x0A044];
    dsp_plat_dpu_counter_t dp_cb[0x03];          /**< DPU Counters B0, B1, B2 Registers */
    volatile uint8_t      reserved3[0x0A07C - 0x0A06C];

    /* Trace Registers */
    volatile uint32_t    dp_tc;                 /**< DPU Trace Control Register */
    volatile uint32_t    dp_tsa;                /**< DPU VTB Start Address Register */
    volatile uint32_t    dp_tea;                /**< DPU VTB End Address Register */
    volatile uint32_t    dp_ter;                /**< DPU Trace Event Request Register */
    volatile uint32_t    dp_tw;                 /**< DPU Trace Write Pointer Register */
    volatile uint32_t    dp_td;                 /**< DPU Trace Data Register */
    volatile uint8_t     reserved4[0x0A300 - 0x0A094];
} dsp_plat_dpu_map_t;

/**************************************************************************//**
 @Description   Platform Timer MAP.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t    tm_tc;                 /**< Timer Control Register */
    volatile uint32_t    tm_tv;                 /**< Timer Value Register */
    volatile uint32_t    tm_tp;                 /**< Timer Pre-load Register */
} dsp_plat_timer_t;

typedef dsp_plat_timer_t    platform_timer_t;   /* Backward compatabile */

/**************************************************************************//**
 @Description   Platform Timer Registers.
*//***************************************************************************/
typedef struct
{
    dsp_plat_timer_t    platform_timer[2];      /**< Timers, see #dsp_plat_timer_t */
    volatile uint8_t    reserved0[0xE8];
} dsp_plat_timer_map_t;


/**************************************************************************//**
 @Description   L2 Unified Cache (L2Cache) Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t    l2_cr0;                /**< L2 cache Control Register 0 */
    volatile uint32_t    l2_cr1;                /**< L2 cache Control Register 1 */
    volatile uint16_t    l2_cr2;                /**< L2 cache Control Register 2 */
    volatile uint8_t     reserved0[0x6];
 
    struct
    {
        volatile uint32_t    l2_pssa;           /**< L2 Cache Partition Segment Start Address Register */
        volatile uint32_t    l2_psea;           /**< L2 Cache Partition Segment End Address Register */
    } l2_ps[NUMBER_L2_CACHE_PARTITIONS];
    /**< L2 Cache Partition Segments */

    volatile uint8_t        reserved1[0x10];
 
    struct
    {
        volatile uint32_t    l2_pf_ctrl;        /**< L2 Cache PF Control Register */
        volatile uint32_t    l2_pf_st_addr;     /**< L2 Cache PF Start Address Register */
        volatile uint32_t    l2_pf_rs;          /**< L2 Cache PF Row Size Register */
        volatile uint32_t    l2_pf_st;          /**< L2 Cache PF Stride Register */
        volatile uint32_t    l2_pf_rows_num;    /**< L2 Cache PF Num of Rows Register */
        volatile uint8_t     reserved2[0xC];
    } l2_pf[NUMBER_L2_CACHE_PF_CHANNELS];
    /**< L2 Cache Prefetch Channels */

    volatile uint32_t    l2_pf_status;          /**< L2 Cache PF Status Register */
    volatile uint32_t    l2_pf_config;          /**< L2 Cache PF Configuration Register */

    volatile uint8_t     reserved3[0x8];
    volatile uint32_t    l2_lsr;                /**< L2 Cache Line State Register */
    volatile uint32_t    l2_lrm_sr;             /**< L2 Cache Line Replacement Mechanism SR Register */
    volatile uint32_t    l2_edc_config;         /**< L2 Cache EDC Configuration Register */
    volatile uint32_t    l2_ecc_config;         /**< L2 Cache ECC Configuration Register */
    volatile uint8_t     reserved4[0x3400];
 
} dsp_plat_l2cache_map_t;

/**************************************************************************//**
 @Description   DSP Subsystem Memory Map
 
                This structure inlucdes the memory map for the DSP
                Subsystem. The structure resides on Bank 0 of the QBus.
                The base address is initialized to MSC815X_QBUS_DEFAULT
                during the bring-up of the kernel and can be accessed
                by referring to the global variable g_msc815x_qbus.
                For example:
                  g_msc815x_qbus->epic.p_elr[1] = 0x00000001;
 
*//***************************************************************************/
typedef struct
{
    volatile uint8_t        reserved0[0x00400 - 0x00000];
    dsp_plat_epic_map_t     epic;       /**< Extended Programmable Interrupt Controller (EPIC) Registers */
    dsp_plat_dcache_map_t   dcache;     /**< L1 Data Cache (DCache) Registers */
    dsp_plat_icache_map_t   icache;     /**< L1 Instruction Cache (ICache) Registers */
    volatile uint8_t        reserved1[0x06000 - 0x01000];
    dsp_plat_mmu_map_t      mmu;        /**< Memory Management Unit (MMU) Registers */
    dsp_plat_dpu_map_t      dpu;        /**< Debug and Profiling Unit (DPU) Registers */
    dsp_plat_timer_map_t    timer;      /**< Platform Timer Registers */
    volatile uint8_t        reserved2[0x0F000 - 0x0A400];
    dsp_plat_l2cache_map_t  l2cache;    /**< L2 Unified Cache (L2Cache) Registers */
    volatile uint8_t        reserved3[0x10000 - 0x0F400];
} dsp_plat_sc3850_map_t;


/** @} */ // dsp_map_sc3x00_id
/** @} */ // end of dsp_mem_map_id

_OS_END_EXTERN_C

#endif  // __SC3X00_PLATFORM_MEMMAP_H

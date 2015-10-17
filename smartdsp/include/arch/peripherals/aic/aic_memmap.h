/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:27:33 $
 $Id: aic_memmap.h,v 1.11 2012/11/22 16:27:33 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/aic/aic_memmap.h,v $
 $Revision: 1.11 $
******************************************************************************/

/**************************************************************************//**

 @File          aic_memmap.h

 @Description   AIC (Antenna interface Controller) memory map header file.

*//***************************************************************************/

#ifndef __AIC_MEMMAP_H
#define __AIC_MEMMAP_H

#include "smartdsp_os.h"
#include "smartdsp_os_device.h"

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         arch_common_id  Peripheral Memory Maps

 @Description   AIC block API.

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         aic_mem_map_id AIC memory map
 
 @Description   AIC memory map structure.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   IP version and connectivity configuration registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t ip_version;       /**< IP Version register */
    volatile uint32_t interconnect;     /**< Interconnect register */
} ip_version_and_connectivity_configuration_registers_t;

/**************************************************************************//**
 @Description   PLL registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t pll_cntl_0;       /**< PLL control 0 */
    volatile uint32_t pll_cntl_1;       /**< PLL control 1 */
} pll_registers_t;


/**************************************************************************//**
 @Description   Sniff block registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t refcnt;           /**< Reference Counter */
    volatile uint32_t refcnt_off;       /**< Reference Counter Offset*/
    volatile uint32_t capt_off;         /**< Chip Capture Offset */
    volatile uint32_t capt_dur;         /**< Chip Capture Duration */
    union
    {
        struct
        {
            volatile uint32_t bcar;     /**< Binary Constant A Register */
            volatile uint32_t bcbr;     /**< Binary Constant B Register */
            volatile uint32_t brmcntrl; /**< Prescale Modulus select */
        } brm_configurations;
        OS_MEM_RESERVED(0x068, 0x074);
    };
    volatile uint32_t cntrl;            /**< Control */
    volatile uint32_t inten;            /**< Interrupt Enable */
    volatile uint32_t int_stat;         /**< Interrupt Status */
    volatile uint32_t refcnt_val;       /**< Reference Counter Value */
    volatile uint32_t frmint;           /**< Frame Interrupt */
} sniff_block_registers_t;

/**************************************************************************//**
 @Description   Sniff registers.
*//***************************************************************************/
typedef struct
{
    sniff_block_registers_t sniff0_registers;
    OS_MEM_RESERVED(0x040, 0x058);
    sniff_block_registers_t sniff1_registers;
    OS_MEM_RESERVED(0x088, 0x158);
} sniff_registers_t;

/**************************************************************************//**
 @Description   MAXIM registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t rxcntl;           /**< RX Control register */
    volatile uint32_t cntl;             /**< Control register */
    volatile uint32_t rssi;             /**< RSSI Status register */
    volatile uint32_t txcoeff[9];       /**< TX Programmable Filter Coefficients 0...17 (Each coefficients is 16 bits wide)*/
    volatile uint32_t status;           /**< Status register */
    volatile uint32_t dlfracdelay;      /**< DL Fractional Delay register */
    volatile uint32_t spirxgain[2];     /**< SPI RX Gain register */
    OS_MEM_RESERVED(0x198, 0x1B0);
} maxim_registers_t;


/**************************************************************************//**
 @Description   ADI/JESD registers.
*//***************************************************************************/
typedef struct
{
    struct
    {
        volatile uint32_t cntl;                     /**< Control register */
        volatile uint32_t status;                   /**< Status register */
    }jesd_cntl_status[3];
    volatile uint32_t tmcntrl;                      /**< AIC RF Timer TMCNTRL1 register */
    volatile uint32_t interrupt_mux_crtl_reg;       /**< AIC Interrupt Mux Control Register */
    OS_MEM_RESERVED(0x1D0, 0x1E0);
    volatile uint32_t ppc_interrupt_ctrl_reg;       /**< PPC Interrupt Control register */
    volatile uint32_t ppc_interrupt_status_reg;     /**< PPC Interrupt Status Register */
    volatile uint32_t dsp_lte_interrupt_ctrl_reg;   /**< LTE DSP Interrupt Control register */
    volatile uint32_t dsp_lte_interrupt_status_reg; /**< LTE DSP Interrupt Status Register */
    OS_MEM_RESERVED(0x1F0, 0x200);
} adi_registers_t;


/**************************************************************************//**
 @Description   AIC registers type 2 (see #aic_map_t description).
*//***************************************************************************/
typedef struct
{
    ip_version_and_connectivity_configuration_registers_t ip_version_and_connectivity_configuration_registers; /**<  AIC IP version and connectivity configuration registers. */
    pll_registers_t pll_registers; /**<  AIC PLL registers. */
    sniff_registers_t sniff_registers; /**<  AIC Sniff registers. */
    maxim_registers_t maxim_registers; /**<  AIC MAXIM registers. */
    adi_registers_t adi_registers; /**<  AIC ADI/JESD registers. */
} aic_registers_type2_t;


/**************************************************************************//**
 @Description   DMA configuration registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t riqmts;       /**< Receive IQ MBus Transaction Size */
    volatile uint32_t tiqmts;       /**< Transmit IQ MBus Transaction Size */
    volatile uint32_t riqmpl1;      /**< Receive IQ MBus Priority Level 1 */
    volatile uint32_t riqmpl23;     /**< Receive IQ MBus Priority Level 23 */
    volatile uint32_t tiqmpl1;      /**< Transmit IQ MBus Priority Level 1 */
    volatile uint32_t tiqmpl23;     /**< Transmit IQ MBus Priority Level 23 */
    volatile uint32_t mss;          /**< Maximum Symbol Size */
    volatile uint32_t riqba[2];     /**< Receive IQ Base Address 0 and Receive IQ Base Address 1 */
    OS_MEM_RESERVED(0x024, 0x040);
    volatile uint32_t riqbs;        /**< Receive IQ Buffer Size */
    volatile uint32_t rmba;         /**< Receive MAPLE Base Address */
    volatile uint32_t rmbs;         /**< Receive MAPLE Buffer Size */
    volatile uint32_t tiqba[2];     /**< Transmit IQ Base Address 0 and transmit IQ Base Address 1 */
    OS_MEM_RESERVED(0x054, 0x074);
    volatile uint32_t tiqbs;        /**< Transmit IQ Buffer Size */
    volatile uint32_t dmsr;         /**< DMA Mode Select Register */
    volatile uint32_t dcr;          /**< DMA Control Register */
    volatile uint32_t riqt;         /**< Receive IQ Threshold */
    volatile uint32_t riqft;        /**< Receive IQ First Threshold */
    volatile uint32_t riqst;        /**< Receive IQ Second Threshold */
    volatile uint32_t tiqt;         /**< Transmit IQ Threshold */
    volatile uint32_t tiqft;        /**< Transmit IQ First Threshold */
    volatile uint32_t tiqst;        /**< Transmit IQ Second Threshold */
    volatile uint32_t ier;          /**< Interrupt Enable Register */
    OS_MEM_RESERVED(0x09C, 0x0E8);
} dma_configuration_registers_t;


/**************************************************************************//**
 @Description   Framer/Timing Control Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t framconf;                 /**< Frame structure Configuration register */
    volatile uint32_t netwconf;                 /**< Network Mode Configuration register */
    volatile uint32_t netwconf2;                /**< Network Mode Configuration register 2 */
    volatile uint32_t netwconf3;                /**< Network Mode Configuration register 3 */
    volatile uint32_t dluldelay;                /**< Down Link and UP Link delay Register */
    volatile uint32_t dlslot;                   /**< Downlink Slot control register */
    volatile uint32_t ulslot;                   /**< Uplink Slot control register */
    volatile uint32_t spslot;                   /**< Special Slot control register */
    volatile uint32_t framecount;               /**< Frame Count register */
    volatile uint32_t tmctrl;                   /**< Timestamp Control register */
    OS_MEM_RESERVED(0X110, 0X11C);
    volatile uint32_t symconf[8];               /**< Symbol Configuration registers 0...7 */
    volatile uint32_t intpretime;               /**< Interrupt Pretiming register */
    volatile uint32_t refframconf;              /**< Reference clock domain Frame structure Configuration register */
    volatile uint32_t framerollover;            /**< Frame count Rollover register */
    OS_MEM_RESERVED(0x148, 0x14C);
    volatile uint32_t dmaisr;                   /**< Interrupt Status register */
    volatile uint32_t dmamiar;                  /**< Maple Interrupt Acknowledge Register */
    volatile uint32_t dmadsr;                   /**< DMA Status Register */
    volatile uint32_t dmariqbdr;                /**< Receive IQ Buffer Displacement Register */
    volatile uint32_t dmatiqbdr;                /**< Transmit IQ Buffer Displacement Register */
    OS_MEM_RESERVED(0x160, 0x170);
    volatile uint32_t rftimer_intr_ctrl_reg;    /**< RF Timer Interrupt Control Register */
    volatile uint32_t rftimer_isr_reg;          /**< RF timer Interrupt Status Register */
    OS_MEM_RESERVED(0x178, 0x200);
} framer_timer_control_registers_t;

/**************************************************************************//**
 @Description   AIC registers type 1 (see #aic_map_t description).
*//***************************************************************************/
typedef struct
{
    dma_configuration_registers_t dma_configuration_registers; /**< DMA configuration registers. */
    framer_timer_control_registers_t framer_timer_control_registers; /**<  Framer/Timing Control registers. */
} aic_registers_type1_t;

/**************************************************************************//**
 @Description   AIC memory-mapped registers.

                The AIC registers are divided to 2 logical groups, which are
                referred in RM as type1 and type2.
*//***************************************************************************/
typedef struct
{
    aic_registers_type2_t           aic_type2; /**< AIC type 2 registers. */
    aic_registers_type1_t           adi_lanes[3]; /**< ADI lanes registers . */
    OS_MEM_RESERVED(0x800, 0xA00);
    aic_registers_type1_t           maxim_lanes[3]; /**< MAXIM lanes registers . */
} aic_map_t;

/** @} */ // end of arch_common_id
/** @} */ // end of aic_id

_OS_END_EXTERN_C

#endif __AIC_MEMMAP_H

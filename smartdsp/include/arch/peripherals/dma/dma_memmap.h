/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/01/08 15:31:21 $
 $Id: dma_memmap.h,v 1.4 2012/01/08 15:31:21 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/dma/dma_memmap.h,v $
 $Revision: 1.4 $
******************************************************************************/

/**************************************************************************//**
 
 @File          dma_memmap.h

 @Description   DMA Memory map file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __DMA_MEMMAP_H
#define __DMA_MEMMAP_H

#include "os_datatypes.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         arch_common_id  Peripheral Memory Maps
 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         dma_map_id    DMA Memory Map
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   DMA Early Dead First (EDF) Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint16_t       dma_edf_tdl_current_count;          /**< Current counter value */
    volatile uint8_t        dma_edf_tdl_threshold;              /**< Time to dead line threshold */
    volatile uint8_t        dma_edf_tdl_base_time;              /**< Current counter value */
} dma_edf_tdl_t;

/**************************************************************************//**
 @Description   DMA Registers
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       dma_bdbr[NUMBER_OF_DMA_CHANNELS];   /**< DMA Buffer Descriptor Base Register */
    volatile uint8_t        reserved0[0xC0];
    volatile uint32_t       dma_chcr[NUMBER_OF_DMA_CHANNELS];   /**< DMA Channel Configuration Register */
    volatile uint8_t        reserved1[0xC0];
    volatile uint32_t       dma_gcr;                            /**< DMA Global Configuration Register */
    volatile uint32_t       dma_cher;                           /**< DMA Channels Enable Register */
    volatile uint8_t        reserved2[0x4];
    volatile uint32_t       dma_chdr;                           /**< DMA Channels Disable Register */
    volatile uint8_t        reserved3[0x4];
    volatile uint32_t       dma_chfr;                           /**< DMA Channels Freeze Register */
    volatile uint8_t        reserved4[0xC];
    volatile uint32_t       dma_chdfr;                          /**< DMA Channels Defrost Register */
    volatile uint8_t        reserved5[0xC];
    dma_edf_tdl_t           dma_edf_tdl[NUMBER_OF_DMA_CHANNELS];  /**< DMA EDF Time to Dead Line Register */
    volatile uint8_t        reserved6[0xC0];
    volatile uint32_t       dma_edf_ctrl;                       /**< DMA EDF Control Register */
    volatile uint32_t       dma_edf_mr;                         /**< DMA EDF Mask Register */
    volatile uint8_t        reserved8[0x4];
    volatile uint32_t       dma_edf_mur;                        /**< DMA EDF Mask Update Register */
    volatile uint32_t       dma_edf_str;                        /**< DMA EDF Status Register */
    volatile uint8_t        reserved9[0x4];
    volatile uint32_t       dma_mr;                             /**< DMA Mask Register */
    volatile uint8_t        reserved10[0xC];
    volatile uint32_t       dma_mur;                            /**< DMA Mask Update Register */
    volatile uint32_t       dma_str;                            /**< DMA Status Register */
    volatile uint8_t        reserved11[0xC];
    volatile uint32_t       dma_err;                            /**< DMA Error Register */
    volatile uint32_t       dma_desr;                           /**< DMA Debug Event Status Register */
    volatile uint32_t       dma_lpcr;                           /**< DMA Local Profiling Configuration Register */
    volatile uint32_t       dma_rrpgur;                         /**< DMA Round Robin Priority Group Update Register */
    volatile uint32_t       dma_chastr;                         /**< DMA Channel Active Status Register */
    volatile uint8_t        reserved12[0x4];
    volatile uint32_t       dma_chfstr;                         /**< DMA Channel Freeze Status Register */
    volatile uint8_t        reserved13[0x74];
} dma_memmap_t;

/** @} */ // end of dma_map_id
/** @} */ // end of arch_common_id

_OS_END_EXTERN_C

#endif // __DMA_MEMMAP_H

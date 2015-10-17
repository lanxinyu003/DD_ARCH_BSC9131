/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:17 $
 $Id: dma_init.h,v 1.7 2012/11/22 16:28:17 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/dma/include/dma_init.h,v $
 $Revision: 1.7 $
******************************************************************************/

/**************************************************************************//**
 
 @File          dma_init.h

 @Description   DMA Initialization header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __DMA_INIT_H
#define __DMA_INIT_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         dma_id DMA Module API
 
 @Description   DMA Setup and control
 
 @{
*//***************************************************************************/
/***********************************************************//**
 @Group         dma_common_id Common DMA Module API
 @{
*//************************************************************/
/**************************************************************************//**
 @Group         dma_init_id DMA Initialization
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Sources of EDF clock.
                The first source is always the DMA clock, which is 1/2 the core
                clock, divided by 16.Other sources depend on the chip.
*//***************************************************************************/
typedef enum {
    EDF_CLK_SRC_DMA_CLK_DIV_16,
    EDF_CLK_SRC_1,
    EDF_CLK_SRC_2,
    EDF_CLK_SRC_3
} dma_edf_clk_src_t;


/**************************************************************************//**
 @Description   Modes of arbitration among DMA channels.
*//***************************************************************************/
typedef enum {
    GROUP_RR_ARBIRTATION = 0,
    EDF_ARBIRTATION
} dma_arbitration_t;

/**************************************************************************//**
 @Description   DMA bus ports.
*//***************************************************************************/
typedef enum {
    DMA_PORT_A = 0x00000000,
    DMA_PORT_B = 0x10000000
} dma_port_t;


/**************************************************************************//**
 @Description   Describe the bus error type.
*//***************************************************************************/
typedef struct parity_error_t
{
    bool  pram;           /**< BUS error occured in DMA PRAM. */
    bool  fifo;           /**< BUS error occured in DMA FIFO. */
    bool  bus_interface;  /**< BUS error occured in BUS interface. */
} parity_error_t;


/**************************************************************************//**
 @Description   Bus error callback type.
*//***************************************************************************/
typedef void (*dma_bus_error_cb_t)(void         *dma_ch,
                                 dma_port_t     port,
                                 bool           destination);


/**************************************************************************//**
 @Description   Parity error callback type.
*//***************************************************************************/
typedef void (*dma_parity_error_cb_t)(void               *dma_ch,
                                      parity_error_t     *par,
                                      bool               destination);


/**************************************************************************//**
 @Description   EDF violation callback type.
*//***************************************************************************/
typedef void (*dma_edf_violation_error_cb_t)(void *dma_ch);

/**************************************************************************//**
 @Description   Size is illegal callback type - e.g. size equals 0.
*//***************************************************************************/
typedef void (*dma_bd_size_error_cb_t)();


/**************************************************************************//**
 @Description   Implementation specific Configuration parameters of a DMA controller.
*//***************************************************************************/
typedef struct soc_dma_config_t
{
    dma_arbitration_t            dma_arbitration;
    /**< Either EDF or group RR */
    /**< Default - GROUP_RR_ARBIRTATION */
    dma_edf_clk_src_t            dma_edf_clk_src;
    /**< Reference clock for EDF counter. */
    /**< Default - EDF_CLK_SRC_DMA_CLK_DIV_16 */
    uint16_t                     dma_edf_clock_divider;
    /**< Reference clock for EDF divider. */
    /**< Default -  dma_edf_clk_src/dma_edf_clock_divider ~ 1/4ms. */
    dma_bus_error_cb_t           dma_bus_error_cb;
    /**< Callback for DMA bus error. */
    /**< Default -  NULL. */
    dma_parity_error_cb_t        dma_parity_error_cb;
    /**< Callback for DMA parity error. */
    /**< Default -  NULL. */
    dma_edf_violation_error_cb_t dma_edf_violation_error_cb;
    /**< Callback for DMA EDF violation. */
    /**< Default -  NULL. */
    dma_bd_size_error_cb_t       dma_bd_size_error_cb;
    /**< Callback for DMA BD size violation. */
    /**< Default -  NULL. */
    uint8_t                      dma_channel_enable[NUMBER_OF_DMA_CHANNELS];
    /**< On which core to enable the channel.
     * Default -  CORE0_ID - last COREX_ID, CORE0_ID - last COREX_ID and etc.
     * Every index in the array above  correlates to a DMA channel and is owned by the core that's ID is the value. */

} soc_dma_config_t;

/** @} */ // end of dma_init_id
/** @} */ // end of dma_common_id
/** @} */ // end of dma_id

_OS_END_EXTERN_C

#endif //__DMA_INIT_H

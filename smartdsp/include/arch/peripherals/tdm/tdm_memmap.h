/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/01/08 15:31:22 $
 $Id: tdm_memmap.h,v 1.3 2012/01/08 15:31:22 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/tdm/tdm_memmap.h,v $
 $Revision: 1.3 $
******************************************************************************/

/**************************************************************************//**

 @File          tdm_memmap.h

 @Description   TDM Internal Memory Map definitions and structures.

 @Cautions      None.

*//***************************************************************************/

#ifndef __TDM_MEMMAP_H
#define __TDM_MEMMAP_H


#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#include "os_datatypes.h"
#include "smartdsp_os_device.h"
#include "memmap.h"

/**************************************************************************//**
 @Group         arch_common_id  Peripheral Memory Maps
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         tdm_map_group   TDM Memory Map

 @Description   Definition of TDM registers and memory map

 @{
*//***************************************************************************/

typedef struct
{
    volatile uint8_t        tdmrxlclmem[2048];  /**< TDM Receive Local Memory    */
    volatile uint8_t        reserved1[2048];
    volatile uint32_t       tdmrcpr [256];      /**< TDM Receive Channel Parameters Register n. (n=0-255) */
    volatile uint8_t        reserved2[1024];
    volatile uint8_t        tdmtxlclmem[2048];  /**< TDM Transmit Local Memory */
    volatile uint8_t        reserved3[2048];
    volatile uint32_t       tdmtcpr [256];      /**< TDM Transmit Channel Parameters Register n. (n=0-255) */
    volatile uint8_t        reserved4[1024];
    volatile uint8_t        reserved5[3872];
    volatile uint32_t       tdmtsr;             /**< TDM Transmit Status Register    */
    volatile uint8_t        reserved6[4];
    volatile uint32_t       tdmrsr;             /**< TDM Receive Status Register    */
    volatile uint8_t        reserved7[4];
    volatile uint32_t       tdmasr;             /**< TDM Adaptation Status Register */
    volatile uint8_t        reserved8[4];
    volatile uint32_t       tdmter;             /**< TDM Transmit Event Register */
    volatile uint8_t        reserved9[4];
    volatile uint32_t       tdmrer;             /**< TDM Receive Event Register */
    volatile uint8_t        reserved10[4];
    volatile uint32_t       tdmtnb;             /**< TDM Transmit Number of Buffers */
    volatile uint8_t        reserved11[4];
    volatile uint32_t       tdmrnb;             /**< TDM Receive Number of Buffers */
    volatile uint8_t        reserved12[4];
    volatile uint32_t       tdmtdbdr;           /**< TDM Transmit Data Buffer Displacement Register */
    volatile uint8_t        reserved13[4];
    volatile uint32_t       tdmrdbdr;           /**< TDM Receive Data Buffer Displacement Register */
    volatile uint8_t        reserved14[4];
    volatile uint32_t       tdmasdr;            /**< TDM Adaptation Sync Distance Register */
    volatile uint8_t        reserved15[4];
    volatile uint32_t       tdmtier;            /**< TDM Transmit Interrupt Enable Register */
    volatile uint8_t        reserved16[4];
    volatile uint32_t       tdmrier;            /**< TDM Receive Interrupt Enable Register */
    volatile uint8_t        reserved17[4];
    volatile uint32_t       tdmtdbst;           /**< TDM Transmit Data Buffer Second Threshold */
    volatile uint8_t        reserved18[4];
    volatile uint32_t       tdmrdbst;           /**< TDM Receive Data Buffer Second Threshold */
    volatile uint8_t        reserved19[4];
    volatile uint32_t       tdmtdbft;           /**< TDM Transmit Data Buffer First Threshold */
    volatile uint8_t        reserved20[4];
    volatile uint32_t       tdmrdbft;           /**< TDM Receive Data Buffer First Threshold */
    volatile uint8_t        reserved21[4];
    volatile uint32_t       tdmtcr;             /**< TDM Transmit Control Register */
    volatile uint8_t        reserved22[4];
    volatile uint32_t       tdmrcr;             /**< TDM Receive Control Register */
    volatile uint8_t        reserved23[4];
    volatile uint32_t       tdmacr;             /**< TDM Receive Control Register */
    volatile uint8_t        reserved24[4];
    volatile uint32_t       tdmtgba;            /**< TDM Transmit Global Base Address */
    volatile uint8_t        reserved25[4];
    volatile uint32_t       tdmrgba;            /**< TDM Receive Global Base Address */
    volatile uint8_t        reserved26[4];
    volatile uint32_t       tdmtdbs;            /**< TDM Transmit Data Buffer Size */
    volatile uint8_t        reserved27[4];
    volatile uint32_t       tdmrdbs;            /**< TDM Receive Data Buffer Size */
    volatile uint8_t        reserved28[4];
    volatile uint32_t       tdmtfp;             /**< TDM Transmit Frame Parameters */
    volatile uint8_t        reserved29[4];
    volatile uint32_t       tdmrfp;             /**< TDM Receive Frame Parameters */
    volatile uint8_t        reserved30[4];
    volatile uint32_t       tdmtir;             /**< TDM Transmit Interface Register */
    volatile uint8_t        reserved31[4];
    volatile uint32_t       tdmrir;             /**< TDM Receive Interface Register */
    volatile uint8_t        reserved32[4];
    volatile uint32_t       tdmgir;             /**< TDM General Interface Register */
    volatile uint8_t        reserved33[4];
} tdm_map_t;




/** @} */ // end of tdm_map_group
/** @} */ // arch_common_id

_OS_END_EXTERN_C

#endif  // __TDM_MEMMAP_H

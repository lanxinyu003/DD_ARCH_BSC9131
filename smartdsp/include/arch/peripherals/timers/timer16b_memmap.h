/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:28:24 $
 $Id: timer16b_memmap.h,v 1.7 2012/11/22 16:28:24 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/timers/timer16b_memmap.h,v $
 $Revision: 1.7 $
******************************************************************************/

/**************************************************************************//**
 
 @File          timer16b_memmap.h

 @Description   Timer 16 bit Memory map file.

 @Cautions      None.
 
*//***************************************************************************/
#ifndef TIMER16B_MEMMAP_H_
#define TIMER16B_MEMMAP_H_

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
 @Group         timer16b_map_id    Timers 16 bit Memory Map
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   SoC Timer Quadrate Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint16_t       tmr_cmp1;       /**< Timer channel Compare Register 1 */
    volatile uint16_t       reserved0;
    volatile uint16_t       tmr_cmp2;       /**< Timer channel Compare Register 2 */
    volatile uint16_t       reserved1;
    volatile uint16_t       tmr_cap;        /**< Timer channel Capture Register */
    volatile uint16_t       reserved2;
    volatile uint16_t       tmr_load;       /**< Timer channel Load Register */
    volatile uint16_t       reserved3;
    volatile uint16_t       tmr_hold;       /**< Timer channel Hold Register */
    volatile uint16_t       reserved4;
    volatile uint16_t       tmr_cntr;       /**< Timer channel Counter Register */
    volatile uint16_t       reserved5;
    volatile uint16_t       tmr_ctrl;       /**< Timer channel Control Register */
    volatile uint16_t       reserved6;
    volatile uint16_t       tmr_sctl;       /**< Timer channel Status and Control Register */
    volatile uint16_t       reserved7;
    volatile uint16_t       tmr_cmpld1;     /**< Timer channel Compare 1 Load Register */
    volatile uint16_t       reserved8;
    volatile uint16_t       tmr_cmpld2;     /**< Timer channel Compare 2 Load Register */
    volatile uint16_t       reserved9;
    volatile uint16_t       tmr_comscr;     /**< Timer channel Comparator Status and Control Register */
    volatile uint16_t       reserved10;
    volatile uint8_t        reserved11[20];
} timer_map_t;

/**************************************************************************//**
 @Description   SoC Timer Registers.
*//***************************************************************************/
typedef struct
{
    timer_map_t             tmr[NUM_OF_HW_TIMERS_16b_PER_MODULE];    /**< Timer Channels Registers */
 
} tmr_map_t;

/** @} */ // end of timer16b_map_id
/** @} */ // end of arch_common_id

_OS_END_EXTERN_C

#endif /* TIMER16B_MEMMAP_H_ */

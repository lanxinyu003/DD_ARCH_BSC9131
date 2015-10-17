/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:28:24 $
 $Id: timer32b_memmap.h,v 1.9 2012/11/22 16:28:24 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/timers/timer32b_memmap.h,v $
 $Revision: 1.9 $
******************************************************************************/

/**************************************************************************//**
 
 @File          timer32b_memmap.h

 @Description   Timer 32 bit Memory map file.

 @Cautions      None.
 
*//***************************************************************************/
#ifndef TIMER32B_MEMMAP_H_
#define TIMER32B_MEMMAP_H_

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
 @Group         timer32b_map_id    Timers 32 bit Memory Map
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   SoC 32 bit Timer registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       tmr_cmp1;       /**< Timer channel Compare Register 1 */
    volatile uint32_t       tmr_cmp2;       /**< Timer channel Compare Register 2 */
    volatile uint32_t       tmr_cap;        /**< Timer channel Capture Register */
    volatile uint32_t       tmr_load;       /**< Timer channel Load Register */
    volatile uint32_t       tmr_hold;       /**< Timer channel Hold Register */
    volatile uint32_t       tmr_cntr;       /**< Timer channel Counter Register */
    volatile uint32_t       tmr_ctrl;       /**< Timer channel Control Register */
    volatile uint32_t       tmr_sctl;       /**< Timer channel Status and Control Register */
    volatile uint32_t       tmr_cmpld1;     /**< Timer channel Compare 1 Load Register */
    volatile uint32_t       tmr_cmpld2;     /**< Timer channel Compare 2 Load Register */
    volatile uint32_t       tmr_comscr;     /**< Timer channel Comparator Status and Control Register */
    volatile uint8_t        reserved1[0x14];
} timer_32b_map_t;

/**************************************************************************//**
 @Description   SoC 32 bit Timer Module.
*//***************************************************************************/
typedef struct
{
    timer_32b_map_t         tmr[NUM_OF_HW_TIMERS_32b_PER_MODULE];    /**< 32 bit Timer Channels Registers */
    volatile uint32_t       tmr_glb;        /**< Timer_32b Global System Timer Register */
    volatile uint32_t       tmr_glbctl;     /**< Timer_32b Global System Timer Control Register */
    volatile uint32_t       tmr_saf;        /**< Timer_32b Timer Set and Forget Register */
    volatile uint32_t       tmr_clrl;       /** < Timer_32b Timer Clear Lock Register */
    volatile uint8_t        reserved1[TIMER32_MODULE_RESERVED];
}tmr32b_map_t;

/** @} */ // end of timer32b_map_id
/** @} */ // end of arch_common_id

_OS_END_EXTERN_C

#endif /* TIMER32B_MEMMAP_H_ */

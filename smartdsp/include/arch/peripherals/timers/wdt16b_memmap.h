/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:28:24 $
 $Id: wdt16b_memmap.h,v 1.5 2012/11/22 16:28:24 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/timers/wdt16b_memmap.h,v $
 $Revision: 1.5 $
******************************************************************************/

/**************************************************************************//**
 
 @File          wdt16b_memmap.h

 @Description   WatchDog Timer 16 bit Memory map file.

 @Cautions      None.
 
*//***************************************************************************/
#ifndef WDT16B_MEMMAP_H_
#define WDT16B_MEMMAP_H_

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
 @Group         wdt_map_id    Watchdog timer Memory Map
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Watch Dog Timer 16 bit (WDT) Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint8_t        reserved0[0x04];
    volatile uint32_t       swcrr;          /**< System Watchdog Control Register */
    volatile uint32_t       swcnr;          /**< System Watchdog Count Register */
    volatile uint8_t        reserved1[0x02];
    volatile uint16_t       swsrr;          /**< System Watchdog Service Register */
    volatile uint8_t        reserved2[0xF0];
} wdt_map_t;

/** @} */ // end of wdt_map_id
/** @} */ // end of arch_common_id

_OS_END_EXTERN_C

#endif /* WDT16B_MEMMAP_H_ */

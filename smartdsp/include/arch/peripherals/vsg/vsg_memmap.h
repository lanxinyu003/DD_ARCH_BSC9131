/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/05/15 10:42:34 $
 $Id: vsg_memmap.h,v 1.9 2014/05/15 10:42:34 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/vsg/vsg_memmap.h,v $
 $Revision: 1.9 $
******************************************************************************/

/**************************************************************************//**
 
 @File          vsg_memmap.h

 @Description   Virtual Interrupts, HW Semaphores, GPIO Memory maps file.

 @Cautions      None.
 
*//***************************************************************************/
#ifndef __VSG_MEMMAP_H_
#define __VSG_MEMMAP_H_

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
 @Group         vsg_map_id    VSG Memory Map
 
 @Description   Virtual Interrupts, HW Semaphores, GPIO Memory maps.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Virtual Interrupts Generator (GIC) Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       vigr;           /**< Virtual Interrupt Generation Register */
    OS_MEM_RESERVED(0x4, 0x8);
    volatile uint32_t       visr;           /**< Virtual Interrupt Status Register */
    OS_MEM_RESERVED(0xC, 0x100);
} vint_map_t;

/**************************************************************************//**
 @Description   Hardware Semaphore Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       hsmpr;          /**< Hardware Semaphore Register, 8 LST bits */
    volatile uint8_t        reserved2[0x4];
} hs_map_t;


/**************************************************************************//**
 @Description   General Purpose I/O (GPIO) Registers.
*//***************************************************************************/
#if defined(PSC9131) || defined(PSC9132)
typedef struct
{
    volatile uint32_t gpdir;        /**< direction register */
    volatile uint32_t gpodr;        /**< open drain register */
    volatile uint32_t gpdat;        /**< data register */
    volatile uint32_t gpier;        /**< interrupt event register */
    volatile uint32_t gpimr;        /**< interrupt mask register */
    volatile uint32_t gpicr;        /**<  external interrupt control register */
    OS_MEM_RESERVED(0x18, 0x100);
}gpio_map_t;
#else
typedef struct
{
    volatile uint32_t       podr;           /**< Pin Open-Drain Register */
    volatile uint8_t        reserved1[4] ;
    volatile uint32_t       pdat;           /**< Pin Data Register */
    volatile uint8_t        reserved2[4] ;
    volatile uint32_t       pdir;           /**< Pin Data Direction Registers */
    volatile uint8_t        reserved3[4] ;
    volatile uint32_t       par;            /**< Pin Assignment Register */
    volatile uint8_t        reserved4[4] ;
    volatile uint32_t       psor;           /**< Pin Special Options Registers */
    volatile uint8_t        reserved5[0xDC];
} gpio_map_t;
#endif // PSC9132
/** @} */ // end of vsg_map_id
/** @} */ // end of arch_common_id

_OS_END_EXTERN_C

#endif /* __VSG_MEMMAP_H_ */

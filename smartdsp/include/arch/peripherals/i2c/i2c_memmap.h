/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2013/06/10 11:39:00 $
 $Id: i2c_memmap.h,v 1.2 2013/06/10 11:39:00 b42006 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/i2c/i2c_memmap.h,v $
 $Revision: 1.2 $
******************************************************************************/
 
/******************************************************************************
 Copyright © 1995-2003,2004,2005-2010 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not
 evidence any actual or intended publication of such source code.
 
*******************************************************************************/

/**************************************************************************//**

 @File          i2c_memmap.h

 @Description   I2C Memory Map definitions and structures.

 @Cautions      None.

*//***************************************************************************/

#ifndef __I2C_MEMMAP_H
#define __I2C_MEMMAP_H


#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#include "smartdsp_os_device.h"

/**************************************************************************//**
 @Group         arch_common_id  Peripheral Memory Maps
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         i2c_map_group   I2C Memory Map

 @Description   Definition of I2C registers and memory map

 @{
*//***************************************************************************/


/**************************************************************************//**
 @Description   I2C Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint8_t        i2cadr;         /**< I2C address register */
    OS_MEM_RESERVED(0X1, 0X4);
    volatile uint8_t        i2cfdr;         /**< I2C frequesncy divider register */
    OS_MEM_RESERVED(0X5, 0X8);
    volatile uint8_t        i2ccr;          /**< I2C control register */
    OS_MEM_RESERVED(0X9, 0XC);
    volatile uint8_t        i2csr;          /**< I2C status register */
    OS_MEM_RESERVED(0XD, 0X10);
    volatile uint8_t        i2cdr;          /**< I2C data register */
    OS_MEM_RESERVED(0X11, 0X14);
    volatile uint8_t        i2cdfsrr;       /**< I2C digital filter sampling rate register */
    OS_MEM_RESERVED(0X15, 0X100);
} i2c_map_t;

/** @} */ // end of i2c_map_group
/** @} */ // arch_common_id


_OS_END_EXTERN_C

#endif  // __I2C_MEMMAP_H

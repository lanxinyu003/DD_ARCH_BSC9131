/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/01/15 14:07:36 $
 $Id: psc913x_gpio.h,v 1.2 2013/01/15 14:07:36 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/psc913x_gpio.h,v $
 $Revision: 1.2 $
******************************************************************************/

/******************************************************************************

 @File          psc913x_gpio.h

 @Description   PSC913X GPIO header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __PSC913X_GPIO_H
#define __PSC913X_GPIO_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         gpio_id  GPIO (general purpose I/O) API
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         gpio_913x_id  PSC913x GPIO (general purpose I/O) API
 @{
*//***************************************************************************/

#define OS_NUM_OF_GPIO          3
#define OS_NUM_OF_GPIO_PORTS    32

/**************************************************************************//**
 @Description   GPIO interrupts edge options enumeration.
*//***************************************************************************/
typedef enum
{
    OS_GPIO_EDGE_ANY_CHANGE     = 0,            /**< Any change on the state of the port generates an interrupt request */
    OS_GPIO_EDGE_HIGH_TO_LOW    = 1             /**< High-to-low change on the port generates an interrupt request */
}gpio_edge_t;

/**************************************************************************//**
 @Function      osGpioHwiConfigure

 @Description   Initializes the selected GPIO and port to generate interrupts
 

 @Param         gpio_num - GPIO module
 @Param         port     - GPIO port
 @Param         edge     - 0 - Any change on the state of the port generates an interrupt request
                           1 - High-to-low change on the port generates an interrupt request
 
 @Return        OS_SUCCESS on success; OS_FAIL otherwise.

 @Cautions      It is also neccessary to call osHwiCreate in order to enable
                receiving GPIO interrutps and handler

*//****************************************************************************/
os_status osGpioHwiConfigure(int gpio_num, int port, bool edge);

/**************************************************************************//**
 @Function      osGpioOutputSet

 @Description   Initializes the selected GPIO and port as an output and sets
                the value
 

 @Param         gpio_num - GPIO module
 @Param         port     - GPIO port
 @Param         value    - the value to put on the port
 
 @Return        OS_SUCCESS on success; OS_FAIL otherwise.

 @Cautions      None

*//****************************************************************************/
os_status osGpioOutputSet(int gpio_num, int port, bool value);

/**************************************************************************//**
 @Function      osGpioDataRead

 @Description   Read GPIO port and return its value
 

 @Param         gpio_num - GPIO module
 @Param         port     - GPIO port
 
 @Return        value of the port (0/1)

 @Cautions      None

*//****************************************************************************/
bool osGpioDataRead(int gpio_num, int port);

/**************************************************************************//**
 @Function      osGpioEventReadAndClear

 @Description   Read if event occured on GPIO port. If event occured - clean it

 @Param         gpio_num - GPIO module
 @Param         port     - GPIO port
 
 @Return        0 - no event occured
                1 - event occured

 @Cautions      None

*//****************************************************************************/
bool osGpioEventReadAndClear(int gpio_num, int port);

/** @} */ //gpio_913x_id

/** @} */ // gpio_id

_OS_END_EXTERN_C

#endif // __PSC913X_GPIO_H

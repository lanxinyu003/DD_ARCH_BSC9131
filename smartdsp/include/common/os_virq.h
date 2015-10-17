/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/02/20 13:05:37 $
 $Id: os_virq.h,v 1.10 2014/02/20 13:05:37 b42006 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_virq.h,v $
 $Revision: 1.10 $
******************************************************************************/

/**************************************************************************//**
 @File          os_virq.h

 @Description   OS virtual interrupts header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_VIRQ_H
#define __OS_VIRQ_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         vi_id Virtual Interrupts API

 @Description   Virtual interrupts setup and control.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osVirtualInterruptInitialize

 @Description   Initialize the virtual interrupts mechanism.

 @Retval        os_status
*//***************************************************************************/
os_status osVirtualInterruptsInitialize();

/**************************************************************************//**
 @Function      osVirtualInterruptFind

 @Description   Get a virtual interrupt handle.
 
 
 @Param[out]    interrupt_handle - Interrupt number, can be used to call osHwiCreate.

 @Retval        OS_SUCCESS              : Found.
 @Retval        OS_ERR_VIRQ_UNAVAILABLE : No more virq in the system.
*//***************************************************************************/
os_status osVirtualInterruptFind(uint32_t *interrupt_handle);


/**************************************************************************//**
 @Function      osVirtualInterruptRelease

 @Description   Free a virtual interrupt handle.
 
 
 @Param[in]     interrupt_handle - Handle to an interrupt number.

 @Retval        OS_SUCCESS.
*//***************************************************************************/
os_status osVirtualInterruptRelease(uint32_t interrupt_handle);

/**************************************************************************//**
 @Function      osVirtualInterruptReserve

 @Description   Reserve a virtual interrupt handle; Same as osVirtualInterruptFind()
                but for a specific VIRQ number.
 
 @Param[in]     interrupt_handle - Handle to an interrupt number.

 @Retval        OS_SUCCESS.
*//***************************************************************************/
os_status osVirtualInterruptReserve(uint32_t interrupt_handle);

/**************************************************************************//**
 @Function      osVirtualInterruptActivate

 @Description   Activate virtual interrupt.
 
 
 @Param[out]    interrupt_handle - Interrupt number, received from osVirtualInterruptFind.

 @Retval        OS_SUCCESS              : Activated.
*//***************************************************************************/
os_status osVirtualInterruptActivate(uint32_t interrupt_handle);


/** @} */ // vi_id

_OS_END_EXTERN_C

#endif // __OS_VIRQ_H

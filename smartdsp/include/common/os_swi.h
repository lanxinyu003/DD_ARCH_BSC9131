/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/04/01 15:23:50 $
 $Id: os_swi.h,v 1.21 2012/04/01 15:23:50 b20727 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_swi.h,v $
 $Revision: 1.21 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_swi.h

 @Description   OS Software Interrupts header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __OS_SWI_H
#define __OS_SWI_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#ifdef MMU_SUPPORT
#include "os_mmu.h"
#endif

/**************************************************************************//**
 @Group         swi_id Software Interrupts API

 @Description   Software Interrupts setup and control.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         swi_runtime_id Software Interrupts Runtime
 
 @{
*//***************************************************************************/


/*************************************************************//**
 @Collection    SWI Objects values
 
                SWI objects can be created dynamically by using osSwiFind() or
                they can be statically referenced by the definitions below.
 @{
*//**************************************************************/
#define OS_SWI0                 0       /**< Reserved for OS Tick functionality. */
#define OS_SWI1                 1
#define OS_SWI2                 2
#define OS_SWI3                 3
#define OS_SWI4                 4
#define OS_SWI5                 5
#define OS_SWI6                 6
#define OS_SWI7                 7
#define OS_SWI8                 8
#define OS_SWI9                 9
#define OS_SWI10                10
#define OS_TIMER_SWI            OS_SWI0 /**< Equals OS_SWI0 - reserved for OS Tick functionality. */
/* @} */


/*************************************************************//**
 @Collection    SWI Priorities
 @{
*//**************************************************************/
#define OS_SWI_PRIORITY0        0       /**< Highest SWI priority. */
#define OS_SWI_PRIORITY1        1
#define OS_SWI_PRIORITY2        2
#define OS_SWI_PRIORITY3        3
#define OS_SWI_PRIORITY4        4
#define OS_SWI_PRIORITY5        5
#define OS_SWI_PRIORITY6        6
#define OS_SWI_PRIORITY7        7
#define OS_SWI_PRIORITY8        8
#define OS_SWI_PRIORITY9        9
#define OS_SWI_PRIORITY10       10
#define OS_SWI_PRIORITY11       11
#define OS_SWI_PRIORITY12       12
#define OS_SWI_PRIORITY13       13
#define OS_SWI_PRIORITY14       14
#define OS_SWI_PRIORITY15       15
#define OS_SWI_PRIORITY_ALL     16
#define OS_SWI_PRIORITY_NONE    254     /**< Disabled SWI priority */
/* @} */


/*************************************************************//**
 @Collection    SWI Status values (of os_swi_status)
 @{
*//**************************************************************/
#define OS_SWI_UNUSED           0
#define OS_SWI_DELETED          1
#define OS_SWI_ACQUIRED         2
#define OS_SWI_CREATED          3
#define OS_SWI_READY            4
/* @} */


/**************************************************************************//**
 @Function      osSwiCreate

 @Description   Attaches an interrupt handler to a software interrupt.
 
 @Param[in]     handler     -  The software interrupt function.
 @Param[in]     swi_num     -  The software interrupt number.
 @Param[in]     priority    -  The software interrupt priority.
 @Param[in]     user_id     -  User ID.

 @Retval        OS_SUCCESS                  - The software interrupt handler was successfully attached.
 @Retval        OS_ERR_SWI_INVALID          - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_FUNCTION_INVALID - NULL software interrupt handler.
 @Retval        OS_ERR_SWI_PRIORITY_INVALID - Invalid software interrupt priority.
 @Retval        OS_ERR_SWI_ALREADY_CREATED  - The software interrupt was already created.
*//***************************************************************************/
os_status       osSwiCreate(os_swi_function handler,
                            os_swi_handle   swi_num,
                            os_swi_priority priority,
                            os_user_id      user_id);


/**************************************************************************//**
 @Function      osSwiDelete

 @Description   Detaches the given software interrupt from its handler.
 
 @Param[in]     swi_num -  The software interrupt number.

 @Retval        OS_SUCCESS              - The software interrupt handler was successfully detached.
 @Retval        OS_ERR_SWI_INVALID      - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_NOT_CREATED  - The software interrupt was not created, or it was already deleted.
*//***************************************************************************/
os_status       osSwiDelete(os_swi_handle swi_num);


/**************************************************************************//**
 @Function      osSwiActivate

 @Description   Activates the given software interrupt.
 
 @Param[in]     swi_num - The software interrupt number.

 @Retval        OS_SUCCESS              - The software interrupt was successfully activated.
 @Retval        OS_ERR_SWI_INVALID      - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_NOT_CREATED  - The software interrupt was not created, or it was already deleted.
*//***************************************************************************/
os_status       osSwiActivate(os_swi_handle swi_num);


/**************************************************************************//**
 @Function      osSwiEnable

 @Description   Enables all software interrupts with a given priority.
 
 @Param[in]     priority -  Priority of software interrupts to enable.
*//***************************************************************************/
void            osSwiEnable(os_swi_priority priority);


/**************************************************************************//**
 @Function      osSwiDisable

 @Description   Disables all software interrupts.
 
 @Return        Previous enabled priority of software interrupts
*//***************************************************************************/
os_swi_priority osSwiDisable();


/**************************************************************************//**
 @Function      osSwiCountInc

 @Description   Increments the count of the given software interrupt.

                The software interrupt will also be activated.

 @Param[in]     swi_num -  The software interrupt number.
 
 @Retval        OS_SUCCESS              - The software interrupt count was incremented and activated.
 @Retval        OS_ERR_SWI_INVALID      - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_NOT_CREATED  - The software interrupt was not created, or it was already deleted.
*//***************************************************************************/
os_status       osSwiCountInc(os_swi_handle swi_num);


/**************************************************************************//**
 @Function      osSwiCountDec

 @Description   Decrements the count of the given software interrupt.
 
                The software interrupt will also be activated if the count
                is zero after the operation.

 @Param[in]     swi_num -  The software interrupt number.
 
 @Retval        OS_SUCCESS              - The count was decremented successfully (may be followed by activation).
 @Retval        OS_ERR_SWI_INVALID      - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_NOT_CREATED  - The software interrupt was not created, or it was already deleted.
*//***************************************************************************/
os_status       osSwiCountDec(os_swi_handle swi_num);


/**************************************************************************//**
 @Function      osSwiCountGet

 @Description   Gets the count of the given software interrupt.

 @Param[in]     swi_num -  The software interrupt number.
 @Param[out]    count   -  Receives the software interrupt count.
 
 @Retval        OS_SUCCESS              - The value in @a count is valid.
 @Retval        OS_ERR_SWI_INVALID      - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_NOT_CREATED  - The software interrupt was not created, or it was already deleted.
*//***************************************************************************/
os_status       osSwiCountGet(os_swi_handle swi_num, os_swi_count  *count);


/**************************************************************************//**
 @Function      osSwiCountSet

 @Description   Sets the count of the given software interrupt.

 @Param[in]     swi_num -  The software interrupt number.
 @Param[in]     count   -  The new software interrupt count.
 
 @Retval        OS_SUCCESS              - The software interrupt count was set.
 @Retval        OS_ERR_SWI_INVALID      - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_NOT_CREATED  - The software interrupt was not created, or it was already deleted.
*//***************************************************************************/
os_status       osSwiCountSet(os_swi_handle swi_num, os_swi_count  count);


/**************************************************************************//**
 @Function      osSwiPriorityGet

 @Description   Gets the priority of the given software interrupt.

 @Param[in]     swi_num -  The software interrupt number.
 @Param[out]    priority -  Receives the software interrupt priority.

 @Retval        OS_SUCCESS              - The value in @a priority is valid.
 @Retval        OS_ERR_SWI_INVALID      - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_NOT_CREATED  - The software interrupt was not created, or it was already deleted.
*//***************************************************************************/
os_status       osSwiPriorityGet(os_swi_handle      swi_num, os_swi_priority    *priority);


/**************************************************************************//**
 @Function      osSwiPrioritySet

 @Description   Sets the priority of the given software interrupt.

 @Param[in]     swi_num     -  The software interrupt number.
 @Param[in]     priority    -  The new software interrupt priority.
 
 @Retval        OS_SUCCESS                  - The software interrupt priority was set.
 @Retval        OS_ERR_SWI_INVALID          - Invalid software interrupt number.
 @Retval        OS_ERR_SWI_PRIORITY_INVALID - Invalid software interrupt priority.
 @Retval        OS_ERR_SWI_NOT_CREATED      - The software interrupt was not created, or it was already deleted.

 @Cautions      The new priority will be valid only for the next activation.
*//***************************************************************************/
os_status       osSwiPrioritySet(os_swi_handle      swi_num, os_swi_priority    priority);


/**************************************************************************//**
 @Function      osSwiStatus

 @Description   Gets the status of the given software interrupt.

 @Param[in]     swi_num -  The software interrupt number.
 
 @Return        The status of the specified software interrupt; if the software
                interrupt is not valid, SWI_UNUSED will be returned.
*//***************************************************************************/
os_swi_status   osSwiStatus(os_swi_handle swi_num);


/**************************************************************************//**
 @Function      osSwiFind

 @Description   Finds the first available software interrupt number.

 @Param[out]    swi_num -  Receives the software interrupt number.
 
 @Retval        OS_SUCCESS              - Found an available software interrupt number.
 @Retval        OS_ERR_SWI_UNAVAILABLE  - No software interrupt number is available.
*//***************************************************************************/
os_status       osSwiFind(os_swi_handle *swi_num);


/**************************************************************************//**
 @Function      osSwiSelf

 @Description   Gets the number of the currently active software interrupt.
 
 @Param[out]    swi_num -  Receives the active software interrupt number.

 @Retval        OS_SUCCESS              - The value in @a swi_num is valid.
 @Retval        OS_ERR_SWI_NOT_ACTIVE   - No software interrupt is active; the function was not called from within an active software interrupt function.
*//***************************************************************************/
os_status       osSwiSelf(os_swi_handle *swi_num);

#ifdef MMU_SUPPORT
#if defined(SC3X00) || defined(SC3X50)
/**************************************************************************//**
 @Function      osSwiMmuProgContextSet

 @Description   Sets program MMU context of SWI (by value)
 
 @Param[in]     swi_num -  The software interrupt number.
 @Param[in]     context -  Context to set.
*//***************************************************************************/
void osSwiMmuProgContextSet(os_swi_handle  swi_num, os_mmu_context_t* context);

/**************************************************************************//**
 @Function      osSwiMmuDataContextSet

 @Description   Sets data MMU context of SWI (by value)
 
 @Param[in]     swi_num - The software interrupt number.
 @Param[in]     context - Context to set.
*//***************************************************************************/
void osSwiMmuDataContextSet(os_swi_handle  swi_num, os_mmu_context_t* context);

/**************************************************************************//**
 @Function      osSwiMmuDataContextGet

 @Description   Gets a reference to data MMU context of SWI
 
 @Param[in]     swi_num - The software interrupt number.

 @Return        context - pointer to context.
*//***************************************************************************/
os_mmu_context_t const * osSwiMmuDataContextGet(os_swi_handle swi_num);


/**************************************************************************//**
 @Function      osSwiMmuProgContextGet

 @Description   Gets a reference to MMU context of SWI
 
 @Param[in]     swi_num -  The software interrupt number.

 @Return        context - pointer to context.
*//***************************************************************************/
os_mmu_context_t const * osSwiMmuProgContextGet(os_swi_handle swi_num);
#endif
#endif MMU_SUPPORT

/** @} */ // swi_runtime_id
/** @} */ // swi_id

_OS_END_EXTERN_C

#endif // __OS_SWI_H

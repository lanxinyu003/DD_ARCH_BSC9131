/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/04/25 14:22:18 $
 $Id: os_hw_timer.h,v 1.31 2013/04/25 14:22:18 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_hw_timer.h,v $
 $Revision: 1.31 $
******************************************************************************/

/******************************************************************************
 
 @File          os_hw_timer.h

 @Description   OS Hardware Timers header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_HW_TIMER_H
#define __OS_HW_TIMER_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         hwt_id   Hardware Timers API

 @Description   Hardware Timers setup and control.
 
 @{
*//***************************************************************************/


/*************************************************************************//**
 @Description   OS Hardware Timer objects values
 
                Hardware Timer objects can be created dynamically by using
                osHwTimerFind() or they can be statically referenced by the
                definitions below.
 
*//***************************************************************************/
enum
{
    OS_HW_TIMER0,
    OS_HW_TIMER1,
    OS_HW_TIMER2,
    OS_HW_TIMER3,
    OS_HW_TIMER4,
    OS_HW_TIMER5,
    OS_HW_TIMER6,
    OS_HW_TIMER7,
    OS_HW_TIMER8,
    OS_HW_TIMER9,
    OS_HW_TIMER10,
    OS_HW_TIMER11,
    OS_HW_TIMER12,
    OS_HW_TIMER13,
    OS_HW_TIMER14,
    OS_HW_TIMER15
};


/**************************************************************************//**
 @Function      osHwTimerCreate

 @Description   Creates a hardware timer and sets its parameters.
 
 @Param[in]    timer_num -  The hardware timer number.
 @Param[in]    mode -  Timer mode (one-shot, periodic, etc.).
 @Param[in]    interval -  The timer interval.
 @Param[in]    source -  The source clock for the timer.
 @Param[in]    handler -  The function to call when the timer expires.
 @Param[in]    priority -  The hardware timer priority.

 @Retval        OS_SUCCESS                          - The hardware timer was
                                                      successfully created.
 @Retval        OS_ERR_HW_TM_INVALID                - Invalid hardware timer
                                                      number.
 @Retval        OS_ERR_HW_TM_MODE_INVALID           - Invalid or unsupported
                                                      hardware timer mode.
 @Retval        OS_ERR_HW_TM_INTERVAL_UNSUPPORTED   - Unsupported interval.
 @Retval        OS_ERR_HW_TM_SOURCE_INVALID         - Invalid source clock.
 @Retval        OS_ERR_HW_TM_PRIORITY_INVALID       - Invalid HWI priority.
 @Retval        OS_ERR_HW_TM_ALREADY_CREATED        - The hardware timer is
                                                      already created.
*//***************************************************************************/
os_status       osHwTimerCreate(os_timer_handle       timer_num,
                                os_timer_mode         mode,
                                os_timer_interval     interval,
                                os_hw_timer_source    source,
                                os_hwi_function       handler,
                                os_hwi_priority       priority);
 


/**************************************************************************//**
 @Function      osHwTimerSetInterval

 @Description   Creates a hardware timer and sets its parameters.
 
 @Param[in]    timer_num -  The hardware timer number.
 @Param[in]    interval -  The timer interval.

 @Retval        OS_SUCCESS  - The interval was successfully set.
 @Retval        OS_ERR_HW_TM_INVALID          - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_INTERVAL_UNSUPPORTED   - Unsupported interval.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.

*//***************************************************************************/
os_status   osHwTimerSetInterval(os_timer_handle timer_num, os_timer_interval interval);


/**************************************************************************//**
 @Function      osHwTimerDelete

 @Description   Stops and deletes the given hardware timer from the system.
 
 @Param[in]    timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS                  - The hardware timer was
                                              successfully deleted.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.

*//***************************************************************************/
os_status       osHwTimerDelete(os_timer_handle timer_num);


/**************************************************************************//**
 @Function      osHwTimerStart

 @Description   Starts the given hardware timer.
 
 @Param[in]    timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS                  - The hardware timer was
                                              successfully started.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.
 
*//***************************************************************************/
os_status       osHwTimerStart(os_timer_handle timer_num);


/**************************************************************************//**
 @Function      osHwTimerStop

 @Description   Stops the given hardware timer.
 
 @Param[in]    timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS                  - The hardware timer was
                                              successfully stopped.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.

*//***************************************************************************/
os_status       osHwTimerStop(os_timer_handle timer_num);


/**************************************************************************//**
 @Function      osHwTimerClearEvent

 @Description   Clears the event bit of the given hardware timer.
 
 @Param[in]    timer_num -  The hardware timer number.

*//***************************************************************************/
void            osHwTimerClearEvent(os_timer_handle timer_num);


/**************************************************************************//**
 @Function      osHwTimerFind

 @Description   Finds the first available hardware timer number.
 
 @Param[out]    timer_num -  Receives the hardware timer number.

 @Retval        OS_SUCCESS                  - Found an available hardware
                                              timer number.
 @Retval        OS_ERR_HW_TM_UNAVAILABLE    - No hardware timer number is
                                              available.

*//***************************************************************************/
os_status       osHwTimerFind(os_timer_handle *timer_num);


/**************************************************************************//**
 @Function      osHwTimerTypeFind

 @Description   Finds the first available hardware timer number of a specific type.
 
 @Param[out]    timer_num -  Receives the hardware timer number.
 @Param[out]    type      -  Type of timer to find - architecture dependent.

 @Retval        OS_SUCCESS                  - Found an available hardware
                                              timer number.
 @Retval        OS_ERR_HW_TM_UNAVAILABLE    - No hardware timer number is
                                              available.

 @Cautions      Not all architectures support this API or support types
                other than 0

*//***************************************************************************/
os_status       osHwTimerTypeFind(os_timer_handle *timer_num, int type);


/**************************************************************************//**
 @Function      osHwTimerFindForce

 @Description   Allocates a given hardware timer number.
 
 @Param[in]     timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS                  - The hardware timer was
                                              successfully allocated.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_UNAVAILABLE    - The hardware timer number is not
                                              available (already allocated).

*//***************************************************************************/
os_status       osHwTimerFindForce(os_timer_handle timer_num);


/**************************************************************************//**
 @Function      osHwTimerValueGet

 @Description   Return the hardware timer value.
 
 @Param[in]     timer_num -  The hardware timer number.
 @Param[out]    value     -  The hardware timer value

 @Retval        OS_SUCCESS              - Succeeded in reading the value.
 @Retval        OS_ERR_HW_TM_INVALID    - Invalid hardware timer number.

*//***************************************************************************/
os_status       osHwTimerValueGet(os_timer_handle timer_num, os_timer_interval *value);

/**************************************************************************//**
 @Function      osHwTimerSyncStart

 @Description   Start a synchronized run.
 
 @Param[out]    core_mask -  The synchronized cores mask.
                             In order to start all six cores timers together,
                             one should write 0x0000003F to this register.

*//***************************************************************************/
void osHwTimerSyncStart(uint32_t core_mask);

/**************************************************************************//**
 @Function      osHwTimerSyncStop

 @Description   Stop a synchronized run.
 
 @Param[out]    core_mask -  The synchronized cores mask.
                             In order to stop all six cores timers together,
                             one should write 0x0000003F to this register.

*//***************************************************************************/
void osHwTimerSyncStop(uint32_t core_mask);

/**************************************************************************//**
 @Function      osHwTimerFreeze

 @Description   Freeze the hardware timer run.
 
 @Param[in]     timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS              - Succeeded to freezing the timer.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.

*//***************************************************************************/
os_status osHwTimerFreeze(os_timer_handle timer_num);

/**************************************************************************//**
 @Function      osHwTimerUnfreeze

 @Description   Unfreeze the hardware timer run.
 
 @Param[in]     timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS              - Succeeded to unfreezing the timer.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.

*//***************************************************************************/
os_status osHwTimerUnfreeze(os_timer_handle timer_num);
/** @} */ // end of HWTIMER group

_OS_END_EXTERN_C

#endif // __OS_HW_TIMER_H


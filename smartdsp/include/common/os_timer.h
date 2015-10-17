/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/10/31 15:17:53 $
 $Id: os_timer.h,v 1.20 2011/10/31 15:17:53 eitanb Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_timer.h,v $
 $Revision: 1.20 $
******************************************************************************/

/******************************************************************************
 
 @File          os_timer.h

 @Description   OS Timer module header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_TIMER_H
#define __OS_TIMER_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         swt_id Software Timers API

 @Description   Software Timers setup and control.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Collection    OS Software Timer objects values
 
                Software Timer objects can be created dynamically by using
                osTimerFind() or they can be statically referenced by the
                definitions below.
 @{
*//***************************************************************************/
#define OS_TIMER0           0
#define OS_TIMER1           1
#define OS_TIMER2           2
#define OS_TIMER3           3
#define OS_TIMER4           4
#define OS_TIMER5           5
#define OS_TIMER6           6
#define OS_TIMER7           7
#define OS_TIMER8           8
#define OS_TIMER9           9
#define OS_TIMER10          10
/* @} */


/**************************************************************************//**
 @Group         swt_runtime_id Software Timers

 @Description   Software Timers setup and control.
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osTimerCreate

 @Description   Creates a software timer and sets its parameters.
 
                The timer is not started until you call osStart()
 
 @Param[in]     timer_num   - The software timer number.
 @Param[in]     mode        - Timer mode (one-shot, periodic, free-run).
 @Param[in]     interval    - The timer interval (in system ticks).
 @Param[in]     handler     - The function to call when the timer expires.

 @Retval        OS_SUCCESS                  - The timer was successfully created.
 @Retval        OS_ERR_TM_INVALID           - Invalid timer number.
 @Retval        OS_ERR_TM_MODE_INVALID      - Invalid timer mode.
 @Retval        OS_ERR_TM_FUNCTION_INVALID  - NULL timer function.
 @Retval        OS_ERR_TM_ALREADY_CREATED   - The timer is already created.
*//***************************************************************************/
os_status       osTimerCreate(os_timer_handle   timer_num,
                              os_timer_mode     mode,
                              os_timer_interval interval,
                              os_timer_function handler);


/**************************************************************************//**
 @Function      osTimerDelete

 @Description   Deletes the given software timer from the system.
 
 @Param[in]     timer_num -  The software timer number.

 @Retval        OS_SUCCESS              - The timer was successfully deleted.
 @Retval        OS_ERR_TM_INVALID       - Invalid timer number.
 @Retval        OS_ERR_TM_NOT_CREATED   - The timer was not created, or it was already deleted.
*//***************************************************************************/
os_status       osTimerDelete(os_timer_handle timer_num);


/**************************************************************************//**
 @Function      osTimerStart

 @Description   Starts the given software timer.
 
                This function should be used to start a software timer
 
                This function can be used to start/restart a one-shot
                software timer.
 
 @Param[in]     timer_num -  The software timer number.

 @Retval        OS_SUCCESS              - The timer was successfully started.
 @Retval        OS_ERR_TM_INVALID       - Invalid timer number.
 @Retval        OS_ERR_TM_NOT_CREATED   - The timer was not created, or it was already deleted.
*//***************************************************************************/
os_status       osTimerStart(os_timer_handle timer_num);

/**************************************************************************//**
 @Function      osTimerStartDelayed

 @Description   Starts the given software timer.
 
                This function should be used to start a software timer
 
                This function can be used to start/restart a one-shot
                software timer.
 
 @Param[in]     timer_num -  The software timer number.
 @Param[in]     delay -  Delay of first period

 @Retval        OS_SUCCESS              - The timer was successfully started.
 @Retval        OS_ERR_TM_INVALID       - Invalid timer number.
 @Retval        OS_ERR_TM_NOT_CREATED   - The timer was not created, or it was already deleted.
*//***************************************************************************/
os_status       osTimerStartDelayed(os_timer_handle timer_num,os_timer_interval delay);


/**************************************************************************//**
 @Function      osTimerStop

 @Description   Stops the given software timer.
 
 @Param[in]     timer_num -  The software timer number.

 @Retval        OS_SUCCESS              - The timer was successfully stopped.
 @Retval        OS_ERR_TM_INVALID       - Invalid timer number.
 @Retval        OS_ERR_TM_NOT_CREATED   - The timer was not created, or it was already deleted.
*//***************************************************************************/
os_status       osTimerStop(os_timer_handle timer_num);


/**************************************************************************//**
 @Function      osTimerSetInterval

 @Description   Sets the interval of the given software timer.
 
 @Param[in]    timer_num    -  The software timer number.
 @Param[in]    interval     -  The new interval for the timer.

 @Retval        OS_SUCCESS              - The timer interval was successfully set.
 @Retval        OS_ERR_TM_INVALID       - Invalid timer number.
 @Retval        OS_ERR_TM_NOT_CREATED   - The timer was not created, or it was already deleted.
*//***************************************************************************/
os_status       osTimerSetInterval(os_timer_handle  timer_num, os_timer_interval interval);


/**************************************************************************//**
 @Function      osTimerGetDelay

 @Description   Gets number of ticks left for the timer to fire.
 
 @Param[in]     timer_num   - The software timer number.
 @Param[out]    delay       - Ticks left for timer to fire.

 @Retval        OS_SUCCESS              - The timer interval was successfully set.
 @Retval        OS_ERR_TM_INVALID       - Invalid timer number.
 @Retval        OS_ERR_TM_NOT_CREATED   - The timer was not created, or it was already deleted.
*//***************************************************************************/
os_status osTimerGetDelay(os_timer_handle timer_num,os_timer_interval* delay);

/**************************************************************************//**
 @Function      osTimerFind

 @Description   Finds the first available software timer number.
 
 @Param[out]    timer_num -  Receives the software timer number.

 @Retval        OS_SUCCESS              - Found an available timer number.
 @Retval        OS_ERR_TM_UNAVAILABLE   - No timer number is available.
*//***************************************************************************/
os_status       osTimerFind(os_timer_handle *timer_num);


/**************************************************************************//**
 @Function      osTimerSelf

 @Description   Gets the number of the currently active software timer.
 
 @Param[out]    timer_num -  Receives the active software timer number.

 @Retval        OS_SUCCESS              - The value in @a timer_num is valid.
 @Retval        OS_ERR_TM_NOT_ACTIVE    - No timer is active; the function was
                                          not called from within an active timer
                                          function.
*//***************************************************************************/
os_status       osTimerSelf(os_timer_handle *timer_num);

/** @} */ // swt_runtime_id
/** @} */ // swt_id

_OS_END_EXTERN_C

#endif // __OS_TIMER_H

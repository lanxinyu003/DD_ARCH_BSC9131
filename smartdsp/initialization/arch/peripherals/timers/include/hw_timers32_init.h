/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2014/02/20 13:05:37 $
 $Id: hw_timers32_init.h,v 1.8 2014/02/20 13:05:37 b42006 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/timers/include/hw_timers32_init.h,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************

 @File         hw_timers_init.h

 @Description   HW timers Initialization header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __HW_TIMERS_INIT_H
#define __HW_TIMERS_INIT_H


#include "os_cpp.h"


_OS_BEGIN_EXTERN_C

#if defined(MSC8157_FAMILY)
#include "msc8157_family_timers32.h"
#else
#include "b4860_family_timers.h"
#endif

#define NO_CORE_ID (0xFF)

/************************************************************************//**
 @Description   structure of 32 bit timers data.
 
*//*************************************************************************/
typedef struct
{
     uint8_t timer32_core_id[NUM_OF_HW_TIMERS_32b];
     /**< owning core for each of the 32-bit timers */
} hw_timers32_data_t;

/*********************************************************//**
 @Description    SoC HW timers32 MUX input selection
 
*//*************************************************************/
typedef enum
{
    TIN_MUX0,
    TIN_MUX1,
    TIN_MUX2,
    TIN_MUX3
}timer32_input_select_t;

/**************************************************************************//**
 @Description   Timer32 counting trigger initialization structure
*//***************************************************************************/
typedef struct
{
    hw_timer_trigger_input_t       trigger_type;          /**< counting trigger type       */
    timer32_input_select_t         trigger_input;         /**< trigger mux input selection */
}timer32_trigger_params_t;

#if defined(B4860_FAMILY)
/**************************************************************************//**
 @Description   parameters for timer32 module initialization structure
*//***************************************************************************/
typedef struct
{
    timer32_trigger_params_t trigger_params[TIMER32_NUM_OF_TRIGGERS_PER_GROUP]; /**< counting trigger initialization parameters             */
    timer_clock_input_t     clock_in;         /**< clock input selection (architecture dependent)         */
    soc_timer32_group_t     group_num;        /**< HW timers module to initialize. architecture dependent */
}timer32_group_init_params_t;
#else
/**************************************************************************//**
 @Description   parameters for timer32 module initialization structure
*//***************************************************************************/
typedef struct
{
    timer32_trigger_params_t trigger_params[TIMER32_NUM_OF_TRIGGERS_PER_MODULE]; /**< counting trigger initialization parameters             */
    timer_clock_input_t      clock_in;          /**< clock input selection (architecture dependent          */
    soc_timer32_module_t     module_num;        /**< HW timers module to initialize. architecture dependent */
}timer32_module_init_params_t;
#endif

/**************************************************************************//**
 @Function      hwTimer32Initialize

 @Description   Initializes the hardware timers32 module.
 
 @Return        OS_SUCCESS
 
*//***************************************************************************/
os_status   hwTimer32Initialize();

#if defined(B4860_FAMILY)
/**************************************************************************//**
 @Function      hwTimer32GroupInitialize

 @Description   Initialize a group of timer modules (4 modules per group).
 
 @Param[in]    params -  The module initialization params
 
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
os_status       hwTimer32GroupInitialize(timer32_group_init_params_t    *params);
#else
/**************************************************************************//**
 @Function      hwTimer32ModuleInitialize

 @Description   Initialize a timer module.
 
 @Param[in]    params -  The module initialization params
 
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
os_status       hwTimer32ModuleInitialize(timer32_module_init_params_t    *params);
#endif

_OS_END_EXTERN_C

#endif // __HW_TIMERS_INIT_H


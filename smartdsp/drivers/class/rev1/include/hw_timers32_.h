/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/11/09 14:45:35 $
 $Id: hw_timers32_.h,v 1.14 2014/11/09 14:45:35 b41932 Exp $
 $Source: /cvsdata/SmartDSP/drivers/timers/rev1/include/hw_timers32_.h,v $
 $Revision: 1.14 $
******************************************************************************/

/**************************************************************************//**
 
 @File          hw_timers32_.h

 @Description   Hardware Timers32 internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __HW_TIMERS32_H_
#define __HW_TIMERS32_H_

#define TMR32_SCTL_TCF        0x00008000
#define TMR32_SCTL_TCFIE      0x00004000
#define TMR32_SCTL_MSTR       0x00000020
#define TMR32_SCTL_EEOF       0x00000010
#define TMR32_CTRL_COINIT     0x00000008
#define FIRST_TIMER_IN_MODULE(module) module*NUM_OF_HW_TIMERS_32b_PER_MODULE

typedef struct
{
    uint8_t num_of_timers;
#if NUM_OF_HW_TIMERS_32b_PER_MODULE < 9
    union
    {
        uint64_t    all;
        uint8_t     timer[NUM_OF_HW_TIMERS_32b_PER_MODULE];
    };
#else
#error//find another solution for defining this union
#endif
}quad_event_spinlock_t;

/******************************************************************************
 HW_TIMER32 STRUCTURE
******************************************************************************/
typedef struct hw_timer32_t_
{
    uint16_t            status;
    uint16_t            timer_config;
    uint32_t            timer;
    uint32_t            reload_value;
    os_hwi_function     timer_function;
} hw_timer32_t_;


typedef union hw_timer32_t
{
    hw_timer32_t_ *list;      // pointer to array of hw_timer32_t_
    hw_timer32_t_ **array;    // pointer to array of hw_timer32_t_*
} hw_timer32_t;

/**************************************************************************//**
 @Function      hwTimer32TriggerInputSelect
 @Description   Select trigger input for a mux of a timer module
 
 @Param[in]    module     -  timer module          (arch specific)
 @Param[in]    mux_select -  mux select            (arch specific)
 @Param[in]    trigger    -  trigger count input   (arch specific)
 
 
 @Retval        OS_SUCCESS
*//***************************************************************************/
os_status   hwTimer32TriggerInputSelect(uint32_t module, timer32_input_select_t mux_select, hw_timer_trigger_input_t trigger);

#if defined(B4860_FAMILY)
/**************************************************************************//**
 @Function      hwTimer32TriggerInputSelect

 @Description   Select trigger input for a mux of a timer module
 
 @Param[in]    group      -  group number          (arch specific)
 @Param[in]    source     -  clock source          (arch specific)
 
 
 @Retval        OS_SUCCESS
*//***************************************************************************/
os_status   hwTimer32InputClkSelect(uint32_t group, timer_clock_input_t source);
#else
/**************************************************************************//**
 @Function      hwTimer32TriggerInputSelect

 @Description   Select trigger input for a mux of a timer module
 
 @Param[in]    module     -  timer module          (arch specific)
 @Param[in]    source     -  clock source          (arch specific)
 
 
 @Retval        OS_SUCCESS
*//***************************************************************************/
os_status   hwTimer32InputClkSelect(uint32_t module, timer_clock_input_t source);
#endif
/**************************************************************************//**
 @Function      hwTimer32InterruptGet

 @Description   Get IRQ number for timer num
 
 @Param[in]    module     -  timer module          (arch specific)
 @Param[in]    source     -  clock source          (arch specific)
 
 
 @Retval        OS_SUCCESS
*//***************************************************************************/
os_status hwTimer32InterruptGet(uint32_t timer_num, uint32_t *timer_interrupt);

/**************************************************************************//**
 @Function      hwTimer32InterruptCreate

 @Description   Create HW interrupt for SoC timer32.

                This function allocates the interrupt line for a given timer
                and creates an interrupt with the given interrupt priority and
                and the given interrupt handler.
 
 @Param[in]     timer_num -  The SoC timer number (source).
 @Param[in]     priority -  The interrupt priority.
 @Param[in]     mode     -  The interrupt mode.
 @Param[in]     handler  -  The interrupt function (ISR).
 @Param[in]     argument -  Argument to be passed to the interrupt function.

 @Retval        OS_SUCCESS                  - The interrupt handler was
                                              successfully installed.
 @Retval        OS_ERR_HWI_INVALID          - Invalid interrupt number (source).
 @Retval        OS_ERR_HWI_MODE_INVALID     - Invalid interrupt mode.
 @Retval        OS_ERR_HWI_FUNCTION_INVALID - NULL interrupt handler.
 @Retval        OS_ERR_HWI_PRIORITY_INVALID - Invalid interrupt priority.

*//****************************************************************************/
os_status hwTimer32InterruptCreate(soc_timer32_num_t timer_num,
                                   os_hwi_priority priority,
                                   os_hwi_mode     mode,
                                   os_hwi_function handler);
#if defined(B4860)

/**************************************************************************//**
 @Function      hwTimer32MpicInterruptAssign

 @Description   Assign timer interrupts to MPIC.
 
 @Param[in]    timer_num - the timer number of the timer which is trigerring the interrupt - Must be in groups 4 - 7
 
 @Retval        OS_SUCCESS                          - The hardware interrupt was
                                                      successfully registered to MPIC.
                OS_ERROR                            - Error in passed timer number
*//***************************************************************************/
os_status hwTimer32MpicInterruptAssign(soc_timer32_num_t timer_num);
#endif
/**************************************************************************//**
 @Function      hwTimer32InterruptDelete

 @Description   Delete HW interrupt for SoC timer32.
 
 @Param[in]     timer_num -  The SoC timer number (source).

 @Retval        OS_SUCCESS                  - The HWI was
                                              successfully deleted.

*//****************************************************************************/
os_status hwTimer32InterruptDelete(soc_timer32_num_t timer_num);

os_status hwTimer32PutInList(void **timer);
void hwTimer32ArrSet(void *timers, os_timer_handle timer_num);
void hwTimer32InterruptRoute(os_hwi_arg timer32_module);

#endif // __HW_TIMERS32_H_

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/11/09 14:45:35 $
 $Id: hw_timers32.h,v 1.12 2014/11/09 14:45:35 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/timers/hw_timers32.h,v $
 $Revision: 1.12 $
******************************************************************************/

/**************************************************************************//**
 
 @File          hw_timers.h

 @Description   Hardware Timers header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __HW_TIMERS32_H
#define __HW_TIMERS32_H

#include "smartdsp_os_device.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/*********************************************************//**
 @Group         timers_id Timers
 
 @Description   Group including all timers supported by the kernel.
 
 @{
*//**********************************************************/

/*********************************************************//**
 @Group         hwt_id Hardware Timers API
 
 @Description   Hardware timers in the SoC.
 
 @{
*//**********************************************************/

/**************************************************************************//**
 @Description   Timer32 modes of operation
*//***************************************************************************/
typedef enum
{
    SOC_TIMER32_ONE_SHOT = 1,      /**< count once */
    SOC_TIMER32_PERIODIC = 2,      /**< restart after a succesfull comparison */
    SOC_TIMER32_FREE_RUN = 3       /**< run endlessly without comparison */
}timer32_operation_mode_t;

/*********************************************************//**
 @Description    SoC HW timers32 modes of counting
 
*//*************************************************************/
typedef enum
{    SOC_TIMER32_DEFAULT_COUNT              = 0,
    /**< Default count (SOC_TIMER32_COUNT_RISING_EDGE). */
    SOC_TIMER32_COUNT_RISING_EDGE           = 1,
    /**< Count rising edges of the primary source. */
    SOC_TIMER32_COUNT_RISING_AND_FALL_EDGE  = 2,
    /**< Count rising and falling edges of the primary source. */
    SOC_TIMER32_COUNT_RISING_PRIMARY        = 3,
    /** Count rising edges of the primary source while the secondary input is high active*/
    SOC_TIMER32_COUNT_QUADRATURE            = 4,
    /** Quadrature count mode, uses primary clock and secondary input */
    SOC_TIMER32_COUNT_PRIMARY_DIR_SECONDARY = 5,
    /**< Count rising edges of the primary clock; secondary input specifies
         direction (1 = minus) */
    /* SOC_TIMER32_COUNT_SECONDARY_TRIGGER     = 6, //  timers behavior does not match HW documentation */
    /**<Edge of the secondary input triggers primary count until a compare occurs */
    SOC_TIMER32_COUNT_CASCADED              = 7
    /**< Cascaded counter mode (up/down). */
}timer32_counting_mode_t;

/**************************************************************************//**
 @Description   Timer32 primary sources
*//***************************************************************************/
typedef enum
{
    SOC_TIMER32_TIN0SOURCE         =  0,    /**< Timer source clock is TIN0 */
    SOC_TIMER32_TIN1SOURCE         =  0x1,  /**< Timer source clock is TIN1 */
    SOC_TIMER32_TIN2SOURCE         =  0x2,  /**< Timer source clock is TIN2 */
    SOC_TIMER32_TIN3SOURCE         =  0x3,  /**< Timer source clock is TIN2 */
    SOC_TIMER32_OFLAG0SOURCE       =  0x4,  /**< Timer source clock is timer 0 output */
    SOC_TIMER32_OFLAG1SOURCE       =  0x5,  /**< Timer source clock is timer 1 output */
    SOC_TIMER32_OFLAG2SOURCE       =  0x6,  /**< Timer source clock is timer 2 output */
    SOC_TIMER32_OFLAG3SOURCE       =  0x7,  /**< Timer source clock is timer 3 output */
    SOC_TIMER32_PRESCALER1SOURCE   =  0x8,  /**< Timer source clock is (input clock)/1   */
    SOC_TIMER32_PRESCALER2SOURCE   =  0x9,  /**< Timer source clock is (input clock)/2   */
    SOC_TIMER32_PRESCALER4SOURCE   =  0xa,  /**< Timer source clock is (input clock)/4   */
    SOC_TIMER32_PRESCALER8SOURCE   =  0xb,  /**< Timer source clock is (input clock)/8   */
    SOC_TIMER32_PRESCALER16SOURCE  =  0xc,  /**< Timer source clock is (input clock)/16  */
    SOC_TIMER32_PRESCALER32SOURCE  =  0xd,  /**< Timer source clock is (input clock)/32  */
    SOC_TIMER32_PRESCALE64SOURCE   =  0xe,  /**< Timer source clock is (input clock)/64  */
    SOC_TIMER32_PRESCALER128SOURCE =  0xf,  /**< Timer source clock is (input clock)/128 */
    SOC_TIMER32_NOT_IN_USE                  /**< NOT IN USE */
}timer32_primary_source_t;

#define NUM_OF_HW_TIMERS_SOURCES    16

/**************************************************************************//**
 @Description   Timer32 secondary sources
*//***************************************************************************/
typedef enum
{
    SOC_TIMER32_SECONDARY_TIN0,  /**< timer input signal is taken from MUX0 */
    SOC_TIMER32_SECONDARY_TIN1,  /**< timer input signal is taken from MUX1 */
    SOC_TIMER32_SECONDARY_TIN2,  /**< timer input signal is taken from MUX2 */
    SOC_TIMER32_SECONDARY_TIN3,   /**< timer input signal is taken from MUX3 */
    SOC_TIMER32_SECONDARY_NOT_IN_USE
}timer32_secondary_source_t;

/**************************************************************************//**
 @Description   Timer32 primary sources
*//***************************************************************************/
typedef enum{
    SOC_TIMER32_COUNT_UP,
    SOC_TIMER32_COUNT_DOWN
}timer32_count_direction_t;

/**************************************************************************//**
 @Description   Hardware Timer in Quandrate Module
*//***************************************************************************/
typedef enum
{
    SOC_TIMER32_IN_MODULE_TIMER_0 = 0x1,    /**< Timer #0 in quadrate */
    SOC_TIMER32_IN_MODULE_TIMER_1 = 0x2,    /**< Timer #1 in quadrate */
    SOC_TIMER32_IN_MODULE_TIMER_2 = 0x4,    /**< Timer #2 in quadrate */
    SOC_TIMER32_IN_MODULE_TIMER_3 = 0x8     /**< Timer #3 in quadrate */
}timer32_number_in_module_t;


/**************************************************************************//**
 @Description   Hardware Timer preload control
*//***************************************************************************/
typedef enum
{
    SOC_TIMER32_PRELOAD_NEVER     = 0x0,    /**< Preloading is disabled */
    SOC_TIMER32_PRELOAD_UPON_CMP1 = 0x1,    /**< Load preload value to compare1 upon successfull compare1 comparison */
    SOC_TIMER32_PRELOAD_UPON_CMP2 = 0x2     /**< Load preload value to compare2 upon successfull compare2 comparison */
} timer32_preload_ctrl_t;


/**************************************************************************//**
 @Description   Timer32 initialization structure
*//***************************************************************************/
typedef struct
{
    os_timer_interval          interval;           /**< counting interval. Any 32bit value is valid */
    os_timer_interval          preload;            /**< value load to timer after successful comparison */
    timer32_operation_mode_t   operation_mode;     /**< general mode of operation: OS_TIMER_ONE_SHOT, OS_TIMER_PERIODIC, OS_TIMER_FREE_RUN */
    timer32_counting_mode_t    counting_mode;      /**< arch specific mode of counting */
    timer32_count_direction_t  direction;          /**< counting direction (up/down) */
    os_hwi_function            handler;
    /**< interrupt handler, when NULL value supplied, interrupts are disabled */
    /**< user is reponsible to clearing interrupt with hwTimer32ClearEvent */
    os_hwi_priority            priority;           /**< interrupt priority. Irrelevant when interrupt handler is NULL */
    timer32_primary_source_t   primary_source;     /**< timer primary source clock */
    timer32_secondary_source_t secondary_source;   /**< secondary counting source input selection */
    uint32_t                   output_enable:1,    /**< if TRUE output is enabled */
                               preload_ctrl1:2,    /**< wheather and how to preload compare1 value */
                               preload_ctrl2:2     /**< wheather and how to preload compare2 value */
#if !defined(MSC8157)
                               ,
#if defined(B4420)
                               reserved:1,
#else
                               mpic_ie:1,          /**< if TRUE timer interrupts will go to MPIC (available for groups 4-7) */
#endif
                               maple_ie:1,         /**< if TRUE timer interrupts will go to MAPLE */
                               maple_id:2          /**< TIMER32 MPALE IDs can be swt to either 0,1 or 2 used only if maple_ie is TRUE */
#endif //!defined(MSC8157)
                               ;
}timer32_open_params_t;


/**************************************************************************//**
 @Function      hwTimer32Open

 @Description   Open an hardware timer in an initialized module.
 
 @Param[in]    timer_num -  The hardware timer number.
 @Param[in]    params -  timer initizalization parameters.
 
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
os_status       hwTimer32Open(soc_timer32_num_t timer_num, timer32_open_params_t    *params);


/**************************************************************************//**
 @Function      osHwTimer32SetInterval

 @Description   set interval for an hardware timer32.
                valid for stopped timers only
 
 @Param[in]    timer_num -  The hardware timer number.
 @Param[in]    interval -  The timer interval.

 @Retval        OS_SUCCESS  - The interval was successfully set.
 @Retval        OS_ERR_HW_TM_INVALID          - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_INTERVAL_UNSUPPORTED   - Unsupported interval.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.

*//***************************************************************************/
os_status   hwTimer32SetInterval(soc_timer32_num_t timer_num, os_timer_interval interval);

/**************************************************************************//**
 @Function      hwTimer32Delete

 @Description   Stops and deletes the given hardware timer32 from the system.
                valid for open timers only
 
 @Param[in]    timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS                  - The hardware timer was
                                              successfully deleted.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.

*//***************************************************************************/
os_status hwTimer32Delete(soc_timer32_num_t timer_num);

/**************************************************************************//**
 @Function      hwTimer32Start

 @Description   Starts the given hardware timer32.
                valid for non-running open timers.
 
 @Param[in]    timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS                  - The hardware timer was
                                              successfully started.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.
 
*//***************************************************************************/
os_status       hwTimer32Start(soc_timer32_num_t timer_num);

/**************************************************************************//**
 @Function      hwTimer32Stop

 @Description   Stops the given hardware timer32.
                valid for running timers.
 
 @Param[in]    timer_num -  The hardware timer number.

 @Retval        OS_SUCCESS                  - The hardware timer was
                                              successfully stopped.
 @Retval        OS_ERR_HW_TM_INVALID        - Invalid hardware timer number.
 @Retval        OS_ERR_HW_TM_NOT_CREATED    - The hardware timer was not created,
                                              or it was already deleted.

*//***************************************************************************/
os_status       hwTimer32Stop(soc_timer32_num_t timer_num);


/**************************************************************************//**
 @Function      hwTimer32ClearEvent

 @Description   Clears the event bit of the given hardware timer.
                Application must clear timer event for normal timer operation.
 
 @Param[in]    timer_num -  The hardware timer number.

*//***************************************************************************/
void            hwTimer32ClearEvent(soc_timer32_num_t timer_num);

/**************************************************************************//**
 @Function      hwTimer32ValueGet

 @Description   Read the counter of the given hardware timer32.
 
 @Param[in]     timer_num -  The hardware timer number.
 @Param[out]    value     -  The hardware timer value

 @Retval        OS_SUCCESS              - Succeeded in reading the value.
 @Retval        OS_ERR_HW_TM_INVALID    - Invalid hardware timer number.

*//***************************************************************************/
os_status       hwTimer32ValueGet(soc_timer32_num_t timer_num, os_timer_interval *value);


/**************************************************************************//**
 @Function      hwTimer32HoldGet

 @Description   Get the hold value of a given hardware timer32.
 
 @Param[in]     timer_num -  The hardware timer number.
 @Param[out]    value     -  The hardware hold value

 @Retval        OS_SUCCESS              - Succeeded in reading the value.
 @Retval        OS_ERR_HW_TM_INVALID    - Invalid hardware timer number.

*//***************************************************************************/
os_status       hwTimer32HoldGet(soc_timer32_num_t timer_num, uint32_t *value);

/**************************************************************************//**
 @Function      hwTimer32CounterSet

 @Description   Set a  value for a counter of a given hardware timer32.
                Any 32bit value is valid.
                Valid for open and running timers
 
 @Param[in]     timer_num -  The hardware timer number.
 @Param[in]     value     -  the load value to set

 @Retval        OS_SUCCESS              - Succeeded in reading the value.
 @Retval        OS_ERR_HW_TM_INVALID    - Invalid hardware timer number.

*//***************************************************************************/
os_status hwTimer32CounterSet(soc_timer32_num_t timer_num, uint32_t value);

/**************************************************************************//**
 @Function      hwTimer32PreloadSet

 @Description   Set preload value to a given hardware timer32.
                Any 32bit value is valid.
                preload value is set according to the counting direction
                defined in the hwTimer32Open call.
 
 @Param[in]     timer_num       -  the hardware timer number.
 @Param[in]     preload_value   -  preload value to set

 @Retval        OS_SUCCESS              - Succeeded in reading the value.
 @Retval        OS_ERR_HW_TM_INVALID    - Invalid hardware timer number.

*//***************************************************************************/
os_status hwTimer32PreloadSet(soc_timer32_num_t timer_num, uint32_t preload_value);

/**************************************************************************//**
 @Function      hwTimer32CompareSet

 @Description   Set compare values to a given hardware timer32.
                Any 32bit value is valid.
                compare value is set according to the counting direction
                defined in the hwTimer32Open call.
 
 @Param[in]     timer_num -  The hardware timer number.
 @Param[in]     compare  -  compare value to set

 @Retval        OS_SUCCESS              - Succeeded in reading the value.
 @Retval        OS_ERR_HW_TM_INVALID    - Invalid hardware timer number.

*//***************************************************************************/
os_status hwTimer32CompareSet(soc_timer32_num_t timer_num, uint32_t compare);

/**************************************************************************//**
 @Function      hwTimer32SetAndForget

 @Description   Lock timer on high input signal.
 
 @Param[in]     module -  module to lock on the selected trigger.
 @Param[in]     timer_input -  The mux input on which timer will be locked.

 @Retval        OS_SUCCESS              - Succeeded in reading the value.
 @Retval        OS_ERR_HW_TM_INVALID    - Invalid hardware timer number.

*//***************************************************************************/
os_status hwTimer32SetAndForget(soc_timer32_module_t module, timer32_input_select_t timer_input);

/**************************************************************************//**
 @Function      hwTimer32LockClear

 @Description   release lock of hardware timer32.
                valid for open and running timers.
 
 @Param[in]     module -  module to unlock.
 @Param[in]     timer_input -  The locked mux input.

 @Retval        OS_SUCCESS              - Succeeded in reading the value.
 @Retval        OS_ERR_HW_TM_INVALID    - Invalid hardware timer number.

*//***************************************************************************/
os_status hwTimer32LockClear(soc_timer32_module_t module, timer32_input_select_t timer_input);

/**************************************************************************//**
 @Function      hwTimer32GlobalConfig

 @Description   Configures the global system timer.
 
                Works on 32 bit SoC timers
 
 @Param[in]    module -  Number of module, HWT_MODULE_0 or HWT_MODULE_1
 @Param[in]    timers -  Bitmask of timers to lock (e.g. SOC_TIMER32_IN_MODULE_TIMER_0 | SOC_TIMER32_IN_MODULE_TIMER_3)
 @Param[in]    hold0 -  Range starting from from 0, of bits to hold
 @Param[in]    hold1 -  Range starting from from hold0, of bits to hold
 @Param[in]    hold2 -  Range starting from from hold1, of bits to hold

 @Retval        OS_SUCCESS
 @Retval        OS_ERR_TM_MODE_INVALID - The combination of timers and holds
                                         requires more than 32 bits
 
 @Cautions      If one fo the hold0-hold2 are not in use, must set to 0
 
*//***************************************************************************/
os_status hwTimer32GlobalConfig(soc_timer32_module_t module,
                                timer32_number_in_module_t timers,
                                uint32_t     hold0,
                                uint32_t     hold1,
                                uint32_t     hold2);

/**************************************************************************//**
 @Function      hwTimer32GlobalGet

 @Description   Returns the global timer in the module.
 
                Should be called after hwTimer32GlobalConfig().
                Works on 32 bit SoC timers
 
 @Param[in]     module -  Number of module, SOC_TIMER32_MODULEx (arhcitecture dependent)

 @Return        Global system timer value

*//***************************************************************************/
uint32_t hwTimer32GlobalGet(soc_timer32_module_t module);

/**************************************************************************//**
 @Function      hwTimer32SetInitiatorMode

 @Description   Configures broadcast from an Initiator Timer
                Works on 32 bit SoC timers
 
 @Param[in]    module -  Number of module, HWT_MODULE_0 or HWT_MODULE_1
 @Param[in]    initiator_timer - Selected timer to be the initiator timer (if more than one timer is selcted only the lowest timer will be used)
 @Param[in]    reinit_timers -  Bitmask of other timers to be reinitialized when a compare event occurs on the
                         initiator timer(e.g. SOC_TIMER32_IN_MODULE_TIMER_0 | SOC_TIMER32_IN_MODULE_TIMER_3)
 @Param[in]    output_signal_timers -  Bitmask of other timers to forces their output flag signal when a compare
                                       event occurs on the initiator timer
                                       (e.g. SOC_TIMER32_IN_MODULE_TIMER_0 | SOC_TIMER32_IN_MODULE_TIMER_3)
 
 @Retval        OS_SUCCESS
 @Retval        OS_ERR_TM_MODE_INVALID - The combination of timers and holds
                                         requires more than 32 bits
 
 @Cautions     The selected initiator timer cannot be selected as part of
 
*//***************************************************************************/
os_status hwTimer32SetInitiatorMode(soc_timer32_module_t module,
                                    timer32_number_in_module_t initiator_timer,
                                    uint32_t     reinit_timers,
                                    uint32_t     output_signal_timers);
/** @} */ // end of hwt_id

/** @} */ // end of timers_id

_OS_END_EXTERN_C

#endif // __HW_TIMERS32_H

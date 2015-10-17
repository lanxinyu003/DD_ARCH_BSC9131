/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/07/23 05:17:59 $
 $Id: hw_timers.h,v 1.6 2012/07/23 05:17:59 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/timers/hw_timers.h,v $
 $Revision: 1.6 $
******************************************************************************/

/**************************************************************************//**
 
 @File          hw_timers.h

 @Description   Hardware Timers header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __HW_TIMERS_H
#define __HW_TIMERS_H

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

/*********************************************************//**
 @Collection    Clocking sources for SoC timers
 
                For clocking sources of the format PRESCALERnSOURCE
                the input clock is osSlowSystemClockGet()
 
 @{
*//*************************************************************/
#define TIN0SOURCE              0x0                 /**< Timer source clock is TIN0 */
#define TIN1SOURCE              0x1                 /**< Timer source clock is TIN1 */
#define TIN2SOURCE              0x2                 /**< Timer source clock is TIN2 */
#define TIN3SOURCE              0x3                 /**< Timer source clock is TIN3 */
#define OFLAG0SOURCE            0x4                 /**< Timer source clock is timer 0 */
#define OFLAG1SOURCE            0x5                 /**< Timer source clock is timer 1 */
#define OFLAG2SOURCE            0x6                 /**< Timer source clock is timer 2 */
#define OFLAG3SOURCE            0x7                 /**< Timer source clock is timer 3 */
#define PRESCALER1SOURCE        0x8                 /**< Timer source clock is (input clock)/1 */
#define PRESCALER2SOURCE        0x9                 /**< Timer source clock is (input clock)/2 */
#define PRESCALER4SOURCE        0xA                 /**< Timer source clock is (input clock)/4 */
#define PRESCALER8SOURCE        0xB                 /**< Timer source clock is (input clock)/8 */
#define PRESCALER16SOURCE       0xC                 /**< Timer source clock is (input clock)/16 */
#define PRESCALER32SOURCE       0xD                 /**< Timer source clock is (input clock)/32 */
#define PRESCALER64SOURCE       0xE                 /**< Timer source clock is (input clock)/64 */
#define PRESCALER128SOURCE      0xF                 /**< Timer source clock is (input clock)/128 */
#define DEFAULT_TIMER_SOURCE    PRESCALER1SOURCE    /**< Default source */

#define NUM_OF_HW_TIMERS_SOURCES    16
/**< number of SOC timer inputs */

/* @} */    // end of hwt_clock_src


/*********************************************************//**
 @Collection    Types of Hardware Timers

                When calling osHwTimerFindForce(), use one of
                the values, in addition to the requested timer,
                to specify which counter is being used. For example:
                osHwTimerFindForce(HWT_SOC_16BIT | 5) for timer #5
                of the 16 bit type
 @{
*//*************************************************************/
#define HWT_SOC_16BIT           0x0000  /**< 16 bit timer */
#define HWT_SOC_32BIT           0x8000  /**< 32 bit timer */

#if defined(SC39XX)
#define HWT_SOC_64BIT           0x4000  /**< 64 bit timer */
#endif // (SC39XX)

/* @} */    // end of hwt_types


/**************************************************************************//**
 @Description   Hardware Timer in Quandrate Module
*//***************************************************************************/
enum
{
    HWT_TIMER_0 = 0x1,    /**< Timer #0 in quadrate */
    HWT_TIMER_1 = 0x2,    /**< Timer #1 in quadrate */
    HWT_TIMER_2 = 0x4,    /**< Timer #2 in quadrate */
    HWT_TIMER_3 = 0x8     /**< Timer #3 in quadrate */
};

/**************************************************************************//**
 @Description   Hardware Timer Modules
*//***************************************************************************/
enum
{
    HWT_MODULE_0,   /**< Timer Module #0 */
    HWT_MODULE_1    /**< Timer Module #1 */
};

/*********************************************************//**
 @Collection    Counting modes for SoC timers
 @{
*//*************************************************************/
#define COUNT_RISING_EDGE           0x00010000
/**< Count rising edges of the primary source. */
#define COUNT_RISING_AND_FALL_EDGE  0x00020000
/**< Count rising and falling edges of the primary source. */
#define COUNT_RISING_PRIMARY        0x00030000
/** Count rising edges of the primary source while the
    secondary input is high active*/
#define COUNT_QUADRATURE            0x00040000
/** Quadrature count mode, uses primary clock and secondary input */
#define COUNT_PRIMARY_DIR_SECONDARY 0x00050000
/**< Count rising edges of the primary clock; secondary input specifies
     direction (1 = minus) */
#define COUNT_SECONDARY_TRIGGER     0x00060000
/**<Edge of the secondary input triggers primary count until a compare occurs */
#define COUNT_CASCADED              0x00070000
/**< Cascaded counter mode (up/down). */

/* @} */    // end of hwt_cnt_mode

/**************************************************************************//**
 @Function      osHwTimerGlobalTimerGet

 @Description   Returns the global timer in the module.
 
                Should be called after osHwTimerGlobalConfig().
                Works on 32 bit SoC timers
 
 @Param[in]     module -  Number of module, HWT_MODULE_0 or HWT_MODULE_1

 @Return        Global system timer value

*//***************************************************************************/
uint32_t osHwTimerGlobalGet(unsigned int module);


/**************************************************************************//**
 @Function      osHwTimerGlobalConfig

 @Description   Configures the global system timer.
 
                Works on 32 bit SoC timers
 
 @Param[in]    module -  Number of module, HWT_MODULE_0 or HWT_MODULE_1
 @Param[in]    timers -  Bitmask of timers to lock (e.g. HWT_TIMER_0 | HWT_TIMER_3)
 @Param[in]    hold0 -  Range starting from from 0, of bits to hold
 @Param[in]    hold1 -  Range starting from from hold0, of bits to hold
 @Param[in]    hold2 -  Range starting from from hold1, of bits to hold

 @Retval        OS_SUCCESS
 @Retval        OS_ERR_TM_MODE_INVALID - The combination of timers and holds
                                         requires more than 32 bits
 
 @Cautions      If one fo the hold0-hold2 are not in use, must set to 0
 
*//***************************************************************************/
os_status osHwTimerGlobalConfig(unsigned int module,
                                unsigned int timers,
                                uint32_t     hold0,
                                uint32_t     hold1,
                                uint32_t     hold2);

/**************************************************************************//**
 @Function      osHwTimerLock

 @Description   Locks the specified timers in the specified module.
 
                Works on 32 bit SoC timers
 
 @Param[in]    module -  Number of module, HWT_MODULE_0 or HWT_MODULE_1
 @Param[in]    timers -  Bitmask of timers to lock (e.g. HWT_TIMER_0 | HWT_TIMER_3)

 @Retval        OS_SUCCESS

*//***************************************************************************/
os_status osHwTimerLock(unsigned int module, unsigned int timers);


/**************************************************************************//**
 @Function      osHwTimerUnlock

 @Description   Unlocks the specified timers in the specified module.
 
                Works on 32 bit SoC timers
 
 @Param[in]    module -  Number of module, HWT_MODULE_0 or HWT_MODULE_1
 @Param[in]    timers -  Bitmask of timers to lock (e.g. HWT_TIMER_0 | HWT_TIMER_3)

 @Retval        OS_SUCCESS

*//***************************************************************************/
os_status osHwTimerUnlock(unsigned int module, unsigned int timers);


/** @} */ // end of hwt_id

/** @} */ // end of timers_id

_OS_END_EXTERN_C

#endif // __HW_TIMERS_H

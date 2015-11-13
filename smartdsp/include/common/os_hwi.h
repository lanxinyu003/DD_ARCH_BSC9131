/******************************************************************************
 Copyright �1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/09/30 13:35:17 $
 $Id: os_hwi.h,v 1.31 2014/09/30 13:35:17 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_hwi.h,v $
 $Revision: 1.31 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_hwi.h

 @Description   SmartDSP OS Hardware Interrupts header file.
 
 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_HWI_H
#define __OS_HWI_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         hwi_id Hardware Interrupts API

 @Description   Hardware Interrupts setup and control.
 
 @{
*//***************************************************************************/



/**************************************************************************//**
 @Function      osHwiCreate

 @Description   Installs an interrupt handler in the OS dispatcher.

                This function sets the interrupt priority in the interrupt
                assignment register and enables the interrupt.
 
 @Param[in]     hwi_num  -  The interrupt number (source).
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
os_status       osHwiCreate(os_hwi_handle   hwi_num,
                            os_hwi_priority priority,
                            os_hwi_mode     mode,
                            os_hwi_function handler,
                            os_hwi_arg      argument);



/**************************************************************************//**
 @Function      osHwiDispatcherCreate

 @Description   Installs an interrupt handler on a specific OS dispatcher.

                This function sets the interrupt priority in the interrupt
                assignment register and enables the interrupt.
 
 @Param[in]     hwi_num    -  The interrupt number (source).
 @Param[in]     priority   -  The interrupt priority.
 @Param[in]     mode       -  The interrupt mode.
 @Param[in]     handler    -  The interrupt function (ISR).
 @Param[in]     argument   -  Argument to be passed to the interrupt function.
 @Param[in]     dispatcher -  Dispatcher to use.

 @Retval        OS_SUCCESS                  - The interrupt handler was
                                              successfully installed.
 @Retval        OS_ERR_HWI_INVALID          - Invalid interrupt number (source).
 @Retval        OS_ERR_HWI_MODE_INVALID     - Invalid interrupt mode.
 @Retval        OS_ERR_HWI_FUNCTION_INVALID - NULL interrupt handler.
 @Retval        OS_ERR_HWI_PRIORITY_INVALID - Invalid interrupt priority.

*//****************************************************************************/
os_status       osHwiDispatcherCreate(os_hwi_handle     hwi_num,
                                      os_hwi_priority   priority,
                                      os_hwi_mode       mode,
                                      os_hwi_function   handler,
                                      os_hwi_arg        argument,
                                      os_hwi_dispatcher dispatcher);


/**************************************************************************//**
 @Function      osHwiIsCreated

 @Description   Queries whether the interrupt was already registered.

 @Param[in]     hwi_num -   The interrupt number (source).
 @Param[out]    created -  Will be set to TRUE if created or FALSE if not.

 @Retval        OS_SUCCES             - The interrupt handler was installed.
 @Retval        OS_ERR_HWI_INVALID    - Invalid interrupt number (source).
 
 @Cautions      May only be called for interrupts in the Hardware Interrupt Controller

*//****************************************************************************/
os_status       osHwiIsCreated(os_hwi_handle   hwi_num, bool *created);


/**************************************************************************//**
 @Function      osHwiDelete

 @Description   Detaches an interrupt handler from the OS dispatcher.
 
 @Param[in]     hwi_num -  The interrupt number (source).

 @Retval        OS_SUCCESS          - The interrupt handler was successfully
                                      detached.
 @Retval        OS_ERR_HWI_INVALID  - Invalid interrupt number (source).
 
*//***************************************************************************/
os_status       osHwiDelete(os_hwi_handle hwi_num);
#define osHwiRemove   osHwiDelete
/**< see 'osHwiDelete' */

#if 0
/**************************************************************************//**
 @Function      osHwiMultiplexedCreate

 @Description   Installs or adds to a multiplexed interrupt handler in the OS dispatcher.

                This function sets the interrupt priority in the interrupt
                assignment register and enables the interrupt.
 
 @Param[in]     hwi_num  -  The interrupt number (source).
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
 @Retval        OS_ERR_NO_MEMORY - can't create an entry for multiplexed interrupt information

*//****************************************************************************/
os_status       osHwiMultiplexedCreate(os_hwi_handle   hwi_num,
                                       os_hwi_priority priority,
                                       os_hwi_mode     mode,
                                       os_hwi_function handler,
                                       os_hwi_arg      argument);
#endif

/**************************************************************************//**
 @Function      osHwiMultiplexedDispatcherCreate

 @Description   Installs or adds to a multiplexed interrupt handler in the OS dispatcher.

                This function sets the interrupt priority in the interrupt
                assignment register and enables the interrupt.
 
 @Param[in]     hwi_num    -  The interrupt number (source).
 @Param[in]     priority   -  The interrupt priority.
 @Param[in]     mode       -  The interrupt mode.
 @Param[in]     handler    -  The interrupt function (ISR).
 @Param[in]     argument   -  Argument to be passed to the interrupt function.
 @Param[in]     dispatcher -  Dispatcher to use.

 @Retval        OS_SUCCESS                  - The interrupt handler was
                                              successfully installed.
 @Retval        OS_ERR_HWI_INVALID          - Invalid interrupt number (source).
 @Retval        OS_ERR_HWI_MODE_INVALID     - Invalid interrupt mode.
 @Retval        OS_ERR_HWI_FUNCTION_INVALID - NULL interrupt handler.
 @Retval        OS_ERR_HWI_PRIORITY_INVALID - Invalid interrupt priority.

*//****************************************************************************/
os_status       osHwiMultiplexedDispatcherCreate(os_hwi_handle     hwi_num,
                                                 os_hwi_priority   priority,
                                                 os_hwi_mode       mode,
                                                 os_hwi_function   handler,
                                                 os_hwi_arg        argument,
                                                 os_hwi_dispatcher dispatcher);
#if 0
/**************************************************************************//**
 @Function      osHwiMultiplexedDelete

 @Description   Removes an entry from a multiplexed interrupt handler in the OS dispatcher.

 @Param[in]     hwi_num  -  The interrupt number (source).
 @Param[in]     handler  -  The interrupt function (ISR).
 @Param[in]     argument -  Argument to be passed to the interrupt function.

 @Retval        OS_SUCCESS                  - The interrupt handler was
                                              successfully installed.
 @Retval        OS_FAIL                     - Removing during active interrupt.
 @Retval        OS_ERR_HWI_INVALID          - Invalid interrupt number (source).
 @Retval        OS_ERR_HWI_COMMAND_INVALID  - Called for a non multiplexed interrupt.
 @Retval        OS_ERR_ALREADY_FREE         - Already removed this entry.

*//****************************************************************************/
os_status       osHwiMultiplexedDelete(os_hwi_handle     hwi_num,
                                       os_hwi_function   handler,
                                       os_hwi_arg        argument);
#endif

/**************************************************************************//**
 @Function      osHwiPrioritySet

 @Description   Sets the priority of the given hardware interrupt.
 
 @Param[in]     hwi_num  -  The interrupt number (source).
 @Param[in]     priority -  The new interrupt priority.

 @Retval        OS_SUCCESS                  - The interrupt priority was set.
 @Retval        OS_ERR_HWI_INVALID          - Invalid interrupt number (source).
 @Retval        OS_ERR_HWI_PRIORITY_INVALID - Invalid interrupt priority.

*//***************************************************************************/
os_status       osHwiPrioritySet(os_hwi_handle      hwi_num,
                                 os_hwi_priority    priority);

/**************************************************************************//**
 @Function      osHwiPendingClear

 @Description   Clear pending interrupt bit in interrupt controller
 
 @Param[in]     hwi_num -  The interrupt number (source).
 
 @Retval        OS_SUCCESS                  - The interrupt priority was set.
 @Retval        OS_ERR_HWI_INVALID          - Invalid interrupt number (source).

*//***************************************************************************/
os_status osHwiPendingClear(os_hwi_handle     hwi_num);

/**************************************************************************//**
 @Function      osHwiVectorInstall

 @Description   Installs an interrupt vector directly into the interrupt
                vector table.
 
                This function lets the interrupt bypass the OS dispatcher.
                The interrupt must manage the stack, and it cannot call
                OS functions that try to make a task ready.
 
 @Param[in]     hwi_num  -  The interrupt number (source).
 @Param[in]     priority -  The interrupt priority.
 @Param[in]     handler  -  Pointer to the interrupt function.

 @Retval        OS_SUCCESS                  - The interrupt vector was
                                              successfully installed.
 @Retval        OS_ERR_HWI_INVALID          - Invalid interrupt number (source).
 @Retval        OS_ERR_HWI_FUNCTION_INVALID - NULL interrupt handler.
 @Retval        OS_ERR_HWI_PRIORITY_INVALID - Invalid interrupt priority.
 
 @Cautions      This function is not supported on all architectures

*//****************************************************************************/
os_status       osHwiVectorInstall(os_hwi_handle    hwi_num,
                                   os_hwi_priority  priority,
                                   os_hwi_function  handler);


/**************************************************************************//**
 @Function      osHwiDisable

 @Description   Disables interrupts and returns last interrupt status (priority).
 
 @Return        Last hardware interrupt status (priority).
 
*//***************************************************************************/
os_hwi_status   osHwiDisable();


/**************************************************************************//**
 @Function      osHwiEnable

 @Description   Enables interrupts with a given interrupt status (priority).
 
                This function can be used to keep track of nested calls.
 
 @Param[in]     prev_status -  Value returned from osHwiDisable().

*//***************************************************************************/
void            osHwiEnable(os_hwi_status prev_status);


/**************************************************************************//**
 @Function      osHwiSwiftDisable

 @Description   Disables all interrupts immediately.
 
*//***************************************************************************/
#ifndef osHwiSwiftDisable
void            osHwiSwiftDisable();
#endif

/**************************************************************************//**
 @Function      osHwiSwiftEnable

 @Description   Enables interrupts immediately (does not change priorities).
 
*//***************************************************************************/
#ifndef osHwiSwiftEnable
void            osHwiSwiftEnable();
#endif

/**************************************************************************//**
 @Function      osHwiGpioMap

 @Description   Maps external IRQ to GPIO pins.

 @Param[in]     hwi_num  -   The interrupt number (source).
 @Param[out]    gpio_pin -  The GPIO pin (source).

 @Retval        OS_SUCCESS                 - if mapping is legal
 @Retval        OS_ERR_HWI_INVALID         - if hwi_num is not legal
 @Retval        OS_ERR_COMMAND_UNSUPPORTED - on non-supporting architectures
 
*//***************************************************************************/
os_status osHwiGpioMap(os_hwi_handle    hwi_num, unsigned int *gpio_pin);
#define osHwiGetGPIO(hwi_num)   osHwiGpioMap(hwi_num)
/**<see 'osHwiGpioMap' */

/**************************************************************************//**
 @Function      osHwiGpioIrqConfigure

 @Description   Configures a GPIO pin to IRQ functionality.

 @Param[in]     hwi_num  -   The interrupt number (source).

 @Retval        OS_SUCCESS                 - if mapping is legal
 @Retval        OS_ERR_HWI_INVALID         - if hwi_num is not legal
 @Retval        OS_ERR_COMMAND_UNSUPPORTED - on non-supporting architectures
 
*//***************************************************************************/
os_status osHwiGpioIrqConfigure(os_hwi_handle hwi_num);

#if defined(B4860) || defined(B4420) || defined(B3421)
/**************************************************************************//**
 @Function      invoke_interrupt

 @Description   invoke an given interrupt
 
 @Param[in]    num - The number of the Hardware interrupt to invoke.
 
*//***************************************************************************/
void invoke_interrupt(uint32_t num) ;
#endif //defined(B4860) || defined(B4420)  || defined(B3421)

/** @} */ // end of HWI group

_OS_END_EXTERN_C

#endif // __OS_HWI_H

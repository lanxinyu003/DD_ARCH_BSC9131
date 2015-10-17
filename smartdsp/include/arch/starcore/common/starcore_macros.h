/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/07/07 11:55:49 $
 $Id: starcore_macros.h,v 1.32 2013/07/07 11:55:49 b42006 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/common/starcore_macros.h,v $
 $Revision: 1.32 $
******************************************************************************/

/**************************************************************************//**
 
 @File          starcore_macros.h

 @Description   StarCore-specific macros file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __STARCORE_MACROS_H
#define __STARCORE_MACROS_H

#ifdef SIMULATOR
#include <stdio.h>
#endif

#ifdef OS_DOCUMENTATION_ONLY


/**************************************************************************//**
 @Group         hwi_id Hardware Interrupts API

 @Description   Hardware Interrupts setup and control.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      EI

 @Description   Interrupts Enable compiler function.
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void EI();

/**************************************************************************//**
 @Function      DI

 @Description   Interrupts Disable compiler function.
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void DI();

/**************************************************************************//**
 @Function      osHwiSwiftDisableNoLog

 @Description   Disable interrupts, no OS logging performed.
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void osHwiSwiftDisableNoLog();

/**************************************************************************//**
 @Function      osHwiSwiftEnableNoLog

 @Description   Enable interrupts, no OS logging performed.
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void osHwiSwiftEnableNoLog();

/**************************************************************************//**
 @Function      osHwiSwiftNestReduce

 @Description   Reduce interrupt disabling nesting.
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void osHwiSwiftNestReduce();

/** @} */ // end of HWI group

/**************************************************************************//**
 @Group         general_id General System Functions

 @Description   Kernel initialization and general system functions.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      SYS_NOP

 @Description   OS nop function
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void SYS_NOP();

/**************************************************************************//**
 @Function      COPY_ALIGNED_2

 @Description   Copies 4 bytes from y to x using 2 writes - 2 bytes each time.
 
 @Param[in]     x   -  Destination address.
 @Param[in]     y   -  Source address.
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void COPY_ALIGNED_2(uint16_t *x, uint16_t *y);

/**************************************************************************//**
 @Function      osTestDebug

 @Description   Function used for Auto-test process for test passed/failed indication.
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void osTestDebug();

#define STACK_MINIMAL_ALIGNMENT 7       /**< Top of stack minimal alignment */
#define HAVE_OS_ERROR_DEBUG             /**< If defined OS runs in OS error debug mode */

/** @} */ // end of general_id

#else // OS_DOCUMENTATION_ONLY


#define SYS_NOP     NOP()

#ifdef HWI_PRIORITY_LOGGING

#define osHwiSwiftDisable() \
{ extern volatile int g_int_disable_counter; DI(); osLog(0x4007, 0); g_int_disable_counter++; }

/* Reduce interrupt disablement nesting. */
#define osHwiSwiftNestReduce() \
{ extern volatile int g_int_disable_counter; osLog(0x4007, 0); g_int_disable_counter--; }


/* Enable interrupts */
#define osHwiSwiftEnable() \
{ extern volatile int g_int_disable_counter; if (--g_int_disable_counter == 0) { osLog(0x4008, 0); EI(); } }

#define osHwiSwiftDisableNoLog() \
{ extern volatile int g_int_disable_counter; DI(); g_int_disable_counter++; }

/* Enable interrupts */
#define osHwiSwiftEnableNoLog() \
{ extern volatile int g_int_disable_counter; if (--g_int_disable_counter == 0) { EI(); } }


#else // not HWI_LOGGING

#define osHwiSwiftDisableNoLog osHwiSwiftDisable

/* Enable interrupts */
#define osHwiSwiftEnableNoLog osHwiSwiftEnable

/* Disable interrupts */
#define osHwiSwiftDisable() \
{ extern volatile int g_int_disable_counter; DI(); g_int_disable_counter++; }

/* Enable interrupts */
#define osHwiSwiftEnable() \
{ extern volatile int g_int_disable_counter; if (--g_int_disable_counter == 0) EI(); }

/* Reduce interrupt disablement nesting. */
#define osHwiSwiftNestReduce() \
{ extern volatile int g_int_disable_counter; g_int_disable_counter--; }

#endif // HWI_LOGGING

/* Assertion */
#define HAVE_OS_ERROR_DEBUG

INLINE void osErrorDebug()
{
    volatile i = 1;
    DEBUG();
    while(i) //modify i to 0 with to continue
    {
        NOP();
    }
}

#define COPY_ALIGNED_2(X,Y) {*(uint16_t*)&X       = *(uint16_t*)&Y; \
    *((uint16_t*)&X + 1) = *((uint16_t*)&Y + 1);}

#define STACK_MINIMAL_ALIGNMENT 7

/* Auto-test process */
#define osTestDebug \
{ extern volatile bool test_succeeded; test_succeeded = TRUE; DEBUG(); }

#endif // OS_DOCUMENTATION_ONLY

#endif // __STARCORE_MACROS_H



/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_tick_.h,v 1.9 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_tick_.h,v $
 $Revision: 1.9 $
******************************************************************************/

/******************************************************************************
 
 @File          os_tick_.h

 @Description   OS Tick module internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_TICK_H_
#define __OS_TICK_H_



/**************************************************************************//**
 @Function      osTickSetup

 @Description   Creates a software interrupt to handle tick interrupts.

                This function calls the architecture-specific function
                osTickIntProgram(), to set the tick interrupt correctly.
 
 @Param         handler     - the tick interrupt handler
 @Param         priority    - the tick interrupt priority

 @Return        OS status

 @Cautions      This function is private. User should use the osTickStart()
                macro instead.
 
*//***************************************************************************/
os_status   osTickSetup(os_swi_function handler,  os_swi_priority priority);


/**************************************************************************//**
 @Function      osTickSwiHandler

 @Description   Tick task for the OS.
 
 @Param         count - software interrupt count

 @Cautions      Must run inside an interrupt.
 
                This function is private.
 
*//***************************************************************************/
void        osTickSwiHandler(os_swi_count count);


/**************************************************************************//**
 @Function      osTickTimeSwiHandler

 @Description   Tick task for the OS with software timers.
 
 @Param         count - software interrupt count

 @Cautions      Must run inside an interrupt.
 
                This function is private.

*//***************************************************************************/
void        osTickTimeSwiHandler(os_swi_count count);

 
void        osTickDispatcher();
void        osTickHwiHandler(os_hwi_arg arg);

/* Target specific definitions, that are implemented in the
   architecture-specific tick files. */
void        osTickIntProgram();
void        osTickIntStart();
void        osTickIntClear();
uint32_t    osGetTickPerSecond();

#endif // __OS_TICK_H_

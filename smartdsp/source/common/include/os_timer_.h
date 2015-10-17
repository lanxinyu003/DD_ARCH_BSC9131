/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/06/19 10:20:09 $
 $Id: os_timer_.h,v 1.9 2014/06/19 10:20:09 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_timer_.h,v $
 $Revision: 1.9 $
******************************************************************************/

/******************************************************************************
 
 @File          os_timer_.h

 @Description   OS Timer module internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_TIMER_H_
#define __OS_TIMER_H_



/******************************************************************************
 OS TIMERS STRUCTURE
******************************************************************************/
typedef struct os_timer_t_
{
    uint16_t            status;
    os_timer_mode       timer_config;
    os_timer_interval   timer;
    os_timer_interval   reload_value;
    os_timer_function   timer_function;
} os_timer_t_;


typedef union os_timer_t
{
    os_timer_t_ *list;      // pointer to array of os_timer_t_
    os_timer_t_ **array;    // pointer to array of os_timer_t_*
} os_timer_t;


/******************************************************************************
 THESE ARE THE DEFINES FOR THE TIMER FLAGS
 THE FLAGS ARE THE UPPER BYTE OF timer_config
******************************************************************************/
#define RUN_TIMER_FUNCTION      0x4000


bool    osTimerDecrement();
void    osTimerDispatcher();
void    osTimerNullHandler(void);


#endif // __OS_TIMER_H_

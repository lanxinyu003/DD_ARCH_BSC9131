/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/07/07 11:32:23 $
 $Id: os_log_event.c,v 1.17 2013/07/07 11:32:23 b42006 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_log_event.c,v $
 $Revision: 1.17 $
******************************************************************************/

/******************************************************************************
 
 @File          os_log_event.c

 @Description   OS Software Interrrupts runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_.h"
#include "os_event_log_.h"
#include "stdio.h"




#define NUM_OF_ITEMS 100



bool test_succeeded = FALSE;
os_log_event_t os_log_event_buffer[NUM_OF_ITEMS];
unsigned int os_event_counter = 0;
volatile int os_num_of_events = NUM_OF_ITEMS;
extern uint32_t g_tick_parameter;


#define SIZE_OF_ITEM sizeof(os_log_event_t);

void osReportEvent(uint32_t event, const char* file, int line, const char* remarks)
{
    union
    {
        uint64_t up_time;
        uint32_t _up_time[2];
    } _time;
 
    os_log_event_t* ptr;
    osHwiSwiftDisable();
    ptr = &os_log_event_buffer[os_event_counter%os_num_of_events];
    ptr->event = event;
    /* Do if tick is enabled */
    if(g_tick_parameter != 0)
    	osTickTime(&_time.up_time);
    else
    	_time.up_time = 0;
 
    ptr->time1 = _time._up_time[0];
    ptr->time2 = _time._up_time[1];
    ptr->line  = (uint16_t)line;
    ptr->file  = file;
    ptr->remarks  = remarks;
    os_event_counter++;
    osHwiSwiftEnable();
}

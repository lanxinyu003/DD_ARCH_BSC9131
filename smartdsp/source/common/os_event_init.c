/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_event_init.c,v 1.9 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_event_init.c,v $
 $Revision: 1.9 $
******************************************************************************/

/******************************************************************************
 
 @File          os_event_init.c

 @Description   OS Event module initializations.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os.h"
#include "smartdsp_init.h"
#include "os_init.h"
#include "os_list_.h"
#include "os_event_.h"

int g_max_event_queue;
int g_max_event_semaphore;
int g_event_queue_available;
int g_event_semaphore_available;
uint32_t     g_total_events_number;

/*****************************************************************************/
os_status  osEventsInitialize(int max_event_queue, int max_event_semaphore)
{
    g_event_queue_available = g_max_event_queue = max_event_queue;
    g_event_semaphore_available = g_max_event_semaphore = max_event_semaphore;
    g_total_events_number = (uint32_t)(g_max_event_queue + g_max_event_semaphore);
    return OS_SUCCESS;
}



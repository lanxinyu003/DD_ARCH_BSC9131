/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_message_queue_.h,v 1.9 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_message_queue_.h,v $
 $Revision: 1.9 $
******************************************************************************/

/******************************************************************************

 @File          os_message_queue_.h

 @Description   OS Queue internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MESSAGE_QUEUE_H_
#define __OS_MESSAGE_QUEUE_H_

#include "os_queue_.h"

typedef struct
{
    os_queue_t      *queue;        /* This queue is copied from shared */
    uint32_t        message_size;
    os_hwi_handle   hwi_num;
    uint16_t        status;
} os_message_queue_t;

/*
 * These functions should be implemented in one of the architecture depandant files.
 * For example, xxxx_virq_init.c .
 */
os_status messageQueueNotify(os_msg_handle);
os_status messageQueueHwiGet(uint16_t num_of_message_queues);
os_hwi_handle messageQueueHwiCreate(os_msg_handle msg_num, os_hwi_priority priority, os_hwi_arg hwi_arg, os_hwi_function msg_handler);

#endif // __OS_MESSAGE_QUEUE_H_

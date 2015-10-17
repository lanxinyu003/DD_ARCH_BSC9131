/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_message_shared_.h,v 1.2 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_message_shared_.h,v $
 $Revision: 1.2 $
******************************************************************************/

/******************************************************************************

 @File          os_messages_shared_.h

 @Description   Intercore Messages module shared data.

 @Cautions      None.

*//***************************************************************************/

#ifndef __MESSAGES_SHARED_H_
#define __MESSAGES_SHARED_H_


#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_message_.h"

#if (OS_MULTICORE == ON)


// msg_num will be used to transfer the source core id as well
//  as the actual message num
#define MSG_NUM_SRC_CORE(msg_num)           ((msg_num & 0xFF00) >> 8)
#define MSG_NUM_MSG_NUM(msg_num)            (msg_num & 0x00FF)
#define CREATE_MSG_NUM(msg_num, core_num)   ((core_num << 8) | msg_num)


/* messages[source][destination] */
typedef struct
{
    os_core_message_t   messages[OS_SOC_MAX_NUM_OF_CORES][OS_SOC_MAX_NUM_OF_CORES];
} soc_message_t;

extern volatile soc_message_t    *shared_messages;


#endif // OS_MULTICORE == ON

#endif // __MESSAGES_SHARED_H_


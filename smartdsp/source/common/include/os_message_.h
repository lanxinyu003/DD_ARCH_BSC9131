/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_message_.h,v 1.12 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_message_.h,v $
 $Revision: 1.12 $
******************************************************************************/

/******************************************************************************
 
 @File          os_message_.h

 @Description   OS Intercore Messages internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MESSAGE_H_
#define __OS_MESSAGE_H_


#if (OS_MULTICORE == 1)

enum
{
    OS_MESSAGE_SYNC_TYPE,
    OS_MESSAGE_ASYNC_TYPE
};

typedef struct
{
    /* MUST be first */
    os_spinlock_handle    guard;
    uint32_t              data;
    uint16_t              status;
    uint16_t              type;
 
} os_core_message_t;


#endif // OS_MULTICORE

#endif // __OS_MESSAGE_H_


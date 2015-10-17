/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/10/27 14:35:36 $
 $Id: os_queue_.h,v 1.11 2011/10/27 14:35:36 dovl Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_queue_.h,v $
 $Revision: 1.11 $
******************************************************************************/

/******************************************************************************
 
 @File          os_queue_.h

 @Description   OS Queue internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_QUEUE_H_
#define __OS_QUEUE_H_



typedef struct
{
    /* MUST be first */
    os_spinlock_handle  guard;
    uint16_t            status;
    uint16_t            shared;
    uint16_t            used;
    uint16_t            low_indx;
    uint16_t            high_indx;
    uint32_t            size;
    uint32_t            *buff;
 
} os_queue_t;


#endif // __OS_QUEUE_H_

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/08/01 10:45:36 $
 $Id: os_event_log_.h,v 1.1 2011/08/01 10:45:36 dovl Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_event_log_.h,v $
 $Revision: 1.1 $
******************************************************************************/

/******************************************************************************
 
 @File          os_event_log_.h

 @Description   OS event log functions and definitions.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_EVENT_LOG_H_
#define __OS_EVENT_LOG_H_


typedef struct os_log_event_t
{
    uint32_t event;
    uint32_t time1;
    uint32_t time2;
    uint16_t line;
    const char*    file;
    const char*    remarks;
 
}os_log_event_t;


#endif // __OS_EVENT_LOG_H_



/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/02/20 13:05:37 $
 $Id: os_mem_shared_.h,v 1.20 2014/02/20 13:05:37 b42006 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_mem_shared_.h,v $
 $Revision: 1.20 $
******************************************************************************/

/******************************************************************************
 
 @File          os_mem_shared_.h

 @Description   OS Memory Management shared data header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MEM_SHARED_H_
#define __OS_MEM_SHARED_H_

#ifdef _shared_data
#define os_shared
#else
#define os_shared extern
#endif

#if (OS_MULTICORE == 1)
os_shared volatile uint32_t  *g_os_shared_memory;
os_shared volatile uint32_t  g_os_shared_memory_size;

os_shared os_spinlock_handle g_os_mem_guard;

os_shared volatile uint8_t g_heap_shared_initiated
#ifdef _shared_data
= FALSE
#endif
;

#endif  // OS_MULTICORE

#endif // __OS_MEM_SHARED_H_


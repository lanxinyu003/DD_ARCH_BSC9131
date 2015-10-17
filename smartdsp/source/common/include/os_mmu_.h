/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/04/08 13:54:02 $
 $Id: os_mmu_.h,v 1.12 2012/04/08 13:54:02 b20727 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_mmu_.h,v $
 $Revision: 1.12 $
******************************************************************************/

/******************************************************************************
 
 @File          os_mmu_.h

 @Description   OS MMU internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MMU_H_
#define __OS_MMU_H_

#include "os_mmu.h"

#if defined(SC3X00) || defined(SC3X50)
os_status osMmuDataTasksContextSet(uint32_t* task_stack, os_mmu_context_handle mmu_context);
os_status osMmuProgTasksContextSet(uint32_t* task_stack, os_mmu_context_handle mmu_context);
#endif

os_status osMmuProgMattInfo(void const* virt_addr,void ** phys_addr, uint32_t *size, void **phys_base);
os_status osMmuDataMattInfo(void const* virt_addr,void ** phys_addr, uint32_t *size, void **phys_base);

#endif // __OS_MMU_H_

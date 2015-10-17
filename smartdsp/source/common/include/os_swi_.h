/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_swi_.h,v 1.11 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_swi_.h,v $
 $Revision: 1.11 $
******************************************************************************/

/******************************************************************************
 
 @File          os_swi_.h

 @Description   OS Software Interrupts internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_SWI_H_
#define __OS_SWI_H_

#ifdef MMU_SUPPORT
#include "os_mmu.h"
#include "os_mmu_.h"
#endif


typedef struct os_swi_struct_
{
    os_swi_function    handler;
    os_swi_count       count;
    os_swi_status      status;
    os_swi_priority    priority;
    os_user_id         user_id;
#ifdef MMU_SUPPORT
#if defined(SC3X00) || defined(SC3X50)
    os_mmu_context_t*   os_swi_mmu_data_context;
    os_mmu_context_t*   os_swi_mmu_prog_context;
#endif
#endif
} os_swi_struct_;


typedef volatile struct os_swi_struct
{
    os_swi_struct_ *list;
} os_swi_struct;
 

void osSwiTrap(void);


#endif // __OS_SWI_H_

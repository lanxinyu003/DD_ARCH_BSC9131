/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:49 $
 $Id: psc9x3x_shared_.h,v 1.5 2012/11/22 16:28:49 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/include/psc9x3x_shared_.h,v $
 $Revision: 1.5 $
******************************************************************************/

/******************************************************************************
 
 @File          psc9x3x_shared_.h

 @Description   PSC9X3X shared data header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __PSC9X3X_SHARED_H_
#define __PSC9X3X_SHARED_H_


#ifdef _shared_data
#define os_shared
#define SHARED_VIRQ_MASK    0xFFFF /* 16 VIRQ */
#else
#define os_shared extern
#endif


os_shared os_spinlock_handle shared_gic_guard;
os_shared os_spinlock_handle shared_timers_guard;

os_shared volatile uint32_t shared_virq_manager
#ifdef _shared_data
 = SHARED_VIRQ_MASK
#endif
 ;

#endif // __PSC9X3X_SHARED_H_


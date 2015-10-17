/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/08/09 09:19:10 $
 $Id: sc3x00_mmu_.h,v 1.3 2011/08/09 09:19:10 dovl Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/include/sc3x00_mmu_.h,v $
 $Revision: 1.3 $
******************************************************************************/

/******************************************************************************
 
 @File          sc3x00_mmu_.h

 @Cautions      None.
 
*//***************************************************************************/
#ifndef __SC3X00_MMU_H_
#define __SC3X00_MMU_H_

os_status sc3x00MmuInitPlatform(platform_init_params_t* platform_params);

/* DEFAULT MMU INTERRUPT HANDLERS */
void osHwiDMMUError(os_hwi_arg arg);
void osHwiIMMUError(os_hwi_arg arg);



#endif // __SC3X00_MMU_H_


/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2010/03/21 12:20:17 $
 $Id: starcore_swi.c,v 1.6 2010/03/21 12:20:17 b06368 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/common/starcore_swi.c,v $
 $Revision: 1.6 $
******************************************************************************/

/******************************************************************************
 
 @File          starcore_swi.c

 @Description   StarCore-specific Software Interrupts functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_swi.h"
#include "starcore_hwi_.h"

//;*************************************************************************
//;
//;     Function:           osSwiTrap
//:
//:     Inputs:             None
//:
//:     Outputs:            None
//;
//;     Description:        Cause the software interrupt needed to
//;                         service the SWI.
//;
//;     Cautions:           In StarCore, the trap will handle regardless
//;                         of the di bit settings.  So once the trap
//;                         call happens, we are out of here.
//;
//;*************************************************************************

void osSwiTrap(void)
{
   asm("   trap");
}








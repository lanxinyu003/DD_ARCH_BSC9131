/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:49 $
 $Id: psc9x3x_hwi.c,v 1.5 2012/11/22 16:28:49 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_hwi.c,v $
 $Revision: 1.5 $
******************************************************************************/

/******************************************************************************
 
 @File          psc9x3x_hwi.c

 @Description   PSC9X3X Hardware Interrupts runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtimecritical.h"

#include "starcore_hwi_.h"
#include "psc9x3x_hwi.h"


bool osHwiIsPreciseException(os_hwi_handle hwi_num)
{
    return ((hwi_num >= OS_INT_ILLEGAL) && (hwi_num <= OS_INT_DEDC) && (hwi_num != OS_INT_OVERFLOW));
}

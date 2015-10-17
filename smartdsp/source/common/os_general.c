/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2013/06/10 11:34:58 $
 $Id: os_general.c,v 1.2 2013/06/10 11:34:58 b42006 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_general.c,v $
 $Revision: 1.2 $
******************************************************************************/
 
/******************************************************************************
 
 @File          os_general.c

 @Description   OS general module runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"
#include "os_general.h"


/*****************************************************************************/
void osWait(uint32_t num) __attribute__((noinline))
{

#pragma opt_level = "O3"
 
    uint32_t __i;
    for (__i=0; __i<(num) ; __i++) NOP();
}

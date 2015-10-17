/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/07/15 16:46:49 $
 $Id: psc9x3x_het_bsp_stub.h,v 1.6 2012/07/15 16:46:49 b05127 Exp $
 $Source: /cvsdata/SmartDSP/demos/starcore/psc9x3x/rdb_support/psc9x3x_het_bsp_stub.h,v $
 $Revision: 1.6 $
******************************************************************************/

/**************************************************************************//**
 
 @File          psc9x3x_het_bsp_stub.h

 @Description   Simulates initialization of the shared control memory by PA

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __PSC9X3X_HET_BSP_STUB_H
#define __PSC9X3X_HET_BSP_STUB_H

#include "smartdsp_os.h"
#include "smartdsp_os_device.h"
#include "psc913x_heterogeneous.h"

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Function      psc913xHetBspStubInitialize

 @Description   This function mimics what the PA BSP should do in a "real" system.
 
                It writes to shared control space values as defined at ctrl_params
                or sets some default values if ctrl_params == NULL

 @Param         ctrl_base   - Shared control base, use SOC_HET_CTRL_BASE from os_config.h.
 @Param         ctrl_params - Parameters to be written starting from ctrl_base.

*//***************************************************************************/
void psc913xHetBspStubInitialize(void * ctrl_base, os_het_control_t * ctrl_params);

_OS_END_EXTERN_C

#endif // PSC9X3X_HET_BSP_STUB

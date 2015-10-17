/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/07/15 16:46:56 $
 $Id: psc9x3x_tdm_.h,v 1.3 2012/07/15 16:46:56 b05127 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/tdm/include/arch/psc9x3x_tdm_.h,v $
 $Revision: 1.3 $
******************************************************************************/

/******************************************************************************

 @File          msc9x3x_tdm_.h

 @Description   TDM 9x3x specific header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __MSC9X3X_TDM_H_
#define __MSC9X3X_TDM_H_

#include "psc9x3x_hwi.h"
#include "psc9x3x_memmap.h"

#define REGISTERS_TYPE   psc9x3x_ccsr_t
#define DEVICE_REGISTERS g_dsp_ccsr_map

#if defined(PSC9131)
#define NUM_OF_TDMS         1
#define TDM0_DEVICE_NAME    "tdm0"
#elif defined(PSC9132)
#define NUM_OF_TDMS         2
#define TDM0_DEVICE_NAME    "tdm0"
#define TDM1_DEVICE_NAME    "tdm1"
#endif

void tdm_gen_err_int_create_and_enable(void* tdm, uint8_t tdm_num, os_hwi_priority priority);


#endif // __MSC9X3X_TDM_H_


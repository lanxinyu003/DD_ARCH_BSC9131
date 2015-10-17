/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/06/24 06:01:16 $
 $Id: tdm.h,v 1.6 2013/06/24 06:01:16 b17975 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/tdm/tdm.h,v $
 $Revision: 1.6 $
******************************************************************************/

/******************************************************************************
 
 @File          tdm.h

 @Description   TDM header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __TDM_H
#define __TDM_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/***********************************************************//**
 @Group         tdm_id TDM Module API

 @{
*//************************************************************/

/***********************************************************//**
 @Group         tdm_runtime_id TDM Runtime

 @{
*//************************************************************/


#define TDM_NO_CONVERSION   0x0
#define TDM_CONVERT_mLAW    0x1
#define TDM_CONVERT_ALAW    0x2

#define TDMRER_RX_FIRST_THRESH 0x2
#define TDMRER_RX_SECOND_THRESH 0x1

/**************************************************************************//**
 @Description   TDM channel open parameters.
*//***************************************************************************/
typedef struct
{
    uint8_t convert;
} tdm_channel_params_t;

/** @} */ /* end of tdm_runtime_id */
/** @} */ /* end of tdm_id */


_OS_END_EXTERN_C

#endif // __TDM_H


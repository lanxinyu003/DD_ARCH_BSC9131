/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2014/04/08 13:17:30 $
 $Id: os_l1_defense.h,v 1.11 2014/04/08 13:17:30 b41640 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_l1_defense.h,v $
 $Revision: 1.11 $
******************************************************************************/
 

/**************************************************************************//**
 @File          os_l1_defense.h

 @Description   OS L1 defense header file.

 @Cautions      None.
*//***************************************************************************/

#ifndef __OS_L1_DEFENSE_H
#define __OS_L1_DEFENSE_H

#include "smartdsp_os_device.h"

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/* warm reset modes */
#define OS_L1D_NO_WARM_RESET    0x0 /* No l1 warm reset */
#define OS_L1D_MODE_1           0x1 /* L1 scenario mode 1 */
#define OS_L1D_MODE_2           0x2 /* L1 scenario mode 2 */
#define OS_L1D_MODE_3           0x4 /* L1 scenario mode 3 */


#if defined(PSC9131) || defined(PSC9132)
#define osL1dGetResetMode() OS_L1D_NO_WARM_RESET
#else
/************************************************************************//**
 @Function      osL1dGetResetMode
 
 @Description   Get L1 defense warm reset mode (1, 2 or 3)
 
 @Return        Warm Reset mode
 
*//*************************************************************************/
uint32_t osL1dGetResetMode();
#endif

/************************************************************************//**
 @Function      osL1dIsEnabled
 
 @Description   check if l1 defense is enabled
 
 @Return        TRUE/FALSE
 
*//*************************************************************************/
bool osL1dIsEnabled();

/**************************************************************************//**
 @Function      getResetCoresId
 
 @Description   Get mask of L1 defense warm reset cores. The core id's in the
                the core bits are as following:
                core0 - 0x0100000000000000
                core1 - 0x0001000000000000
                core2 - 0x0000010000000000
                core3 - 0x0000000100000000
                core4 - 0x0000000001000000
                core5 - 0x0000000000010000
 
                E.g. if returned value is 0x0100000000010000 then core0 and core5
                are at warm reset initialization
 @Return        Mask of reset cores
*//***************************************************************************/
uint64_t getResetCoresId();

/************************************************************************//**
 @Function      osL1dReportStatus
 
 @Description   Report warm-reset status to L2
 
 @Param[in]     l1d_status  - L1 defense heterogeneous status
 
*//*************************************************************************/
void osL1dReportStatus(uint32_t l1d_status);

/************************************************************************//**
 @Function      osL1dReportStatusEndInit
 
 @Description   Report ended execution of warm reset os initialization

*//*************************************************************************/
void osL1dReportStatusEndInit();

/************************************************************************//**
 @Function      osL1dReportStatusSuccess
 
 @Description   Report warm reset was completed successfully

*//*************************************************************************/
void osL1dReportStatusSuccess();

_OS_END_EXTERN_C

#endif // __OS_L1_DEFENSE_H

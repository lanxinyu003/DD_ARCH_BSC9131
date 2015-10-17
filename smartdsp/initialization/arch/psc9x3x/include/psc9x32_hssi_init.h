/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/09/11 09:57:57 $
 $Id: psc9x32_hssi_init.h,v 1.5 2014/09/11 09:57:57 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/psc9x3x/include/psc9x32_hssi_init.h,v $
 $Revision: 1.5 $
******************************************************************************/

/******************************************************************************
 
 @File          psc9x32_hssi_init.h

 @Description   HSSI Initialization header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __PSC9X32_HSSI_INIT_H
#define __PSC9X32_HSSI_INIT_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Description   serdes loopback modes
*//***************************************************************************/
typedef enum
{
    DISABLE_LB,         /**< No loopback - standard system */
    B2B_DISABLE_LB,     /**< Back to Back board - each device brings itself up */
    DIGITAL_LB,         /**< Digital loopback */
    EXTERNAL_LB         /**< External loopback */
} serdes_connectivity_t;

os_status hssiInitialize(void *init_params);

/**************************************************************************//**
 @Function      hssiCpriGetMaxlLinkrate

 @Description   find maximal available link rate
 
                This function implementation is device specific.
                It retuens the maximal link rate as defined by reset configuration word
 
 @Return         link_rate   - (out) deifned link rate.
 
*//***************************************************************************/
int hssiCpriMaxlLinkrateGet();
void hssiCpriLinkRateSet(int link_rate, uint32_t active_cpri_units);
void hssiCpriDigitalLoopBackSet(uint32_t cpri_num, bool enable);
void hssiCpriEnableLane(uint32_t cpri_num);
void hssiCpriSerdesReset(uint32_t active_cpri_units);
void hssiCpriGcrLinkRateSet(int link_rate, uint32_t active_cpri_units);
void hssiCpriGcrReset(uint32_t active_cpri_units);
os_status hssiCpriResetDoneCheck();

_OS_END_EXTERN_C

#endif __PSC9X32_HSSI_INIT_H

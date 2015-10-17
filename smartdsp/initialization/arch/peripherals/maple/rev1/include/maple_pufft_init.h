/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2013/06/10 11:39:02 $
 $Id: maple_pufft_init.h,v 1.6 2013/06/10 11:39:02 b42006 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/maple/rev1/include/maple_pufft_init.h,v $
 $Revision: 1.6 $
******************************************************************************/
 
/**************************************************************************//**

 @File          rev1/include/maple_pufft_init.h

 @Description   PUFFT initialization header file.

*//***************************************************************************/

#ifndef __MAPLE_PUFFT_INIT_H
#define __MAPLE_PUFFT_INIT_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#include "maple_pe_init.h"

/**************************************************************************//**
@Group         pufft_id  Maple PUFFT API

@Description   MAPLE PUFFT Initialization and Runtime API

@{
*//***************************************************************************/
/**************************************************************************//**
@Group         pufft_init_id  PUFFT Initialization

@Description   PUFFT device initialization API

@{
*//***************************************************************************/

#define PUFFT_DEV_ID   0        /**< PUFFT number in MAPLE */
#define PUFFT_DEV_NAME "PUFFT"  /**< PUFFT device name for registration */

/**************************************************************************//**
 @Description   MAPLE PUFFT initialization parameters type for one PE
*//***************************************************************************/
typedef struct maple_pe_init_params_s   maple_pufft_init_params_s;

/**************************************************************************//**
 @Description   MAPLE PUFFT initialization parameters type for multiple PEs
*//***************************************************************************/
typedef maple_pe_init_params_t          maple_pufft_init_params_t;


/**************************************************************************//**
 @Description   MAPLE PUFFT channel open parameters type
*//***************************************************************************/
typedef maple_pe_ch_open_params_t       maple_pufft_ch_open_params_t;

/**************************************************************************//**
 @Function      maplePufftInitialize

 @Description   Initializes the PUFFT driver's structures

                The driver can supply default MAPLE parameters for
                initialization. The user can override these parameters by
                specifying an alternative MAPLE parameters structure.

                Although this function is called by all cores, only
                the master core performs the initialization of the MAPLE
                registers.

 @Param[in]    init_params - MAPLE Initialization parameters. if NULL,
                             default MAPLE parameters will be used.
 @Param[in]    num_devices - Number of PUFFT devices
 @Param[in]    channel_dispatch - Pointer to channel dispatch function.
 @Param[in]    channel_reap - Pointer to channel reap function.

 @Return       OS_SUCCESS

 @Cautions     This function is generally called by osArchInitialize() as part of the kernel and drivers

*//***************************************************************************/
os_status   maplePufftInitialize(maple_pufft_init_params_s *init_params,
                                unsigned int num_devices,
                                os_status (* channel_dispatch)(void *channel, void *jobs, int *num_jobs),
                                void      (* channel_reap)(void *channel, void *maple));


_OS_END_EXTERN_C

/** @} */ // end of pufft_init_id
/** @} */ // end of pufft_id

#endif

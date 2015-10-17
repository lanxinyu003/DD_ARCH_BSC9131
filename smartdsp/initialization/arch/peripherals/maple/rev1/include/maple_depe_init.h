/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/05/16 11:25:03 $
 $Id: maple_depe_init.h,v 1.3 2012/05/16 11:25:03 b17975 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/maple/rev1/include/maple_depe_init.h,v $
 $Revision: 1.3 $
******************************************************************************/

/**************************************************************************//**
 
 @File          rev1/include/maple_depe_init.h

 @Description   DEPE initialization header file.

*//***************************************************************************/

#ifndef __MAPLE_DEPE_INIT_H
#define __MAPLE_DEPE_INIT_H

#include "maple_pe_init.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C


/**************************************************************************//**
 @Group         depe_id  Maple DEPE API

 @Description   MAPLE DEPE Initialization and Runtime API

 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         depe_init_id  DEPE Initialization

 @Description   DEPE device initialization API

 @{
*//***************************************************************************/

/*************************************************************************//**
 @Collection    MAPLE DEPE Device Names and IDs
 @{
*//**************************************************************************/
#define DEPE_DEV_ID_0       0
#define DEPE_DEV_ID_1       1
#define DEPE_DEV_ID_2       2
#define DEPE_DEV_ID_3       3

#define DEPE_DEV_NAME_0 "DEPE0"
#define DEPE_DEV_NAME_1 "DEPE1"
#define DEPE_DEV_NAME_2 "DEPE2"
#define DEPE_DEV_NAME_3 "DEPE3"
/* @} */

/**************************************************************************//**
 @Description   MAPLE DEPE initialization parameters type for multiple PEs
*//***************************************************************************/
typedef maple_pe_init_params_t          maple_depe_init_params_t;

/**************************************************************************//**
 @Description   MAPLE DEPE initialization parameters type for one PE
*//***************************************************************************/
typedef struct maple_pe_init_params_s   maple_depe_init_params_s;

/**************************************************************************//**
 @Description   MAPLE DEPE channel open parameters type
*//***************************************************************************/
typedef maple_pe_ch_open_params_t       maple_depe_ch_open_params_t;


/**************************************************************************//**
 @Function      mapleDepeInitialize

 @Description   Initializes the DEPE driver's structures
 
 @Param[in]    init_params -  DEPE Initialization parameters
 @Param[in]    num_devices -  Number of DEPE devices
 @Param[in]    channel_dispatch -  Pointer to channel dispatch function
 @Param[in]    channel_reap -  Pointer to channel reap function

 @Return       OS_SUCCESS

 @Cautions     This function is generally called by osArchInitialize() as part of the kernel and drivers
 
*//***************************************************************************/
os_status mapleDepeInitialize(maple_depe_init_params_s *init_params,
                              unsigned int num_devices,
                              os_status (* channel_dispatch)(void *channel, void *jobs, int *num_jobs),
                              void      (* channel_reap)(void *channel, void *maple));


/** @} */ // end of depe_init_id
/** @} */ // end of depe_id

_OS_END_EXTERN_C

#endif /* __MAPLE_DEPE_INIT_H */

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/05/16 11:25:03 $
 $Id: maple_crcpe_init.h,v 1.3 2012/05/16 11:25:03 b17975 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/maple/rev1/include/maple_crcpe_init.h,v $
 $Revision: 1.3 $
******************************************************************************/

/**************************************************************************//**
 
 @File          rev1/include/maple_crcpe_init.h

 @Description   CRCPE initialization header file.

*//***************************************************************************/

#ifndef __MAPLE_CRCPE_INIT_H
#define __MAPLE_CRCPE_INIT_H

#include "maple_pe_init.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         crcpe_id  Maple CRCPE API

 @Description   MAPLE CRCPE Initialization and Runtime API

 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         crcpe_init_id  CRCPE Initialization

 @Description   CRCPE device initialization API

 @{
*//***************************************************************************/

/*************************************************************************//**
 @Collection    MAPLE CRCPE Device Names and IDs
 @{
*//**************************************************************************/
#define CRCPE_DEV_ID_0       0
#define CRCPE_DEV_ID_1       1

#define CRCPE_DEV_NAME_0 "CRCPE0"
#define CRCPE_DEV_NAME_1 "CRCPE1"
/* @} */

/**************************************************************************//**
 @Description   MAPLE CRCPE initialization parameters type for multiple PEs
*//***************************************************************************/
typedef maple_pe_init_params_t          maple_crcpe_init_params_t;

/**************************************************************************//**
 @Description   MAPLE CRCPE initialization parameters type for one PE
*//***************************************************************************/
typedef struct maple_pe_init_params_s   maple_crcpe_init_params_s;

/**************************************************************************//**
 @Description   MAPLE CRCPE channel open parameters type
*//***************************************************************************/
typedef maple_pe_ch_open_params_t       maple_crcpe_ch_open_params_t;


/**************************************************************************//**
 @Function      mapleCrcpeInitialize

 @Description   Initializes the CRCPE driver's structures

                The driver can supply default MAPLE parameters for
                initialization. The user can override these parameters by
                specifying an alternative MAPLE parameters structure.
 
                Although this function is called by all cores, only
                the master core performs the initialization of the MAPLE
                registers.
 
 @Param[in]    init_params -  MAPLE Initialization parameters. if NULL,
                              default MAPLE parameters will be used.
 @Param[in]    num_devices -  Number of CRCPE devices
 @Param[in]    channel_dispatch -  Pointer to channel dispatch function.
 @Param[in]    channel_reap -  Pointer to channel reap function.

 @Return       OS_SUCCESS
 
 @Cautions     This function is generally called by osArchInitialize() as part of the kernel and drivers
 
*//***************************************************************************/
os_status   mapleCrcpeInitialize(maple_crcpe_init_params_s *init_params,
                                 unsigned int num_devices,
                                 os_status (* channel_dispatch)(void *channel, void *jobs, int *num_jobs),
                                 void      (* channel_reap)(void *channel, void *maple));

/** @} */ // end of crcpe_init_id
/** @} */ // end of crcpe_id

_OS_END_EXTERN_C

#endif /* __MAPLE_CRCPE_INIT_H */

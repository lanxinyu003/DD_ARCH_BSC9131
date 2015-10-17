/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:36 $
 $Id: maple_ftpe_init.h,v 1.4 2012/11/22 16:28:36 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/maple/rev1/include/maple_ftpe_init.h,v $
 $Revision: 1.4 $
******************************************************************************/

/**************************************************************************//**

 @File          rev1/include/maple_ftpe_init.h

 @Description   FTPE initialization header file.

*//***************************************************************************/

#ifndef __MAPLE_FTPE_INIT_H
#define __MAPLE_FTPE_INIT_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#include "maple_pe_init.h"

/**************************************************************************//**
 @Group         ftpe_id  Maple FTPE API

 @Description   MAPLE FTPE Initialization and Runtime API

 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         ftpe_init_id   FTPE Initialization

 @Description   FTPE device initialization API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   MAPLE FTPE initialization parameters type for multiple PEs
*//***************************************************************************/
typedef maple_pe_init_params_t          maple_ftpe_init_params_t;

/**************************************************************************//**
 @Description   MAPLE FTPE initialization parameters type for one PE
*//***************************************************************************/
typedef struct maple_pe_init_params_s   maple_ftpe_init_params_s;

/**************************************************************************//**
 @Description   MAPLE FTPE channel open parameters type
*//***************************************************************************/
typedef maple_pe_ch_open_params_t       maple_ftpe_ch_open_params_t;


#ifdef MAPLE_B

/**************************************************************************//**
 @Group         mapleb_ftpe_init_id MAPLE-B FTPE Initialization
 
 @Description   MAPLE-B Spesific FTPE Initialization API
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   MAPLE FFTPE initialization parameters type for multiple PEs
*//***************************************************************************/
#define maple_fftpe_init_params_t maple_ftpe_init_params_t

/**************************************************************************//**
 @Description   MAPLE DFTPE initialization parameters type for multiple PEs
*//***************************************************************************/
#define maple_dftpe_init_params_t maple_ftpe_init_params_t

/*************************************************************************//**
 @Collection    MAPLE-B FFTPE/DFTPE Device Names and IDs
 @{
*//**************************************************************************/
#define FFTPE_DEV_ID_0      0
#define FFTPE_DEV_ID_1      1
#define FFTPE_DEV_ID_2      2
#define DFTPE_DEV_ID_0      0
#define DFTPE_DEV_ID_1      1
#define DFTPE_DEV_ID_2      2
#define FFTPE_DEV_NAME_0    "FFTPE0"
#define FFTPE_DEV_NAME_1    "FFTPE1"
#define FFTPE_DEV_NAME_2    "FFTPE2"
#define DFTPE_DEV_NAME_0    "DFTPE0"
#define DFTPE_DEV_NAME_1    "DFTPE1"
#define DFTPE_DEV_NAME_2    "DFTPE2"
/* @} */

/** @} */ // end of mapleb_ftpe_init_id

#elif defined(MAPLE_B2) || defined(MAPLE_B2F) || defined(MAPLE_B2P)

/**************************************************************************//**
 @Group         mapleb2_ftpe_init_id MAPLE-B2 FTPE Initialization
 
 @Description   MAPLE-B2 Spesific FTPE Initialization API
 @{
*//***************************************************************************/

/*************************************************************************//**
 @Collection    MAPLE-B2 FTPE Device Names and IDs
 
                Device IDs for FTPE0 FTPE1 FTPE2 in MAPLEB2;
                Device Names for FTPE0 FTPE1 FTPE2 in MAPLEB2
 @{
*//**************************************************************************/
#define FTPE_DEV_ID_0       0
#define FTPE_DEV_ID_1       1
#define FTPE_DEV_ID_2       2
#define FTPE_DEV_NAME_0     "FTPE0"
#define FTPE_DEV_NAME_1     "FTPE1"
#define FTPE_DEV_NAME_2     "FTPE2"
/* @} */

/** @} */ // end of mapleb2_ftpe_init_id

#endif /* MAPLE_B2/B2F/B2P */



/**************************************************************************//**
 @Function      mapleFtpeInitialize

 @Description   Initializes the FTPE driver's structures

                The driver can supply default MAPLE parameters for
                initialization. The user can override these parameters by
                specifying an alternative MAPLE parameters structure.

                Although this function is called by all cores, only
                the master core performs the initialization of the MAPLE
                registers.

 @Param[in]    init_params - MAPLE Initialization parameters. if NULL,
                             default MAPLE parameters will be used.
 @Param[in]    num_devices - Number of FTPE devices
 @Param[in]    channel_dispatch - Pointer to channel dispatch function.
 @Param[in]    channel_reap - Pointer to channel reap function.

 @Return       OS_SUCCESS

 @Cautions     This function is generally called by osArchInitialize() as part of the kernel and drivers

*//***************************************************************************/
os_status   mapleFtpeInitialize(maple_ftpe_init_params_s *init_params,
                                unsigned int num_devices,
                                os_status (* channel_dispatch)(void *channel, void *jobs, int *num_jobs),
                                void      (* channel_reap)(void *channel, void *maple));

/** @} */ // end of ftpe_init_id
/** @} */ // end of ftpe_id

_OS_END_EXTERN_C

#endif /* __MAPLE_FTPE_INIT_H */

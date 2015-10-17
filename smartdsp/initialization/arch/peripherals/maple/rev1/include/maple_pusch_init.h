/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2013/06/10 11:39:02 $
 $Id: maple_pusch_init.h,v 1.8 2013/06/10 11:39:02 b42006 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/maple/rev1/include/maple_pusch_init.h,v $
 $Revision: 1.8 $
******************************************************************************/
 
/**************************************************************************//**

 @File          rev1/include/maple_pusch_init.h

 @Description   PUSCH initialization header file.

*//***************************************************************************/

#ifndef __MAPLE_PUSCH_INIT_H
#define __MAPLE_PUSCH_INIT_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#include "maple_pe_init.h"

/**************************************************************************//**
@Group         pusch_id  Maple PUSCH API

@Description   MAPLE PUSCH Initialization and Runtime API

@{
*//***************************************************************************/
/**************************************************************************//**
@Group         pusch_init_id  PUSCH Initialization

@Description   PUSCH device initialization API

@{
*//***************************************************************************/

#define PUSCH_DEV_ID   0        /**< PUSCH number in MAPLE */
#define PUSCH_DEV_NAME "PUSCH"  /**< PUSCH name for device registration */

/**************************************************************************//**
 @Description   MAPLE PUSCH initialization parameters type for one PE
*//***************************************************************************/
typedef struct maple_pe_init_params_s   maple_pusch_init_params_s;

/**************************************************************************//**
 @Description   MAPLE PUSCH initialization parameters type for multiple PEs
*//***************************************************************************/
typedef maple_pe_init_params_t          maple_pusch_init_params_t;

/**************************************************************************//**
 @Description   Interrupt user callback function typedef.
*//***************************************************************************/
typedef void (*add_int_cb)(void * job_handle);

/**************************************************************************//**
 @Description   MAPLE xxPE generic channel open parameters for additional interrupt
*//***************************************************************************/

typedef struct
{
    uint32_t            enable:1; /**< Additional interrupt is enabled */
    os_hwi_priority     int_priority; /**< additional Interrupt priority */
    os_hwi_handle       int_num; /**< Which interrupt line on this device should the interrupt handler
                                      register to. Should be different from the int_num. */
    add_int_cb          int_cb; /**< user callback function pointer */
} maple_pusch_ch_open_additional_interrupt;

/**************************************************************************//**
 @Description   MAPLE PUSCH channel open parameters type
*//***************************************************************************/
typedef struct
{
    maple_pe_ch_open_params_t ;
    maple_pusch_ch_open_additional_interrupt ctrl_interrupt;
}maple_pusch_ch_open_params_t;

/**************************************************************************//**
 @Function     maplePuschInitialize

 @Description  Initializes the PUSCH driver's structures

               The driver can supply default MAPLE parameters for
               initialization. The user can override these parameters by
               specifying an alternative MAPLE parameters structure.

               Although this function is called by all cores, only
               the master core performs the initialization of the MAPLE
               registers.

 @Param[in]    init_params - MAPLE Initialization parameters. if NULL,
                             default MAPLE parameters will be used.
 @Param[in]    num_devices - Number of PUSCH devices
 @Param[in]    channel_dispatch - Pointer to channel dispatch function.
 @Param[in]    channel_reap - Pointer to channel reap function.

 @Return       OS_SUCCESS

 @Cautions     This function is generally called by osArchInitialize() as part of the kernel and drivers

*//***************************************************************************/
os_status   maplePuschInitialize(maple_pusch_init_params_s *init_params,
                                unsigned int num_devices,
                                os_status (* channel_dispatch)(void *channel, void *jobs, int *num_jobs),
                                void      (* channel_reap)(void *channel, void *maple));

#ifdef MAPLE_B2P
/**************************************************************************//**
 @Function     maplePuschCtrlIntRegisterDiffCore

 @Description  Registers the calling core to the PUSCH CTRL interrupt

               This function should be called only in cases which the core that
               handles CTRL interrupts is not the core that dispatches BDs.
 
               This function should be called only by the core that handles CTRL
               interrupts.
 
 @Param[in]    ch           - pointer to the COP channel structure.
 @Param[in]    ch_params    - PUSCH LLD channel open parameters.

 @Return       OS_SUCCESS if registration was successful.

*//***************************************************************************/
os_status maplePuschCtrlIntRegisterDiffCore(cop_channel_t* ch, maple_pusch_ch_open_params_t* ch_params);
#endif

/** @} */ // end of pusch_init_id
/** @} */ // end of pusch_id

_OS_END_EXTERN_C

#endif // __MAPLE_PUSCH_INIT_H

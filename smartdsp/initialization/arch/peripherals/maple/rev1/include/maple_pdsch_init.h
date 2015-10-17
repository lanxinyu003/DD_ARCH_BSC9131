/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/03/12 17:12:44 $
 $Id: maple_pdsch_init.h,v 1.7 2014/03/12 17:12:44 b41640 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/maple/rev1/include/maple_pdsch_init.h,v $
 $Revision: 1.7 $
******************************************************************************/

/**************************************************************************//**
 
 @File          rev1/include/maple_pdsch_init.h

 @Description   PDSCH initialization header file.

*//***************************************************************************/

#ifndef __MAPLE_PDSCH_INIT_H
#define __MAPLE_PDSCH_INIT_H

#include "maple_pe_init.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C


/**************************************************************************//**
 @Group         pdsch_id  Maple PDSCH API

 @Description   MAPLE PDSCH Initialization and Runtime API

 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         pdsch_init_id  PDSCH Initialization

 @Description   PDSCH device initialization API

 @{
*//***************************************************************************/

/*************************************************************************//**
 @Collection    MAPLE PDSCH Device Names and IDs
 @{
*//**************************************************************************/
#define PDSCH_DEV_ID_0       0
#define PDSCH_DEV_ID_1       1

#define PDSCH_DEV_NAME_0 "PDSCH0"
#define PDSCH_DEV_NAME_1 "PDSCH1"
/* @} */

/**************************************************************************//**
 @Description   MAPLE PDSCH initialization parameters type for multiple PEs
*//***************************************************************************/
typedef maple_pe_init_params_t          maple_pdsch_init_params_t;

/**************************************************************************//**
 @Description   MAPLE PDSCH initialization parameters type for one PE
*//***************************************************************************/
typedef struct maple_pe_init_params_s   maple_pdsch_init_params_s;

/*************************************************************************//**
 @Collection    MAPLE PDSCH Mode Flags; Used for dev_init_params.pdsch_mode
 @{
*//**************************************************************************/

#define PDSCH_MODE_PDSCH_EN            0x80000000   /**< Enable PDSCH */
#define PDSCH_MODE_AIC_EN              0x01000000   /**< Output data is read by AIC from MAPLE output buffer */
#define PDSCH_MODE_DEBUG_IFFT_OUT_EN   0x00002000   /**< Debug IFFT output. */
#define PDSCH_MODE_SINGLE_ANT_EN       0x00000200   /**< Receives input of single antenna , and broadcasts the same data to both antennas. */
#define PDSCH_MODE_NOTA2_EN            0x00000100   /**< 2 Transmit Antennas */

/** Target of the Secondary Synchronization Signal */
enum
{
    PDSCH_MODE_SSS_TRG_ANTA = 0x00100000,    /**< Transmit to antenna A only */
    PDSCH_MODE_SSS_TRG_ANTB = 0x00200000,    /**< Transmit to antenna B only */
    PDSCH_MODE_SSS_TRG_BOTH = 0x00300000     /**< Transmit to both antennas  */
};

/** Target of the Primary Synchronization Signal */
enum
{
    PDSCH_MODE_PSS_TRG_ANTA = 0x00010000,    /**< Transmit to antenna A only */
    PDSCH_MODE_PSS_TRG_ANTB = 0x00020000,    /**< Transmit to antenna B only */
    PDSCH_MODE_PSS_TRG_BOTH = 0x00030000     /**< Transmit to both antennas  */
};
/** This field specifies the System Bandwidth so that the PDPE will know the number of Sub-Carriers in the System */
enum
{
    PDSCH_MODE_BW_1_4MHZ = 0x00000000,       /**< 1.4 MHZ (7 RBs and 84 SC) */
    PDSCH_MODE_BW_3MHZ   = 0x00000001,       /**< 3 MHZ (15 RBs and 180 SC) */
    PDSCH_MODE_BW_5MHZ   = 0x00000002,       /**< 5 MHZ (25 RBs and 300 SC) */
    PDSCH_MODE_BW_10MHZ  = 0x00000003,       /**< 10 MHZ (50 RBs and 600 SC) */
    PDSCH_MODE_BW_15MHZ  = 0x00000004,       /**< 15 MHz (75 RBs and 900 SC) */
    PDSCH_MODE_BW_20MHZ  = 0x00000005        /**< 20 MHZ (100 RBs and 1200 SC) */
};

/* @} */

/**************************************************************************//**
 @Function      maplePdschInitialize

 @Description   Initializes the PDSCH driver's structures
 
 @Param[in]    init_params -  PDSCH Initialization parameters
 @Param[in]    num_devices -  Number of PDSCH devices
 @Param[in]    channel_dispatch -  Pointer to channel dispatch function
 @Param[in]    channel_reap -  Pointer to channel reap function

 @Return       OS_SUCCESS

 @Cautions     This function is generally called by osArchInitialize() as part of the kernel and drivers
 
*//***************************************************************************/
os_status maplePdschInitialize(maple_pdsch_init_params_s *init_params,
                              unsigned int num_devices,
                              os_status (* channel_dispatch)(void *channel, void *jobs, int *num_jobs),
                              void      (* channel_reap)(void *channel, void *maple));


/** @} */ // end of pdsch_init_id
/** @} */ // end of pdsch_id

_OS_END_EXTERN_C

#endif /* __MAPLE_PDSCH_INIT_H */

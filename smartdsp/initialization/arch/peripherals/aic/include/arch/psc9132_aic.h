/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:49 $
 $Id: psc9132_aic.h,v 1.8 2012/11/22 16:28:49 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/aic/include/arch/psc9132_aic.h,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************

 @File          psc9132_aic.h

 @Description   AIC PSC9132 specific header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __AIC_PSC9132_H
#define __AIC_PSC9132_H

#include "os_cpp.h"

_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         aic_id   AIC Module

 @Description   AIC block API.

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         aic_psc9132_id   AIC PSC9132 architecture-specific definitions

 @Description   Definition of AIC PSC9132 architecture specific defines, data
                structures and functions.

 @{
*//***************************************************************************/

/*************************************************************//**
 @Collection    Device specific constraints
 @{
*//**************************************************************/
#define AIC_MAX_NUM_OF_CORES            SOC_MAX_NUM_OF_CORES

#define AIC_MAX_NUM_OF_ACTIVE_RFICS     4
/* @} */

/*************************************************************//**
 @Collection    AIC block naming defines
 @{
*//**************************************************************/
#define AIC_DEV_NAME0           "aic0"
/* @} */

/*************************************************************//**
 @Collection    AIC block ADI naming defines
 @{
*//**************************************************************/
#define AIC_ADI_DEV_NAME0       "a_a0"
/* @} */

/*************************************************************//**
 @Collection    AIC lanes naming defines
 @{
*//**************************************************************/
#define AIC_ADI_DEV_NAME1       "adi1"
#define AIC_ADI_DEV_NAME2       "adi2"
#define AIC_ADI_DEV_NAME3       "adi3"
#define AIC_ADI_DEV_NAME4       "adi4"
/* @} */


/**************************************************************************//**
 @Description   AIC ADI lanes.
*//***************************************************************************/
typedef enum
{
    AIC_ADI_LANE1                       = 0x0,
    AIC_ADI_LANE2                       = 0x1,
    AIC_ADI_LANE3                       = 0x2,
    AIC_ADI_LANE4                       = 0x3,
    AIC_ADI_MAX_NUM_OF_LANES
} aic_adi_lane_num_t;


/**************************************************************************//**
 @Description   Cores which can be selected to access and use the AIC.
*//***************************************************************************/
typedef enum
{
    AIC_CORE0                   = 0x00000001,
    AIC_CORE1                   = 0x00000002
} aic_cores_t;

/**************************************************************************//**
 @Description   AIC blocks identification and quantity definitions.
*//***************************************************************************/
typedef enum
{
    AIC_BLOCK0,
    AIC_NUM_OF_BLOCKS
} aic_block_id_t;

/**************************************************************************//**
 @Description   AIC ADI channel identifications.
*//***************************************************************************/
typedef enum
{
    AIC_ADI_CHANNEL0,
    AIC_ADI_CHANNEL1,
    AIC_ADI_MAX_NUM_OF_CHANNELS
} aic_adi_channel_num_t;

/**************************************************************************//**
 @Description   AIC ADI channel identifications.
*//***************************************************************************/
typedef enum
{
    AIC_SNIFF0,
    AIC_SNIFF1,
    AIC_NUM_OF_SNIFFERS
} aic_sniff_id_t;


/** @} */ // end of aic_psc9132_id
/** @} */ // end of aic_id

_OS_END_EXTERN_C

#endif // __AIC_PSC9132_H

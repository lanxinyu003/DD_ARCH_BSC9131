/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:49 $
 $Id: psc9131_aic.h,v 1.13 2012/11/22 16:28:49 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/aic/include/arch/psc9131_aic.h,v $
 $Revision: 1.13 $
******************************************************************************/

/******************************************************************************

 @File          psc9131_aic.h

 @Description   AIC PSC9131 specific header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __AIC_PSC9131_H
#define __AIC_PSC9131_H

#include "os_cpp.h"

_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         aic_id   AIC Module

 @Description   AIC block API.

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         aic_psc9131_id   AIC PSC9131 architecture-specific definitions

 @Description   Definition of AIC PSC9131 architecture specific defines, data
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
 @Collection    AIC block MAXIM naming defines
 @{
*//**************************************************************/
#define AIC_MAXIM_DEV_NAME0     "a_m0"
/* @} */


/*************************************************************//**
 @Collection    AIC lanes naming defines
 @{
*//**************************************************************/
#define AIC_ADI_DEV_NAME1       "adi1"
#define AIC_ADI_DEV_NAME2       "adi2"
#define AIC_ADI_DEV_NAME3       "adi3"
#define AIC_MAX_DEV_NAME1       "max1"
#define AIC_MAX_DEV_NAME2       "max2"
#define AIC_MAX_DEV_NAME3       "max3"
/* @} */


/**************************************************************************//**
 @Description   AIC ADI lanes.
*//***************************************************************************/
typedef enum
{
    AIC_ADI_LANE1                       = 0x0,
    AIC_ADI_LANE2                       = 0x1,
    AIC_ADI_LANE3                       = 0x2,
    AIC_ADI_MAX_NUM_OF_LANES            = 0x3
} aic_adi_lane_num_t;


/**************************************************************************//**
 @Description   AIC MAXIM lanes.
*//***************************************************************************/
typedef enum
{
    AIC_MAXIM_LANE1                     = 0x0,
    AIC_MAXIM_LANE2                     = 0x1,
    AIC_MAXIM_LANE3                     = 0x2,
    AIC_MAXIM_MAX_NUM_OF_LANES          = 0x3
} aic_maxim_lane_num_t;

/**************************************************************************//**
 @Description   Cores which can be selected to access and use the AIC.
*//***************************************************************************/
typedef enum
{
    AIC_CORE0                   = 0x00000001
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
 @Description   AIC MAXIM channel identifications.
*//***************************************************************************/
typedef enum
{
    AIC_MAXIM_CHANNEL0,
    AIC_MAXIM_MAX_NUM_OF_CHANNELS
} aic_maxim_channel_num_t;

/**************************************************************************//**
 @Description   AIC ADI channel identifications.
*//***************************************************************************/
typedef enum
{
    AIC_SNIFF0,
    AIC_SNIFF1,
    AIC_NUM_OF_SNIFFERS
} aic_sniff_id_t;


/** @} */ // end of aic_psc9131_id
/** @} */ // end of aic_id

_OS_END_EXTERN_C

#endif // __AIC_PSC9131_H

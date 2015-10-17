/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/09/14 18:45:50 $
 $Id: psc9x32_family_cpri.h,v 1.15 2014/09/14 18:45:50 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/cpri/include/arch/psc9x32_family_cpri.h,v $
 $Revision: 1.15 $
******************************************************************************/

/******************************************************************************

 @File          psc9x32_family_cpri.h

 @Description   CPRI 9x32 specific header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __CPRI_9X32_FAMILY_H
#define __CPRI_9X32_FAMILY_H


#include "os_cpp.h"


_OS_BEGIN_EXTERN_C

#define CPRI_BLOCK_VER_4_1  //supports protocol 4.1

/**************************************************************************//**
 @Group         cpri_id   CPRI Module API

 @Description   CPRI block API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         cpri_psc9132_id   CPRI PSC9132 architecture-specific definitions

 @Description   Definition of CPRI PSC9132 architecture specific defines, data
                structures and functions.

 @{
*//***************************************************************************/


#define REGISTERS_TYPE   psc9x3x_ccsr_t                                 /**< Chips' CCSR structure */
#define DEVICE_REGISTERS g_dsp_ccsr_map                                 /**< pointer to chips' CCSR base */
#define CPRI_GENERAL_REGISTERS  (DEVICE_REGISTERS->cpri.cpri_general)   /**< pointer to CPRI general registers base */

#define CPRI_MAX_NUM_OF_CORES SOC_MAX_NUM_OF_CORES                           /**< maximum number of cores in this platform */

#ifndef CPRI_FIRST_USED_MODULE
#define CPRI_FIRST_USED_MODULE 0x0 //CPRI_0
#endif


#ifndef NUM_OF_CPRI_MODULES
#define NUM_OF_CPRI_MODULES 2
#endif

#define NUM_OF_MEMMAP_CPRI_UNITS 2

#define CPRI_ICCR_CLOCKS 0x0000003F                                     /**< bitwise constant for all CPRI units' clocks for ICCR register */

/*CPRI units*/
typedef enum
{
    CPRI_0                       = 0x0,
    CPRI_1                       = 0x1,
    CPRI_NOT_USED
} cpri_num_t;

/*CPRI units clock domains*/
#define CPRI_NUMBER_OF_CLOCK_DOMAINS            (1)                     /**< maximum number of clocks domain */
#define CPRI_GET_CLOCK_DOMAIN_INDEX(CPRI_NUM)   0                       /**< returns clock domain for a given CPRI unit */

typedef enum
{
    CPRI_CORE0                   = 0x00000001,
    CPRI_CORE1                   = 0x00000002
} cpri_cores_t;

/**************************************************************************//**
 @Description   Enumeration of CPRI groups
*//***************************************************************************/
typedef enum
{
    CPRI_GROUP_1                        = 0x0,
    NUM_OF_CPRI_GROUPS                  = 0X1
} cpri_group_t;


/*interrupts constraints*/
#define CPRI_NUM_OF_TER_RER_INTERRUPTS_PER_UNIT         4   /**< maximum number of transmit and receive interrupts per CPRI unit */
#define CPRI_NUM_OF_TX_CONTROL_INTERRUPTS_PER_UNIT      1   /**< maximum number of transmit control interrupts interrupts per CPRI unit */
#define CPRI_NUM_OF_RX_CONTROL_INTERRUPTS_PER_UNIT      1   /**< maximum number of receive control interrupts per CPRI unit */
#define CPRI_NUM_OF_TX_TIMING_INTERRUPTS_PER_UNIT       1   /**< maximum number of transmit timing interrupts per CPRI unit */
#define CPRI_NUM_OF_RX_TIMING_INTERRUPTS_PER_UNIT       1   /**< maximum number of receive timing interrupts per CPRI unit */
#define CPRI_NUM_OF_ERROR_INTERRUPTS_PER_UNIT           1   /**< maximum number of error interrupts per CPRI unit */

#define CPRI_TOTAL_NUM_OF_TER_RER_INTERRUPTS            4   /**< maximum number of transmit and receive interrupts for all CPRI units */
#define CPRI_TOTAL_NUM_OF_TX_CONTROL_INTERRUPTS         2   /**< maximum number of transmit control interrupts for all CPRI units */
#define CPRI_TOTAL_NUM_OF_RX_CONTROL_INTERRUPTS         2   /**< maximum number of receive control interrupts for all CPRI units */
#define CPRI_TOTAL_NUM_OF_TX_TIMING_INTERRUPTS          2   /**< maximum number of transmit timing interrupts for all CPRI units */
#define CPRI_TOTAL_NUM_OF_RX_TIMING_INTERRUPTS          2   /**< maximum number of receive timing interrupts for all CPRI units */
#define CPRI_TOTAL_NUM_OF_ERROR_INTERRUPTS              2   /**< maximum number of error interrupts for all CPRI units */

/**************************************************************************//**
 @Description   Enumeration of CPRI available link rates.
*//***************************************************************************/
typedef enum
{
    CPRI_1228_LINK_RATE                             = 1,/*numbers should be consecutive*/
    CPRI_2457_LINK_RATE                             = 2,
    CPRI_3072_LINK_RATE                             = 3,
    CPRI_4914_LINK_RATE                             = 4,
    CPRI_6144_LINK_RATE                             = 5,
    CPRI_INVALID_LINK_RATE
} cpri_link_rate_t;


/**************************************************************************//**
 @Description   Global initialization structure of CPRI.
                This structure is passed once for a device init and
                include parameters for initialization of all the CPRI
                units used on the device.
*//***************************************************************************/
typedef struct
{
    cpri_cores_t        initializing_core;          /**<initializing_core*/
    uint8_t             cpri_num_of_used_units;     /**<number of active cpri units*/
    cpri_link_rate_t    maximal_desired_link_rate;  /**<maximal link rate. must not be greater then RCWLR defined rate*/
    cpri_link_rate_t    minimal_accepted_link_rate; /**<minimal link rate accepted without failure*/
}cpri_global_init_params_t;

/**************************************************************************//**
 @Function      cpriArchInterruptEnable

 @Description   enablement of interrupt events
 
 @Param[in]    int_type       -   type of interrupt to be enabled
 @Param[in]    cpri_num       -   number of cpri module
 
 @Return        none

*//***************************************************************************/
void        cpriArchInterruptEnable (uint32_t int_type, cpri_num_t cpri_num);

/**************************************************************************//**
 @Function      cpriPairNumGet

 @Description   get cpri pair for a spcific cpri unit
 
 @Param[in]    cpri_num       -  cpri CPRI block number.
 
 @Return       cpri_num_t     -  number of pair cpri module

*//***************************************************************************/
cpri_num_t  cpriPairNumGet(cpri_num_t cpri_num);

/**************************************************************************//**
 @Function      cpriInitializeExtendedFeatures

 @Description   initializes cpri extended features
 
 @Param[in]    cpri_num             -   number of cpri module
 @Param[in]    extended_features    -   extended features to be initalized
 
 @Return        os_status    -   OS status

*//***************************************************************************/
os_status   cpriInitializeExtendedFeatures(cpri_num_t cpri_num, void* extended_features);

/**************************************************************************//**
 @Function      cpriArchStatisticsSet

 @Description   activates statistics measurement in device timers
 
 @Return        os_status    -   OS status

*//***************************************************************************/
os_status   cpriArchStatisticsSet();

/**************************************************************************//**
 @Function      cpriExternalSyncSet

 @Description   enable external 10ms signal to pass through GPIO
 
 @Param[in]    cpri_num     -   number of cpri module
 @Param[in]    sync_mode    -   desired synchronization mode
 
 @Return        none

*//***************************************************************************/
void cpriExternalSyncSet(cpri_num_t cpri_num, bool sync_mode);

/**************************************************************************//**
 @Function      cpriSerdesDelaysCalculate

 @Description   calculate CPRI serdes delays
 
 @Param[in]    cpri_num   -   number of cpri module
 
 @Return       double     - value of serdes delay (in seconds)

*//***************************************************************************/
double cpriSerdesDelaysCalculate(cpri_num_t cpri_num);

/** @} */ // end of cpri_psc9132_id
/** @} */ // end of cpri_id

_OS_END_EXTERN_C

#endif // __CPRI_9X32_FAMILY_H


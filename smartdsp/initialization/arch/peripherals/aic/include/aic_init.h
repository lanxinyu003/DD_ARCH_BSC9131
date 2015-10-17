/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:27:33 $
 $Id: aic_init.h,v 1.14 2012/11/22 16:27:33 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/aic/include/aic_init.h,v $
 $Revision: 1.14 $
******************************************************************************/

/******************************************************************************

 @File          aic_init.h

 @Description   AIC Initialization header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __AIC_INIT_H
#define __AIC_INIT_H

#ifdef PSC9131
#include "psc9131_aic.h"
#elif defined(PSC9132)
#include "psc9132_aic.h"
#endif //PSC9132

#include "aic_memmap.h"

#include "os_cpp.h"

_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         aic_id   AIC Module

 @Description   AIC block API.

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         aic_init_id AIC initialization
 
 @Description   AIC initialization API.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Enumeration of AIC IQ MBus transaction sizes.
*//***************************************************************************/
typedef enum
{
    AIC_IQ_TRANSACTION_64_BYTES                    = 0x0,
    AIC_IQ_TRANSACTION_128_BYTES                   = 0x1,
    AIC_IQ_TRANSACTION_256_BYTES                   = 0x2,
    AIC_IQ_NOT_USED
} iq_mbus_transaction_size_t;


/**************************************************************************//**
 @Description   Gloabl initialization structure of AIC ADI.
*//***************************************************************************/
typedef struct
{
    uint8_t dummy;              /**< Prevents from struct being empty which is syntax error. Struct is reserved for future-compatibity*/
} aic_adi_global_init_params_t;

/**************************************************************************//**
 @Description   Global initialization structure of AIC MAXIM.
*//***************************************************************************/
typedef struct
{
    uint8_t dummy;              /**< Prevents from struct being empty which is syntax error. Struct is reserved for future-compatibity*/
} aic_maxim_global_init_params_t;


/**************************************************************************//**
 @Description   Initialization structure of AIC ADI.
*//***************************************************************************/
typedef struct
{
    uint8_t dummy;              /**< Prevents from struct being empty which is syntax error. Struct is reserved for future-compatibity*/
} aic_adi_init_params_t;

/**************************************************************************//**
 @Description   Initialization structure of AIC MAXIM.
*//***************************************************************************/
typedef struct
{
    uint8_t dummy;              /**< Prevents from struct being empty which is syntax error. Struct is reserved for future-compatibity*/
} aic_maxim_init_params_t;

/**************************************************************************//**
 @Description   Initialization structure of AIC.
*//***************************************************************************/
typedef struct
{
    aic_cores_t                     initializing_core;              /**<Bit mask of initializing core */
    aic_map_t                       *aic_memory_map_registers_base; /**<AIC block base address */
 
    aic_block_id_t                  aic_block_id;                   /**< ID number of the AIC */
 
    /* If ADI or MAXIM is not used, the ADI or MAXIM pointers must be NULL, appropriately. */
    aic_adi_init_params_t           *aic_adi_init_params;           /**< ADI initialization parameters */
    aic_maxim_init_params_t         *aic_maxim_init_params;         /**< MAXIM initialization parameters */
} aic_init_params_t;


/**************************************************************************//**
 @Function      aicInitialize

 @Description   AIC initialization file.
 
 @Param[in]     aic_init_params -   AIC initialization parameters.
 
 @Retval        OS_SUCCESS                  - Success.
 @Retval        OS_ERR_NO_MEMORY            - Not enough memory to allocate for AIC structures.
 @Retval        OS_FAIL                     - Registration of SIO device failed.
 
 @Cautions      This function is generally called by osArchInitialize() as part of the kernel and drivers
*//***************************************************************************/
os_status aicInitialize (aic_init_params_t *aic_init_params);


/** @} */ // end of aic_init_id
/** @} */ // end of aic_id

_OS_END_EXTERN_C

#endif // __AIC_INIT_H

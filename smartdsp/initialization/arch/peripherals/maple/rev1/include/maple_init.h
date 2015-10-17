/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:36 $
 $Id: maple_init.h,v 1.4 2012/11/22 16:28:36 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/maple/rev1/include/maple_init.h,v $
 $Revision: 1.4 $
******************************************************************************/

/**************************************************************************//**
 
 @File          rev1/include/maple_init.h

 @Description   MAPLE-B/B2 Controller initialization header file.

*//***************************************************************************/

#ifndef __MAPLE_INIT_H
#define __MAPLE_INIT_H

#include "maple.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         maple_dev_id    Maple API

 @Description   MAPLE device API

 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         maple_dev_init_id   Maple Initialization

 @Description   MAPLE device initialization API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Maple device ID; Used in #maple_init_params_t
*//***************************************************************************/
typedef enum
{
    MAPLE_DEV_ID_NONE = 0xFF,   /**< Used for signaling an unregistered MAPLE resource */
    MAPLE_DEV_ID_0 = 0,         /**< Maple device ID 0 */
    MAPLE_DEV_ID_1 = 1,         /**< Maple device ID 1 */
    MAPLE_DEV_ID_2 = 2,         /**< Maple device ID 2 */
    MAPLE_DEV_ID_3 = 3          /**< Maple device ID 3 */
} maple_dev_id;

/*************************************************************//**
 @Collection    MAPLE Device Names used for COP registration
 @{
*//**************************************************************/
#define MAPLE_DEV_NAME_0 "MPL00"
#define MAPLE_DEV_NAME_1 "MPL01"
#define MAPLE_DEV_NAME_2 "MPL02"
#define MAPLE_DEV_NAME_3 "MPL03"
/* @} */

/**************************************************************************//**
 @Description   Maple Initialization parameters
*//***************************************************************************/
typedef struct
{
    unsigned int num_maples;
    /**< Number of elements in maple_init_params_s array */
    struct maple_init_params_s
    {
        char        *name;
        /**< Name of device */
        uint8_t     dev_id;
        /**< MAPLE device ID - should be of enumeraiton  */
        bool        master;
        /**< Boolean flag - is current device MAPLE-B master. Used in systems
             where a single MAPLE-B is shared among multiple devices */
        bool        on_chip;
        /**< Boolean flag - is MAPLE on this device */
        uint32_t    num_devices;
        /**< Number of devices sharing this MAPLE */
    } device_init[];
} maple_init_params_t;


/**************************************************************************//**
 @Function      mapleInitialize

 @Description   Initializes the MAPLE driver's structures

                The user should give the function an applicaiton dependant
                MAPLE parameters structure.
 
                Although this function is called by all cores, only
                the master core performs the initialization of the MAPLE
                registers.
 
 @Param[in]    init_params -  MAPLE Initialization parameters. if NULL,
                              default MAPLE parameters will be used.
 @Param[in]    num_devices -  Number of MAPLE devices to initialize.

 @Return       OS_SUCCESS
 
 @Cautions     This function is generally called by osArchInitialize() as part of the kernel and drivers
 
*//***************************************************************************/
os_status mapleInitialize(struct maple_init_params_s *init_params, unsigned int num_devices);

/** @} */ // end of maple_dev_init_id
/** @} */ // end of maple_dev_id

_OS_END_EXTERN_C

#endif /* __MAPLE_INIT_H */

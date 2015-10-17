/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/06/19 08:26:59 $
 $Id: psc9x3x_class_ports.h,v 1.7 2013/06/19 08:26:59 b17975 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/class/arch/psc9x3x_class_ports.h,v $
 $Revision: 1.7 $
******************************************************************************/

/**************************************************************************//**
 
 @File          psc9x3x_class_ports.h

 @Description   PSC913x CLASS port enumeration.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __PSC9X93_CLASS_PORTS_H
#define __PSC9X93_CLASS_PORTS_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/***********************************************************//**
 @Group         dbg_9x3x_class_id    PSC9x3x CLASS Profiling API

 @Description   MSC815x specific class debug and profiling Monitor API.

 @{
*//************************************************************/

/**************************************************************************//**
 @Description    All ports on CLASS.

*//***************************************************************************/

/* TODO: This is not up to date */

typedef enum soc_class_ports_t
{
    NO_PORT_SELECTED      = 0,                                 /**< No selected port */

    CLASS_CORE_0_INIT           = (CLASS0 | CLASS_INIT5),            /**< Core 0 Initiator port */
    CLASS_CORE_0_TRGT           = (CLASS0 | CLASS_TRGT4),            /**< Core 0 Target port */
    CLASS_MAPLE_WR_INIT         = (CLASS0 | CLASS_INIT1),            /**< MAPLE Write port */
    CLASS_MAPLE_RD_INIT         = (CLASS0 | CLASS_INIT9),            /**< MAPLE Read port */
    CLASS_MAPLE_TRGT            = (CLASS0 | CLASS_TRGT2),            /**< MAPLE Target port */
    CLASS_AIC_TDM_WR            = (CLASS0 | CLASS_INIT3),            /**< AIC/TDM Write port */
    CLASS_AIC_TDM_RD            = (CLASS0 | CLASS_INIT8),            /**< AIC/TDM Read port */
    CLASS_ECM_INIT              = (CLASS0 | CLASS_INIT4),            /**< ECM Initiator port */
    CLASS_ECM_TRGT              = (CLASS0 | CLASS_TRGT1),            /**< ECM Target port */
    CLASS_SAP                   = (CLASS0 | CLASS_INIT0),            /**< SAP (debug) port */
    CLASS_OCN                   = (CLASS0 | CLASS_INIT10),           /**< OCeaN port */
    CLASS_CCSR                  = (CLASS0 | CLASS_TRGT0),            /**< Skyblue (CCSR) Address space on CLASS1 */
    CLASS_DMA_READ              = (CLASS0 | CLASS_INIT7),            /**< DMA port */
    CLASS_DMA_WRITE             = (CLASS0 | CLASS_INIT2)             /**< DMA port */
} soc_class_ports_t;

/** @} */ // end of dbg_9x3x_class_id

_OS_END_EXTERN_C


#endif // __PSC9X93_CLASS_PORTS_H

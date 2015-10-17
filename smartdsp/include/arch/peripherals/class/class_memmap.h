/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:28:01 $
 $Id: class_memmap.h,v 1.4 2012/11/22 16:28:01 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/class/class_memmap.h,v $
 $Revision: 1.4 $
******************************************************************************/

/**************************************************************************//**
 
 @File          class_memmap.h

 @Description   CLASS Memory map file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef CLASS_MEMMAP_H_
#define CLASS_MEMMAP_H_

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         arch_common_id  Peripheral Memory Maps
 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         class_map_id    CLASS Memory Map
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   CLASS System Bus Target Configuration Registers.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       cmctr;                              /**< CLASS MBus Configuration Registers */
    volatile uint8_t        reserved0[0x20 - 0x04];
} class_cmtcr_t;

/**************************************************************************//**
 @Description   CLASS General Registers.
*//***************************************************************************/
typedef struct
{
    /* General Registers */
    volatile uint32_t       cisr;                               /**< CLASS IRQ Status Register */
    volatile uint8_t        reserved0[0xDC0 - 0xD84];
    volatile uint32_t       cier;                               /**< CLASS IRQ Enable Register */
    volatile uint8_t        reserved1[0xE00 - 0xDC4];
    volatile uint32_t       ctpcr;                              /**< CLASS Target Profiling Configuration Register */
    volatile uint32_t       cpcr;                               /**< CLASS Profiling Control Register */
    volatile uint32_t       cwpcr;                              /**< CLASS Watch Point Control Register */
    volatile uint32_t       cwpacr;                             /**< CLASS Watch Point Access Configuration Register */
    volatile uint32_t       cwpeacr;                            /**< CLASS Watch Point Extended Access Configuration Register */
    volatile uint32_t       cwpamr;                             /**< CLASS Watch Point Address Mask Register */
    volatile uint32_t       cptor;                              /**< CLASS Profiling Time Out Register */
    volatile uint32_t       ctwpcr;                             /**< CLASS Target Watch Point Control Register */
    volatile uint32_t       cpisr;                              /**< CLASS Profiling IRQ Status Register */
    volatile uint32_t       cpier;                              /**< CLASS Profiling IRQ Enable Register */
    volatile uint8_t        reserved2[0xE40 - 0xE28];
    volatile uint32_t       cprcr;                              /**< CLASS Profiling Reference Counter Register */
    volatile uint32_t       cpgcr[4];                           /**< CLASS Profiling General Counter Registers */
    volatile uint8_t        reserved3[0xF80 - 0xE54];
    volatile uint32_t       cgpr;                               /**< CLASS General Purpose Register */
    volatile uint8_t        reserved4[0xFC0 - 0xF84];
    volatile uint32_t       cacr;                               /**< CLASS Arbitration Control Register */
    volatile uint8_t        reserved5[0x1000 - 0xFC4];
} class_general_registers_t;

/**************************************************************************//**
 @Description   CLASS Registers.
*//***************************************************************************/
typedef struct
{
    /* Note: The number of registers of each type is            */
    /* specific to each class. In this structure, the           */
    /* maximum number of registers is defined.                  */
    /* Example: class0 has 5 cmtcr registers, class1            */
    /* has 4 cmtcr registers, class2 has only 1 cmtcr register. */
 
    class_cmtcr_t           cmtcr[MAX_NUMBER_OF_CLASS_REGISTERS];   /**< CLASS System Bus Target Configuration Registers */
    volatile uint8_t        reserved0[0x800 - 0x200];
    volatile uint32_t       cpmr[MAX_NUMBER_OF_CLASS_REGISTERS];    /**< CLASS Priority Mapping Registers */
    volatile uint32_t       cpavr[MAX_NUMBER_OF_CLASS_REGISTERS];   /**< CLASS Priority Auto Upgrade Value Registers */
    volatile uint32_t       cpacr[MAX_NUMBER_OF_CLASS_REGISTERS];   /**< CLASS Priority Auto Upgrade Control Registers */
    volatile uint8_t        reserved1[0x980 - 0x8C0];
    volatile uint32_t       cear[MAX_NUMBER_OF_CLASS_REGISTERS];    /**< CLASS Error Address Register */
    volatile uint32_t       ceear[MAX_NUMBER_OF_CLASS_REGISTERS];   /**< CLASS Error Extended Address Register */
    volatile uint32_t       cipcr[MAX_NUMBER_OF_CLASS_REGISTERS];   /**< CLASS Initiator Profiling Configuration Registers */
    volatile uint32_t       ciwpcr[MAX_NUMBER_OF_CLASS_REGISTERS];  /**< CLASS Initiator Watch Point Control Registers */
    volatile uint32_t       cawr[MAX_NUMBER_OF_CLASS_REGISTERS];    /**< CLASS Arbitration Weight Registers */
    volatile uint8_t        reserved2[0xC00 - 0xAC0];
    volatile uint32_t       csad[MAX_NUMBER_OF_CLASS_REGISTERS];    /**< CLASS Start Address Decoders */
    volatile uint32_t       cead[MAX_NUMBER_OF_CLASS_REGISTERS];    /**< CLASS End Address Decoders */
    volatile uint32_t       catd[MAX_NUMBER_OF_CLASS_REGISTERS];    /**< CLASS Attributes Decoders */
    /* Class Base Address + 0xCC0 */
 
    /* Number-Of-Error Related Registers */
    volatile uint32_t       csaed[MAX_NUMBER_OF_CLASS_ERR_WINDOWS]; /**< CLASS Start Address Error Decoders */
    volatile uint8_t        reserved19[0xD00 - 0xCD0];
    volatile uint32_t       ceaed[MAX_NUMBER_OF_CLASS_ERR_WINDOWS]; /**< CLASS End Address Error Decoders */
    volatile uint8_t        reserved21[0xD40 - 0xD10];
    volatile uint32_t       cated[MAX_NUMBER_OF_CLASS_ERR_WINDOWS]; /**< CLASS Attribute Error Decoders */
    volatile uint8_t        reserved23[0xD80 - 0xD50];
    /* Class Base Address + 0xD80 */
    class_general_registers_t    class_general_registers;   /**< CLASS General Registers */
 
} class_map_t;

/** @} */ // end of class_map_id
/** @} */ // end of arch_common_id

_OS_END_EXTERN_C

#endif /* CLASS_MEMMAP_H_ */

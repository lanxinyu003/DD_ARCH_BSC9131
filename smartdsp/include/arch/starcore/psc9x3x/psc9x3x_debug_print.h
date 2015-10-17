/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 Copyright © 1995-2003,2004,2005-2010 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not
 evidence any actual or intended publication of such source code.
 
*******************************************************************************/
/******************************************************************************
 $Date: 2013/06/23 16:33:19 $
 $Id: psc9x3x_debug_print.h,v 1.9 2013/06/23 16:33:19 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/psc9x3x_debug_print.h,v $
 $Revision: 1.9 $
 **************************************************************************//*

 @File          psc9x3x_debug_print.h

 @Cautions      None.

/**************************************************************************//**/

#ifndef __PSC9X3X_DEBUG_PRINT_H
#define __PSC9X3X_DEBUG_PRINT_H

#include "smartdsp_os_device.h"

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         debug_print_runtime_id PSC9x3x Debug Print runtime API

 @Description   Debug Print functions.
 
 @{
 
 */

#define NR_MAC_BYTES        6

#define TIMESTAMP_LENGTH    24
#define TIMESTAMP_VALUE     (1<<TIMESTAMP_LENGTH)

#define DP_SINGLE_MASK      0xFF7FFFFF
#define DP_MASS_MASK        0x00800000

/**************************************************************************//**
 @Description   Debug Print Trigger Level Enumeration
*//***************************************************************************/
typedef enum
{
    DP_TRIGGER_LEVEL_0 = 0x00000000,
    DP_TRIGGER_LEVEL_1 = 0x00010000,
    DP_TRIGGER_LEVEL_2 = 0x00020000,
    DP_TRIGGER_LEVEL_3 = 0x00030000,
    DP_TRIGGER_LEVEL_4 = 0x00040000,
    DP_TRIGGER_LEVEL_5 = 0x00050000,
    DP_TRIGGER_LEVEL_6 = 0x00060000,
    DP_TRIGGER_LEVEL_7 = 0x00070000,
    DP_TRIGGER_LEVEL_8 = 0x00080000,
    DP_TRIGGER_LEVEL_9 = 0x00090000,
    DP_TRIGGER_LEVEL_10 = 0x000A0000,
    DP_TRIGGER_LEVEL_11 = 0x000B0000,
    DP_TRIGGER_LEVEL_12 = 0x000C0000,
    DP_TRIGGER_LEVEL_13 = 0x000D0000,
    DP_TRIGGER_LEVEL_14 = 0x000E0000,
    DP_TRIGGER_LEVEL_15 = 0x000F0000,
    DP_TRIGGER_LEVEL_16 = 0x00100000,
    DP_TRIGGER_LEVEL_17 = 0x00110000,
    DP_TRIGGER_LEVEL_18 = 0x00120000,
    DP_TRIGGER_LEVEL_19 = 0x00130000,
    DP_TRIGGER_LEVEL_20 = 0x00140000,
    DP_TRIGGER_LEVEL_21 = 0x00150000,
    DP_TRIGGER_LEVEL_22 = 0x00160000,
    DP_TRIGGER_LEVEL_23 = 0x00170000,
    DP_TRIGGER_LEVEL_24 = 0x00180000,
    DP_TRIGGER_LEVEL_25 = 0x00190000,
    DP_TRIGGER_LEVEL_26 = 0x001A0000,
    DP_TRIGGER_LEVEL_27 = 0x001B0000,
    DP_TRIGGER_LEVEL_28 = 0x001C0000,
    DP_TRIGGER_LEVEL_29 = 0x001D0000,
    DP_TRIGGER_LEVEL_30 = 0x001E0000,
    DP_TRIGGER_LEVEL_31 = 0x001F0000
}psc9x3x_debug_print_trigger_level_t;

/**************************************************************************//**
 @Description   Debug Print PA Engine State Values.
*//***************************************************************************/
typedef enum
{
    DP_QE_STATE_OFF,
    DP_QE_STATE_STANDBY,
    DP_QE_STATE_ACTIVE
}psc9x3x_debug_print_state_t;

/**************************************************************************//**
 @Function      psc9x3xDebugPrintFast

 @Description   Writes a single DP into the VTB buffer

 @Param[in]     trigger_level -  DP trigger level, left-shifted with 16 bits
 @Param[in]     local_address -  DP address
 @Param[in]     data -  DP value

 @Cautions      If function being called in a HWI or SWI ISR,
                the application MUST evaluate msc815xDebugPrintRunning()
                and validate it returns FALSE prior to calling.

 @Return        OS_SUCCESS
*//***************************************************************************/
os_status psc9x3xDebugPrintFast(uint32_t trigger_level, uint16_t local_address, uint32_t data );

/**************************************************************************//**
 @Function      psc9x3xDebugPrintMass

 @Description   Writes a mass DP into the VTB buffer

 @Param[in]     trigger_level -  DP trigger level
 @Param[in]     local_address -  DP address
 @Param[in]     data -  pointer to the start address of the data to be included in MDP
 @Param[in]     address_increment_offset -  DP address increment offset
 @Param[in]     count -  number of simple DPs contained in the mass DP

 @Retval        OS_SUCCESS - MDP was successfully inserted
 @Retval        OS_ERR_DEBUG_PRINT_NOT_ACTIVE - QE is not in ACTIVE state
 @Retval        OS_ERR_DEBUG_PRINT_LENGTH_INVALID - More than 2048 WORDS were attempted to be written in an MDP
 @Retval        OS_ERR_DEBUG_PRINT_MDP_ACTIVE - Another MDP is currently being written
 @Retval        OS_ERR_DEBUG_PRINT_OVERFLOW - An overflow would occur if the MDP would be written

 @Cautions      Interrupts must be enabled when calling this function
*//***************************************************************************/
os_status psc9x3xDebugPrintMass(uint32_t trigger_level, uint16_t local_address, uint32_t *data, uint16_t address_increment_offset, uint16_t count );

/**************************************************************************//**
 @Function      msc815xDebugPrintState

 @Description   Returns the PA engine state for the current DSP core

 @Param

 @Return        QE state

*//***************************************************************************/
psc9x3x_debug_print_state_t psc9x3xDebugPrintState();

/**************************************************************************//**
 @Function      psc9x3xDebugPrintInActiveState

 @Description   Checks if QE is in ACTIVE state for the current DSP core

 @Param

 @Retval        TRUE - QE is in ACTIVE state
 @Retval        FALSE - QE is not in ACTIVE state

*//***************************************************************************/
#define psc9x3xDebugPrintInActiveState()  ( psc9x3xDebugPrintState() == DP_QE_STATE_ACTIVE )

/**************************************************************************//**
 @Function      psc9x3xDebugPrintRunning

 @Description   Checks if an MDP is in process of being written

 @Param         none

 @Retval        TRUE - an MDP is currently being written
 @Retval        FALSE - an MDP is not currently being written

*//***************************************************************************/
INLINE bool psc9x3xDebugPrintRunning() { extern volatile bool g_mdp_running; return (g_mdp_running == TRUE); }

os_status __psc9x3xDebugPrintFast( uint32_t trg_lvl_local_addr, uint32_t data);
#define psc9x3xDebugPrintFast(trigger_level, local_address, data)  __psc9x3xDebugPrintFast( (trigger_level) | (local_address), data)

os_status __psc9x3xDebugPrintMass(uint32_t trg_lvl_local_addr, uint32_t *data, uint16_t address_increment_offset, uint16_t count);
#define psc9x3xDebugPrintMass(trigger_level, local_address, data, address_increment_offset, count)  __psc9x3xDebugPrintMass( (trigger_level) | (local_address), data, address_increment_offset, count)

_OS_END_EXTERN_C

/** @} */ // end of debug_print_runtime_id

#endif // __PSC9X3X_DEBUG_PRINT_H

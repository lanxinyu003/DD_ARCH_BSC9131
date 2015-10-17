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
 $Id: psc9x3x_debug_print_init.h,v 1.8 2013/06/23 16:33:19 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/psc9x3x/include/psc9x3x_debug_print_init.h,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************

 @File          psc9x3x_debug_print_init.h

 @Cautions      None.

*//***************************************************************************/

#ifndef __PSC9X3X_DEBUG_PRINT_INIT_H
#define __PSC9X3X_DEBUG_PRINT_INIT_H

#include "smartdsp_os_device.h"

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C
/**************************************************************************//**
 @Group         debug_print_init_id PSC9x3x Debug Print init API

 @Description   Debug Print setup.
 
 @{

/************************************************************************************
  @Description    Configuration Flags
************************************************************************************/


/**************************************************************************//**
 @Description   Virtual Trace Buffer Size

*//***************************************************************************/
typedef enum
{
    VTB_SIZE_4K = 12,
    VTB_SIZE_8K,
    VTB_SIZE_16K,
    VTB_SIZE_32K,
    VTB_SIZE_64K,
    VTB_SIZE_128K,
    VTB_SIZE_256K,
    VTB_SIZE_512K,
    VTB_SIZE_1M,
    VTB_SIZE_2M,
    VTB_SIZE_INVALID
} vtb_size_t;

/**************************************************************************//**
 @Description   Virtual Trace Buffer - Number of segments

*//***************************************************************************/
typedef enum{
    VTB_SEGMENTS_2 = 1,
    VTB_SEGMENTS_4,
    VTB_SEGMENTS_8,
    VTB_SEGMENTS_16,
    VTB_SEGMENTS_32,
    VTB_SEGMENTS_INVALID
}vtb_segments_t;

/************************************************************************************
 @Description     Debug Print Defaults
************************************************************************************/
#define VTB_SIZE_DEFAULT                VTB_SIZE_64K
#define VTB_SEGMENTS_DEFAULT            VTB_SEGMENTS_8
#define DEBUG_PRINT_DEF_FLAGS           0x00000000

/**************************************************************************//**
 @Description   Debug print initialization parameters.
*//***************************************************************************/
typedef struct
{
    os_mem_type    vtb_mem_location[SOC_MAX_NUM_OF_CORES];  /**< Memory where VTBs are allocated */
    vtb_size_t     vtb_size[SOC_MAX_NUM_OF_CORES];          /**< Log2 of the VTB size to be used for Debug Prints (VTB_size = 1 << vtb_size) */
    vtb_segments_t num_of_segments[SOC_MAX_NUM_OF_CORES];   /**< Log2 of the VTB number of segments */
    uint32_t       flags;                                   /**< If any core is disabled */
} psc9x3x_debug_print_init_params_t;


/**************************************************************************//**
 @Function      psc9x3xDebugPrintInitialize

 @Description   Configures QE for basic ETH access

 @Param         init_params - debug print initialization params

 @Return        OS_SUCCESS if valid

 @Cautions      New Mode: None
                Old Mode: After calling this function:
                o interrupts are disabled on the UEC used for channels 0,NUM_OF_CORES
                o UTFTT register in UEC memory map is modified

*//***************************************************************************/
os_status psc9x3xDebugPrintInitialize(psc9x3x_debug_print_init_params_t *init_params);

_OS_END_EXTERN_C
/** @} */ // end of debug_print_init_id

#endif // __PSC9X3X_DEBUG_PRINT_INIT_H

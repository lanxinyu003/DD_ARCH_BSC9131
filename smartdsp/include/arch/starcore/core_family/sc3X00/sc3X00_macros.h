/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/06/06 11:16:09 $
 $Id: sc3X00_macros.h,v 1.22 2013/06/06 11:16:09 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/core_family/sc3X00/sc3X00_macros.h,v $
 $Revision: 1.22 $
******************************************************************************/

/**************************************************************************//**
 
 @File          sc3X00_macros.h

 @Description   platform-specific macros file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __SC3X00_MACROS_H
#define __SC3X00_MACROS_H

/**************************************************************************//**
 @Group         dsp_def_id  DSP Platform Definitions
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         dsp_def_s3x00_id  SC3X00 Definitions
 @{
*//***************************************************************************/

/*************************************************************************//**
 @Collection   SC3X00 HWI Priorities
 @{
*//***************************************************************************/
#define SR_PRIORITIES_NUMBER 32
#define SR_PRIORITY_MASK 0x03e00000

#define OS_HWI_PRIORITY0             1      /**< Highest HWI priority */
#define OS_HWI_PRIORITY1             2
#define OS_HWI_PRIORITY2             3
#define OS_HWI_PRIORITY3             4
#define OS_HWI_PRIORITY4             5
#define OS_HWI_PRIORITY5             6
#define OS_HWI_PRIORITY6             7
#define OS_HWI_PRIORITY7             8
#define OS_HWI_PRIORITY8             9
#define OS_HWI_PRIORITY9             10
#define OS_HWI_PRIORITY10            11
#define OS_HWI_PRIORITY11            12
#define OS_HWI_PRIORITY12            13
#define OS_HWI_PRIORITY13            14
#define OS_HWI_PRIORITY14            15
#define OS_HWI_PRIORITY15            16
#define OS_HWI_PRIORITY16            17
#define OS_HWI_PRIORITY17            18
#define OS_HWI_PRIORITY18            19
#define OS_HWI_PRIORITY19            20
#define OS_HWI_PRIORITY20            21
#define OS_HWI_PRIORITY21            22
#define OS_HWI_PRIORITY22            23
#define OS_HWI_PRIORITY23            24
#define OS_HWI_PRIORITY24            25
#define OS_HWI_PRIORITY25            26
#define OS_HWI_PRIORITY26            27
#define OS_HWI_PRIORITY27            28
#define OS_HWI_PRIORITY28            29
#define OS_HWI_PRIORITY29            30
#define OS_HWI_PRIORITY30            31

#define OS_HWI_LAST_PRIORITY         OS_HWI_PRIORITY30      /**< Lowest HWI priority */


#define OS_HWI_PRIORITY_ALL SR_PRIORITIES_NUMBER
#define OS_HWI_PRIORITY_NMI (SR_PRIORITIES_NUMBER + 1)      /**< NMI interrupt priority */

/* @} */

/** syncio command  */
#define SYNCIO  asm("   syncio");

typedef uint8_t                     os_mmu_id;


#define ARCH_CACHE_LINE_SIZE  256   /**< Cache line size */
#define ARCH_CONTEXT_SIZE     6     /**< Architecture dependant context size */
#define TASK_PC_REG_PLACE     0     /**< Task PC register place */
#define TASK_STATUS_REG_PLACE 1     /**< Task status register place */
#define TASK_ARG_REG_PLACE    4     /**< Task argument register place */

/** Registers offsets in the stack; the names reflect the real register names.  */
typedef enum
{
    REG_OFF_R0         ,
    REG_OFF_R1         ,
    REG_OFF_R2         ,
    REG_OFF_R3         ,
    REG_OFF_R4         ,
    REG_OFF_R5         ,
    REG_OFF_R6         ,
    REG_OFF_R7         ,
    REG_OFF_B0         ,
    REG_OFF_B1         ,
    REG_OFF_B2         ,
    REG_OFF_B3         ,
    REG_OFF_B4         ,
    REG_OFF_B5         ,
    REG_OFF_B6         ,
    REG_OFF_B7         ,
    REG_OFF_N0         ,
    REG_OFF_N1         ,
    REG_OFF_N2         ,
    REG_OFF_N3         ,
    REG_OFF_M0         ,
    REG_OFF_M1         ,
    REG_OFF_M2         ,
    REG_OFF_M3         ,
    REG_OFF_SA0        ,
    REG_OFF_LC0        ,
    REG_OFF_SA1        ,
    REG_OFF_LC1        ,
    REG_OFF_SA2        ,
    REG_OFF_LC2        ,
    REG_OFF_SA3        ,
    REG_OFF_LC3        ,
    REG_OFF_D2_E_D3_E  ,
    REG_OFF_EMPTY1     ,
    REG_OFF_D4_E_D5_E  ,
    REG_OFF_D6_E_D7_E  ,
    REG_OFF_D8_E_D9_E  ,
    REG_OFF_D10_E_D11_E,
    REG_OFF_D12_E_D13_E,
    REG_OFF_D14_E_D15_E,
    REG_OFF_D2         ,
    REG_OFF_D3         ,
    REG_OFF_GCR	       ,
    REG_OFF_MCTL       ,
    REG_OFF_D4         ,
    REG_OFF_D5         ,
    REG_OFF_D6         ,
    REG_OFF_D7         ,
    REG_OFF_D8         ,
    REG_OFF_D9         ,
    REG_OFF_D10        ,
    REG_OFF_D11        ,
    REG_OFF_D12        ,
    REG_OFF_D13        ,
    REG_OFF_D14        ,
    REG_OFF_D15        ,
    REG_OFF_BTR0       ,
    REG_OFF_BTR1       ,
    REG_OFF_MMU_DATA_ID ,
    REG_OFF_MMU_DATA_SEG,
    REG_OFF_MMU_PROG_ID ,
    REG_OFF_MMU_PROG_SEG,
    CONTEXT_SIZE
} register_offset_in_stack;

#define TASK_STATUS_REG 0x0000000E  /**< Task status register create value */

/**
   Empty stack content
   pc          = 0x00000000,
   sr          = 0x0000000E,
   d0.e:d1.e   = 0x00000000,
   Empty       = EMPTY_STACK,
   d0          = 0x00000000,
   d1          = 0x00000000,
*/
#define EMPTY_STACK     0xCACACACA

/** Default stack content    */
#define STACK_DEFAULT            \
{                                \
/* r0          = */ 0x00000000,  \
/* r1          = */ 0x00000000,  \
/* r2          = */ 0x00000000,  \
/* r3          = */ 0x00000000,  \
/* r4          = */ 0x00000000,  \
/* r5          = */ 0x00000000,  \
/* r6          = */ 0x00000000,  \
/* r7          = */ 0x00000000,  \
/* b0          = */ 0x00000000,  \
/* b1          = */ 0x00000000,  \
/* b2          = */ 0x00000000,  \
/* b3          = */ 0x00000000,  \
/* b4          = */ 0x00000000,  \
/* b5          = */ 0x00000000,  \
/* b6          = */ 0x00000000,  \
/* b7          = */ 0x00000000,  \
/* n0          = */ 0x00000000,  \
/* n1          = */ 0x00000000,  \
/* n2          = */ 0x00000000,  \
/* n3          = */ 0x00000000,  \
/* m0          = */ 0x00000000,  \
/* m1          = */ 0x00000000,  \
/* m2          = */ 0x00000000,  \
/* m3          = */ 0x00000000,  \
/* sa0         = */ 0x00000000,  \
/* lc0         = */ 0x00000000,  \
/* sa1         = */ 0x607460C4,  \
/* lc1         = */ 0x00000000,  \
/* sa2         = */ 0x41407190,  \
/* lc2         = */ 0x00000000,  \
/* sa3         = */ 0x01002D30,  \
/* lc3         = */ 0x00000000,  \
/* d2.e:d3.e   = */ 0x00000000,  \
/* empty       = */ EMPTY_STACK, \
/* d4.e:d5.e   = */ 0x00000000,  \
/* d6.e:d7.e   = */ 0x00000000,  \
/* d8.e:d9.e   = */ 0x00000000,  \
/* d10.e:d11.e = */ 0x00000000,  \
/* d12.e:d13.e = */ 0x00000000,  \
/* d14.e:d15.e = */ 0x00000000,  \
/* d2          = */ 0x00000000,  \
/* d3          = */ 0x00000000,  \
/* gcr         = */ 0x00000004,  \
/* mctl        = */ 0x00000000,  \
/* d4          = */ 0x00000000,  \
/* d5          = */ 0x00000000,  \
/* d6          = */ 0x00000000,  \
/* d7          = */ 0x00000000,  \
/* d8          = */ 0x00000000,  \
/* d9          = */ 0x00000000,  \
/* d10         = */ 0x00000000,  \
/* d11         = */ 0x00000000,  \
/* d12         = */ 0x00000000,  \
/* d13         = */ 0x00000000,  \
/* d14         = */ 0x00000000,  \
/* d15         = */ 0x00000000,  \
/* btr0        = */ 0x00000000,  \
/* btr1        = */ 0x00000000,  \
/* did         = */ 0x00000001,  \
/* data seg    = */ 0x00000002,  \
/* pid         = */ 0x00000003,  \
/* prog seg    = */ 0x00000004   \
}

/** @} */ // dsp_def_s3x00_id
/** @} */ // dsp_def_id

#endif // __SC3X00_MACROS_H



/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/10/17 14:01:04 $
 $Id: psc913x_hwi.h,v 1.12 2012/10/17 14:01:04 b41640 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/psc913x_hwi.h,v $
 $Revision: 1.12 $
******************************************************************************/

/******************************************************************************

 @File          psc913x_hwi.h

 @Description   PSC913X-specific Hardware Interrupts header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __PSC913X_HWI_H
#define __PSC913X_HWI_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         arch_913x_int_id  PSC913x HW Interrupts
 @{
*//***************************************************************************/

/************************************************************//**
 @Collection    Core Subsystem Interrupt Mesh
 @{
*//*************************************************************/
#define OS_INT_FROM_CORE0_0        (EPIC_INTERRUPTS_OFFSET + 34)
#define OS_INT_FROM_CORE0_1        (EPIC_INTERRUPTS_OFFSET + 35)

#ifdef PSC9132
#define OS_INT_FROM_CORE1_0        (EPIC_INTERRUPTS_OFFSET + 36)
#define OS_INT_FROM_CORE1_1        (EPIC_INTERRUPTS_OFFSET + 37)
#endif /* PSC9132 */
/* @} */

/* Reserved IRQ38 - IRQ41 */

/************************************************************//**
 @Collection    External Peripherals Interrupts
 @{
*//*************************************************************/
#define OS_INT_ERROR                (EPIC_INTERRUPTS_OFFSET + 42)
#define OS_INT_I2C                  (EPIC_INTERRUPTS_OFFSET + 43)
#define OS_INT_DUART1_DUART2        (EPIC_INTERRUPTS_OFFSET + 44)
#define OS_INT_GPIO                 (EPIC_INTERRUPTS_OFFSET + 45)
#define OS_INT_IFC                  (EPIC_INTERRUPTS_OFFSET + 46)
#define OS_INT_PERF_MONITOR         (EPIC_INTERRUPTS_OFFSET + 47)
#define OS_INT_SPI1                 (EPIC_INTERRUPTS_OFFSET + 48)
#define OS_INT_SPI2                 (EPIC_INTERRUPTS_OFFSET + 49)

#define OS_INT_SPI3                 (EPIC_INTERRUPTS_OFFSET + 70)
#define OS_INT_SPI4                 (EPIC_INTERRUPTS_OFFSET + 71)
#define OS_INT_VETSEC1_TX1          (EPIC_INTERRUPTS_OFFSET + 72)
#define OS_INT_VETSEC1_RX1          (EPIC_INTERRUPTS_OFFSET + 73)
#define OS_INT_VETSEC1_ERR1         (EPIC_INTERRUPTS_OFFSET + 74)
#define OS_INT_VETSEC1_TX2          (EPIC_INTERRUPTS_OFFSET + 75)
#define OS_INT_VETSEC1_RX2          (EPIC_INTERRUPTS_OFFSET + 76)
#define OS_INT_VETSEC1_ERR2         (EPIC_INTERRUPTS_OFFSET + 77)

#define OS_INT_IEEE_1588_1          (EPIC_INTERRUPTS_OFFSET + 86)
#define OS_INT_VETSEC2_TX1          (EPIC_INTERRUPTS_OFFSET + 87)
#define OS_INT_VETSEC2_RX1          (EPIC_INTERRUPTS_OFFSET + 88)

#define OS_INT_VETSEC2_ERR1         (EPIC_INTERRUPTS_OFFSET + 91)
#define OS_INT_VETSEC2_TX2          (EPIC_INTERRUPTS_OFFSET + 92)
#define OS_INT_VETSEC2_RX2          (EPIC_INTERRUPTS_OFFSET + 93)
#define OS_INT_VETSEC2_ERR2         (EPIC_INTERRUPTS_OFFSET + 94)
#define OS_INT_IEEE_1588_2          (EPIC_INTERRUPTS_OFFSET + 95)
#define OS_INT_USB                  (EPIC_INTERRUPTS_OFFSET + 96)
#define OS_INT_ESDHC                (EPIC_INTERRUPTS_OFFSET + 97)
#define OS_INT_SIM                  (EPIC_INTERRUPTS_OFFSET + 98)
#define OS_INT_PWM1                 (EPIC_INTERRUPTS_OFFSET + 99)

#define OS_INT_PWM2                 (EPIC_INTERRUPTS_OFFSET + 132)

#define OS_INT_SEC_MONITOR          (EPIC_INTERRUPTS_OFFSET + 137)
#define OS_INT_SEC_GENERAL          (EPIC_INTERRUPTS_OFFSET + 138)
#define OS_INT_SEC_JQ1              (EPIC_INTERRUPTS_OFFSET + 139)
#define OS_INT_SEC_JQ2              (EPIC_INTERRUPTS_OFFSET + 140)
#define OS_INT_SEC_JQ3              (EPIC_INTERRUPTS_OFFSET + 141)
#define OS_INT_SEC_JQ4              (EPIC_INTERRUPTS_OFFSET + 142)

#define OS_INT_PEX_MSG_A            (EPIC_INTERRUPTS_OFFSET + 173)
#define OS_INT_PEX_MSG_B            (EPIC_INTERRUPTS_OFFSET + 174)
#define OS_INT_PEX_MSG_C            (EPIC_INTERRUPTS_OFFSET + 175)
#define OS_INT_PEX_MSG_D            (EPIC_INTERRUPTS_OFFSET + 176)

/* @} */

/************************************************************//**
 @Collection    MAPLE-B2P Interrupts
 @{
*//*************************************************************/
#define OS_TOTAL_MAPLE_INTERRUPTS   32
#define OS_INT_MAPLE_CH_16         (EPIC_INTERRUPTS_OFFSET + 50)
#define OS_INT_MAPLE_CH_17         (EPIC_INTERRUPTS_OFFSET + 51)
#define OS_INT_MAPLE_CH_18         (EPIC_INTERRUPTS_OFFSET + 52)
#define OS_INT_MAPLE_CH_19         (EPIC_INTERRUPTS_OFFSET + 53)
#define OS_INT_MAPLE_CH_20         (EPIC_INTERRUPTS_OFFSET + 54)
#define OS_INT_MAPLE_CH_21         (EPIC_INTERRUPTS_OFFSET + 55)
#define OS_INT_MAPLE_CH_22         (EPIC_INTERRUPTS_OFFSET + 56)
#define OS_INT_MAPLE_CH_23         (EPIC_INTERRUPTS_OFFSET + 57)
#define OS_INT_MAPLE_CH_24         (EPIC_INTERRUPTS_OFFSET + 58)
#define OS_INT_MAPLE_CH_25         (EPIC_INTERRUPTS_OFFSET + 59)
#define OS_INT_MAPLE_CH_26         (EPIC_INTERRUPTS_OFFSET + 60)
#define OS_INT_MAPLE_CH_27         (EPIC_INTERRUPTS_OFFSET + 61)
#define OS_INT_MAPLE_CH_28         (EPIC_INTERRUPTS_OFFSET + 62)
#define OS_INT_MAPLE_CH_29         (EPIC_INTERRUPTS_OFFSET + 63)
#define OS_INT_MAPLE_CH_30         (EPIC_INTERRUPTS_OFFSET + 64)
#define OS_INT_MAPLE_CH_31         (EPIC_INTERRUPTS_OFFSET + 65)

#define OS_INT_MAPLE_CH_0          (EPIC_INTERRUPTS_OFFSET + 209)
#define OS_INT_MAPLE_CH_1          (EPIC_INTERRUPTS_OFFSET + 210)
#define OS_INT_MAPLE_CH_2          (EPIC_INTERRUPTS_OFFSET + 211)
#define OS_INT_MAPLE_CH_3          (EPIC_INTERRUPTS_OFFSET + 212)
#define OS_INT_MAPLE_CH_4          (EPIC_INTERRUPTS_OFFSET + 213)
#define OS_INT_MAPLE_CH_5          (EPIC_INTERRUPTS_OFFSET + 214)
#define OS_INT_MAPLE_CH_6          (EPIC_INTERRUPTS_OFFSET + 215)
#define OS_INT_MAPLE_CH_7          (EPIC_INTERRUPTS_OFFSET + 216)
#define OS_INT_MAPLE_CH_8          (EPIC_INTERRUPTS_OFFSET + 217)
#define OS_INT_MAPLE_CH_9          (EPIC_INTERRUPTS_OFFSET + 218)
#define OS_INT_MAPLE_CH_10         (EPIC_INTERRUPTS_OFFSET + 219)
#define OS_INT_MAPLE_CH_11         (EPIC_INTERRUPTS_OFFSET + 220)
#define OS_INT_MAPLE_CH_12         (EPIC_INTERRUPTS_OFFSET + 221)
#define OS_INT_MAPLE_CH_13         (EPIC_INTERRUPTS_OFFSET + 222)
#define OS_INT_MAPLE_CH_14         (EPIC_INTERRUPTS_OFFSET + 223)
#define OS_INT_MAPLE_CH_15         (EPIC_INTERRUPTS_OFFSET + 224)

/* @} */


/************************************************************//**
 @Collection    CPRI Interrupts
 @{
*//*************************************************************/
#ifdef PSC9132
#define OS_INT_CPRI_0              (EPIC_INTERRUPTS_OFFSET + 66)
#define OS_INT_CPRI_1              (EPIC_INTERRUPTS_OFFSET + 67)
#define OS_INT_CPRI_2              (EPIC_INTERRUPTS_OFFSET + 68)
#define OS_INT_CPRI_3              (EPIC_INTERRUPTS_OFFSET + 69)
#endif /* PSC9132 */

/* @} */

/************************************************************//**
 @Collection    32 Bit Timers Interrupts
 @{
*//*************************************************************/
#define OS_INT_SOC_TMR0            (EPIC_INTERRUPTS_OFFSET + 78)
#define OS_INT_SOC_TMR1            (EPIC_INTERRUPTS_OFFSET + 79)
#define OS_INT_SOC_TMR2            (EPIC_INTERRUPTS_OFFSET + 80)
#define OS_INT_SOC_TMR3            (EPIC_INTERRUPTS_OFFSET + 81)
#define OS_INT_SOC_TMR4            (EPIC_INTERRUPTS_OFFSET + 82)
#define OS_INT_SOC_TMR5            (EPIC_INTERRUPTS_OFFSET + 83)
#define OS_INT_SOC_TMR6            (EPIC_INTERRUPTS_OFFSET + 84)
#define OS_INT_SOC_TMR7            (EPIC_INTERRUPTS_OFFSET + 85)
/* @} */



/************************************************************//**
 @Collection   General Configuration Interrupts
 @{
*//*************************************************************/
#ifdef PSC9132
#define OS_INT_CPRI_RX_CTRL        (EPIC_INTERRUPTS_OFFSET + 89)
#define OS_INT_CPRI_TX_CTRL        (EPIC_INTERRUPTS_OFFSET + 90)

#define OS_INT_CPRI_RX_TIMING      (EPIC_INTERRUPTS_OFFSET + 253)
#define OS_INT_CPRI_TX_TIMING      (EPIC_INTERRUPTS_OFFSET + 254)
#endif /* PSC9132 */

#define OS_INT_ORED_DEBUG          (EPIC_INTERRUPTS_OFFSET + 244)
#define OS_INT_ORED_GENERAL        (EPIC_INTERRUPTS_OFFSET + 245)

#define OS_INT_ORED_MAPLE          (EPIC_INTERRUPTS_OFFSET + 247)

/* @} */


/************************************************************//**
 @Collection   AIC Interrupts
 @{
*//*************************************************************/
#define OS_INT_AIC_MAPLE_RX_RIQT0       (EPIC_INTERRUPTS_OFFSET + 100)
#define OS_INT_AIC_MAPLE_RX_RIQT1       (EPIC_INTERRUPTS_OFFSET + 101)
#define OS_INT_AIC_MAPLE_TX_TIQT0       (EPIC_INTERRUPTS_OFFSET + 102)
#define OS_INT_AIC_MAPLE_TX_TIQT1       (EPIC_INTERRUPTS_OFFSET + 103)
#define OS_INT_AIC_NXT_TX_SYM0          (EPIC_INTERRUPTS_OFFSET + 104)
#define OS_INT_AIC_TTI_DL0              (EPIC_INTERRUPTS_OFFSET + 105)
#define OS_INT_AIC_WCDMA_FRAME          (EPIC_INTERRUPTS_OFFSET + 106)
#define OS_INT_AIC_RESERVED             (EPIC_INTERRUPTS_OFFSET + 107)
#define OS_INT_AIC_FRAME0               (EPIC_INTERRUPTS_OFFSET + 108)
#define OS_INT_AIC_FRAME1               (EPIC_INTERRUPTS_OFFSET + 109)
#define OS_INT_AIC_FRAME2               (EPIC_INTERRUPTS_OFFSET + 110)
#define OS_INT_AIC_FRAME3               (EPIC_INTERRUPTS_OFFSET + 111)
#define OS_INT_AIC_NXT_RX_SYM0          (EPIC_INTERRUPTS_OFFSET + 112)
#define OS_INT_AIC_NXT_RX_SYM1          (EPIC_INTERRUPTS_OFFSET + 113)
#define OS_INT_AIC_NXT_RX_SYM2          (EPIC_INTERRUPTS_OFFSET + 114)
#define OS_INT_AIC_NXT_RX_SYM3          (EPIC_INTERRUPTS_OFFSET + 115)
#define OS_INT_AIC_NXT_RX_SYM4          (EPIC_INTERRUPTS_OFFSET + 116)
#define OS_INT_AIC_SI_PRG_PRETIMED_DL0  (EPIC_INTERRUPTS_OFFSET + 117)
#define OS_INT_AIC_SI_PRG_PRETIMED_DL1  (EPIC_INTERRUPTS_OFFSET + 118)
#define OS_INT_AIC_SI_PRG_PRETIMED_DL2  (EPIC_INTERRUPTS_OFFSET + 119)
#define OS_INT_AIC_SI_PRG_PRETIMED_DL3  (EPIC_INTERRUPTS_OFFSET + 120)
#define OS_INT_AIC_SNIFF_CAPT_COMP0     (EPIC_INTERRUPTS_OFFSET + 121)
#define OS_INT_AIC_SNIFF_CAPT_COMP1     (EPIC_INTERRUPTS_OFFSET + 122)
#define OS_INT_AIC_TTI_UL0              (EPIC_INTERRUPTS_OFFSET + 123)
#define OS_INT_AIC_TTI_UL1              (EPIC_INTERRUPTS_OFFSET + 124)
#define OS_INT_AIC_TTI_UL2              (EPIC_INTERRUPTS_OFFSET + 125)
#define OS_INT_AIC_TTI_UL3              (EPIC_INTERRUPTS_OFFSET + 126)
#define OS_INT_AIC_TTI_UL4              (EPIC_INTERRUPTS_OFFSET + 127)
#define OS_INT_AIC                      (EPIC_INTERRUPTS_OFFSET + 128)
#define OS_INT_AIC_ERROR                (EPIC_INTERRUPTS_OFFSET + 129)
#define OS_INT_AIC_SNIFF_FRAME0         (EPIC_INTERRUPTS_OFFSET + 130)
#define OS_INT_AIC_SNIFF_FRAME1         (EPIC_INTERRUPTS_OFFSET + 131)

/* @} */


/************************************************************//**
 @Collection   DMA Interrupts
 @{
*//*************************************************************/
#ifdef PSC9132
#define OS_INT_DMA_CH_0            (EPIC_INTERRUPTS_OFFSET + 144)
#define OS_INT_DMA_CH_1            (EPIC_INTERRUPTS_OFFSET + 145)
#define OS_INT_DMA_CH_2            (EPIC_INTERRUPTS_OFFSET + 146)
#define OS_INT_DMA_CH_3            (EPIC_INTERRUPTS_OFFSET + 147)
#define OS_INT_DMA_CH_4            (EPIC_INTERRUPTS_OFFSET + 148)
#define OS_INT_DMA_CH_5            (EPIC_INTERRUPTS_OFFSET + 149)
#define OS_INT_DMA_CH_6            (EPIC_INTERRUPTS_OFFSET + 150)
#define OS_INT_DMA_CH_7            (EPIC_INTERRUPTS_OFFSET + 151)
#define OS_INT_DMA_CH_8            (EPIC_INTERRUPTS_OFFSET + 152)
#define OS_INT_DMA_CH_9            (EPIC_INTERRUPTS_OFFSET + 153)
#define OS_INT_DMA_CH_10           (EPIC_INTERRUPTS_OFFSET + 154)
#define OS_INT_DMA_CH_11           (EPIC_INTERRUPTS_OFFSET + 155)
#define OS_INT_DMA_CH_12           (EPIC_INTERRUPTS_OFFSET + 156)
#define OS_INT_DMA_CH_13           (EPIC_INTERRUPTS_OFFSET + 157)
#define OS_INT_DMA_CH_14           (EPIC_INTERRUPTS_OFFSET + 158)
#define OS_INT_DMA_CH_15           (EPIC_INTERRUPTS_OFFSET + 159)
#endif /* PSC9132 */

/* @} */

/************************************************************//**
 @Collection   TDM Interrupts
 @{
*//*************************************************************/
#define OS_INT_PARITY_ERR          (EPIC_INTERRUPTS_OFFSET + 160)
#define OS_INT_RX_ERR_INT0         (EPIC_INTERRUPTS_OFFSET + 161)
#define OS_INT_TDM0RFTE            (EPIC_INTERRUPTS_OFFSET + 162)
#define OS_INT_TDM0RSTE            (EPIC_INTERRUPTS_OFFSET + 163)
#define OS_INT_TX_ERR_INT0         (EPIC_INTERRUPTS_OFFSET + 164)
#define OS_INT_TDM0TFTE            (EPIC_INTERRUPTS_OFFSET + 165)
#define OS_INT_TDM0TSTE            (EPIC_INTERRUPTS_OFFSET + 166)
#ifdef PSC9132
#define OS_INT_RX_ERR_INT1         (EPIC_INTERRUPTS_OFFSET + 167)
#define OS_INT_TDM1RFTE            (EPIC_INTERRUPTS_OFFSET + 168)
#define OS_INT_TDM1RSTE            (EPIC_INTERRUPTS_OFFSET + 169)
#define OS_INT_TX_ERR_INT1         (EPIC_INTERRUPTS_OFFSET + 170)
#define OS_INT_TDM1TFTE            (EPIC_INTERRUPTS_OFFSET + 171)
#define OS_INT_TDM1TSTE            (EPIC_INTERRUPTS_OFFSET + 172)
#endif /* PSC9132 */

/* @} */


/************************************************************//**
 @Collection   VIRQ Interrupts
 @{
*//*************************************************************/
#define OS_INT_VIRQ0               (EPIC_INTERRUPTS_OFFSET + 177)
#define OS_INT_VIRQ1               (EPIC_INTERRUPTS_OFFSET + 178)
#define OS_INT_VIRQ2               (EPIC_INTERRUPTS_OFFSET + 179)
#define OS_INT_VIRQ3               (EPIC_INTERRUPTS_OFFSET + 180)
#define OS_INT_VIRQ4               (EPIC_INTERRUPTS_OFFSET + 181)
#define OS_INT_VIRQ5               (EPIC_INTERRUPTS_OFFSET + 182)
#define OS_INT_VIRQ6               (EPIC_INTERRUPTS_OFFSET + 183)
#define OS_INT_VIRQ7               (EPIC_INTERRUPTS_OFFSET + 184)
#define OS_INT_VIRQ8               (EPIC_INTERRUPTS_OFFSET + 185)
#define OS_INT_VIRQ9               (EPIC_INTERRUPTS_OFFSET + 186)
#define OS_INT_VIRQ10              (EPIC_INTERRUPTS_OFFSET + 187)
#define OS_INT_VIRQ11              (EPIC_INTERRUPTS_OFFSET + 188)
#define OS_INT_VIRQ12              (EPIC_INTERRUPTS_OFFSET + 189)
#define OS_INT_VIRQ13              (EPIC_INTERRUPTS_OFFSET + 190)
#define OS_INT_VIRQ14              (EPIC_INTERRUPTS_OFFSET + 191)
#define OS_INT_VIRQ15              (EPIC_INTERRUPTS_OFFSET + 192)

/* @} */

/************************************************************//**
 @Collection   OCN2MAG Interrupts
 @{
*//*************************************************************/
#define OS_INT_OCN2MAG             (EPIC_INTERRUPTS_OFFSET + 202)

/* @} */

/************************************************************//**
 @Collection   OCN DMA Interrupts
 @{
*//*************************************************************/
#define OS_INT_OCN_DMA0_CH0        (EPIC_INTERRUPTS_OFFSET + 203)
#define OS_INT_OCN_DMA0_CH1        (EPIC_INTERRUPTS_OFFSET + 204)
#define OS_INT_OCN_DMA0_CH2        (EPIC_INTERRUPTS_OFFSET + 205)
#define OS_INT_OCN_DMA0_CH3        (EPIC_INTERRUPTS_OFFSET + 206)

/* @} */

/************************************************************//**
 @Collection   UDE Interrupts
 @{
*//*************************************************************/
#define OS_INT_UDE0                (EPIC_INTERRUPTS_OFFSET + 207)
#ifdef PSC9132
#define OS_INT_UDE1                (EPIC_INTERRUPTS_OFFSET + 208)
#endif /* PSC9132 */

/* @} */



/************************************************************//**
 @Collection   External IRQs Interrupts
 @{
*//*************************************************************/
#define OS_INT_IRQ0                (EPIC_INTERRUPTS_OFFSET + 226)
#define OS_INT_IRQ1                (EPIC_INTERRUPTS_OFFSET + 227)
#define OS_INT_IRQ2                (EPIC_INTERRUPTS_OFFSET + 228)
#define OS_INT_IRQ3                (EPIC_INTERRUPTS_OFFSET + 229)
#define OS_INT_IRQ4                (EPIC_INTERRUPTS_OFFSET + 230)
#define OS_INT_IRQ5                (EPIC_INTERRUPTS_OFFSET + 231)
#define OS_INT_IRQ6                (EPIC_INTERRUPTS_OFFSET + 232)
#define OS_INT_IRQ7                (EPIC_INTERRUPTS_OFFSET + 233)

#define OS_INT_IRQ8                (EPIC_INTERRUPTS_OFFSET + 237)
#define OS_INT_IRQ9                (EPIC_INTERRUPTS_OFFSET + 238)
#define OS_INT_IRQ10               (EPIC_INTERRUPTS_OFFSET + 239)
#define OS_INT_IRQ11               (EPIC_INTERRUPTS_OFFSET + 240)

/* @} */

/************************************************************//**
 @Collection   Watch Dog Timer 0 Interrupts
 @{
*//*************************************************************/
#define OS_INT_ORED_WDT            (EPIC_INTERRUPTS_OFFSET + 246)

/* @} */

#define OS_INT_LAST                (EPIC_INTERRUPTS_OFFSET + 255)   /**< last interrupt index */
#define MAX_INTERRUPTS             (OS_INT_LAST + 1)                /**< total number of interrupts */



/************************************************************************/
/* General interrupts */
/************************************************************************/

#define OS_INT_GENERAL_OFFSET      (OS_INT_LAST+1)                  /**< index of 1st general interrupt */
#define OS_INT_GEN_FIRST           (OS_INT_GENERAL_OFFSET + 0)      /**< same as #OS_INT_GENERAL_OFFSET */

/************************************************************//**
 @Collection   General interrupts - Debug
 @{
*//*************************************************************/
#define OS_INT_CLASS_OVR          (OS_INT_GENERAL_OFFSET + 0)  /**< CLASS overrun */
#define OS_INT_CLASS_WP           (OS_INT_GENERAL_OFFSET + 1)  /**< CLASS watchpoint */
#define OS_INT_CLASS_ERR          (OS_INT_GENERAL_OFFSET + 2)  /**< CLASS error */
#define OS_INT_CPRI_GEN_ERR       (OS_INT_GENERAL_OFFSET + 3)  /**< CPRI general error */

/* @} */

/************************************************************//**
 @Collection   General interrupts - General
 @{
*//*************************************************************/
#define OS_INT_MAPLE_ECC_ERR       (OS_INT_GENERAL_OFFSET + 4)  /**< Multi ECC error interrupt of the MAPLE */
#define OS_INT_DDR_ERR             (OS_INT_GENERAL_OFFSET + 5)  /**< DDR error */
#define OS_INT_DMA_ERR             (OS_INT_GENERAL_OFFSET + 6)  /**< DMA error */

/* @} */

/************************************************************//**
 @Collection   General interrupts - MapleB2P/F
 @{
*//*************************************************************/
#define OS_INT_MAPLE_SYS_ERR       (OS_INT_GENERAL_OFFSET + 7)  /**< MAPLE general error */

/* @} */

#define OS_INT_GEN_LAST            (OS_INT_MAPLE_SYS_ERR)       /**< index of last general interrupt */

/** @} */ // arch_913x_int_id

_OS_END_EXTERN_C

#endif // __PSC913X_HWI_H



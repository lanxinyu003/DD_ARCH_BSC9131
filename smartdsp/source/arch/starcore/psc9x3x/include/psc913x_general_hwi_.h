/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:28:49 $
 $Id: psc913x_general_hwi_.h,v 1.6 2012/11/22 16:28:49 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/include/psc913x_general_hwi_.h,v $
 $Revision: 1.6 $
******************************************************************************/

/******************************************************************************

 @File          psc913x_general_hwi_.h

 @Description   Ored general Hardware Interrupts defines and structures.

 @Cautions      None.

*//***************************************************************************/

#ifndef __PSC913X_GENERAL_HWI__H
#define __PSC913X_GENERAL_HWI__H

#define GCR_ADDR     (SOC_DSP_CCSR_DEFAULT + 0x18000)
#define GIR_ADDR_1   (volatile uint32_t*)(GCR_ADDR + 0x80)
#define GIR_ADDR_3   (volatile uint32_t*)(GCR_ADDR + 0xA4)
#define GIR_ADDR_5   (volatile uint32_t*)(GCR_ADDR + 0xEC)
#define GIR_ADDR_7   (volatile uint32_t*)(GCR_ADDR + 0x194)

#define GIER_ADDR_1   (volatile uint32_t*)(GIR_ADDR_1 + 0x4)
#define GIER_ADDR_3   (volatile uint32_t*)(GIR_ADDR_3 + 0x4)
#define GIER_ADDR_5   (volatile uint32_t*)(GIR_ADDR_5 + 0x4)
#define GIER_ADDR_7   (volatile uint32_t*)(GIR_ADDR_7 + 0x4)

#define MAX_ORED_INTERRUPTS    32

/* IMPORTANT: The order of populating this array MUST match the order of the
 * interrupts in psc9132_hwi.h
 * CPRI Rx/Tx general interrupts will be handled by the CPRI driver */

/* Used for OS_INT_ORED_GENERAL interrupt line */
general_interrupt_t general_ored_interrupt[] =
{
/*  gir_addr       gier_addr     mask        callback          arg                HWI num                */
    {GIR_ADDR_3,   GIER_ADDR_3,  0x00020000, osHwiNullHandler, (os_hwi_arg) NULL, OS_INT_MAPLE_ECC_ERR},
    {GIR_ADDR_3,   GIER_ADDR_3,  0x00200000, osHwiNullHandler, (os_hwi_arg) NULL, OS_INT_DDR_ERR},
    {GIR_ADDR_1,   GIER_ADDR_1,  0x00100000, osHwiNullHandler, (os_hwi_arg) NULL, OS_INT_DMA_ERR},
    {0,            0,            0x0,        osHwiNullHandler, (os_hwi_arg) NULL, 0}
};

/* Used for OS_INT_ORED_DEBUG interrupt line */
general_interrupt_t debug_ored_interrupt[] =
{
    /*  gir_addr       gier_addr     mask        callback          arg                HWI num             */
    {GIR_ADDR_3,   GIER_ADDR_3,  0x00000001, osHwiNullHandler, (os_hwi_arg) NULL, OS_INT_CLASS_OVR},
    {GIR_ADDR_3,   GIER_ADDR_3,  0x00000002, osHwiNullHandler, (os_hwi_arg) NULL, OS_INT_CLASS_WP},
    {GIR_ADDR_3,   GIER_ADDR_3,  0x00000010, osHwiNullHandler, (os_hwi_arg) NULL, OS_INT_CLASS_ERR},
    {GIR_ADDR_7,   GIER_ADDR_7,  0x00000001, osHwiNullHandler, (os_hwi_arg) NULL, OS_INT_CPRI_GEN_ERR},
    {0,            0,            0x0,        osHwiNullHandler, (os_hwi_arg) NULL, 0}
};

/* Used for OS_INT_ORED_MAPLE interrupt line */
general_interrupt_t maple_ored_interrupt[] =
{
     {GIR_ADDR_3,   GIER_ADDR_3,  0x00001000, osHwiNullHandler, (os_hwi_arg) NULL, OS_INT_MAPLE_SYS_ERR},
     {0,            0,            0x0,        osHwiNullHandler, (os_hwi_arg)NULL, 0}
};


#endif // __PSC913X_GENERAL_HWI__H


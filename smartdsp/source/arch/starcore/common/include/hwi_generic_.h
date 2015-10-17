/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:24 $
 $Id: hwi_generic_.h,v 1.3 2012/11/22 16:28:24 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/common/include/hwi_generic_.h,v $
 $Revision: 1.3 $
******************************************************************************/

/******************************************************************************
 
 @File          hwi_generic_.h

 @Description   Generic Interrupt support header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __HWI_GENERIC__H
#define __HWI_GENERIC__H

typedef struct _general_interrupt_t
{
    volatile uint32_t *gir_addr;
    volatile uint32_t *gier_addr;
    uint32_t          mask;
    os_hwi_function   callback;
    os_hwi_arg        arg;
    uint16_t          hwi_num;
} general_interrupt_t;

void osHwiGeneralIsr(os_hwi_arg arg);


os_status osHwiCreateGeneral(os_hwi_handle     hwi_num,
                             os_hwi_function   handler,
                             os_hwi_arg        argument);


#endif // __HWI_GENERIC__H

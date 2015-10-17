/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:21 $
 $Id: hw_sem_init.c,v 1.5 2012/11/22 16:28:21 sw Exp $
 $Source: /cvsdata/SmartDSP/drivers/vsg/hw_sem_init.c,v $
 $Revision: 1.5 $
******************************************************************************/

/******************************************************************************
 
 @File          hw_sem_init.c

 @Description   Hardware semaphores initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_init.h"
#include "hw_sem_init.h"
#include "vsg_memmap.h"


extern hs_map_t*   soc_hw_sem_module;

os_status   hwSemaphoreInitialize()
{
    soc_hw_sem_module = &g_dsp_ccsr_map->hs[0];
    return OS_SUCCESS;
}

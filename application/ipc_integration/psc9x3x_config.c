/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:32 $
 $Id: psc9x3x_config.c,v 1.4 2012/11/22 16:28:32 sw Exp $
 $Source: /cvsdata/SmartDSP/demos/starcore/psc9x3x/ipc_integration/psc9x3x_config.c,v $
 $Revision: 1.4 $
******************************************************************************/


#include "smartdsp_os.h"
#include "psc9x3x_init.h"
#include "psc9x3x_hwi.h"
#include "os_config.h"

platform_init_params_t platform_params =
{
    FALSE,  /* Automatic Branch Target Buffer Flush */
    TRUE,   /* Cache Commands Error Enabled */
    TRUE,   /* Capture Violation Address Enable */
    TRUE,   /* Data Non-aligned Memory Exception Enable */
    FALSE,  /* Write to Same Memory Byte Exception Enable */
    TRUE,   /* Debug and Profiling Unit Enable */
    TRUE,   /* Memory Protection Enable */
    TRUE,   /* Address Translation Enable */
    TRUE    /* Error Detection Code Exception Enable*/
};


/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2014/03/12 17:12:44 $
 $Id: psc9x3x_l1_defense_init.c,v 1.3 2014/03/12 17:12:44 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_l1_defense_init.c,v $
 $Revision: 1.3 $
******************************************************************************/
 

/******************************************************************************
 
 @File          psc9x3x_l1_defense_init.c

 @Description   PSC9x3x specific L1-Defense initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"
#include "smartdsp_os_device.h"#include "smartdsp_init.h"

#include "os_l1_defense.h"
#include "psc9x3x_l1_defense.h"

static bool l1d_enabled = OFF;
#define g_l1d_warm_reset_mode OS_L1D_NO_WARM_RESET;

/*****************************************************************************/
os_status osL1dResetFlowSet(l1d_init_params_t * soc_l1d_init_params, l1d_clean_params_t * clean_params)
{
	VAR_UNUSED(soc_l1d_init_params);
	VAR_UNUSED(clean_params);
	
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osL1dInitialize(l1d_init_params_t * soc_l1d_init_params)
{
	VAR_UNUSED(soc_l1d_init_params);
 
    return OS_SUCCESS;
}

/*****************************************************************************/
bool osL1dIsEnabled()
{
    return l1d_enabled;
}

/*****************************************************************************/
uint64_t getResetCoresId()
{
    return 0;
}

/*****************************************************************************/
uint32_t osL1dGetResetMode()
{
    return g_l1d_warm_reset_mode;
}

/*****************************************************************************/
void osL1dReportStatus(uint32_t l1d_status)
{
	VAR_UNUSED(l1d_status);
}
/*****************************************************************************/void osL1dResetRequest(os_hwi_arg arg)
{
    VAR_UNUSED(arg);
 
	/* report: ready for reset */
    osL1dReportStatus(0);
 
    while(1) {
        NOP();
    }
}

/*****************************************************************************/os_status osL1dHardwareClean(l1d_clean_params_t * clean_params){
    VAR_UNUSED(clean_params);     	return OS_SUCCESS;}
/* Report ended execution of warm reset os initialization */
void osL1dReportStatusEndInit() {
    osL1dReportStatus(0 /*END_OF_WARM_RESET_OS_INIT*/);
}

/* Report warm reset was completed successfully */
void osL1dReportStatusSuccess() {
    osL1dReportStatus(0 /*WARM_RESET_SUCCESS*/);
}

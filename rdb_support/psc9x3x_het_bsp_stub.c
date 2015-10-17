/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2013/06/10 11:34:59 $
 $Id: psc9x3x_het_bsp_stub.c,v 1.11 2013/06/10 11:34:59 b42006 Exp $
 $Source: /cvsdata/SmartDSP/demos/starcore/psc9x3x/rdb_support/psc9x3x_het_bsp_stub.c,v $
 $Revision: 1.11 $
******************************************************************************/
 
#include "smartdsp_os_device.h"
#include "psc9x3x_het_bsp_stub.h"

static os_het_smartdsp_log_t smartsdp_log[SOC_MAX_NUM_OF_CORES];

static const os_het_control_t het_ctrl_default_params =
{
    {
        OS_HET_INITIALIZED,     // pa_initialized
        OS_HET_UNINITIALIZED    // sa_initialized
    },
    0x2000,                     // shared_ctrl_size (8KB)
    /*  pa_shared_mem (16K) */
    {
         0x10000000,    // start_addr
         0x10000        // size
    },
    /*  sc_shared_mem (16K) */
    {
         0x10020000,    // start_addr
         0x00010000     // size
    },
    /* os_het_ipc_t *ipc */
    NULL,
    NULL,               // *aic;
    NULL                // *sdos_debug
};


/* This function mimics what the PA BSP should do in a "real" system */
void psc913xHetBspStubInitialize(void * ctrl_base, os_het_control_t * ctrl_params)
{
    os_het_control_t *shared_ctrl = (os_het_control_t *)ctrl_base;
 
    if (ctrl_params == NULL)
    {
        *shared_ctrl = het_ctrl_default_params;
        shared_ctrl->smartdsp_debug = (os_het_smartdsp_log_t (*)[])smartsdp_log;
    }
    else
    {
        *shared_ctrl = *ctrl_params;
    }
 
    shared_ctrl->initialized.pa_initialized = OS_HET_INITIALIZED;
}

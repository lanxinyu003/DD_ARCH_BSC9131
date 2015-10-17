/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/10/31 14:54:41 $
 $Id: psc9x3x_recovery.h,v 1.13 2012/10/31 14:54:41 b17975 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/psc9x3x_recovery.h,v $
 $Revision: 1.13 $
 **************************************************************************//*

 @File          psc9x3x_recovery.h

 @Description   This file includes all the relevant API for PSC913x recovery support.
                SmartDSP OS recovery support enables restart of SmartDSP OS application without
                resetting the whole PSC913x device.
 
 @Cautions      None.

/**************************************************************************//**/

#ifndef __PSC9X3X_RECOVERY_H
#define __PSC9X3X_RECOVERY_H

#include "smartdsp_os.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         recovery_9x3x_id  PSC913x Recovery API
 
 @Description   SmartDSP OS recovery support enables restart of SmartDSP OS application without
                resetting the whole PSC913x device.
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Description   AIC recovery parameters structure
 
                This structure includes all the relevant parameters for AIC ADI lanes;
                Pointer to this structure with parameters for all the existing lanes will
                be passed to psc9x3xOsRecovery()
 
*//***************************************************************************/
typedef struct
{
        sio_dev_handle aic_adi_lane;
        /**< AIC ADI lane device */
        sio_channel_t  **rx_tx_channels;
        /**< AIC ADI lane RX and TX channels pointers, should be the same as received from osSioChannelOpen() */
        uint8_t         num_channels;
        /**< The number of RX and TX channels used by this AIC ADI lane */
        bool use_unsafe_api;
        /**< Safe API waits for 2 TTI before disabling DMA;
         *   Unsafe API does not wait for 2 TTI after disabling timer before disabling DMA
         *   which may result in underrun error after returning from recovery */
}os_recovery_aic_t;


/**************************************************************************//**
 @Function      psc9x3xOsRecoveryInit

 @Description   Initialization of recovery functionality; use it inside appInit.

 @Param         vnmi            - Pointer to virtual nmi handler; If find_virq == TRUE it will be set inside this function.
 @Param         recovery_cb     - Pointer to recovery callback.
 @Param         prog_mmu_func   - Pointer to debug hook function to override at program MMU debug hook;
                                  If set to NULL the debug hook is not overridden.
 @Param         data_mmu_func   - Pointer to debug hook function to override at data MMU debug hook;
                                  If set to NULL the debug hook is not overridden.
 @Param         find_virq       - If TRUE use osVirtualInterruptFind() or osVirtualInterruptReserve() for VIRQ allocation
                                  and return it inside vnmi parameter; If FALSE use the VIRQ as passed inside *vnmi parameter.
 
 @Return        OS_SUCCESS on success or OS_FAIL otherwise.

 @Cautions      None
 
*//***************************************************************************/
os_status psc9x3xOsRecoveryInit(os_hwi_handle *vnmi,
                                os_hwi_function recovery_cb,
                                debug_hook_function prog_mmu_func,
                                debug_hook_function data_mmu_func,
                                bool find_virq);

typedef os_status (*os_aic_user_func)(void *);
/**************************************************************************//**
 @Function      psc9x3xOsRecovery

 @Description   Recover of PSC9X3X OS without device resetting before DSP image reboot;
                Activate it inside the recovery_cb function passed to psc9x3xOsRecoveryInit().

 @Param         maple_en        - Indicates whether MAPLE is enabled; pass MAPLE.
 @Param         maple_param     - Pointer to maple soft reset parameters.
 @Param         aic_en          - Indicates whether AIC is enabled; pass AIC.
 @Param         aic_adi_lanes   - Pointer to AIC ADI lane devices and all it's RX TX channels;
                                  If set to NULL AIC ADI DMA won't be disabled.
 @Param         num_adi_lanes   - Number of AIC ADI lanes devices pointed by aic_adi_lanes.
 @Param         aic_user_func   - AIC re-synchronization function.
 @Param         aic_params      - AIC re-synchronization function parameters.
 @Param         dcache_enable   - Indicates whether data cache is enabled; pass DCACHE_ENABLE.
 @Param         pcache_enable   - Indicates whether program cache is enabled; pass ICACHE_ENABLE.
 @Param         l2cache_enable  - Indicates whether l2 cache is enabled; L2CACHE_ENABLE.

 @Return        OS_SUCCESS on success or OS_FAIL otherwise.

 @Cautions      None
 
*//***************************************************************************/
os_status psc9x3xOsRecovery(bool maple_en,
                            void *maple_param,
                            bool aic_en,
                            os_recovery_aic_t *aic_adi_lanes,
                            int num_adi_lanes,
                            os_aic_user_func aic_user_func,
                            void *aic_params,
                            bool dcache_enable,
                            bool pcache_enable,
                            bool l2cache_enable);

/**************************************************************************//**
 @Function      psc9x3xOsRecoveryCheck

 @Description   Performs a checksum starting from psc9x3xOsRecovery till psc9x3xOsRecovery_end
                which should be defined inside application linker file.

 @Return        Return a uint64_t number of checksum calculation

 @Cautions      During checksum calculation MMU protection is OFF.
 
*//***************************************************************************/
uint64_t psc9x3xOsRecoveryCheck();

/**************************************************************************//**
 @Function      psc9x3xOsRecoveryReady

 @Description   Activate it right after  psc9x3xOsRecovery() inside recovery_cb, it handles the handshake
                between PA and DSP in the same manner as during the real boot flow:
                Set DSP_READY, poll on PPC_READY, jump to BOOT_JMP_ADDR.
                Before PPC_READY bit is set PPC should make sure that the shared heterogeneous space is cleared
                and properly set; SC MMU is disabled right before the jump to BOOT_JMP_ADDR;

 @Param         send_dsp_ready  - Indicates whether GCR_DSP_STATE[DSP_READY] should be set instead of using IPC;
                Sending DSP ready indication is optional and can be done by application through IPC

 @Return        The function does not return if it succeeds to jump to GCR_BOOT_JMP_ADDR;
                It returns OS_FAIL if the jump to the entry point failed

 @Cautions      None
 
*//***************************************************************************/
os_status psc9x3xOsRecoveryReady(bool send_dsp_ready);

/** @} */ // end of 913x recovery

_OS_END_EXTERN_C

#endif // __PSC9X3X_RECOVERY_H

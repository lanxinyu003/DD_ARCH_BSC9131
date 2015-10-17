/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:50 $
 $Id: psc9x3x_init.h,v 1.12 2012/11/22 16:28:50 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/psc9x3x/include/psc9x3x_init.h,v $
 $Revision: 1.12 $
******************************************************************************/

/**************************************************************************//**
 
 @File          psc9x3x_init.h

 @Description   PSC9X3X Initialization header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __PSC9X3X_INIT_H
#define __PSC9X3X_INIT_H

#include "smartdsp_os.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         init_9x3x_id  PSC9x3x Initialization API

 @Description   API for initializing the PSC9x3x kernel
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Platform configuration parameters.
 
 @Cautions      Due to hardware erratum, wsbee MUST be set to FALSE.
*//***************************************************************************/
typedef struct
//struct platform_init_params_t
{

/**<Automatic Branch Target Buffer Flush - flush BTB on
program task switch (valid also if ENBTB bit is disabled).
if set, a PID update, will cause an automatic BTB flush.  */
bool abtbf;


/**<Cache Commands Error Enabled - if set, the MMU
indicates the core for
DFETCH/DFL2Cx/DMALLOC/PFETCH/PFL2Cx
address error. If clear, for erroneous access, the MMU
will close the access inside the platform, but will not
inform the core for the error.

Note: for DFLUSH/DSYNC erroneous access,
indication to the core will always be issued regardless
the CCEE bit state. */
bool ccee;


/**<Capture Violation Address Enable — Disable/Enable
the capture mechanism of the violated access address. If
disabled, the MMU registers: M_PVA and M_DVA are
not updated in case of error (power saving mode).

***** Attention!!!****** This bit is not a multiple-use
enable/disable bit. The CVAE can only be
disabled once. To use the CVAE again, the DSP Platform must be reset.
*/
bool cvae;


/**<Data Non-aligned Memory Exception Enable - If set, a
data non-aligned access exception is enabled. This
condition occurs when the LSBs of the data address are
not aligned with the width of the access (for example, a
word access with an odd LSB).*/
bool dnamee;


/**<Write to Same Memory Byte Exception Enable - If set,
a write to the same memory byte exception is enabled.
This condition occurs when two parallel data accesses (on
XA and XB buses) attempt to write to the same byte
during the same clock cycle.1 This condition is not an
error but involves a case of unclean code, in which some
write is redundant.*/
bool wsbee;


/**<Debug and Profiling Unit Enable—Enables or disables
the DPU system. If disabled, the registers of the DPU are
also disabled for read and write accesses.
***** Attention!!!****** This bit is not a multiple-use
enable/disable bit. The DPU can only be
enabled once and disabled once. After the
DPU clocks are stopped by resetting this
bit, in order for the user to use the DPU
again, DSP Platform must
be reset before setting the bit.*/
bool dpue;


/**<Memory Protection Enable — A central bit that
enables/disables the protection-checking function in all
enabled segment descriptors, and enables/disables the
miss interrupt support on miss access.*/
bool mpe;

/**<Address Translation Enable — Enables or disables the
address translation mechanism. If disabled, addresses are
not translated (such as, from a virtual address to a
physical address).*/
bool ate;

/**<Error Detection Code Exception Enable - if set, the
EDC exception is Enabled. This bit is disabled after reset.*/
bool edcee;

} platform_init_params_t;

/**************************************************************************//**
 @Function      psc9x3xInitialize

 @Description   Initializes PSC9X3X general architecture parts.

 @Param         clock_in        - PSC9X3X CLKIN frequency (MHz).
 @Param         ctrl_base       - PSC9X3X shared control base address.
 @Param         pa_sys_regs     - PSC9X3X power architecture system registers base address.
 @Param         sys_regs        - PSC9X3X system registers base address.
 @Param         dsp_plat        - PSC9X3X dsp platform registers base address.
 @Param         ctrl_base       - SC PA shared control base address.
 @Param         dcache_enable   - Indicates whether data cache should be enabled.
 @Param         pcache_enable   - Indicates whether program cache should be enabled.
 @Param         l2cache_enable  - Indicates whether l2 cache should be enabled.
 @Param         l2cache_size    - Indicates the size of the shared M2/L2 to be used as L2.
 @Param         platform_params - See #platform_init_params_t.
 @Return        OS status

 @Cautions      None
 
*//***************************************************************************/
os_status psc9x3xInitialize (float  clock_in,
                            void    *dsp_plat,
                            void    *pa_sys_regs,
                            void    *sys_regs,
                            void    *ctrl_base,
                            bool    dcache_enable,
                            bool    pcache_enable,
                            bool    l2cache_enable,
                            unsigned int l2cache_size,
                            platform_init_params_t* platform_params);

#ifndef PSC9X3X_SC_ONLY_BSP
/**************************************************************************//**
 @Function      psc9x3xHetBootInitialize

 @Description   Finalizes getrogeneous boot using HW semaphores;
 
                SC take semaphore and waits till PA releases it

 @Cautions      None
 
*//***************************************************************************/
void psc9x3xHetBootInitialize();
#endif

#ifdef PSC9131_FAMILY
/**************************************************************************//**
 @Function      psc9x3xMmuAliasUpdate

 @Description   Set GCR_NON_SNOOP_MAP and update all MMU segments with start_addr - end_addr
                physical addresses to aliased addresses according to GCR_NON_SNOOP_MAP
 
                Activate this API only inside appInit()
 
 @Param         start_addr     - start of physical address of DSP side DDR; use DDR1_b from linker file
 @Param         end_addr       - end of physical address of DSP side DDR; use DDR1_e from linker file
 
 @Cautions      Supported only for psc9131
 
*//***************************************************************************/
os_status psc9x3xMmuAliasUpdate(os_phys_ptr start_addr, os_phys_ptr end_addr);
#endif

/** @} */ // end of init_9x3x_id

_OS_END_EXTERN_C

#endif // __PSC9X3X_INIT_H

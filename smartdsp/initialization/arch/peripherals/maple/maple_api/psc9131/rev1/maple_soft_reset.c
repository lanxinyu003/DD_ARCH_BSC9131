/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
   Copyright © 1995-2003,2004,2005-2010 Freescale Semiconductor Inc.
   All Rights Reserved
   this is proprietary source code of Freescale Semiconductor Inc., and its use
   is subject to the CodeWarrior EULA. The copyright notice above does not
   evidence any actual or intended publication of such source code.
   ---------------------------------------------------------------------------
   $RCSfile: maple_soft_reset.c,v $
   Tag $Name:  $
   $Revision: 1.26 $
   $Date: 2014/05/28 07:20:11 $
   Target Processor: MAPLE RISC
   Target Device: PSC9131
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_soft_reset.c
 @brief  MAPLE Soft Reset routine file
*/
//----------------------------------------------------------------------------
#include <maple_map.h>
#define MAPLEAPI_GLOBALS
#include <maple_api.h>
#include <stdint.h>
#ifdef _ENTERPRISE_C_
#pragma opt_level = "O0"
#endif
void maple_soft_reset(maple_dev_init_params_t *maple_param)
{
   uint32_t *maple_eccr1_register;
   uint32_t *maple_eccr2_register;
   uint32_t *maple_pcr_register;
   uint32_t  volatile data;
   uint32_t * maple_tmrval_register_ptr;
   uint32_t * maple_MCUBCP_ptr;
   t_CRPEULBParam          *pstCRPEULBParam ;
 
   //enable ulb host command mode and disable ulb dedicated timer
   pstCRPEULBParam = (t_CRPEULBParam*)(((uint8_t *)maple_param->mbus_base)+MAPLE_CRPEULBPARAMETER_BASE_ADDR);
   maple_MCUBCP_ptr = &pstCRPEULBParam->uliMCUBCP;
   MAPLEREAD_INTERNAL(maple_MCUBCP_ptr, &data);
   data |= (1<<20);
   MAPLEWRITE32(maple_MCUBCP_ptr,data);
   MAPLEREAD_INTERNAL(maple_MCUBCP_ptr, &data);
   for (data = 0; data <100; data++) {}
   maple_tmrval_register_ptr = (uint32_t *)  (((uint8_t *)maple_param->sbus_base) + MAPLE_PSTTMRVAL3);
   MAPLEWRITE32(maple_tmrval_register_ptr,0x0);
 
   // Stop new dma commands and wait untill dma queue is empty
   maple_pcr_register =   (uint32_t *) (((uint8_t *)maple_param->sbus_base) + MAPLE_PCR);
   MAPLEWRITE32(maple_pcr_register,(0x00010000|DMA_CLOSE_OPEN_ACC_OPCODE));
   MAPLEREAD_INTERNAL(maple_pcr_register, &data);
   while (data & 0x00010000) // poll end of pcr command
   {
      MAPLEREAD_INTERNAL(maple_pcr_register, &data);
   }

   // Apply soft reset.
   MAPLEWRITE32(maple_pcr_register,0x80000000);
   for (data = 0; data <1000; data++) {}
 
}



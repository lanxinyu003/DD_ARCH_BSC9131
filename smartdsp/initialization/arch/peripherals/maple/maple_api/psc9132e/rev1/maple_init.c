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
   $RCSfile: maple_init.c,v $
   Tag $Name:  $
   $Revision: 1.7 $
   $Date: 2014/11/26 15:30:56 $
   Target Processor: MAPLE RISC
   Target Device: PSC9131
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_init.c
 @brief  MAPLE Init routine file
*/
//----------------------------------------------------------------------------
#include <maple_map.h>
#define MAPLEAPI_GLOBALS
#include <maple_api.h>
#include <stdint.h>
#ifdef _ENTERPRISE_C_
#pragma opt_level = "O0"
#endif
extern void maple_driver_load(maple_dev_init_params_t *maple_param);
extern void maple_driver_config_ftpe(maple_dev_init_params_t *maple_param);
extern void maple_driver_config_tvpe(maple_dev_init_params_t *maple_param);
extern void maple_driver_config_pdsch(void *maple_param);
extern void maple_driver_config_crpe(void *input);

void maple_init(maple_dev_init_params_t *maple_param)
{
   uint32_t volatile i;
   uint32_t  data;
   uint32_t  maple_sdmr;
   uint32_t *maple_peecr_register;
   uint32_t *maple_iadd_register;
   uint32_t *maple_idata_register;
   uint32_t *maple_picgcr_register;
   uint32_t *maple_ready_register;
   uint32_t *maple_mpccr_register;
   uint32_t *maple_sdmr_register;
   uint32_t *maple_mmc1p_parameter;
   uint32_t *maple_mmc2p_parameter;
   uint32_t *maple_mtvcp_parameter;
   uint32_t *maple_mpdschcp_parameter;
   uint32_t *maple_mpuschcp_parameter;
   uint32_t *maple_timer_parameter;
   uint32_t *maple_dram_addr;
   uint32_t not_after_soft_reset;
   t_PUPE   *pstPUPE;
   t_EQPE   *pstEQPE;
   t_MaplePRAM  *pstPRAM;

   pstPUPE = (t_PUPE *) ((uint32_t *)(((uint8_t *)maple_param->mbus_base) + PUPE_REGISTER_BASE_ADDR));
   pstPRAM = (t_MaplePRAM *)(((uint8_t *)maple_param->mbus_base)+MAPLE_PRAM_BASE_ADDR);
   // if ready register is set then we are after soft reset, no need to reload ucode
   maple_ready_register = (uint32_t *) (((uint8_t *)maple_param->sbus_base) + MAPLE_READY);
   MAPLEREAD_INTERNAL(maple_ready_register,&data);
   not_after_soft_reset = (data & 0x80000000)==0;

   if(1)//(maple_param->ecc_protect > 0)
   {
      if (not_after_soft_reset)
      {
          // initialize iram
          maple_iadd_register = (uint32_t *) (((uint8_t *)maple_param->sbus_base) + MAPLE_IADD);
          MAPLEWRITE32(maple_iadd_register,0x80000000);
          maple_idata_register = (uint32_t *) (((uint8_t *)maple_param->sbus_base) + MAPLE_IDATA);
          for(i = 0;i < 0x4800;i++) // initialize all iram
          {
             MAPLEWRITE32(maple_idata_register,0x0);
          }
      }
      maple_peecr_register =(uint32_t *) (((uint8_t *)maple_param->sbus_base) + MAPLE_PEECR);
      MAPLEWRITE32(maple_peecr_register,(1<<30));// enable iram ecc
 
      pstEQPE = (t_EQPE *) ((uint32_t *)(((uint8_t *)maple_param->mbus_base) + EQPE_REGISTER_BASE_ADDR));
      MAPLEWRITE32(((uint32_t *)&(pstEQPE->uliEQECC)),1);// enable ecc
      MAPLEWRITE32(((uint32_t *)&(pstPUPE->uliGCR)),1);// enable ecc
   }
   maple_dram_addr = (uint32_t *) (((uint8_t *)maple_param->mbus_base) + MP_BD_BASE_ADDR);
   for(i = 0;i <  0x1000;i++) // initialize BD area
   {
      MAPLEWRITE32(maple_dram_addr,0x0);
      maple_dram_addr++;
   }
   maple_dram_addr = (uint32_t *)(((uint8_t *)maple_param->mbus_base + 0x40));
   for(i = 0;i < ((0x800-0x40)/4);i++)//Initialize rings area
   {
      MAPLEWRITE32(maple_dram_addr,0x0);
      maple_dram_addr++;
   }
 
   // Write maple modes to DRAM
   maple_mmc1p_parameter = (uint32_t *)  (((uint8_t *)maple_param->mbus_base) + MAPLE_MMC1P);
   MAPLEWRITE32(maple_mmc1p_parameter,maple_param->mode1|0x10);// enable also ecc in mode1
 
   maple_mmc2p_parameter = (uint32_t *)  (((uint8_t *)maple_param->mbus_base) + MAPLE_MMC2P);
   MAPLEWRITE32(maple_mmc2p_parameter,maple_param->mode2);
 
   maple_mtvcp_parameter = (uint32_t *) (((uint8_t *)maple_param->mbus_base) + MAPLE_MTVMCP);
   MAPLEWRITE32(maple_mtvcp_parameter,maple_param->tvpe_mode);
 
   maple_mpdschcp_parameter = (uint32_t *)  (((uint8_t *)maple_param->mbus_base) + MAPLE_MPDSCHCP);
   MAPLEWRITE32(maple_mpdschcp_parameter,maple_param->pdsch_mode);
 
   maple_mpuschcp_parameter = (uint32_t *)  (((uint8_t *)maple_param->mbus_base) + MAPLE_MPUSCHCP);
   MAPLEWRITE32(maple_mpuschcp_parameter,maple_param->pusch_mode);
 
   maple_mpuschcp_parameter = (uint32_t *)  (((uint8_t *)maple_param->mbus_base) + MAPLE_MCONVCP);
   MAPLEWRITE32(maple_mpuschcp_parameter,maple_param->conv_mode);


 
   MAPLEWRITE32(((uint32_t *)&(pstPUPE->uliDMCR)),((((maple_param->tvpe_mode)>>4) & 0x1)<<31));

   // set dma scheme and default priorities in sdmr register
   maple_sdmr_register = (uint32_t *) (((uint8_t *)maple_param->sbus_base) + MAPLE_SDMR);
   data = maple_param->mode1;
   maple_sdmr = ((data>>24)&0x3)|((data>>14)&0x30)|(data&0x30000);
   MAPLEWRITE32(maple_sdmr_register,maple_sdmr);
 
   // Config pe drivers
   maple_driver_config_ftpe((void *)maple_param);
   maple_driver_config_tvpe((void *)maple_param);
   maple_driver_config_pdsch((void *)maple_param);
   maple_driver_config_crpe((void *)maple_param);
 
   if (not_after_soft_reset)
      maple_driver_load(maple_param); // load ucode
 
   // deassert interal endOfMapleInitRoutine indication
   MAPLEWRITE32((uint32_t *)(((uint8_t *)maple_param->mbus_base) + 0x7fc),0x0);

   // Activate Risc
   MAPLEWRITE32(maple_ready_register,0x80000000);
   maple_picgcr_register = (uint32_t *)  (((uint8_t *)maple_param->sbus_base) + MAPLE_GCR);
   MAPLEWRITE32(maple_picgcr_register,0x10);  // use gcr register to start ucode because ready register is not reset by soft reset and can not be written.

#ifdef _ENTERPRISE_C_
        asm(" syncio");
#endif
 
   // poll internal endOfInitRoutine indication
   data = 0x0;
   while (data==0)
   {
        MAPLEREAD_INTERNAL((uint32_t *)(((uint8_t *)maple_param->mbus_base) + 0x7fc),&data);
#ifdef _ENTERPRISE_C_
        asm(" syncio");
#endif
   }
 
   MAPLEWRITE32(&pstPRAM->uliMUCVP,0x50d);
   MAPLEWRITE32(&pstPRAM->uliMBDRCP0,0);
   MAPLEWRITE32(&pstPRAM->uliMBDRCP1 ,0);
   MAPLEWRITE32(&pstPRAM->uliMBDRCP2 , 0);
   /* Initialize FTPE Guard Insertion/Removal DSS 7 to be used by PUFFT & PDSCH. LTE only */
   MAPLEWRITE32(&pstPRAM->usiFTPEDSSxPyR[6][0],0x0024005a);
   MAPLEWRITE32(&pstPRAM->usiFTPEDSSxPyR[6][2],0x0096012c);
   MAPLEWRITE32(&pstPRAM->usiFTPEDSSxPyR[6][4],0x01c20258);
   if (maple_param->pusch_mode & 0x80000000) // PUSCH Enable
   {
		// Initialize PUSCH TVPE Ring
         if (maple_param->pusch_mode & 0x40) // PUSCH_TVPE is High Priority
         {
            MAPLEWRITE32(&pstPRAM->usiMTVBDRHPxR[0][0] , ((PUSCH_TVPE_RING_BA << 16) | PUSCH_TVPE_RING_BA)); // BD Ring Base Address
            MAPLEWRITE32(&pstPRAM->usiMTVBDRHPxR[0][2] , 0x00000001);   // Set Valid to TVPE High Ring
         }
         else // PUSCH_TVPE is Low Priority
         {
            MAPLEWRITE32(&pstPRAM->usiMTVBDRLPxR[0][0] , ((PUSCH_TVPE_RING_BA << 16) | PUSCH_TVPE_RING_BA)); // BD Ring Base Address
            MAPLEWRITE32(&pstPRAM->usiMTVBDRLPxR[0][2] , 0x00000001);   // Set Valid to TVPE Low Ring
         }

         // Initialize PUSCH FTPE ring
         if (maple_param->pusch_mode & 0x20) // PUSCH_FTPE is High Priority
         {
            MAPLEWRITE32(&pstPRAM->usiMFTBDRHPxR[0][0] , ((PUSCH_FTPE_RING_BA << 16) | PUSCH_FTPE_RING_BA)); // BD Ring Base Address
            MAPLEWRITE32(&pstPRAM->usiMFTBDRHPxR[0][2] , 0x00000001);   // Set Valid to FTPE High Ring
         }
         else // PUSCH_FTPE is Low Priority
         {
            MAPLEWRITE32(&pstPRAM->usiMFTBDRLPxR[0][0] , ((PUSCH_FTPE_RING_BA << 16) | PUSCH_FTPE_RING_BA)); // BD Ring Base Address
            MAPLEWRITE32(&pstPRAM->usiMFTBDRLPxR[0][2] , 0x00000001);   // Set Valid to FTPE Low Ring
         }
   }
   if (maple_param->conv_mode & 0x80000000)
   {
      // Initialize CONVPE FTPE ring
      if (maple_param->conv_mode & 0x20) // PUSCH_FTPE is High Priority
      {
         MAPLEWRITE32(&pstPRAM->usiMFTBDRHPxR[1][0] , 0x9b709b70); // BD Ring Base Address
         MAPLEWRITE32(&pstPRAM->usiMFTBDRHPxR[1][2] , 0x00000001);   // Set Valid to FTPE High Ring
      }
      else // PUSCH_FTPE is Low Priority
      {
         MAPLEWRITE32(&pstPRAM->usiMFTBDRLPxR[1][0] , 0x9b709b70) ; // BD Ring Base Address
         MAPLEWRITE32(&pstPRAM->usiMFTBDRLPxR[1][2] , 0x00000001);   // Set Valid to FTPE Low Ring
      }
   }
   MAPLEWRITE32(&pstPRAM->usiMTRCWPP[0] , (( MAPLE_DEBUG_TRACE_BUFFER0)<<16)|(MAPLE_DEBUG_TRACE_BUFFER1));
   // set timer period
   maple_timer_parameter = (uint32_t *) (((uint8_t *)maple_param->mbus_base) + MAPLE_TPP);
 
   //set minimum default value of 0x10, to decrease RISC overloading with parse routines
   maple_param->timer_period = (maple_param->timer_period<0x10) ? (0x10): (maple_param->timer_period);

   MAPLEWRITE32(maple_timer_parameter,(maple_param->timer_period&0x3f));
   // Activate Timer
   maple_mpccr_register = (uint32_t *) (((uint8_t *)maple_param->sbus_base) + MAPLE_MPCCR);
   MAPLEWRITE32(maple_mpccr_register, (0x80000000|((maple_param->timer_period&0x3f)<<24)));

}


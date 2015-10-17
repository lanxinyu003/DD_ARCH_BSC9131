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
   $RCSfile: maple_driver_config_crpe.c,v $
   Tag $Name:  $
   $Revision: 1.7 $
   $Date: 2014/11/26 15:30:56 $
   Target Processor: MAPLE RISC
   Target Device: MSC8155
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_driver_config_crpe.c
 @brief  MAPLE Driver Config CRPE routine file
*/
//----------------------------------------------------------------------------
#include <maple_map.h>
#include <maple_api.h>
#include <stdint.h>
#ifdef _ENTERPRISE_C_
#pragma opt_level = "O0"
#endif
void maple_driver_config_crpe(void *input);
void maple_driver_config_crpe(void *input)
{
    t_MaplePRAM             *pstPRAM;
    t_CRPEDLParam           *pstCRPEDLParam;
    t_CRPEULBParam          *pstCRPEULBParam ;
    t_CRPEULFParam          *pstCRPEULFParam;
    t_CRPEULF               *pstCRPEULF;
    volatile t_CRPEDL       *pstCRPEDL ;
    volatile t_CRPEULB      *pstCRPEULB;
    maple_dev_init_params_t *maple_param;
    uint32_t                 dlmode;
    uint32_t                 ulbmode;
    uint32_t                 data;
    uint32_t                 addr;
    uint32_t                 i;
    uint32_t                 j;

    maple_param = (maple_dev_init_params_t *)input;
    pstPRAM = (t_MaplePRAM *)(((uint8_t *)maple_param->mbus_base)+MAPLE_PRAM_BASE_ADDR);
    ulbmode = maple_param->crpeulb_mode & 0x000f8859;
    MAPLEWRITE32(&pstPRAM->uliMCUBMCP,ulbmode);
    if ((ulbmode&0x8000)==0) // disable ULB bit
    {
        pstCRPEULBParam = (t_CRPEULBParam*)(((uint8_t *)maple_param->mbus_base)+MAPLE_CRPEULBPARAMETER_BASE_ADDR);
        MAPLEWRITE32(&pstCRPEULBParam->uliMCUBOBICP ,0);
        pstCRPEULB = (t_CRPEULB *)(((uint8_t *)maple_param->mbus_base)+CRPEULB_REGISTER_BASE_ADDR);
        if (1/*pstPRAM->uliMMC0P & 0x10*/) // ecc enabled
        {
            MAPLEWRITE32(&pstCRPEULB->uliCRUBGCR , (1<<31)|(1<<23)); //de bug, no ecc
            // write anything to addresses at the end of input when dlyspread=0 to align to 80 bytes (from 0x620 to 0x640)
            // ulb has no byte enable so the 4 byte write will create 16 byte write.
            for (i=0; i<CRPE_ULB_NUM_OF_IBS; i++)
               for (j=0; j<2; j++)
               {
                   //addr = (CRPEULB_INPUT_BUFFER0_BASE_ADDR + (i<<CRPE_ULB_IB_BASE_ADDR_JUMP_LOG) + (j*CRPE_ULB_IB_OVS1_OFST) + CRPE_ULB_IB_ECC_INIT_ADDR) ;
                   addr = (uint32_t)(((uint8_t *)maple_param->mbus_base)+0x1d000+(i<<13)+j*0x500+0x4a0);
                   MAPLEWRITE32(addr,0);
                   MAPLEWRITE32((addr+0x10),0);
                   MAPLEWRITE32((addr+0x20),0);
                }
            MAPLEWRITE32(&pstCRPEULB->uliCRUBGCR , 0);
        }
        MAPLEWRITE32(&pstCRPEULB->uliCRUBMCCR , 1); // start mem init of ULB memory, wait for completion is done at the end of the routine
         // wait for ulb mem initialization completion
        do
        {
            MAPLEREAD_INTERNAL(&pstCRPEULB->uliCRUBMCSR,&data);
        }
        while (data==0);
        // set ulb configuration register
        //if (*(pstPRAM->uliMMC0P & 0x10)==0*/)
        //   data |= (1<<23); //ecc disable
        MAPLEWRITE32(&pstCRPEULB->uliCRUBGCR , ulbmode);
     }
     // DL:
     dlmode= maple_param->crpedl_mode & 0x0003f8007;
     MAPLEWRITE32(&pstPRAM->uliMCDOMCP, dlmode);
     if ((dlmode&0x8000)==0) // disable DL bit
     {
        pstCRPEDLParam = (t_CRPEDLParam*)(((uint8_t *)maple_param->mbus_base)+MAPLE_CRPEDLPARAMETER_BASE_ADDR);
        for (i = 0; i<CRPE_CRDL_SLOT_CHANNELS_NUM; i++)
        {
             MAPLEWRITE32(&pstCRPEDLParam->astMCDLSC[i].uliMCDSCP0,0);
        }
        for (i = 0; i<CRPE_CRDL_SUB_SLOT_CHANNELS_NUM; i++)
        {
             MAPLEWRITE32(&pstCRPEDLParam->astMCDLFC[i].uliMCDSCP0,0);
        }
        for(i = 0;i< CRPE_CRDL_MAX_ANTENNA;i++)
        {
                MAPLEWRITE32(&pstCRPEDLParam->astMCDLOB[i].uliMCDOBWP ,0);
        }
 
        pstCRPEDL = (t_CRPEDL *) (((uint8_t *)maple_param->mbus_base)+CRPEDL_REGISTER_BASE_ADDR);
        MAPLEWRITE32(&pstCRPEDL->uliCRDOMCR ,  (dlmode | 0x1000)); // start mem init for DL wait for completion is done at the end of the routine
        do
        {
            MAPLEREAD_INTERNAL(&pstCRPEDL->uliCRDESR,&data);
        }
        while((data&0x1000)==0);
        // unmask obs,frame,slot interrupts
        data = 0x010c;
        if ((dlmode&0x10)==0)
            data |= 0x2; // sbs interrups mask  enabled when not in cdma2k mode
        if (dlmode &1)
            data |=1;//chunk interrupts in maple output mode
        MAPLEWRITE32(&pstCRPEDL->uliCRDEMCR , data);
     }
    pstCRPEULFParam = (t_CRPEULFParam*)(((uint8_t *)maple_param->mbus_base) + MAPLE_CRPEULFPARAMETER_BASE_ADDR);
    for(i = 0;i < CRPE_CRULF_NUM_OF_IB_GROUP;i++)
    {
       MAPLEWRITE32(&pstCRPEULFParam->auliMCUFCIBRP[i] , 0);
       MAPLEWRITE32(&pstCRPEULFParam->auliMCUFCIBWP[i] , 0);
    }
    for (i = 0; i < CRPE_CRULF_NUM_OF_IB_GROUP; i++)
         MAPLEWRITE32(&pstCRPEULFParam->auliMCUFIBGA[i] , 0);
 
	
    pstCRPEULF = (t_CRPEULF *) (((uint8_t *)maple_param->mbus_base)+CRPEULF_REGISTER_BASE_ADDR);
	MAPLEWRITE32((((unsigned long int)pstCRPEULF)+0x4) , 0xe);
 
 }


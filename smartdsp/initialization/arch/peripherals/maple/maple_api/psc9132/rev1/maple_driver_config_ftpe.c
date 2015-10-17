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
   $RCSfile: maple_driver_config_ftpe.c,v $
   Tag $Name:  $
   $Revision: 1.17 $
   $Date: 2014/11/26 15:30:55 $
   Target Processor: MAPLE RISC
   Target Device: PSC9132
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_driver_config_ftpe.c
 @brief  MAPLE Driver Config FTPE routine file
*/
//----------------------------------------------------------------------------
#include <maple_map.h>
#include <maple_api.h>
#include <stdint.h>
#ifdef _ENTERPRISE_C_
#pragma opt_level = "O0"
#endif
void maple_driver_config_ftpe(void *input);
void maple_driver_config_ftpe(void *input)
{
    uint32_t                 i;
    uint32_t                 data;
    uint32_t                *puliFTPESizeToTLID;
    t_FTPEContext           *pstFTContext;
    t_FTPE                  *pstFTPE;
    maple_dev_init_params_t *maple_param;
 
    maple_param = (maple_dev_init_params_t *)input;
    pstFTPE = (t_FTPE *)(((uint8_t *)maple_param->mbus_base) +\
                         FTPE_REGISTER_BASE_ADDR);
    // Set IDB/ODB interrupt type to level
    MAPLEWRITE32(&pstFTPE->uliITR1,0x11);
    MAPLEWRITE32(&pstFTPE->uliIER,0x11);
    if (1)//(maple_param->ecc_protect>0)
    {
        MAPLEWRITE32(&pstFTPE->uliEER,0x1);
    }
    // Init FTPE Index to TL and Offset
    pstFTContext = (t_FTPEContext *)(((uint8_t *)maple_param->mbus_base) +\
                                       FTPE_CONTEXT_BASE_ADDR);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[0],0x000c0018);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[2],0x00240030);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[4],0x003c0048);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[6],0x0060006c);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[8],0x00780090);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[10],0x00b400c0);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[12],0x00d800f0);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[14],0x0120012c);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[16],0x01440168);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[18],0x018001b0);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[20],0x01e0021c);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[22],0x02400258);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[24],0x028802d0);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[26],0x03000360);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[28],0x038403c0);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[30],0x03cc0438);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[32],0x048004b0);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[34],0x06000080);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[36],0x01000200);
    MAPLEWRITE32(&pstFTContext->ausiIndexToTL[38],0x04000800);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[0],0x06090c0e);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[4],0x0f111415);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[8],0x16191a1b);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[12],0x1c1d2021);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[16],0x22232425);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[20],0x2628292a);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[24],0x2b2c2d2e);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[28],0x2f303133);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[32],0x34353617);
    MAPLEWRITE32(&pstFTContext->aucIndexToOffset[36],0x1f273237);
    for(i = 0;i < 64;i = i + 4)
    {
       MAPLEWRITE32(&pstFTContext->aucIndexToDS[i],0);
    }
 
    /* Init FTPE size to TLID conversion */
    puliFTPESizeToTLID = (uint32_t *)(((uint8_t *)maple_param->mbus_base) +\
                                                MAPLE_FTPE_SIZE_TO_TLID_ADDR);
 
    MAPLEWRITE32(&puliFTPESizeToTLID[0],0x00000001);
    MAPLEWRITE32(&puliFTPESizeToTLID[1],0x02000304);
    MAPLEWRITE32(&puliFTPESizeToTLID[2],0x00050000);
    MAPLEWRITE32(&puliFTPESizeToTLID[3],0x06070008);
    MAPLEWRITE32(&puliFTPESizeToTLID[4],0x00000900);
    MAPLEWRITE32(&puliFTPESizeToTLID[5],0x00000a00);
    MAPLEWRITE32(&puliFTPESizeToTLID[6],0x0b00000c);
    MAPLEWRITE32(&puliFTPESizeToTLID[7],0x00000d00);
    MAPLEWRITE32(&puliFTPESizeToTLID[8],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[9],0x0e0f0000);
    MAPLEWRITE32(&puliFTPESizeToTLID[10],0x10000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[11],0x00110000);
    MAPLEWRITE32(&puliFTPESizeToTLID[12],0x12000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[13],0x00001300);
    MAPLEWRITE32(&puliFTPESizeToTLID[14],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[15],0x14000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[16],0x00000015);
    MAPLEWRITE32(&puliFTPESizeToTLID[17],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[18],0x16000017);
    MAPLEWRITE32(&puliFTPESizeToTLID[19],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[20],0x00180000);
    MAPLEWRITE32(&puliFTPESizeToTLID[21],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[22],0x00001900);
    MAPLEWRITE32(&puliFTPESizeToTLID[23],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[24],0x1a000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[25],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[26],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[27],0x1b000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[28],0x1c000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[29],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[30],0x1d1e0000);
    MAPLEWRITE32(&puliFTPESizeToTLID[31],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[32],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[33],0x0000001f);
    MAPLEWRITE32(&puliFTPESizeToTLID[34],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[35],0x00000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[36],0x20000000);
    MAPLEWRITE32(&puliFTPESizeToTLID[37],0x00002100);
}


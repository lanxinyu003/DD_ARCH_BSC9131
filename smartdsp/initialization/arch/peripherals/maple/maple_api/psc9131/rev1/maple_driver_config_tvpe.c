/******************************************************************************
 Copyright � 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
   Copyright � 1995-2003,2004,2005-2010 Freescale Semiconductor Inc.
   All Rights Reserved
   this is proprietary source code of Freescale Semiconductor Inc., and its use
   is subject to the CodeWarrior EULA. The copyright notice above does not
   evidence any actual or intended publication of such source code.
   ---------------------------------------------------------------------------
   $RCSfile: maple_driver_config_tvpe.c,v $
   Tag $Name:  $
   $Revision: 1.25 $
   $Date: 2014/05/28 07:20:11 $
   Target Processor: MAPLE RISC
   Target Device: PSC9131
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_driver_config_tvpe.c
 @brief  MAPLE Driver Config TVPE routine file
*/
//----------------------------------------------------------------------------
#include <maple_map.h>
#include <maple_api.h>
#include <stdint.h>
#ifdef _ENTERPRISE_C_
#pragma opt_level = "O0"
#endif
void maple_driver_config_tvpe(void *input);
void maple_driver_config_tvpe(void *input)
{
    t_TVPE                  *pstTVPE;
    maple_dev_init_params_t *maple_param;
    uint32_t                 data;
 
    maple_param = (maple_dev_init_params_t *)input;
    pstTVPE = (t_TVPE *)(((uint8_t *)maple_param->mbus_base) + TVPE_REGISTER_BASE_ADDR);
    if (1)//(maple_param->ecc_protect >0)
    {
       MAPLEWRITE32(&pstTVPE->uliTVPEC1R,(maple_param->tvpe_mode|0x020a1008));
    }
    else
    {
       MAPLEWRITE32(&pstTVPE->uliTVPEC1R,(maple_param->tvpe_mode|0x060a1008));
    }
    MAPLEWRITE32(&pstTVPE->uliTVCINTDCR,0x1);
    MAPLEWRITE32(&pstTVPE->uliTVPEEMCR,0x11);
    if(maple_param->tvpe_mode&0x100000)
    {
      MAPLEWRITE32(&pstTVPE->uliTVHEMCR,0x22);
    }
}
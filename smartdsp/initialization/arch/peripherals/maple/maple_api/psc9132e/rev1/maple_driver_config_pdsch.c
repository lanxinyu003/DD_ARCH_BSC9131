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
   $RCSfile: maple_driver_config_pdsch.c,v $
   Tag $Name:  $
   $Revision: 1.6 $
   $Date: 2014/11/26 15:30:56 $
   Target Processor: MAPLE RISC
   Target Device: PSC9132
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_driver_config_pdsch.c
 @brief  MAPLE Driver Config PDSCH EDF routine file
*/
//----------------------------------------------------------------------------
#include <maple_map.h>
#include <maple_api.h>
#include <stdint.h>
#ifdef _ENTERPRISE_C_
#pragma opt_level = "O0"
#endif
void maple_driver_config_pdsch(void *input);
void maple_driver_config_pdsch(void *input)
{
    maple_dev_init_params_t *maple_param;
    uint32_t                 data;
 
    maple_param = (maple_dev_init_params_t *)input;
 
    MAPLEWRITE32((((uint8_t *)maple_param->mbus_base) + PDPE_CONFIGURATION_BASE_ADDR + 0xc),\
                 (0x26252020 | ((maple_param->pdsch_mode & 0x00030000)>>16) | ((maple_param->pdsch_mode & 0x00300000)>>12)));
 
    MAPLEWRITE32((((uint8_t *)maple_param->mbus_base) + PDPE_PGCR),(maple_param->pdsch_mode & 0x00000107));
}

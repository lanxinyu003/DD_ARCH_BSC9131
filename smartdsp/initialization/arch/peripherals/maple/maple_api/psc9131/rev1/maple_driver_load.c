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
   $RCSfile: maple_driver_load.c,v $
   Tag $Name:  $
   $Revision: 1.27 $
   $Date: 2014/05/28 07:20:11 $
   Target Processor: MAPLE RISC
   Target Device: PSC9131
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_driver_load.c
 @brief  MAPLE Driver Load routine file
*/
//----------------------------------------------------------------------------
#include <maple_map.h>
#include <maple_api.h>
#include <stdint.h>
#ifdef _ENTERPRISE_C_
#pragma opt_level = "O0"
#endif
void maple_driver_load(maple_dev_init_params_t *maple_param);
void maple_driver_load(maple_dev_init_params_t *maple_param)
{
   uint32_t  i;
   uint32_t  data;
   uint32_t  segment_index;
   uint32_t  segment_addr;
   uint32_t  segment_size;
   uint32_t  segment_num;
   uint32_t *maple_iadd_register;
   uint32_t *maple_idata_register;
   uint32_t *data_addr;
   uint32_t *code_data_base_addr;
 
   code_data_base_addr = ((uint32_t *)maple_param->ucode) + 1;
   // CODE Loading
   maple_iadd_register = (uint32_t *)\
            (((uint8_t *)maple_param->sbus_base) + MAPLE_IADD);
   maple_idata_register = (uint32_t *)\
            (((uint8_t *)maple_param->sbus_base) + MAPLE_IDATA);
   segment_num = *(code_data_base_addr++);
   data_addr = ((uint32_t *)maple_param->ucode) + ((segment_num * 2) + 2);
   for(segment_index = 0;segment_index < segment_num;segment_index++)
   {
      segment_addr = (*(code_data_base_addr++)) << 2;
      segment_size = *(code_data_base_addr++);
      MAPLEWRITE32(maple_iadd_register,(segment_addr | 0x80000000));
      for(i = 0;i < segment_size;i++)
      {
         MAPLEWRITE32(maple_idata_register,*(data_addr++));
      }
   }
}

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: starcore_utils.c,v 1.11 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/common/starcore_utils.c,v $
 $Revision: 1.11 $
******************************************************************************/

/******************************************************************************
 
 @File          starcore_utils.c

 @Description   StarCore-specific utility functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "starcore_utils_.h"


/*****************************************************************************/

asm int get_num_of_first_one_bit(uint32_t value)
{
asm_header
    .arg
    _value in $d0;
    return in $d0;
    .reg $d0;
asm_body
    clb d0,d0
    add #30,d0
asm_end
}

#if defined(SC3X00) || defined(SC3X50)
asm uint32_t reverse_bits(uint32_t value)
{
asm_header
    .arg
    _value in $d0;
    return in $d0;
    .reg $d0;
asm_body
    brev d0,d0
asm_end
}
#else
uint32_t reverse_bits(uint32_t value)
{
    uint32_t ret = 0, mask=0x80000000;
    int i;
 
    for (i=0 ; i<32, i++, mask>>=1)
    {
        ret |= (value & mask) ? 1 : 0;
    }
    return ret;
}
#endif



/*****************************************************************************/

asm int readEMR()
{
asm_header
    return in $d0;
    .reg $d0;
asm_body
    nop
    move.l emr,d0
asm_end
}




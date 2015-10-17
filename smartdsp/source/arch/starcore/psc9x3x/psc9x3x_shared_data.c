/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/07/15 16:46:58 $
 $Id: psc9x3x_shared_data.c,v 1.6 2012/07/15 16:46:58 b05127 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_shared_data.c,v $
 $Revision: 1.6 $
******************************************************************************/

/**************************************************************************//**
 
 @File          psc9x3x_shared_data.c

 @Description   psc9x3x structures placed at shared section

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"

#pragma data_seg_name ".os_shared_data"
#pragma bss_seg_name  ".os_shared_data_bss"

#define  _shared_data
#include "os_hw_watchdogs_.h"
#include "sc3x00_messages_shared_.h"
#include "psc9x3x_shared_.h"
#include "hw_timers_shared_.h"
#undef   _shared_data

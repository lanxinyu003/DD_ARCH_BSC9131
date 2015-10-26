/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/09/20 06:50:52 $
 $Id: smartdsp_os_.h,v 1.15 2012/09/20 06:50:52 b08551 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/smartdsp_os_.h,v $
 $Revision: 1.15 $
******************************************************************************/

/******************************************************************************
 
 @File          smartdsp_os_.h

 @Description   SmartDSP OS internal include file.

*//***************************************************************************/

#ifndef __SMARTDSP_OS_H_
#define __SMARTDSP_OS_H_

#include "os_cpp.h"

#include "os_configdebug_.h"
#include "os_configstat_.h"
#include "os_utils_.h"
#include "smartdsp_os.h"
#include "smartdsp_init.h"

typedef enum tasktype
{
    PHOENIX_TSK_PRITASK,
    PHOENIX_TSK_INTTASK
}PHOENIX_TASKTYPE;

typedef void (* PHOENIX_TSKFUNC)(void);

typedef uint8_t   PHOENIX_TASKPRI;
typedef uint16_t  PHOENIX_TASKID;
typedef uint16_t  PHOENIX_STKSIZE;

typedef enum PHOENIX_ITC_INTVECTOR
{/* This type will be redefined in kernel. But for using easily, maybe we can locate them here also. */
    NULLVECTOR = -1,
}PHOENIX_INTVECTOR;

#endif // __SMARTDSP_OS_H_


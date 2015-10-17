/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/12/02 07:18:48 $
 $Id: os_version_.h,v 1.81 2014/12/02 07:18:48 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_version_.h,v $
 $Revision: 1.81 $
******************************************************************************/

/******************************************************************************

 @File          os_version_.h

 @Description   OS Version information.

 @Cautions      None.

*//***************************************************************************/

#ifndef __OS_VERSION_H_
#define __OS_VERSION_H_


/******************************************************************************/
/* OS RELEASE INFORMATION                                                      */
/******************************************************************************/

#define OS_VERSION_MAJOR    4
#define OS_VERSION_MINOR    11
#define OS_VERSION_CUSTOM   0
#define OS_VERSION_PATCH    0

#define MAJOR               OS_VERSION_MAJOR
#define MINOR               OS_VERSION_MINOR
#define CUSTOM              OS_VERSION_CUSTOM
#define PATCH               OS_VERSION_PATCH

/******************************************************************************/
/* BUILD DATE INFORMATION                                                     */
/******************************************************************************/
#define OS_BUILD_DATE   __DATE__
#define OS_BUILD_TIME   __TIME__


#endif // __OS_VERSION_H_



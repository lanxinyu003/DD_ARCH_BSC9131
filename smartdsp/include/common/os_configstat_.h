/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2010/03/21 12:17:30 $
 $Id: os_configstat_.h,v 1.6 2010/03/21 12:17:30 b06368 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_configstat_.h,v $
 $Revision: 1.6 $
******************************************************************************/

/******************************************************************************
 
 @File          os_configstat_.h

 @Description   OS Debug configuration header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_CONFIGSTAT_H_
#define __OS_CONFIGSTAT_H_


/* ***************************************************************************** */
/* BELOW ARE ALL THE COMPILE OPTIONS FOR THE OPERATING SYSTEM'S STATISTICS */
/* ***************************************************************************** */


/*****************************************************************************/
/* THESE ARE GENERAL DEFINES THAT ENABLE OR DISABLE ALL ERROR CHECKING AND   */
/* ASSERTS ON ERRORS                                                         */
/*****************************************************************************/
//#define DISABLE_ALL_ERROR_CHECKING
//#define DISABLE_ALL_ASSERTS
//#define DISABLE_ALL_DYNAMIC_CONTROL


/*****************************************************************************/
/* BELOW EACH OBJECT CAN BE CONFIGURED FOR ITS LEVEL OF ERROR CHECKING       */
/*****************************************************************************/

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifdef DISABLE_ALL_STATISTICS
#define BIO_STATISTICS OFF
#define COP_STATISTICS OFF
#else
#define BIO_STATISTICS ON
#define COP_STATISTICS ON
#endif


#endif // __OS_CONFIGSTAT_H_

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_cpp.h,v 1.9 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_cpp.h,v $
 $Revision: 1.9 $
******************************************************************************/


#ifndef __OS_CPP_H
#define __OS_CPP_H

#ifdef __cplusplus

    #define _OS_BEGIN_EXTERN_C extern "C" {
    #define _OS_END_EXTERN_C }
 
    #define _OS_BEGIN_NAMESPACE_STD namespace std {
    #define _OS_END_NAMESPACE_STD }
 
    #define __std(ref) ::std::ref
    #define __global() ::
 
#else
    #define _OS_BEGIN_EXTERN_C
    #define _OS_END_EXTERN_C
 
    #define _OS_BEGIN_NAMESPACE_STD
    #define _OS_END_NAMESPACE_STD
 
    #define __std(ref) ref
    #define __global()
#endif

#endif /* ndef __OS_CPP_H */

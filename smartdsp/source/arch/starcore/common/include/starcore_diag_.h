/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2010/03/21 12:20:17 $
 $Id: starcore_diag_.h,v 1.6 2010/03/21 12:20:17 b06368 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/common/include/starcore_diag_.h,v $
 $Revision: 1.6 $
******************************************************************************/

/******************************************************************************
 
 @File          starcore_diag_.h

 @Description   StarCore-specific Hardware Interrupts header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __STARCORE_DIAG_H_
#define __STARCORE_DIAG_H_


#include <stdio.h>
void __crt0_end(int);

#define OS_DIAGNOSTICS_PASS  0
#define OS_DIAGNOSTICS_FAIL    1

#define OS_DIAGNOSTICS_TEST_PASS  OS_END_DIAGNOSTICS_TEST(OS_DIAGNOSTICS_PASS)
#define OS_DIAGNOSTICS_TEST_FAIL    OS_END_DIAGNOSTICS_TEST(OS_DIAGNOSTICS_FAIL)

static inline void OS_END_DIAGNOSTICS_TEST(int status)
{
    if (status == OS_DIAGNOSTICS_PASS)
        printf("DIAGNOSTICS TEST PASSED\n");
    else
        printf("DIAGNOSTICS TEST FAILED\n");
    __crt0_end(status);
}
#define OS_DIAGNOSTICS_ASSERT_COND(expression)  { if (!(expression)) OS_END_DIAGNOSTICS_TEST(OS_DIAGNOSTICS_FAIL); }


/*Override default SmartDSP OS macros */
#ifdef OS_ASSERT_COND
    #undef OS_ASSERT_COND
    #define OS_ASSERT_COND(expression)  OS_DIAGNOSTICS_ASSERT_COND(expression)
#endif
#ifdef OS_ASSERT
    #undef OS_ASSERT
    #define OS_ASSERT  OS_DIAGNOSTICS_TEST_FAIL
#endif
#ifdef OS_TEST_PASSED
    #undef OS_TEST_PASSED
    #define OS_TEST_PASSED  OS_DIAGNOSTICS_TEST_PASS
#endif
 

#endif // __STARCORE_DIAG_H_



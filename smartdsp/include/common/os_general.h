/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/09/30 13:35:17 $
 $Id: os_general.h,v 1.44 2014/09/30 13:35:17 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_general.h,v $
 $Revision: 1.44 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_general.h

 @Description   SmartDSP OS General System API.

*//***************************************************************************/

#ifndef __OS_GENERAL_H
#define __OS_GENERAL_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C


extern uint8_t g_dev_id;

#include <stdio.h>

#if defined(B4860) || defined(B4420)
#define OS_MPIC_CORES_ID_OFFSET  8
/**< The DSP cores ID have an offset of 8 for MPIC use */
#elif defined(B3421)
#define OS_MPIC_CORES_ID_OFFSET  8
#endif // B4860 || B4420


/**************************************************************************//**
 @Group         general_id General System Functions

 @Description   Kernel initialization and general system functions.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   OS Version Information Structure.
 
*//***************************************************************************/
typedef struct
{
    uint16_t  major;    /**< Major version number */
    uint16_t  minor;    /**< Minor version number */
    uint16_t  custom;   /**< Custom build number */
    uint16_t  patch;    /**< Patch number */
 
} os_version_t;

/**************************************************************************//**
 @Description   Core Identity Information Structure.
 
*//***************************************************************************/
typedef struct
{
    uint8_t  soc_core_num;          /**< Core number within SoC - hardware defined */
    uint8_t  cluster_core_num;      /**< Core number within cluster - hardware defined */
    uint8_t  cluster_num;           /**< Cluster number within SoC - hardware defined */
    uint8_t  soc_master_core;       /**< SoC master core number - software defined */
    uint8_t  cluster_master_core;   /**< Cluster master core number - software defined */
} os_core_info_t;

extern os_core_info_t     g_core_info;
/**< Information structure including information regarding *this* core;
      Users MAY NOT access this variable directly!!! */

/**************************************************************************//**
 @Function      osInitialize

 @Description   Initializes the OS.

                This function initializes the various modules of the OS,
                using the parameters taken from the configuration file.
 
 @Retval        OS_SUCCESS - kernel and drivers initialized properly.

 @Cautions      Must be called before osStart().
 
*//***************************************************************************/
os_status   osInitialize();


/**************************************************************************//**
 @Function      osStart

 @Description   Starts the OS operation.

                This function receives a pointer to a background task and
                switches to that task. If the background task runs infinitely
                (as it should), this function never returns.
 
                This function also activates the system tick, if it is required.
 
 @Param[in]     background_task -  The background task to run.

 @Return        Error status, encoded in os_error.h, for any reason; as this
                function does not, normally, return

 @Cautions      osInitialize() must be called before osStart().
 
                Do not use local variables here, because this function does
                not return, and its stack will not be cleared.
 
*//***************************************************************************/
os_status   osStart(os_background_task_function background_task);


/**************************************************************************//**
 @Function      osGetVersionInfo

 @Description   Returns the OS version information in the structure passed in.
 
 @Param[out]    version_info -  Pointer to a version information structure.

*//***************************************************************************/
void        osGetVersionInfo(os_version_t *version_info);


/**************************************************************************//**
 @Function      osErrorDebug

 @Description   Breaks execution (used for debug).

*//***************************************************************************/
#ifndef HAVE_OS_ERROR_DEBUG
void        osErrorDebug();
#endif // HAVE_OS_ERROR_DEBUG


/**************************************************************************//**
 @Description   Compiler assertion, for performance.

                The support is restricted to establishing range (minimum, maximum values) and modulo (or alignment) for user
                variables. The condition can establish order relations (>,>=,<,<=,==) between C variables and
                constants, or modulo properties (var&0x01..1 == 0 OR var%const == 0). If
*//***************************************************************************/
#ifndef OS_COMPILER_ASSERT
#ifdef _ENTERPRISE_C_   /* Freescale StarCore C compiler */
#define OS_COMPILER_ASSERT(expression)  cw_assert(expression)
#else
#define OS_COMPILER_ASSERT(expression)
#endif  // _ENTERPRISE_C_
#endif  // OS_COMPILER_ASSERT

#ifndef DISABLE_ALL_ASSERTS
/**************************************************************************//**
 @Description   Assertion macro, for debugging - calls osErrorDebug()

*//***************************************************************************/
#ifndef OS_ASSERT
#define OS_ASSERT                   osErrorDebug()
#endif // OS_ASSERT



/**************************************************************************//**
 @Description   Assertion if condition is false, for debugging -
                calls osErrorDebug()

*//***************************************************************************/
#ifndef OS_ASSERT_COND
#define OS_ASSERT_COND(expression)  { if (!(expression)) osErrorDebug(); }
#endif // OS_ASSERT_COND
#else  // DISABLE_ALL_ASSERTS
#define OS_ASSERT
#define OS_ASSERT_COND(expression)
#endif // DISABLE_ALL_ASSERTS

/**************************************************************************//**
 @Description   Compiler and conditional assertions, for performance and debugging

                The support is restricted to establishing range (minimum, maximum values) and modulo (or alignment) for user
                variables. The condition can establish order relations (>,>=,<,<=,==) between C variables and
                constants, or modulo properties (var&0x01..1 == 0 OR var%const == 0). If
*//***************************************************************************/
#define OS_ASSERT_COMPILER_COND(expression)\
    OS_ASSERT_COND(expression) \
    OS_COMPILER_ASSERT(expression)


/**************************************************************************//**
 @Function      osGetSystemClock

 @Description   Gets the system clock value.
 
                The returned value is the same value that was given in the
                configuration structure when the OS was initialized.
 
 @Return        System clock (MHz)

*//***************************************************************************/
uint32_t      osGetSystemClock();


/**************************************************************************//**
 @Function      osGetDeviceID

 @Description   Gets the device ID.
 
 @Return        The device ID

*//***************************************************************************/
#define     osGetDeviceID()   (g_dev_id + 0) // The + prevents usage as lvalue

#ifdef OS_LOAD_CALCULATION
/**************************************************************************//**
 @Function      osGetCoreLoad

 @Description   Gets the load value of the calling core.
 
 @Return        The core load (percents)

*//***************************************************************************/
uint32_t      osGetCoreLoad();


/**************************************************************************//**
 @Description   Call this macro in background task to perform load calculation.

*//***************************************************************************/
#define OS_CALCULATE_LOAD() {           \
        extern int      g_run_counter;  \
        os_hwi_status   int_status;     \
        int_status = osHwiDisable();    \
        g_run_counter++;                \
        osHwiEnable(int_status); }
#else
#define OS_CALCULATE_LOAD()

#endif // OS_LOAD_CALCULATION


#if (OS_MULTICORE == 1)

/**************************************************************************//**
 @Function      osGetNumberOfCores

 @Description   Retrieves the number of cores in the system.

 @Return        The number of cores in the system

*//***************************************************************************/
uint8_t       osGetNumberOfCores();


/**************************************************************************//**
 @Function      osGetSocMasterCore

 @Description   Retrieves the ID of the SoC master core.

 @Return        The ID of the master core

*//***************************************************************************/
#define osGetSocMasterCore()        (g_core_info.soc_master_core + 0)


/**************************************************************************//**
 @Function      osGetSocCoreID

 @Description   Retrieves the ID of the core within the SoC.

 @Return        The ID of the core

*//***************************************************************************/
#define osGetSocCoreID()            (g_core_info.soc_core_num + 0)

/**************************************************************************//**
 @Function      osGetMpicCoreID

 @Description   Retrieves the ID of the core for MPIC use.

 @Return        The ID of the core

*//***************************************************************************/
#define osGetMpicCoreID()            (g_core_info.soc_core_num + OS_MPIC_CORES_ID_OFFSET)

/**************************************************************************//**
 @Function      osGetMpicCoreID

 @Description   Retrieves the ID of the master core for MPIC use.

 @Return        The ID of the master core

*//***************************************************************************/
#define osGetMpicMasterCore()            (g_core_info.soc_master_core + OS_MPIC_CORES_ID_OFFSET)

/**************************************************************************//**
 @Function      osGetClusterMasterCore

 @Description   Retrieves the ID of the core within the cluster.

 @Return        The ID of the master core

*//***************************************************************************/
#define osGetClusterMasterCore()    (g_core_info.cluster_master_core + 0)


/**************************************************************************//**
 @Function      osGetClusterCoreID

 @Description   Retrieves the ID of the core within the cluster.

 @Return        The ID of the master core

*//***************************************************************************/
#define osGetClusterCoreID()        (g_core_info.cluster_core_num + 0)


/**************************************************************************//**
 @Function      osGetClusterNum

 @Description   Retrieves the cluster number within the SoC.

 @Return        The ID of cluster

*//***************************************************************************/
#define osGetClusterNum()           (g_core_info.cluster_num + 0)


#else // OS_MULTICORE == 0

#define osGetSocMasterCore()        (0+0)    // The + prevents usage as lvalue
#define osGetSocCoreID()            (0+0)    // The + prevents usage as lvalue
#define osGetClusterMasterCore()    (0+0)    // The + prevents usage as lvalue
#define osGetClusterCoreID()        (0+0)    // The + prevents usage as lvalue
#define osGetClusterNum()           (0+0)    // The + prevents usage as lvalue
#define osGetNumberOfCores()        (1+0)    // The + prevents usage as lvalue

#endif // OS_MULTICORE

/**************************************************************************//**
 @Function      osGetMasterCore

 @Description   Retrieves the ID of the master core.

 @Return        The ID of the master core
 
 @Cautions      Backward compatible use - deprecation alert

*//***************************************************************************/
#define osGetMasterCore()           osGetSocMasterCore()

/**************************************************************************//**
 @Function      osGetCoreID

 @Description   Retrieves the ID of calling master core.

 @Return        The ID of the calling core
 
 @Cautions      Backward compatible use - deprecation alert

*//***************************************************************************/
#define     osGetCoreID()           osGetSocCoreID()


/**************************************************************************//**
 @Function      OS_WAIT

 @Description   Macro for looping waiting in loop.
 
                The macro will not get optimized by the compiler, at any level
                of optimization, and will loop by performing a simple for loop
 
 @Param         num - number of times to loop

*//***************************************************************************/
void osWait(uint32_t num) __attribute__((noinline));


#define OS_WAIT(num)  osWait((uint32_t) num);


/**************************************************************************//**
 @Function      osMulticoreSupport

 @Description   Returns the value of OS_MULTICORE
 
 @Return        1 if OS_MULTICORE == 1 or 0 otherwise

*//***************************************************************************/
int osMulticoreSupport();

/** @} */ // end of general_id

_OS_END_EXTERN_C


#endif // __OS_GENERAL_H







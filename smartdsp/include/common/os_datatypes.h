/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/07/01 04:38:21 $
 $Id: os_datatypes.h,v 1.57 2014/07/01 04:38:21 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_datatypes.h,v $
 $Revision: 1.57 $
******************************************************************************/

/**************************************************************************//**
 @File          os_datatypes.h

 @Description   OS data types and public structures.

 @Cautions      None.

*//***************************************************************************/

#ifndef __OS_DATATYPES_H
#define __OS_DATATYPES_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         types_id Data Types

 @Description   Data types used in SmartDSP OS
 
 @{
*//***************************************************************************/


#define HAVE_C99_DATATYPES
/**< Line should be uncommented if external C99 header files are included */


/* #define USING_OLD_DATATYPES */


#define INLINE                          static inline
/**< static inline function macro */

/* OS native data type mappings. This can be architecture specific. */
#if defined(SC1XX) || defined(STARCORE) || defined(SC3X00) || defined(SC3X50)
  #define OS_BYTE_TYPE                  char        /**< 1 byte object */
  #define OS_SHORT_TYPE                 short       /**< 2 byte object */
  #define OS_LONG_TYPE                  long        /**< 4 byte object */
  #define OS_LONG_LONG_TYPE             long long   /**< 8 byte object */
  #define OS_FLOAT_TYPE                 float       /**< floating point object */
  #define OS_DOUBLE_TYPE                double      /**< double floating point object */
#else
  #define OS_BYTE_TYPE                  char
  #define OS_SHORT_TYPE                 short
  #define OS_LONG_TYPE                  long
  #define OS_LONG_LONG_TYPE             long long
  #define OS_FLOAT_TYPE                 float
  #define OS_DOUBLE_TYPE                double

#endif /* SC types. */


#ifndef QE_UCODE

#ifdef HAVE_C99_DATATYPES

_OS_END_EXTERN_C
#include <stdint.h>     /* for integer types */
#include <math.h>       /* for floating-point types */
_OS_BEGIN_EXTERN_C

#else /* not HAVE_C99_DATATYPES */


/* type definitions */
typedef unsigned OS_BYTE_TYPE           uint8_t;    /* Unsigned  8-bit integer */
typedef unsigned OS_SHORT_TYPE          uint16_t;   /* Unsigned 16-bit integer */
typedef unsigned OS_LONG_TYPE           uint32_t;   /* Unsigned 32-bit integer */
typedef unsigned OS_LONG_LONG_TYPE      uint64_t;   /* Unsigned 64-bit integer */

typedef signed OS_BYTE_TYPE             int8_t;     /* Signed  8-bit integer */
typedef signed OS_SHORT_TYPE            int16_t;    /* Signed 16-bit integer */
typedef signed OS_LONG_TYPE             int32_t;    /* Signed 32-bit integer */
typedef signed OS_LONG_LONG_TYPE        int64_t;    /* Signed 64-bit integer */

typedef OS_FLOAT_TYPE                   float_t;    /* Single precision floating point */
typedef OS_DOUBLE_TYPE                  double_t;   /* Double precision floating point */

#endif /* HAVE_C99_DATATYPES */

#endif /* QE_UCODE */

/**************************************************************************//**
 @Collection    Boolean definitions
 
                If not compiling with C++ support, this is used to define
                boolean support

 @{
*//***************************************************************************/

#ifndef __cplusplus
typedef unsigned char                   bool;
#endif

#if !defined(FALSE)
#define FALSE                           (0) /**< Boolean false */
#endif
#if !defined(TRUE)
#define TRUE                            (1) /**< Boolean true */
#endif

/* @} */ /* end of Boolean definitions */

/**************************************************************************//**
 @Description   Null pointer definition
*//***************************************************************************/
#if !defined(NULL)
#ifndef __cplusplus
#define NULL                            ((void*)0)
#else
#define NULL                            0L
#endif  /* __cplusplus */
#endif  /* NULL */

/* @} */ /* end of Null pointer definitions */


#if defined(SC39XX)
typedef  void*                          os_virt_ptr;
typedef  uint64_t                       os_phys_ptr;
typedef  const uint64_t                 os_const_phys_ptr;
typedef  const void*                    os_const_virt_ptr;
#else
typedef  void*                          os_virt_ptr;
typedef  void*                          os_phys_ptr;
typedef  const void*                    os_const_phys_ptr;
typedef  const void*                    os_const_virt_ptr;
#endif // SC39XX


#define VAR_UNUSED(var)                 (void)var
/**< Macro used for stripping unused variables from the compiled code
     in a manner that removed the "unused variable" compiler warning */
#define AVOID_NOT_USED(F)               {if (F) NOP();}
/**< Macro preventing compiler stripping of variables */


typedef uint32_t                        os_status;
/**< Os status; See %os_error.h for detailed error values */

typedef void*                           arch_specific;
/**< Architecture specific implementation; generally used as a pointer in
     to architecture dependent features in an architecture independent structure */

typedef uint16_t                        os_user_id;
/**< User ID */

typedef uint16_t                        os_task_priority;
/**< Task Priority */
typedef uint32_t                        os_task_arg;
/**< Task argument - passed to the task when scheduled */
typedef uint32_t                        os_task_handle;
/**< Handle to task object */

typedef uint32_t                        os_event_handle;
/**< Handle to event object */
typedef uint32_t                        os_event_type;
/**< Event type (semaphore, queue) */

typedef uint16_t                        os_hwi_handle,
                                        os_hwi_object;      /* DEPRECATED */
/**< Handle to hardware interrupt object */


typedef uint16_t                        os_hwi_priority;
/**< Hardware interrupt priority (e.g. OS_HWI_PRIORITY0) */
typedef uint32_t                        os_hwi_status;
/**< Hardware interrupt status */
typedef uint8_t                         os_hwi_mode;
/**< Hardware interrupt mode (e.g. LEVEL_MODE) */
typedef uint32_t                        os_hwi_arg;
/**< Hardware interrupt argument - passed to the interrupt service routine (ISR) */

typedef uint16_t                        os_swi_handle,
                                        os_swi_object;      /* DEPRECATED */
/**< Handle to software interrupt object */

typedef uint16_t                        os_swi_priority;
/**< Software interrupt priority (e.g. OS_SWI_PRIORITY1) */
typedef uint16_t                        os_swi_status;
/**< Software interrupt status */
typedef uint32_t                        os_swi_count;
/**< Software interrupt counter - passed to the interrupt service routine (ISR) */

typedef void*                           os_dma_handle;
/**< Handle to DMA object */
typedef uint32_t                        os_dma_xfer;
/**< Handle to DMA transfer object */

typedef uint32_t                        os_watchdog_handle;
/**< Handle to watchdog timer object */

typedef void*                           debug_hook_arg;
/**< Debug hook (see %os_debughooks.h) function argument */

typedef os_hwi_arg                        os_mpic_arg;
/**<  Mpic ISR argumant */

#if defined(SC39XX)
typedef  struct
{
    uint16_t    disabled;
    uint16_t    pid;        /**< Program ID of task that owns the lock, valid if count = 1 */
    uint16_t    core_id;    /**< Core ID that owns the lock, valid if count = 1 */
    uint16_t    count;
} os_spinlock_handle __attribute__((aligned(4)));
#define SPINLOCK_INIT (os_spinlock_handle)  {0, 0, 0, 0}

#else   // SC39XX
typedef volatile uint32_t               os_spinlock_handle;
#define SPINLOCK_INIT (os_spinlock_handle)  0
#endif  // SC39XX
/**< Handle to spinlock (binary mutex) object */

/**************************************************************************//**
 @Description   Byte order type.

*//***************************************************************************/
typedef enum { OS_BO_LITTLE_ENDIAN = 0,
               OS_BO_BIG_ENDIAN }       os_byte_order;


typedef uint16_t                        os_timer_handle,
                                        os_timer_object;    /* DEPRECATED */
/**< Handle to timer (hardware and software) object */
#if defined(SC39XX)
typedef uint64_t                        os_timer_interval;
/**< Length for timer to count */
#else
typedef uint32_t                        os_timer_interval;
/**< Length for hardware timer to count */

#endif // SC39XX

typedef uint32_t                        os_timer_mode;
/**< Timer mode for specific timer - see @ref timer_modes "Timer Modes" */
typedef uint32_t                        os_hw_timer_source;
/**< Clock source for the timer */

/************************************************************//**
 @Anchor        timer_modes
 
 @Collection    Available modes for software and hardware timers

 @{
*//*************************************************************/
#define OS_TIMER_ONE_SHOT               1
/**< Timer runs only once until pre-defined value */
#define OS_TIMER_PERIODIC               2
/**< Timer runs until pre-defined value, then runs again */
#define OS_TIMER_FREE_RUN               3
/**< Timer runs continuously, wrapping around at hardware limits */

#if defined(SC39XX)
#define OS_TIMER_CHAINED                0x80000000
/**< Timer chained with the successive timer. This value is used as a parameter for bitwise operation */
#endif // SC39XX

/* @} */ /* end of timer_modes */

#if defined(SC39XX)
/************************************************************//**
 @Anchor        timer_sources
 
 @Collection    Available sources for hardware timers

 @{
*//*************************************************************/
#define OS_TIMER_EXT_SYNC                0x00000001
/**< External synchronization enable. This value is used as a parameter for a bitwise operation */
/* @} */ /* end of timer_sources */
#endif // SC39XX

typedef uint16_t                        os_msg_handle,
                                        os_msg_object;      /* DEPRECATED */
/**< Handle to message object */


/**************************************************************************//**
 @Description   Hardware Interrupt function prototype.

*//***************************************************************************/
//typedef void (*os_hwi_function)(os_hwi_arg);
typedef void (*os_hwi_function)(void);

/**************************************************************************//**
 @Description   Hardware Interrupt dispatcher prototype.

*//***************************************************************************/
typedef void (*os_hwi_dispatcher)();


/**************************************************************************//**
 @Description   Software Interrupt function prototype.

*//***************************************************************************/
typedef void (*os_swi_function)(os_swi_count);

/**************************************************************************//**
 @Description   Software Timer function prototype.

*//***************************************************************************/
typedef void (*os_timer_function)(void);

/**************************************************************************//**
 @Description   Message function prototype.

*//***************************************************************************/
typedef os_status (*os_msg_function)(os_hwi_arg hwi_arg,uint32_t* msg_data);

/**************************************************************************//**
 @Description   Task function prototype.

*//***************************************************************************/
typedef void (*os_task_function)(os_task_arg);
typedef void (*os_background_task_function)(void);

/**************************************************************************//**
 @Description   Debug hook function prototype.

*//***************************************************************************/
typedef void (*debug_hook_function)(debug_hook_arg arg_1, debug_hook_arg arg_2);

/**************************************************************************//**
 @Description   mpic function prototype.

*//***************************************************************************/
typedef void (*os_mpic_isr)(os_mpic_arg);

/* The definitions below are obsolete, but are still here for compatibility */
#ifdef USING_OLD_DATATYPES

/* type definitions */
typedef unsigned OS_BYTE_TYPE           uint8;  /* Unsigned  8-bit integer */
typedef unsigned OS_SHORT_TYPE          uint16; /* Unsigned 16-bit integer */
typedef unsigned OS_LONG_TYPE           uint32; /* Unsigned 32-bit integer */

typedef signed OS_BYTE_TYPE             int8;   /* Signed  8-bit integer */
typedef signed OS_SHORT_TYPE            int16;  /* Signed 16-bit integer */
typedef signed OS_LONG_TYPE             int32;  /* Signed 32-bit integer */

typedef OS_FLOAT_TYPE                   fp32;   /* Single precision floating point */
typedef OS_DOUBLE_TYPE                  fp64;   /* Double precision floating point */

#endif // USING_OLD_DATATYPES

/** @} */ // end of types_id group


_OS_END_EXTERN_C


#endif // __OS_DATATYPES_H

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/12/03 09:44:19 $
 $Id: os_accessors.h,v 1.11 2013/12/03 09:44:19 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_accessors.h,v $
 $Revision: 1.11 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_accessors.h

 @Description   Memory and register accessor functions.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_ACCESSORS_H
#define __OS_ACCESSORS_H

#include "os_arch_macros.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         accessor_id Accessor API

 @Description   Macros for accessing registers and memory.
 
                These macros are written in such a manner to make them portable
                across platforms and architectures. Any address that is passed
                should be passed as dereferenced pointer
 
 @Cautions      Although the macros for accessing memory and registers are,
                in general, implemented in the same manner; for future
                compatibility use the proper version
 
 @{
*//***************************************************************************/

#ifdef OS_DOCUMENTATION_ONLY
/**************************************************************************//**
 @Function      READ_UINT8

 @Description   Read a uint8_t register and store the value
 
                This function performs: data = (uint8_t)(arg)
 
 @Param[out]    data    -  The location to store the read data in
 @Param[in]     arg     -  The address to read from
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void READ_DATA8(uint8_t *data, uint8_t arg);

/**************************************************************************//**
 @Function      READ_UINT16

 @Description   Read a uint16_t register and store the value
 
                This function performs: data = (uint16_t)(arg)
 
 @Param[out]    data    -  The location to store the read data in
 @Param[in]     arg     -  The address to read from

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void READ_DATA16(uint16_t *data, uint16_t arg);


/**************************************************************************//**
 @Function      READ_UINT32

 @Description   Read a uint32_t register and store the value
 
                This function performs: data = (uint32_t)(arg)
 
 @Param[out]    data    -  The location to store the read data in
 @Param[in]     arg     -  The address to read from

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void READ_DATA32(uint32_t *data, uint32_t arg);


/**************************************************************************//**
 @Function      READ_UINT64

 @Description   Read a uint64_t register and store the value
 
                This function performs: data = (uint64_t)(arg)
 
 @Param[out]    data    -  The location to store the read data in
 @Param[in]     arg     -  The address to read from

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void READ_DATA64(uint64_t *data, uint64_t arg);


/**************************************************************************//**
 @Function      GET_UINT8

 @Description   Read a uint8_t register and return the value
 
                This function performs: (uint8_t)(arg)
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in register

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
uint8_t GET_DATA8(uint8_t arg);


/**************************************************************************//**
 @Function      GET_UINT16

 @Description   Read a uint16_t register and return the value
 
                This function performs: (uint16_t)(arg)
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in register

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
uint16_t GET_DATA16(uint13_t arg);


/**************************************************************************//**
 @Function      GET_UINT32

 @Description   Read a uint32_t register and return the value
 
                This function performs: (uint32_t)(arg)
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in register

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
uint32_t GET_DATA32(uint32_t arg);


/**************************************************************************//**
 @Function      GET_UINT64

 @Description   Read a uint54_t register and return the value
 
                This function performs: (uint64_t)(arg)
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in register

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
uint64_t GET_DATA64(uint64_t arg);


/**************************************************************************//**
 @Function      WRITE_UINT8

 @Description   Write a uint8_t register
 
                This function performs: arg = (uint8_t)(data)
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_UINT8(uint8_t arg, uint8_t data);


/**************************************************************************//**
 @Function      WRITE_UINT16

 @Description   Write a uint16_t register
 
                This function performs: arg = (uint16_t)(data)
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_UINT16(uint16_t arg, uint16_t data);


/**************************************************************************//**
 @Function      WRITE_UINT32

 @Description   Write a uint32_t register
 
                This function performs: arg = (uint32_t)(data)
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_UINT32(uint32_t arg, uint32_t data);


/**************************************************************************//**
 @Function      WRITE_UINT64

 @Description   Write a uint64_t register
 
                This function performs: arg = (uint64_t)(data)
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_UINT64(uint64_t arg, uint64_t data);


/**************************************************************************//**
 @Function      READ_MEM_UINT8

 @Description   Read a uint8_t memory location and store the value
 
                This function performs: data = (uint8_t)(arg)
 
 @Param[out]    data    -  The location to store the read data in
 @Param[in]     arg     -  The address to read from

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void READ_MEM_UINT8(uint8_t *data, uint8_t arg);


/**************************************************************************//**
 @Function      READ_MEM_UINT16

 @Description   Read a uint16_t memory location and store the value
 
                This function performs: data = (uint16_t)(arg)
 
 @Param[out]    data    -  The location to store the read data in
 @Param[in]     arg     -  The address to read from

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void READ_MEM_UINT16(uint16_t *data, uint16_t arg);


/**************************************************************************//**
 @Function      READ_MEM_UINT32

 @Description   Read a uint32_t memory location and store the value
 
                This function performs: data = (uint32_t)(arg)
 
 @Param[out]    data    -  The location to store the read data in
 @Param[in]     arg     -  The address to read from

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void READ_MEM_UINT32(uint32_t *data, uint32_t arg);


/**************************************************************************//**
 @Function      GET_MEM_UINT8

 @Description   Read a uint8_t memory location and return the value
 
                This function performs: (uint8_t)(arg)
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in memory
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
uint8_t GET_MEM_UINT8(uint8_t arg);


/**************************************************************************//**
 @Function      GET_MEM_UINT16

 @Description   Read a uint16_t memory location and return the value
 
                This function performs: (uint16_t)(arg)
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in memory
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
uint16_t GET_MEM_UINT16(uint16_t arg);


/**************************************************************************//**
 @Function      GET_MEM_UINT32

 @Description   Read a uint32_t memory location and return the value
 
                This function performs: (uint32_t)(arg)
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in memory
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
uint32_t GET_MEM_UINT32(uint32_t arg);


/**************************************************************************//**
 @Function      WRITE_MEM_UINT8

 @Description   Write a uint8_t memory location
 
                This function performs: arg = (uint8_t)(data)
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_MEM_UINT8(uint8_t arg, uint8_t data);


/**************************************************************************//**
 @Function      WRITE_MEM_UINT816

 @Description   Write a uint16_t memory location
 
                This function performs: arg = (uint16_t)(data)
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_MEM_UINT16(uint16_t arg, uint16_t data);


/**************************************************************************//**
 @Function      WRITE_MEM_UINT32

 @Description   Write a uint32_t memory location
 
                This function performs: arg = (uint32_t)(data)
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_MEM_UINT32(uint32_t arg, uint32_t data);


/**************************************************************************//**
 @Function      CLEAR_UINT8

 @Description   Clear a bitmask on a uint8_t register
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to clear - set 1 in each bit location that
                           should be cleared
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void CLEAR_UINT8(uint8_t addr, uint8_t bitmask);


/**************************************************************************//**
 @Function      SET_UINT8

 @Description   SET a bitmask on a uint8_t register
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to set - set 1 in each bit location that
                           should be set
 
 @Cautions      The function signature is for documentation purposes only
 
*//***************************************************************************/
void SET_UINT8(uint8_t addr, uint8_t bitmask);


/**************************************************************************//**
 @Function      CLEAR_UINT16

 @Description   Clear a bitmask on a uint16_t register
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to clear - set 1 in each bit location that
                           should be cleared
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void CLEAR_UINT16(uint16_t addr, uint16_t bitmask);


/**************************************************************************//**
 @Function      SET_UINT16

 @Description   SET a bitmask on a uint16_t register
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to set - set 1 in each bit location that
                           should be set
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void SET_UINT8(uint16_t addr, uint16_t bitmask);


/**************************************************************************//**
 @Function      CLEAR_UINT32

 @Description   Clear a bitmask on a uint32_t register
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to clear - set 1 in each bit location that
                           should be cleared
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void CLEAR_UINT32(uint32_t addr, uint32_t bitmask);


/**************************************************************************//**
 @Function      SET_UINT32

 @Description   SET a bitmask on a uint32_t register
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to set - set 1 in each bit location that
                           should be set
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void SET_UINT8(uint32_t addr, uint32_t bitmask);



/** @} */ // end of accessor_id group

#else // OS_DOCUMENTATION_ONLY


#include "os_datatypes.h"

#ifdef VERBOSE
#include <stdio.h>
#include "smartdsp_os.h"
extern FILE *log_file;

 
INLINE uint8_t get_uint8_verbose(volatile uint8_t *arg)
{
    fprintf(log_file, "CoreID %d GET_UINT8 0x%02x FROM %08p\n", osGetCoreID(), *arg, (uint32_t)(arg));
    fflush(log_file);
    return *arg;
}

INLINE uint16_t get_uint16_verbose(volatile uint16_t *arg)
{
    fprintf(log_file, "CoreID %d GET_UINT16 0x%04x FROM %08p\n", osGetCoreID(), *arg, (uint32_t)(arg));
    fflush(log_file);
    return *arg;
}

INLINE uint32_t get_uint32_verbose(volatile uint32_t *arg)
{
    fprintf(log_file, "CoreID %d GET_UINT32 0x%08x FROM %08p\n", osGetCoreID(), *arg, (uint32_t)(arg));
    fflush(log_file);
    return *arg;
}

#define READ_UINT8(data, arg)       data = get_uint8_verbose((volatile uint8_t *)&(arg))
#define READ_UINT16(data, arg)      data = get_uint16_verbose((volatile uint16_t *)&(arg))
#define READ_UINT32(data, arg)      data = get_uint32_verbose((volatile uint32_t *)&(arg))
#define GET_UINT8(arg)              get_uint8_verbose((volatile uint8_t *)&(arg))
#define GET_UINT16(arg)             get_uint16_verbose((volatile uint16_t *)&(arg))
#define GET_UINT32(arg)             get_uint32_verbose((volatile uint32_t *)&(arg))
#define WRITE_UINT8(arg, data)      { arg = (uint8_t)(data);    \
            fprintf(log_file, "CoreID %d WRITE_UINT8: %08p = 0x%02X\n", osGetCoreID(), &(arg), (data)); \
            fflush(log_file);}
#define WRITE_UINT16(arg, data)     { arg = (uint16_t)(data);   \
            fprintf(log_file, "CoreID %d WRITE_UINT16: %08p = 0x%04X\n", osGetCoreID(), &(arg), (data)); \
            fflush(log_file);}
#define WRITE_UINT32(arg, data)     { arg = (uint32_t)(data);   \
            fprintf(log_file, "CoreID %d WRITE_UINT32: %08p = 0x%08X\n", osGetCoreID(), &(arg), (data)); \
            fflush(log_file);}


#else // not VERBOSE

#define READ_UINT8(data, arg)       data = (uint8_t)(arg)
#define READ_UINT16(data, arg)      data = (uint16_t)(arg)
#define READ_UINT32(data, arg)      data = (uint32_t)(arg)
#define READ_UINT64(data, arg)      data = (uint64_t)(arg)
#define GET_UINT8(arg)              (uint8_t)(arg)
#define GET_UINT16(arg)             (uint16_t)(arg)
#define GET_UINT32(arg)             (uint32_t)(arg)
#define GET_UINT64(arg)             (uint64_t)(arg)
#define WRITE_UINT8(arg, data)      arg = (uint8_t)(data)
#define WRITE_UINT16(arg, data)     arg = (uint16_t)(data)
#define WRITE_UINT32(arg, data)     arg = (uint32_t)(data)
#define WRITE_UINT64(arg, data)     arg = (uint64_t)(data)

#if defined(SC3X00) || defined(SC3X50)
#include "sc3x00_accessors.h"
#elif defined(SC39XX)
#include "sc39xx_accessors.h"
#endif  // defined (SC3X00) || defined (SC3X50)

#define EXPLAIN(A)
#define EXPLAIN_ARG(A,B)

#endif // VERBOSE

#define EXPLAIN(A)
#define EXPLAIN_ARG(A,B)

#define CLEAR_UINT32(addr, bitmask)        WRITE_UINT32(addr, (GET_UINT32(addr) &~ (bitmask)))
#define SET_UINT32(addr, bitmask)          WRITE_UINT32(addr, (GET_UINT32(addr) | (bitmask)))

#define CLEAR_UINT16(addr, bitmask)        WRITE_UINT16(addr, (GET_UINT16(addr) &~ (bitmask)))
#define SET_UINT16(addr, bitmask)          WRITE_UINT16(addr, (GET_UINT16(addr) | (bitmask)))

#define CLEAR_UINT8(addr, bitmask)         WRITE_UINT8(addr, (GET_UINT8(addr) &~ (bitmask)))
#define SET_UINT8(addr, bitmask)           WRITE_UINT8(addr, (GET_UINT8(addr) | (bitmask)))

#define IS_SET_UINT32(addr, bitmask)       GET_UINT32(addr) & (bitmask)
#define IS_SET_UINT16(addr, bitmask)       GET_UINT16(addr) & (bitmask)
#define IS_SET_UINT8(addr, bitmask)        GET_UINT8(addr) & (bitmask)

/* Provides coding separation between accesses to CCSR and to memory. Implementation is the same */
#define READ_MEM_UINT8(data, arg)           READ_UINT8(data, arg)
#define READ_MEM_UINT16(data, arg)          READ_UINT16(data, arg)
#define READ_MEM_UINT32(data, arg)          READ_UINT32(data, arg)
#define GET_MEM_UINT8(arg)                  GET_UINT8(arg)
#define GET_MEM_UINT16(arg)                 GET_UINT16(arg)
#define GET_MEM_UINT32(arg)                 GET_UINT32(arg)
#define WRITE_MEM_UINT8(arg, data)          WRITE_UINT8(arg, data)
#define WRITE_MEM_UINT16(arg, data)         WRITE_UINT16(arg, data)
#define WRITE_MEM_UINT32(arg, data)         WRITE_UINT32(arg, data)

#endif // OS_DOCUMENTATION_ONLY


_OS_END_EXTERN_C


#endif // __OS_ACCESSORS_H


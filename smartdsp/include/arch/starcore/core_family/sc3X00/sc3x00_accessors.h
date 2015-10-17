/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_accessors.h,v 1.4 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/core_family/sc3X00/sc3x00_accessors.h,v $
 $Revision: 1.4 $
******************************************************************************/

/**************************************************************************//**
 
 @File          sc3x00_accessors.h

 @Description   Memory and register accessor functions (SC3X00 specific).

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __SC3X00_ACCESSORS_H
#define __SC3X00_ACCESSORS_H

#include "os_arch_macros.h" /* keep this before other API headers */
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

/**************************************************************************//**
 @Group         sc3x00_accessor_id SC3X00 Accessor API

 @Description   Macros for accessing registers and memory (SC3X00 specific).
 
                These macros are written in such a manner to make them portable
                across platforms and architectures. Any address that is passed
                should be passed as dereferenced pointer
 
 
 @{
*//***************************************************************************/

#ifdef OS_DOCUMENTATION_ONLY
/**************************************************************************//**
 @Function      GET_SYNCIO_UINT8

 @Description   Read a uint8_t register with a syncio and return the value
 
                This function performs: (uint8_t)(arg) + syncio
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in register
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple GET_UINT8()

                The function signature is for documentation purposes only

*//***************************************************************************/
uint8_t GET_SYNCIO_UINT8(uint8_t arg);


/**************************************************************************//**
 @Function      GET_SYNCIO_UINT16

 @Description   Read a uint16_t register with a syncio and return the value
 
                This function performs: (uint16_t)(arg) + syncio
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in register
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple GET_UINT16()

                The function signature is for documentation purposes only

*//***************************************************************************/
uint16_t GET_SYNCIO_UINT16(uint16_t arg);


/**************************************************************************//**
 @Function      GET_SYNCIO_UINT32

 @Description   Read a uint32_t register with a syncio and return the value
 
                This function performs: (uint32_t)(arg) + syncio
 
 @Param[in]     arg     -  The address to read from
 
 @Return        value in register
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple GET_UINT32()

                The function signature is for documentation purposes only

*//***************************************************************************/
uint32_t GET_SYNCIO_UINT32(uint32_t arg);


/**************************************************************************//**
 @Function      WRITE_SYNCIO_UINT8

 @Description   Write a uint8_t register with a syncio
 
                This function performs: arg = (uint8_t)(data) + syncio
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple WRITE_UINT8()

                The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_SYNCIO_UINT8(uint8_t arg, uint8_t data);


/**************************************************************************//**
 @Function      WRITE_SYNCIO_UINT16

 @Description   Write a uint8_t register with a syncio
 
                This function performs: arg = (uint16_t)(data) + syncio
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple WRITE_UINT16()

                The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_SYNCIO_UINT16(uint16_t arg, uint16_t data);


/**************************************************************************//**
 @Function      WRITE_SYNCIO_UINT32

 @Description   Write a uint32_t register with a syncio
 
                This function performs: arg = (uint32_t)(data) + syncio
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple WRITE_UINT32()

                The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_SYNCIO_UINT32(uint32_t arg, uint32_t data);


/**************************************************************************//**
 @Function      WRITE_SYNCIO_UINT64

 @Description   Write a uint64_t register with a syncio
 
                This function performs: arg = (uint64_t)(data) + syncio
 
 @Param[in]     arg     -  The address to write to
 @Param[in]     data    -  The data value to write
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple WRITE_UINT64()

                The function signature is for documentation purposes only

*//***************************************************************************/
void WRITE_SYNCIO_UINT64(uint64_t arg, uint64_t data);


/**************************************************************************//**
 @Function      CLEAR_SYNCIO_UINT8

 @Description   Clear a bitmask on a uint8_t register and perform a syncio
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to clear - set 1 in each bit location that
                           should be cleared
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple CLEAR_UINT8()
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void CLEAR_SYNCIO_UINT8(uint8_t addr, uint8_t bitmask);


/**************************************************************************//**
 @Function      SET_SYNCIO_UINT8

 @Description   Set a bitmask on a uint8_t register and perform a syncio
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to set - set 1 in each bit location that
                           should be set
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple SET_UINT8()
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void SET_SYNCIO_UINT8(uint8_t addr, uint8_t bitmask);


/**************************************************************************//**
 @Function      CLEAR_SYNCIO_UINT16

 @Description   Clear a bitmask on a uint16_t register and perform a syncio
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to clear - set 1 in each bit location that
                           should be cleared
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple CLEAR_UINT16()
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void CLEAR_SYNCIO_UINT16(uint16_t addr, uint16_t bitmask);


/**************************************************************************//**
 @Function      SET_SYNCIO_UINT16

 @Description   Set a bitmask on a uint16_t register and perform a syncio
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to set - set 1 in each bit location that
                           should be set
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple SET_UINT16()
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void SET_SYNCIO_UINT8(uint16_t addr, uint16_t bitmask);


/**************************************************************************//**
 @Function      CLEAR_SYNCIO_UINT32

 @Description   Clear a bitmask on a uint32_t register and perform a syncio
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to clear - set 1 in each bit location that
                           should be cleared
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple CLEAR_UINT32()
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void CLEAR_SYNCIO_UINT32(uint32_t addr, uint32_t bitmask);


/**************************************************************************//**
 @Function      SET_SYNCIO_UINT32

 @Description   Set a bitmask on a uint32_t register and perform a syncio
 
 @Param[in]     addr    -  The address to clear
 @Param[in]     bitmask -  The bits to set - set 1 in each bit location that
                           should be set
 
 @Cautions      syncio may not be supported on all supported platforms, in
                which case this macro will perform a simple SET_UINT32()
 
 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void SET_SYNCIO_UINT8(uint32_t addr, uint32_t bitmask);


/** @} */ // end of sc3x00_accessor_id

/** @} */ // end of accessor_id group

#else // OS_DOCUMENTATION_ONLY



#include <prototype.h>

#ifdef VERBOSE
#include <stdio.h>
#include "smartdsp_os.h"
extern FILE *log_file;

 


INLINE uint8_t get_syncio_uint8_verbose(uint8_t *arg)
{
    fprintf(log_file, "CoreID %d GET_SYNCIO_UINT8 0x%04x FROM %08p\n", osGetCoreID(), *arg, (uint32_t)(arg));
    fflush(log_file);
    return syncio_read_ub(arg);
}

INLINE uint16_t get_syncio_uint16_verbose(uint16_t *arg)
{
    fprintf(log_file, "CoreID %d GET_SYNCIO_UINT16 0x%04x FROM %08p\n", osGetCoreID(), *arg, (uint32_t)(arg));
    fflush(log_file);
    return syncio_read_uw(arg);
}

INLINE uint32_t get_syncio_uint32_verbose(uint32_t *arg)
{
    fprintf(log_file, "CoreID %d GET_SYNCIO_UINT32 0x%08x FROM %08p\n", osGetCoreID(), *arg, (uint32_t)(arg));
    fflush(log_file);
    return syncio_read_ul(arg);
}

#define GET_SYNCIO_UINT8(arg)              get_syncio_uint8_verbose((uint8_t *)&(arg))
#define GET_SYNCIO_UINT16(arg)             get_syncio_uint16_verbose((uint16_t *)&(arg))
#define GET_SYNCIO_UINT32(arg)             get_syncio_uint32_verbose((uint32_t *)&(arg))
#define WRITE_SYNCIO_UINT8(arg, data)      \
{ syncio_write_ub((uint8_t*)&(arg),(uint8_t)(data));   \
  fprintf(log_file, "CoreID %d WRITE_SYNCIO_UINT8: %08p = 0x%02X\n", osGetCoreID(), &(arg), (data)); \
  fflush(log_file); \
}

#define WRITE_SYNCIO_UINT16(arg, data)     \
{ syncio_write_uw((uint16_t*)&(arg),(uint16_t)(data));   \
  fprintf(log_file, "CoreID %d WRITE_SYNCIO_UINT16: %08p = 0x%04X\n", osGetCoreID(), &(arg), (data)); \
  fflush(log_file); \
}

#define WRITE_SYNCIO_UINT32(arg, data)     \
{ syncio_write_ul((uint32_t*)&(arg),(uint32_t)(data));   \
  fprintf(log_file, "CoreID %d WRITE_SYNCIO_UINT32: %08p = 0x%08X\n", osGetCoreID(), &(arg), (data)); \
  fflush(log_file); \
}


#else // not VERBOSE


#define GET_SYNCIO_UINT8(arg)            syncio_read_ub((unsigned char*)&(arg))
#define GET_SYNCIO_UINT16(arg)           syncio_read_uw((unsigned short*)&(arg))
#define GET_SYNCIO_UINT32(arg)           syncio_read_ul((unsigned int*)&(arg))
#define WRITE_SYNCIO_UINT8(arg, data)    syncio_write_ub((unsigned char*)&(arg),(uint8_t)(data))
#define WRITE_SYNCIO_UINT16(arg, data)   syncio_write_uw((unsigned short*)&(arg),(uint16_t)(data))
#define WRITE_SYNCIO_UINT32(arg, data)   syncio_write_ul((unsigned int*)&(arg),(uint32_t)(data))

#if defined(_ENTERPRISE_C_) && (_MW_SCC_GENERATION_ < 2147483647)
#define WRITE_SYNCIO_UINT64(arg, data)   syncio_write_ull((uint64_t *)&(arg),(uint64_t)(data))
#else
static inline asm void syncio_write_ull(uint64_t* ptr, uint64_t* data)
{
asm_header
    .arg
    _ptr in $r0;
    _data in $r1;
    .reg $r0,$r1,$d0,$d1;
asm_body
    move.2l (r1),d0:d1
    [
      move.2l d0:d1,(r0)
      syncio
    ]
asm_end
}
#endif // _ENTERPRISE_C_, _MW_SCC_GENERATION_

#endif // VERBOSE



#define CLEAR_SYNCIO_UINT32(addr, bitmask) WRITE_SYNCIO_UINT32(addr, (GET_UINT32(addr) &~ (bitmask)))
#define SET_SYNCIO_UINT32(addr, bitmask)   WRITE_SYNCIO_UINT32(addr, (GET_UINT32(addr) | (bitmask)))

#define CLEAR_SYNCIO_UINT16(addr, bitmask) WRITE_SYNCIO_UINT16(addr, (GET_UINT16(addr) &~ (bitmask)))
#define SET_SYNCIO_UINT16(addr, bitmask)   WRITE_SYNCIO_UINT16(addr, (GET_UINT16(addr) | (bitmask)))

#define CLEAR_SYNCIO_UINT8(addr, bitmask)  WRITE_SYNCIO_UINT8(addr, (GET_UINT8(addr) &~ (bitmask)))
#define SET_SYNCIO_UINT8(addr, bitmask)    WRITE_SYNCIO_UINT8(addr, (GET_UINT8(addr) | (bitmask)))

#endif // OS_DOCUMENTATION_ONLY


_OS_END_EXTERN_C


#endif // __SC3X00_ACCESSORS_H


/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:36 $
 $Id: maple_pe_init.h,v 1.7 2012/11/22 16:28:36 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/maple/rev1/include/maple_pe_init.h,v $
 $Revision: 1.7 $
******************************************************************************/

/**************************************************************************//**

 @File          rev1/include/maple_pe_init.h

 @Description   MAPLE-B Proccessing Elelement initialization header file.
                        Includes initializations shared by all PE.

*//***************************************************************************/

#ifndef __MAPLE_PE_INIT_H
#define __MAPLE_PE_INIT_H

#include "smartdsp_os_device.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         pe_id   Maple PE Generic API

 @Description   MAPLE all PE devices generic API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         pe_init_id  xxPE Initialization

 @Description   MAPLE all PE devices generic initialization API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   BD rings priority scheduling
 
                The scheduling mechanism used by the MAPLE u-code for scanning
                BD rings. The u-code will execute as many high priority BD as
                indicated by the number of "H" in the enumeration prior to
                executing a low priority BD.
 
                Example: in case of MAPLE_PE_BD_RING_H_H_L after executing two
                jobs from any of the High priority rings of the XXXPE, it scans
                the next job in the Low priority rings of the XXXPE
 
*//***************************************************************************/
typedef enum
{
    MAPLE_PE_BD_RING_H,
    /**< MAPLE scans the High BD rings of the XXXPE only */
    MAPLE_PE_BD_RING_H_L,
    /**< MAPLE executes one high followed by one low priority BD */
    MAPLE_PE_BD_RING_H_H_L,
    /**< MAPLE executes two high followed by one low priority BD */
#if defined(MAPLE_B2) || defined(MAPLE_B2F) || defined(MAPLE_B2P)
    MAPLE_PE_BD_RING_H_H_H_L,
    /**< MAPLE executes three high followed by one low priority BD */
    MAPLE_PE_BD_RING_H_H_H_H_L,
    /**< MAPLE executes four high followed by one low priority BD */
#endif
    MAPLE_PE_BD_LAST_PRIORITY_SCHEME
    /**< Software indicator - not to be used by the applicaiton! */
} maple_pe_bd_priority_t;

/**************************************************************************//**
 @Description   Nunber of BD rings
 
                The number of BD rings for each priority. Low priority
                BD ring can be disabled by choosing paramater of
                #maple_pe_bd_priority_t
*//***************************************************************************/
typedef enum
{
    MAPLE_PE_NUM_BD_RINGS_1, /**< Only 1 BD ring per priority level is potentially valid */
    MAPLE_PE_NUM_BD_RINGS_2, /**< Only 2 BD rings per priority level are potentially valid */
#if defined(MAPLE_B) || defined(MAPLE_B2) || defined(MAPLE_B2P)
    MAPLE_PE_NUM_BD_RINGS_3, /**< Only 3 BD rings per priority level are potentially valid */
    MAPLE_PE_NUM_BD_RINGS_4, /**< Only 4 BD rings per priority level are potentially valid */
#endif
#if defined(MAPLE_B) || defined(MAPLE_B2)
    MAPLE_PE_NUM_BD_RINGS_5, /**< Only 5 BD rings per priority level are potentially valid */
    MAPLE_PE_NUM_BD_RINGS_6, /**< Only 6 BD rings per priority level are potentially valid */
    MAPLE_PE_NUM_BD_RINGS_7, /**< Only 7 BD rings per priority level are potentially valid */
    MAPLE_PE_NUM_BD_RINGS_8, /**< Only 8 BD rings per priority level are potentially valid */
#endif
    MAPLE_PE_NUM_BD_RINGS_LAST
} maple_pe_num_bd_t;




/**************************************************************************//**
 @Description   MAPLE xxPE types.
 
 @Cautions      Not all types are supported by all MAPLE generations
*//***************************************************************************/
typedef enum
{
    TVPE_TYPE,
    FFTPE_TYPE,
    DFTPE_TYPE,
    FTPE_TYPE,
    DEPE_TYPE,
    EQPE_TYPE,
    PUFFT_TYPE,
    PUSCH_TYPE,
    PDSCH_TYPE
} maple_pe_type_t;

/**************************************************************************//**
 @Description   MAPLE xxPE Interrupt options
*//***************************************************************************/
typedef enum
{
    INT_LINE            = 0x00000000,
    /**< Regular interrupt is generated according to MTVBRHPBxP[INT_TRGT] */
    SRIO_DOORBELL_PORT0 = 0x00000100,
    /**< The door-bell should be initiated via Serial RapidIO port0 */
    SRIO_DOORBELL_PORT1 = 0x00000200
    /**< The door-bell should be initiated via Serial RapidIO port1 */
} maple_pe_int_t;

/**************************************************************************//**
 @Description   MAPLE xxPE Interrupt line options
 
                Only relevant if #maple_pe_int_t is INT_LINE; Defines which regular interrupt
                is to be asserted due to task completion in the BD ring
*//***************************************************************************/
typedef enum
{
    MAPLE_IRQ0 = 0x00000000,
    MAPLE_IRQ1 = 0x00000010,
    MAPLE_IRQ2 = 0x00000020,
    MAPLE_IRQ3 = 0x00000030,
    MAPLE_IRQ4 = 0x00000040,
    MAPLE_IRQ5 = 0x00000050,
    MAPLE_IRQ6 = 0x00000060,
    MAPLE_IRQ7 = 0x00000070,
    MAPLE_IRQ8 = 0x00000080,
    MAPLE_IRQ9 = 0x00000090,
    MAPLE_IRQ10= 0x000000A0,
    MAPLE_IRQ11= 0x000000B0,
    MAPLE_IRQ12= 0x000000C0,
    MAPLE_IRQ13= 0x000000D0,
    MAPLE_IRQ14= 0x000000E0,
    MAPLE_IRQ15= 0x000000F0
#if defined(MAPLE_B2) || defined(MAPLE_B2F) || defined(MAPLE_B2P)
    ,MAPLE_IRQ16= 0x00000100,
    MAPLE_IRQ17= 0x00000110,
    MAPLE_IRQ18= 0x00000120,
    MAPLE_IRQ19= 0x00000130,
    MAPLE_IRQ20= 0x00000140,
    MAPLE_IRQ21= 0x00000150,
    MAPLE_IRQ22= 0x00000160,
    MAPLE_IRQ23= 0x00000170,
    MAPLE_IRQ24= 0x00000180,
    MAPLE_IRQ25= 0x00000190,
    MAPLE_IRQ26= 0x000001A0,
    MAPLE_IRQ27= 0x000001B0,
    MAPLE_IRQ28= 0x000001C0,
    MAPLE_IRQ29= 0x000001D0,
    MAPLE_IRQ30= 0x000001E0,
    MAPLE_IRQ31= 0x000001F0
#endif /* B2/B2F/B2P */
} maple_pe_int_trgt_t;


/**************************************************************************//**
 @Description   MAPLE xxPE generic device open parameters
*//***************************************************************************/
typedef struct
{
    unsigned int    num_devices;
    /**< Number of elements in maple_pe_init_params_s array*/
    struct maple_pe_init_params_s
    {
        char            *name;
        /**< Name of device */
        uint8_t         pe_id;
        /**< xxPE number in MAPLE */
        maple_pe_type_t pe_type;
        /**< xxPE Type (VT, FFT, DFT) */
    } pe_init [];

} maple_pe_init_params_t;


/**************************************************************************//**
 @Description   MAPLE xxPE generic channel open parameters
*//***************************************************************************/
typedef struct
{
    union
    {
        uint32_t        flags;
        /**< Users should set to 0 prior to setting any of the indivudal flags to ensure future compatibility.
             Future releases may add more optimizations and flags to this 32 bit field. In order to ensure
             that these features, which will always be enabled by setting the relevant bit(s) to a value other
             than 0, won't be used unintentionally - the user should set #flags to 0 */
        struct
        {
            uint32_t    high_priority:1;
            /**< Set to 1 for a high priority channel or 0 low priority channel */
            uint32_t    int_enable:1;
            /**< Set to 1 to enable interrupts for this channel */
            uint32_t    no_translation:1;
            /**< Set to 1 to disable address translation on pointers passed from the applicaiton */
            uint32_t    no_automatic_reap:1;
            /**< Set to 1 to disable automatic reaping after dispatching when interrupts are disabled */
            uint32_t    single_channel_to_int:1;
            /**< Set to 1 for the driver to register the channel directly on the interrupt with no
                 support for multiple channels on the interrupt line */
        };
    };

    os_mem_type         channel_location;
    /**< Memory location for channel management. Should be cacheable for performance */
    maple_pe_int_t      interrupt_type;
    /* Which type of interrupt should channel expect. MUST be valid regardless of int_enable */
    maple_pe_int_trgt_t maple_int_trgt;
    /**< Which MAPLE line should the channel register to.
           Ignored when registering to an on chip MAPLE */
    os_hwi_handle       int_num;
    /**< Which interrupt line on this device should the interrupt handler register to */
    os_hwi_priority     int_priority;
    /**< Interrupt priority */
    uint16_t            host_id;
    /**< If interrupt_type is set to SRIO_DOORBELL_PORTx, to which sRIO ID should
            the doorbells be sent */
} maple_pe_ch_open_params_t;


typedef uint16_t maple_pe_ch;           /**< Maple channel number type */
typedef uint8_t  maple_pe_hw_sem_id;    /**< Maple HW semaphore ID type */

/**************************************************************************//**
 @Description   MAPLE xxPE Dispatch function type
*//***************************************************************************/
typedef os_status (*maple_xxpe_dispatch)(void *channel, void *jobs, int *num_jobs);

/**************************************************************************//**
 @Description   MAPLE xxPE Reap function type
*//***************************************************************************/
typedef void      (*maple_xxpe_reap)(void *channel, void *maple);

/** @} */ // end of pe_init_id
/** @} */ // end of pe_id


_OS_END_EXTERN_C

#endif // __MAPLE_PE_INIT_H

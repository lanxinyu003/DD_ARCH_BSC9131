/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/10/01 07:59:37 $
 $Id: maple_b2f_b2p_memmap.h,v 1.20 2013/10/01 07:59:37 b41640 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/maple/rev1/maple_b2f_b2p_memmap.h,v $
 $Revision: 1.20 $
******************************************************************************/

/**************************************************************************//**

 @File          maple_b2f_b2p_memmap.h

 @Description   MAPLE-B2F/P Memory map file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __MAPLE_B2F_B2P_MEMMAP_H
#define __MAPLE_B2F_B2P_MEMMAP_H

#include "os_datatypes.h"
#include "smartdsp_os_device.h"

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         arch_common_id  Peripheral Memory Maps
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         mapleb2fp_map_id    Maple-B2F/B2P Memory Map
 @{
*//***************************************************************************/

#define PDSCH_NUM_CELLS                    2         /**< Number of supported cells configurations */
#define PRAM_BD_RING_SIZE    (0x08000 - 0x04000)     /**< BD ring size in Maple PRAM */
#define FTPE_DSS_NUM                       7         /**< The number of Data Size Sets 0 - 5, including set number 7 which is used internally */
#define TVPE_VITERBI_POLY_SETS_NUM         3         /**< The number of Maple Turbo Viterbi Polynomial Vector Set */
#define MAPLE_MAP_MAX_NUM_PE_BD_RINGS      4         /**< Maximal number of BD rings in B2F and B2P */
#define MAPLE_AIC_NUM_ANT                  2         /**< The Maximal number of available antennas */
#define MAPLE_BD_NUM_OF_PRIORITIES         2         /**< Number of types of priorities (currently only high and low) */
#define MAPLE_TRACE_BD_SIZE	            0x1800		 /**< Size of each RISC MAPLE Trace buffers Description */
/**************************************************************************//**
 @Description   BD ring parameter A and parameter B
*//***************************************************************************/
typedef struct
{
    volatile uint32_t           brpap; /**< BD Ring X Priority A Parameter */
    volatile uint32_t           brpbp; /**< BD Ring X Priority B Parameter */
} maple_pe_bd_params_t;

/**************************************************************************//**
 @Description   PUFFT BD rings parameters
*//***************************************************************************/
typedef struct
{
    maple_pe_bd_params_t        high_bd[MAPLE_MAP_MAX_NUM_PE_BD_RINGS];   /**< Hight priority rings */
} maple_pufft_bd_ring_params_t;

/**************************************************************************//**
 @Description   PE BD rings parameters
*//***************************************************************************/
typedef struct
{
    maple_pe_bd_params_t        high_bd[MAPLE_MAP_MAX_NUM_PE_BD_RINGS];   /**< Hight priority rings */
    OS_MEM_RESERVED(0xA0, 0xC0);
    maple_pe_bd_params_t        low_bd[MAPLE_MAP_MAX_NUM_PE_BD_RINGS];    /**< Low priority rings */
} maple_pe_bd_ring_params_t;

/**************************************************************************//**
 @Description   Maple PRAM memory map
*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0x0, 0x4);
    volatile uint32_t           mbdrcp;             /**< Maple BD Rings Configuration Parameter 0 */
    volatile uint32_t           mbdrcp1;            /**< Maple BD Rings Configuration Parameter 1 */
    volatile uint32_t           mbdrcp2;            /**< Maple BD Rings Configuration Parameter 2 */
    volatile uint32_t           mucvp;              /**< Maple UCode Version Parameter */
    OS_MEM_RESERVED(0x14, 0x18);
    volatile uint32_t           mp_tpp;             /**< Maple Timer Period Parameter. This option is not available through SmartDSP OS.*/
    OS_MEM_RESERVED(0x1C, 0x20);
    volatile uint32_t           mmc0p;              /**< Maple Mode Configurations 0 Parameter; Set by API */
    volatile uint32_t           mmc1p;              /**< Maple Mode Configurations 1 Parameter; Set by API */
    volatile uint32_t           mtvmp;              /**< Maple ETVPE_MODE parameter; Set by API */
    volatile uint32_t           cdomcp;             /**< CRPE-DL Output Mode Configuration Parameter; Set by API */
    volatile uint32_t           cubmcp;             /**< CRPE-ULB Mode Configuration Parameter; Set by API */
    volatile uint32_t           mpdschcp;           /**< PDSCH EDF Configuration Parameter. */
    volatile uint32_t           mpuschcp;           /**< PUSCH EDF Configuration Parameter. */
    OS_MEM_RESERVED(0x3C, 0x40);
    volatile uint32_t           mcrrcip;            /**< Maple CRPE Reset Completion Indication Parameter */
    OS_MEM_RESERVED(0x44, 0x60);
    maple_pufft_bd_ring_params_t    pufft_bd;           /** PUFFT BD Rings Configuration */
 
    OS_MEM_RESERVED(0x80, 0x88);
    maple_pe_bd_ring_params_t       tvpe_bd;            /**< TVPE BD Ring Priority Configuration; Only 2 Hight/Low BD rings are available in MAPLEB2F */
 
    OS_MEM_RESERVED(0xe8, 0x110);
    maple_pe_bd_ring_params_t       ftpe_bd[MAX_NUM_MAPLE_FTPE];   /**< FFTPE_x BD Ring Priority Configuration; Only 2 Hight/Low BD rings are available in MAPLEB2F */

    OS_MEM_RESERVED(0x170, 0x180);
    maple_pe_bd_ring_params_t       eqpe_bd;            /**< MAPLE_B2P only; EQPE BD Ring Priority Configuration */
 
    OS_MEM_RESERVED(0x1E0, 0x200);
    maple_pe_bd_ring_params_t       convpe_bd;          /**< MAPLE_B2P only; CONVPE BD Ring Priority Configuration */
 
    OS_MEM_RESERVED(0x260, 0x280);
    maple_pe_bd_ring_params_t       depe_bd;            /**< DEPE BD Ring Priority Configuration; Only 2 Hight/Low BD rings are available in MAPLEB2F */
 
    OS_MEM_RESERVED(0x2e0, 0x300);
    maple_pe_bd_ring_params_t       crcpe_bd;           /**< CRCPE BD Ring Priority Configuration; Only 2 Hight/Low BD rings are available in MAPLEB2F */
 
    OS_MEM_RESERVED(0x360, 0x380);
    maple_pe_bd_ring_params_t       pusch_bd;           /**< PUSCH EDF BD Ring Priority Configuration; Only 2 Hight/Low BD rings are available in MAPLEB2F */
 
    OS_MEM_RESERVED(0x3e0, 0x400);
    maple_pe_bd_ring_params_t       pdsch_bd;           /**< PDSCH EDF BD Ring Priority Configuration; Only 2 Hight/Low BD rings are available in MAPLEB2F */
 
    OS_MEM_RESERVED(0x460, 0x480);
    maple_pe_bd_ring_params_t       cgpe_bd;            /**< CGPE BD Ring Priority Configuration; Only 2 Hight/Low BD rings are available in MAPLEB2F */
 
    OS_MEM_RESERVED(0x4e0, 0x500);

    // eTVPE Parameters 0x500
    struct
    {
        volatile uint32_t       mtvpvhcp;           /**< Maple Turbo Viterbi Puncturing Vector x High Configuration Parameter */
        volatile uint32_t       mtvpvlcp;           /**< Maple Turbo Viterbi Puncturing Vector x Low Configuration Parameter */
    } tvpe_puncture_vector[MAPLE_NUM_TVPE_PUNCTURING_VECTORS];
    /**< Maple TVPE  Puncturing Vector parameters */

    union
    {
        struct
        {
            volatile uint8_t    pper[MAPLE_NUM_TVPE_PUNCTURING_VECTORS]; /**< Maple Turbo Viterbi Puncturing Period */
            volatile uint8_t    reserved0[2];
        } punc_period;
        volatile uint32_t       mtvppcp[3];         /**< Maple Turbo Viterbi Puncturing Period Configuration y Parameter */
 
    } tvpe_puncture_period;
    /**< Maple TVPE  Puncturing Period parameters */
 
    OS_MEM_RESERVED(0x55C, 0x560);

    struct
    {
        struct
        {
            volatile uint16_t   m_polygen1;
            volatile uint16_t   m_polygen0;
        } mtvpvsc0p;
        /**< Maple Turbo Viterbi Polynomial Vector Set x Configuration 0 parameter */
 
        struct
        {
            volatile uint16_t   m_polygen3;
            volatile uint16_t   m_polygen2;
        }mtvpvsc1p;
        /**< Maple Turbo Viterbi Polynomial Vector Set x Configuration 1 parameter */
 
    } mtvpvscp[TVPE_VITERBI_POLY_SETS_NUM];
    /**< Maple Tvpe Viterbi Polynomial Sets */

    OS_MEM_RESERVED(0x578, 0x580);
 
#ifdef MAPLE_B2F
    volatile uint32_t mharqwrspltsize;  /**< Size in Bytes of the Max DMA transaction for HARQ Write operation. (In case the HARQ size is bigger that this value , the Maple will split the DMA jobs to N= ceil (HARQ_SIZE/ HARQ Max size) .*/
    volatile uint32_t mharqrdspltsize;  /**< Size in Bytes of the Max DMA transaction for HARQ Read operation. (In case the HARQ size is bigger that this value , the Maple will split the DMA jobs to N= ceil (HARQ_SIZE/ HARQ Max size) .*/
#else
    OS_MEM_RESERVED(0x580, 0x588);
#endif

    OS_MEM_RESERVED(0x588, 0x5A0);
    // AIC handshake configuration parameters
    volatile uint32_t mpaifdladdr[MAPLE_AIC_NUM_ANT]; /**< Maple Antenna Interface Downlink Address 1 and 2 */
    volatile uint32_t mpaifdldata[MAPLE_AIC_NUM_ANT]; /**< Maple Antenna Interface Downlink Data 1 and 2 */
    volatile uint32_t mpaifuladdr[MAPLE_AIC_NUM_ANT]; /**< Maple Antenna Interface Uplink Address 1 and 2 */
    volatile uint32_t mpaifuldata[MAPLE_AIC_NUM_ANT]; /**< Maple Antenna Interface Uplink Data 1 and 2 */

    //MAPLE Trace Parameters
    volatile uint32_t mtrcecp; /**< MAPLE Trace Enable Configuration Parameter */
    volatile uint32_t mtrcwpp; /**< MAPLE Trace Write Pointer Parameter */
 
    OS_MEM_RESERVED(0x5C8, 0x600);

    // eFTPE Parameters 0x600
    union ftpedss_t
    {
        struct
        {
            volatile uint16_t   ds[6];                 /* Size of the Right/Left data of the iFFT/FFT Transform input/output */
            volatile uint8_t    reserved0[4];
        }data_size_set[FTPE_DSS_NUM];

        struct
        {
            volatile uint32_t   ftpedsspy[3];         /**< FTPE Data Size Set x Parameter y */
            volatile uint8_t    reserved0[4];
        }ftpedxss[FTPE_DSS_NUM];

    } ftpedss;
    /**< Maple FTPE Data Size parameters */
 
    struct
    {
            volatile uint32_t uprmbpp;  /**< Update Pre-Multiplication Buffer Pointer */
            volatile uint32_t upsmbpp;  /**< Update Post-Multiplication Buffer Pointer */
            volatile uint32_t ubsp;     /**< Update Buffers Size Parameter */
    }multiply_ftpe[MAX_NUM_MAPLE_FTPE];
    /**< eFTPE x Update Post/Pre-Multiplication parameters */
 
    volatile uint32_t ftpecubrp;        /**< UBC—Update Buffers Complete indication */
 
    // PDSCH EDF configuraton parameters
    OS_MEM_RESERVED(0x680, 0x684);
    volatile uint32_t pdschccp[PDSCH_NUM_CELLS];   /**< PDSCH Cell Config Parameters */
    OS_MEM_RESERVED(0x68C, 0x6A0);

    // EQPE Parameters 0x6A0
    OS_MEM_RESERVED(0x6A0, 0x6C0);
    volatile uint32_t           meqybmp;            /**< Maple EQPE Y Buffer Mode Parameter */
    OS_MEM_RESERVED(0x6C4, 0x710);
    volatile uint16_t ausiPUSCHCTRLCNT[MAPLE_BD_NUM_OF_PRIORITIES][MAPLE_MAX_NUM_PE_BD_RINGS_HIGH_PRIORITY];
    /** PUSCH Ctrl interrupt counter - For internal use */
    volatile uint8_t reserved[(0x800 - 0x710 - 2 * MAPLE_BD_NUM_OF_PRIORITIES * MAPLE_MAX_NUM_PE_BD_RINGS_HIGH_PRIORITY)];
 
    volatile uint8_t mtrcebd[2][MAPLE_TRACE_BD_SIZE];       /**< MAPLE Trace buffers for each RISC*/
 
    OS_MEM_RESERVED(0x3800, 0x4000);
 
} maple_pram_map_t;

/**************************************************************************//**
 @Description   Maple PSIF memory map
*//***************************************************************************/
typedef struct
{
    maple_pram_map_t            pram;                                    /**< See #maple_pram_map_t */
    volatile uint8_t            bd_rings [PRAM_BD_RING_SIZE];            /**< BD rings space in PRAM */
    OS_MEM_RESERVED(0x8000, 0xB600);
    volatile uint8_t            smartdsp_os_reserved[0xB800 - 0xB600];   /**< Area reserved for SmartDSP OS */
    OS_MEM_RESERVED(0xB800, 0x40000);
} maple_psif_dram_map_t;

/**************************************************************************//**
 @Description   TVPE registers memory map
*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0x40000, 0x7E000);
    volatile uint32_t           tvpec0r;                        /**< ETVPE Configuration 0 Register */
    OS_MEM_RESERVED(0x7E004, 0x7E03C);
    volatile uint32_t           tvaqcr;                         /**< ETVPE Aposteriori Quality Configuration Register */
    OS_MEM_RESERVED(0x7E040, 0x80000);
 
} maple_tvpe_map_t;

/**************************************************************************//**
 @Description   FTPE registers memory map
*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0x80000, 0xBE140);
    volatile uint32_t           ftpedsr0;                       /**< FTPE Data Size Register 0 */
    volatile uint32_t           ftpedsr1;                       /**< FTPE Data Size Register 1 */
    volatile uint32_t           ftpedsr2;                       /**< FTPE Data Size Register 2 */
    OS_MEM_RESERVED(0xBE14C, 0xBE1A0);
    volatile uint32_t           ftpecr;                         /**< FTPE_x Configuration Register */
    OS_MEM_RESERVED(0xBE1A4, 0xBE248);
    volatile uint32_t           ftpestr;                        /**< FTPE_x ECC Status Register */
    OS_MEM_RESERVED(0xBE24C, 0xC0000);

} maple_ftpe_map_t;

/**************************************************************************//**
 @Description   PUPE registers memory map
*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0xC0000, 0x100000);
 
} maple_pupe_map_t;

/**************************************************************************//**
 @Description   PDPE registers memory map
*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0x100000, 0x140000);
 
} maple_pdpe_map_t;

/**************************************************************************//**
 @Description   DEPE registers memory map
*//***************************************************************************/
typedef struct
{
    volatile uint8_t            reserved0[0x180000 - 0x140000];
 
} maple_depe_map_t;

/**************************************************************************//**
 @Description   EQPE registers memory map
*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0x200000, 0x23F208);
    volatile uint32_t           eq_thresh;                      /**< EQPE Threshold Register */
    OS_MEM_RESERVED(0x23F20C, 0x23F308);
    volatile uint32_t           eq_eccevent;                    /**< EQPE ECC Event Register */
    OS_MEM_RESERVED(0x23F30C, 0x240000);
} maple_eqpe_map_t;

/**************************************************************************//**
 @Description   CRPE registers memory map
*//***************************************************************************/
typedef struct
{
        OS_MEM_RESERVED(0x180000, 0x200000);
}maple_crpe_map_t;

/**************************************************************************//**
 @Description   Maple MBUS memory map
*//***************************************************************************/
typedef struct
{
    maple_psif_dram_map_t       psif;
    maple_tvpe_map_t            tvpe;                       /**< eTVPE Registers */
    maple_ftpe_map_t            ftpe[MAX_NUM_MAPLE_FTPE];   /**< FTPE  Registers */
    maple_pupe_map_t            pupe;                       /**< PUPE  Registers */
    maple_pdpe_map_t            pdpe;                       /**< PDPE  Registers */
    maple_depe_map_t            depe;                       /**< DEPE  Registers */
    maple_crpe_map_t            cpre;                       /**< CRPE Registers */
    maple_eqpe_map_t            eqpe;                       /**< MAPLEB2P only; EQPE  Registers */
} maple_mbus_memmap_t;

/**************************************************************************//**
 @Description   Maple SBUS memory map
*//***************************************************************************/
typedef struct
{
    volatile uint8_t            reserved0[0x1000];
    volatile uint32_t           pcr;                        /**< PSIF Command Register */
    volatile uint8_t            reserved1[0x1600 - 0x1004];
    volatile uint32_t           pspicer0;                   /**< PSIF Pic Event Register 0 */
    volatile uint32_t           pspicer1;                   /**< PSIF Pic Event Register 1 */
    volatile uint32_t           pspicer2;                   /**< PSIF Pic Event Register 2 */
    volatile uint32_t           pspicelr;                   /**< PSIF Pic Edge/Level Register */
    volatile uint32_t           pspicmr0;                   /**< PSIF Pic Mask Register 0 */
    volatile uint32_t           pspicmr1;                   /**< PSIF Pic Mask Register 1 */
    volatile uint32_t           pspicmr2;                   /**< PSIF Pic Mask Register 2 */
    volatile uint32_t           pspiciacr;                  /**< PSIF PIC Interrupt Assertion Clocks Registers */
    volatile uint8_t            reserved2[0xFFFF - 0x161F];
} maple_sbus_memmap_t;

/** @} */ // end of mapleb2f_map_id
/** @} */ // arch_common_id

_OS_END_EXTERN_C

#endif // __MAPLE_B2F_B2P_MEMMAP_H

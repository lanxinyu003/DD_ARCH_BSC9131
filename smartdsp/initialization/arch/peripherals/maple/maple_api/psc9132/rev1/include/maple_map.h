/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
   Copyright © 1995-2003,2004,2005-2010 Freescale Semiconductor Inc.
   All Rights Reserved
   this is proprietary source code of Freescale Semiconductor Inc., and its use
   is subject to the CodeWarrior EULA. The copyright notice above does not
   evidence any actual or intended publication of such source code.
   ---------------------------------------------------------------------------
   $RCSfile: maple_map.h,v $
   Tag $Name:  $
   $Revision: 1.17 $
   $Date: 2014/11/26 15:30:55 $
   Target Processor: MAPLE RISC
   Target Device: MSC8155
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_map.h
 @brief  MAPLE memory map file
*/
//----------------------------------------------------------------------------
#ifndef _MAPLE_H
#define _MAPLE_H
// MBus Memory Map
/** PRAM Section                               - 0x0    to  0x7ff.  Size - 2048 bytes **/
#define MAPLE_PRAM_BASE_ADDR                  0x0000
/** DEBUG TRACE BUFFER 0                       - 0x800  to  0x1fff  Size - 6KB **/
#define MAPLE_DEBUG_TRACE_BUFFER0             0x800
/** DEBUG TRACE BUFFER 1                       - 0x2000 to  0x37ff  Size - 6KB **/
#define MAPLE_DEBUG_TRACE_BUFFER1             0x2000
/** CRPE ULB Parameters                        - 0x800  to  0x17ff. Size - 4096 bytes **/
#define MAPLE_CRPEULBPARAMETER_BASE_ADDR      0x800
/** CRPE DL Parameters                         - 0x1800 to  0x38ff. Size - 8448 bytes **/
#define MAPLE_CRPEDLPARAMETER_BASE_ADDR       0x1800
/** CRPE ULF Parameter                         - 0x3900 to  0x39ff. Size - 256 bytes **/
#define MAPLE_CRPEULFPARAMETER_BASE_ADDR      0x3900
/** FTPE DFT translation table                - 0x3a00 to  0x3a95. Size - 150 bytes **/
#define MAPLE_FTPE_SIZE_TO_TLID_ADDR          0x3a00
/** BD Ring memory                             - 0x4000 to  0x7fff. Size - 16384 bytes **/
#define MP_BD_BASE_ADDR                       0x4000
/** PDSCH Context                              - 0x8000 to  0x97ff. Size - 6144 bytes **/
#define MAPLE_CONTEXTPDSCHPE_BASE_ADDR        0x8000
/** PUSCH Context                              - 0x9800 to  0xafff. Size - 6144 bytes **/
#define MAPLE_CONTEXTPUSCHPE_BASE_ADDR        0x9800
/** PUFFT Context                              - 0xb000 to  0xb1ff. Size - 512 bytes **/
#define MAPLE_CONTEXTPUFFT_BASE_ADDR          0xb200
/** DEPE BD Context                            - 0xb400 to  0xb5ff. Size - 512 bytes **/
#define MAPLE_CONTEXTDEPEBD_BASE_ADDR         0xb400
/** SDOS Reserved Area                         - 0xb600 to  0xb7ff. Size - 512 bytes **/
#define MAPLE_SDOS_RESERVED_BASE_ADDR         0xb600
/** MAPLE Context 0                            - 0xb800 to  0xbbff. Size - 1024 bytes **/
#define MAPLE_CONTEXT0_BASE_ADDR              0xb800
/** MAPLE Context 1                            - 0xbc00 to  0xbfff. Size - 1024 bytes **/
#define MAPLE_CONTEXT1_BASE_ADDR              0xbc00
/** BD Context                                 - 0xc000 to  0xc7ff. Size - 2048 bytes **/
#define MAPLE_CONTEXTBD_BASE_ADDR             0xc000
/** EQPE Context                               - 0xd000 to  0xd2ff .Size - 768 bytes **/
#define MAPLE_EQPE_DATA_BASE_ADDR             0xd000
/** EQPE BDs                                   - 0xd300 to  0xd5ff .Size - 768 bytes **/
#define MAPLE_EQPEBD_DATA_BASE_ADDR           0xd300
/** FTPE Context                               - 0xd800 to  0xd900. Size - 256 bytes **/
#define FTPE_CONTEXT_BASE_ADDR                0xd800
/** AIF Tx Buffer Ant 0                        - 0xe000  to 0x107ff. Size - 10240 bytes **/
#define PDSCHPE_AIF_BASE0_ADDR                0xe000
/** AIF Tx Buffer Ant 1                        - 0x10800 to 0x12fff. Size - 10240 bytes **/
#define PDSCHPE_AIF_BASE1_ADDR                0x10800
/** AIF Rx Buffer Ant 0                        - 0x13000 to 0x157ff. Size - 10240 bytes **/
#define PUFFT_AIF_BASE0_ADDR                  0x13000
/** AIF Rx Buffer Ant 1                        - 0x15800 to 0x17fff. Size - 10240 bytes **/
#define PUFFT_AIF_BASE1_ADDR                  0x15800

#define MAPLE_CRPEULB_MCUPCH_ADDR              MAPLE_CRPEULBPARAMETER_BASE_ADDR
#define MAPLE_CRPEULB_MCULGCP_ADDR            (MAPLE_CRPEULBPARAMETER_BASE_ADDR + 0x800)
#define MAPLE_CRPEULB_MCULANTD_ADDR           (MAPLE_CRPEULBPARAMETER_BASE_ADDR + 0x860)
#define MAPLE_CRPEULB_MCULCP_ADDR             (MAPLE_CRPEULBPARAMETER_BASE_ADDR + 0x8c0)
#define MAPLE_CRPEULB_MCULOBINTCFG_ADDR       (MAPLE_CRPEULBPARAMETER_BASE_ADDR + 0x8d0)
#define MAPLE_CRPEULB_MCULBFCB_ADDR           (MAPLE_CRPEULBPARAMETER_BASE_ADDR + 0x900)
#define MAPLE_CRPEULB_COREPARAMETER_BASE_ADDR (MAPLE_CRPEULBPARAMETER_BASE_ADDR + 0xd00)

// TVPE Addresses
#define TVPE_BASE_ADDR                        0x20000
#define TVPE_EXTRINSIC_BASE_ADDR              0x40000
#define TVPE_HARQO1_BASE_ADDR                 0x50000
#define TVPE_HARQO2_BASE_ADDR                 0x51000
#define TVPE_CD_BASE_ADDR                     0x60000
#define TVPE_NNIHII_BASE_ADDR                 0x70000
#define TVPE_NIHII_BASE_ADDR                  0x70200
#define TVPE_HEO1_BASE_ADDR                   0x70000
#define TVPE_HO_BASE_ADDR                     0x71000
#define TVPE_HEO2_BASE_ADDR                   0x72000
#define TVPE_HOO2_BASE_ADDR                   0x73000
#define TVPE_HARQO_BASE_ADDR                  0x78000
#define TVPE_REGISTER_BASE_ADDR               0x7e000
#define TVPE_VITERBI_REGISTER_BASE_ADDR       0x7f000
#define FTPE_BASE_ADDR                        0x80000
#define FTPE_IB_ADDR                          FTPE_BASE_ADDR
#define FTPE_OB_ADDR                          (FTPE_BASE_ADDR + 0x4000)
#define FTPE_POSTMULT_ADDR                    (FTPE_BASE_ADDR + 0x8000)
#define FTPE_PREMULT_ADDR                     (FTPE_BASE_ADDR + 0xc000)
#define FTPE_REGISTER_BASE_ADDR               (FTPE_BASE_ADDR + 0x3e000)
// PUPE Addresses
#define PUPE_BASE_ADDR                        (0xC0000)
#define PUPE_IN_BUFF_ADDR                     (PUPE_BASE_ADDR + 0x00000)
#define PUPE_IN_SCL_BUFF_ADDR                 (PUPE_BASE_ADDR + 0x20000)
#define PUPE_OUT0_BUFF_ADDR                   (PUPE_BASE_ADDR + 0x21000)
#define PUPE_OUT1_BUFF_ADDR                   (PUPE_BASE_ADDR + 0x22000)
#define PUPE_REGISTER_BASE_ADDR               (PUPE_BASE_ADDR + 0x3FC00)
// PDPE Memory map
#define PDPE_BASE_ADDR                        0x100000
#define PDPE_PARAMETERS_BASE_ADDR             (PDPE_BASE_ADDR + 0x10000)
#define PDPE_OUTPUT_BASE_ADDR                 (PDPE_BASE_ADDR + 0x18000)
#define PDPE_NDIS_OUTPUT_BASE_ADDR            (PDPE_BASE_ADDR + 0x1A000)
#define PDPE_CB_INPUT_BASE_ADDR               (PDPE_BASE_ADDR + 0x1C000)
#define PDPE_CRE_INPUT_BASE_ADDR              (PDPE_BASE_ADDR + 0x1D000)
#define PDPE_CONFIGURATION_BASE_ADDR          (PDPE_BASE_ADDR + 0x1E000)
#define PDPE_PEMCR_ADDR                       (PDPE_CONFIGURATION_BASE_ADDR + 0x00088)
#define PDPE_PGCR                             (PDPE_CONFIGURATION_BASE_ADDR + 0x0008C)
#define PDPE_CONTROL_BASE_ADDR                (PDPE_BASE_ADDR + 0x1E400)
#define PDPE_PCE1LR_REGISTER                  (PDPE_CONTROL_BASE_ADDR + 0x800)
#define PDPE_PCE0LR_REGISTER                  (PDPE_PCE1LR_REGISTER + 0x4)
#define PDPE_PSLTLR                           (PDPE_CONTROL_BASE_ADDR + 0x00818)
#define PDPE_POSTLR                           (PDPE_CONTROL_BASE_ADDR + 0x00814)
#define PDPE_STATUS_BASE_ADDR                 (PDPE_BASE_ADDR + 0x1F000)
#define PDPE_PESR_ADDR                        (PDPE_STATUS_BASE_ADDR + 0x00008)
// PDPE registers defines
#define PDPE_NUM_CW                            128
#define PDPE_NUM_CRE                           128
#define PDPE_NUM_SF                            2
#define PDPE_NUM_SLOT                          2
#define PDPE_NUM_SYM                           2
// DEPE
#define DEPE_BASE_ADDR                        0x140000
#define DEPE_REGISTER_BASE_ADDR               0x145000
//EQPE
#define EQPE_BASE_ADDR                    0x200000
#define EQPE_OUTPUTBUFFER2_BASE_ADDR      (EQPE_BASE_ADDR + 0x2000)
#define EQPE_OUTPUTBUFFER4_BASE_ADDR      (EQPE_BASE_ADDR + 0x6000)
#define EQPE_OUTPUTSCALEBUFFER1_BASE_ADDR (EQPE_BASE_ADDR + 0x8000)
#define EQPE_OUTPUTSCALEBUFFER2_BASE_ADDR (EQPE_BASE_ADDR + 0xa000)
#define EQPE_OUTPUTSCALEBUFFER3_BASE_ADDR (EQPE_BASE_ADDR + 0xc000)
#define EQPE_OUTPUTSCALEBUFFER4_BASE_ADDR (EQPE_BASE_ADDR + 0xe000)
#define EQPE_H0BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x10000)
#define EQPE_H1BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x14000)
#define EQPE_H2BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x18000)
#define EQPE_H3BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x1c000)
#define EQPE_H4BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x20000)
#define EQPE_H5BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x24000)
#define EQPE_H6BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x28000)
#define EQPE_H7BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x2c000)
#define EQPE_SBUFFER_BASE_ADDR            (EQPE_BASE_ADDR + 0x30000)
#define EQPE_FEEDBACKBUFFER0_BASE_ADDR    (EQPE_BASE_ADDR + 0x34000)
#define EQPE_FEEDBACKBUFFER1_BASE_ADDR    (EQPE_BASE_ADDR + 0x36000)
#define EQPE_W0BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x38000)
#define EQPE_W1BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x38800)
#define EQPE_YBUFFER_BASE_ADDR            (EQPE_BASE_ADDR + 0x3a000)
#define EQPE_SSCALEBUFFER_BASE_ADDR       (EQPE_BASE_ADDR + 0x3c000)
#define EQPE_HSCALEBUFFER_BASE_ADDR       (EQPE_BASE_ADDR + 0x3c100)
#define EQPE_YSCALEBUFFER_BASE_ADDR       (EQPE_BASE_ADDR + 0x3c180)
#define EQPE_CBUFFER_BASE_ADDR            (EQPE_BASE_ADDR + 0x3c1c0)
#define EQPE_Y0BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x3d000)
#define EQPE_Y1BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x3d400)
#define EQPE_Y2BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x3d800)
#define EQPE_Y3BUFFER_BASE_ADDR           (EQPE_BASE_ADDR + 0x3dc00)
#define EQPE_REGISTER_BASE_ADDR           (EQPE_BASE_ADDR + 0x3f000)
// CRPE

#define CRPE_BASE_ADDR                           0x180000
#define CRPEDL_BASE_ADDR                          CRPE_BASE_ADDR
#define CRPEDL_INPUT_SYMBOL_DATA_BASE_ADDR       (CRPEDL_BASE_ADDR + 0x18000)
#define CRPEDL_VIRTUAL_ANTENNA_GAIN_BASE_ADDR    (CRPEDL_BASE_ADDR + 0x20000)
#define CRPEDL_OUTPUT_CHIP_DATA_BASE_ADDR        (CRPEDL_BASE_ADDR + 0x28000)
#define CRPEDL_SLOT_FORMAT_LUT_BASE_ADDR         (CRPEDL_BASE_ADDR + 0x28800)
#define CRPEDL_REGISTER_BASE_ADDR                (CRPEDL_BASE_ADDR + 0x3f000)
#define CRPEDL_SCRAMBLING_INITIALIZATION_LUT_BASE_ADDR (CRPEDL_BASE_ADDR + 0x3f800)
#define CRPEULF_BASE_ADDR                         CRPE_BASE_ADDR
#define CRPEULF_INPUT_BUFFER_BASE_ADDR            CRPEULF_BASE_ADDR
#define CRPEULF_COMMAND_FIFO_BASE_ADDR           (CRPEULF_BASE_ADDR + 0x4000)
#define CRPEULF_REGISTER_BASE_ADDR               (CRPEULF_BASE_ADDR + 0x3e000)
#define CRPEULB_BASE_ADDR                        (CRPE_BASE_ADDR + 0x40000)
#define CRPEULB_OUTPUT_BUFFER_BASE_ADDR          (CRPEULB_BASE_ADDR + 0x0)
#define CRPEULB_INPUT_BUFFER0_BASE_ADDR          (CRPEULB_BASE_ADDR+ 0x10000)
#define CRPEULB_INPUT_BUFFER1_BASE_ADDR          (CRPEULB_BASE_ADDR + 0x12000)
#define CRPEULB_COMMAND_QUEUE_BASE_ADDR          (CRPEULB_BASE_ADDR + 0x18000)
#define CRPEULB_REGISTER_BASE_ADDR               (CRPEULB_BASE_ADDR + 0x1f000)
#define CGPE_REGISTER_BASE_ADDR                  (CRPE_BASE_ADDR + 0x3e800)
#define CGPE_CYCLIC_OUTPUT_BUF                   (CRPE_BASE_ADDR + 0x17000)
// SBus Memory Map
#define MAPLE_EQPE_PGC_BASE             0x0
#define MAPLE_SCHED_BASE                0x1000
#define MAPLE_DMA_BASE                  0x1200
#define MAPLE_IMEM_BASE                 0x1400
#define MAPLE_PSIF_PIC_BASE             0x1600
#define MAPLE_RSP1_BASE                 0x1800
#define MAPLE_RSP2_BASE                 0x1a00
#define MAPLE_TVPE_PGC_BASE             0x2c00
// MAPLE EQPE PGC Memory Map
#define MAPLE_EQPE_PGCR                 MAPLE_EQPE_PGC_BASE
#define MAPLE_EQPE_PUPSCR               (MAPLE_EQPE_PGC_BASE + 0x4)
#define MAPLE_EQPE_PDNSCR               (MAPLE_EQPE_PGC_BASE + 0x8)
#define MAPLE_EQPE_PGSR                 (MAPLE_EQPE_PGC_BASE + 0xc)
#define MAPLE_EQPE_PGDR                 (MAPLE_EQPE_PGC_BASE + 0x10)
#define MAPLE_EQPE_PGSWCR               (MAPLE_EQPE_PGC_BASE + 0x14)
// SCHED Memory map
#define MAPLE_PCR                       MAPLE_SCHED_BASE
#define MAPLE_MPCCR                     (MAPLE_SCHED_BASE + 0x4)
#define MAPLE_PEECR                     (MAPLE_SCHED_BASE + 0x38)
// DMA Memory map
#define MAPLE_SDMR                      (MAPLE_DMA_BASE + 0x4)
// IMEM Memory map
#define MAPLE_IADD                      MAPLE_IMEM_BASE
#define MAPLE_IDATA                     (MAPLE_IMEM_BASE + 0x4)
#define MAPLE_READY                     (MAPLE_IMEM_BASE + 0xc)
// RISC1 Memory Map
#define MAPLE_ECCR1                     (MAPLE_RSP1_BASE + 0xf0)
// RISC2 Memory Map
#define MAPLE_ECCR2                     (MAPLE_RSP2_BASE + 0xf0)
// PIC Memory Map
#define MAPLE_PSPICER0                  MAPLE_PSIF_PIC_BASE
#define MAPLE_PSPICER1                  (MAPLE_PSIF_PIC_BASE + 0x0004)
#define MAPLE_PSPICER2                  (MAPLE_PSIF_PIC_BASE + 0x0008)
#define MAPLE_PSPICELR                  (MAPLE_PSIF_PIC_BASE + 0x000c)
#define MAPLE_PSPICMR0                  (MAPLE_PSIF_PIC_BASE + 0x0010)
#define MAPLE_PSPICMR1                  (MAPLE_PSIF_PIC_BASE + 0x0014)
#define MAPLE_PSPICMR2                  (MAPLE_PSIF_PIC_BASE + 0x0018)
#define MAPLE_PSPICIACR                 (MAPLE_PSIF_PIC_BASE + 0x001c)
#define MAPLE_PSPICSER0                 (MAPLE_PSIF_PIC_BASE + 0x0020)
#define MAPLE_PSPICSER1                 (MAPLE_PSIF_PIC_BASE + 0x0024)
#define MAPLE_PSTTMRVAL0                (MAPLE_PSIF_PIC_BASE + 0x008c)
#define MAPLE_PSTTMRVAL1                (MAPLE_PSIF_PIC_BASE + 0x0090)
#define MAPLE_PSTTMRVAL2                (MAPLE_PSIF_PIC_BASE + 0x0094)
#define MAPLE_PSTTMRVAL3                (MAPLE_PSIF_PIC_BASE + 0x0098)
#define MAPLE_PSTTMRVAL4                (MAPLE_PSIF_PIC_BASE + 0x009c)
#define MAPLE_GCR	                    (MAPLE_PSIF_PIC_BASE + 0x0100)
// MAPLE TVPE PGC Memory Map
#define MAPLE_TVPE_PGCR                 MAPLE_TVPE_PGC_BASE
#define MAPLE_TVPE_PUPSCR               (MAPLE_TVPE_PGC_BASE + 0x4)
#define MAPLE_TVPE_PDNSCR               (MAPLE_TVPE_PGC_BASE + 0x8)
#define MAPLE_TVPE_PGSR                 (MAPLE_TVPE_PGC_BASE + 0xc)
#define MAPLE_TVPE_PGDR                 (MAPLE_TVPE_PGC_BASE + 0x10)
#define MAPLE_TVPE_PGSWCR               (MAPLE_TVPE_PGC_BASE + 0x14)
// PRAM Memory Map
#define MAPLE_TPP                       0x18
#define MAPLE_MMC1P                     0x20
#define MAPLE_MMC2P                     0x24
#define MAPLE_MTVMCP                    0x28
#define MAPLE_MPDSCHCP                  0x34
#define MAPLE_MPUSCHCP                  0x38
#define MAPLE_MCONVCP                   0x3c
// Maple define
#define PARSE_BD_OPCODE                 0x1
#define PARSE_TVPE_BD_OPCODE            0x2
#define PARSE_FTPE_BD_OPCODE            0x3
#define PARSE_DEPE_BD_OPCODE            0x4
#define PARSE_PUPE_BD_OPCODE            0x5
#define PARSE_PDPE_BD_OPCODE            0x6
#define PARSE_PUSCHPE_BD_OPCODE         0x7
#define PARSE_PDSCHPE_BD_OPCODE         0x8
#define PARSE_PUFFT_BD_OPCODE           0x9
#define PARSE_CRCPE_BD_OPCODE           0xa
#define PARSE_CGPE_BD_OPCODE            0xb
#define CRPERESET_OPCODE                0xc
#define CRPEULBINIT_OPCODE              0xd
#define CRPEULFIB_OPCODE                0xe
#define PDSCH_EXT_SYM_START_OPCODE      0xf
#define EFTPEUPDATEPREPOSTBUFS_OPCODE   0x10
#define PARSE_EQPE_BD_OPCODE            0x11
#define PARSE_CONVPE_BD_OPCODE          0x12
#define DMA_CLOSE_OPEN_ACC_OPCODE       0x19
#define PDSCH_DEBUG_INT_ANT0            0x1c
#define PDSCH_DEBUG_INT_ANT1            0x1d
#define PUFFT_DEBUG_INT_ANT0            0x1e
#define PUFFT_DEBUG_INT_ANT1            0x1f

#define CRPE_ULB_NUM_OF_GROUP 4
#define CRPE_ULB_NUM_OF_PCH 128
#define CRPE_ULB_NUM_OF_ANT 24
#define CRPE_ULB_NUM_OF_IBS 2
#define CRPE_ULB_SIZE_OF_FCB  128
#define CRPE_CRDL_SLOT_CHANNELS_NUM     368
#define CRPE_CRDL_SUB_SLOT_CHANNELS_NUM 140
#define CRPE_CRDL_MAX_ANTENNA           6
#define CRPE_CRULF_NUM_OF_CMD_CORE 2
#define CRPE_CRULF_NUM_OF_IB_GROUP      2
#define CRPE_CRULF_NUM_OF_IB_ANTENNA    6
#define MAPLEWRITE32(addr,data_val) data=data_val;\
                           maple_param->config_write((void *)addr,\
                             (void *)&data,4,maple_param->dev_id,maple_param->config_param);

#define MAPLEREAD_INTERNAL(addr,data_ptr) maple_param->config_read((void *)addr,\
                             (void *)data_ptr,4,maple_param->dev_id,maple_param->config_param);
#define MAPLERESERVED(start_addr,next_addr) unsigned char ucReserved ## start_addr [next_addr - start_addr]
/*----------------------------- PUSCHPE Definitions -------------------------*/
/* LTE related definitions */
#define PUSCHPE_CRC_SIZE    24
#define PUSCHPE_MAX_CB_SIZE 6144
#define PUSCHPE_NUM_OF_LAYERS 2

// MAPLE Struct
typedef struct MaplePRAM {
    /** Reserved 1                                                      - 0x0 **/
    unsigned char      ucReserved1[4];
    /** Maple BD Rings Configuration Parameter                          - 0x4 **/
    unsigned int       uliMBDRCP0;
    /** Maple BD Rings Configuration Parameter                          - 0x8 **/
    unsigned int       uliMBDRCP1;
    /** Maple BD Rings Configuration Parameter                          - 0xc **/
    unsigned int       uliMBDRCP2;
    /** Maple uCode Version Parameter                                   - 0x10 **/
    unsigned int       uliMUCVP;
    /** Reserved 2                                                      - 0x14 to 0x17 **/
    unsigned char      ucReserved2[4];
    /** Maple Timer Period                                              - 0x18 **/
    unsigned int       uliTPP;
    /** Reserved 3                                                      - 0x1c to 0x1f **/
    unsigned char      ucReserved3[4];
    /** Maple Mode Configuration Parameter                              - 0x20 **/
    unsigned int       uliMMC0P;
    /** Maple Mode Configuration Parameter                              - 0x24 **/
    unsigned int       uliMMC1P;
    /** Maple TVPE Mode Configuration Parameter                         - 0x28 **/
    unsigned int       uliMTVMCP;
    /** Maple CRPE-DL Output Mode Configuration Parameter               - 0x2c **/
    unsigned int       uliMCDOMCP;
    /** Maple CRPE-ULB Mode Configuration Parameter                     - 0x30 **/
    unsigned int       uliMCUBMCP;
    /** MAPLE PDSCH EDF Mode Configuration Parameter                    - 0x34 **/
    unsigned int       uliMPDSCHCP;
    /** MAPLE PUSCH EDF Mode Configuration Parameter                    - 0x38 **/
    unsigned int       uliMPUSCHCP;
    /** MAPLE CONVPE Configuration Parameter                            - 0x3c **/
    unsigned int       uliMCONVCP;
    /** Maple CRPE Reset Completion Indication Parameter                - 0x40 **/
    unsigned int       uliMCRRCIP;
    /** Reserved 5                                                      - 0x44 to 0x5f **/
    unsigned char      ucReserved5[28];
    /** Maple PUFFT BD Ring A/B x Parameter                             - 0x60 to 0x7f **/
    unsigned short int usiMPUFFTBDRLPxR[4][4];
    /** Maple TVPE BD Ring High Priority A/B x Parameter                - 0x80 to 0xbf **/
    unsigned short int usiMTVBDRHPxR[8][4];
    /** Maple TVPE BD Ring Low Priority A/B x Parameter                 - 0xc0 to 0xff **/
    unsigned short int usiMTVBDRLPxR[8][4];
    /** Maple FTPE BD Ring High Priority A/B x Parameter                - 0x100 to 0x13f **/
    unsigned short int usiMFTBDRHPxR[8][4];
    /** Maple FTPE BD Ring Low Priority A/B x Parameter                 - 0x140 to 0x17f **/
    unsigned short int usiMFTBDRLPxR[8][4];
    /** Maple EQPE BD Ring High Priority A/B x Parameter                - 0x180 to 0x1bf **/
    unsigned short int usiMEQBDRHPxR[8][4];
    /** Maple EQPE BD Ring Low Priority A/B x Parameter                 - 0x1c0 to 0x1ff **/
    unsigned short int usiMEQBDRLPxR[8][4];
    /** Maple CONVPE BD Ring High Priority A/B x Parameter              - 0x200 to 0x23f **/
    unsigned short int usiMCONVBDRHPxR[8][4];
    /** Maple CONVPE BD Ring Low Priority A/B x Parameter               - 0x240 to 0x27f **/
    unsigned short int usiMCONVBDRLPxR[8][4];
    /** Maple DEPE BD Ring High Priority A/B x Parameter                - 0x280 to 0x2bf **/
    unsigned short int usiMDEBDRHPxR[8][4];
    /** Maple DEPE BD Ring Low Priority A/B x Parameter                 - 0x2c0 to 0x2ff **/
    unsigned short int usiMDEBDRLPxR[8][4];
    /** Maple CRCPE BD Ring High Priority A/B x Parameter               - 0x300 to 0x33f **/
    unsigned short int usiMCRCBDRHPxR[8][4];
    /** Maple CRCPE BD Ring Low Priority A/B x Parameter                - 0x340 to 0x37f **/
    unsigned short int usiMCRCBDRLPxR[8][4];
    /** Maple PUSCHPE BD Ring High Priority A/B x Parameter             - 0x380 to 0x3bf **/
    unsigned short int usiMPUSCHBDRHPxR[8][4];
    /** Maple PUSCHPE BD Ring Low Priority A/B x Parameter              - 0x3c0 to 0x3ff **/
    unsigned short int usiMPUSCHBDRLPxR[8][4];
    /** Maple PDSCHPE BD Ring High Priority A/B x Parameter             - 0x400 to 0x43f **/
    unsigned short int usiMPDSCHBDRHPxR[8][4];
    /** Maple PDSCHPE BD Ring Low Priority A/B x Parameter              - 0x440 to 0x47f **/
    unsigned short int usiMPDSCHBDRLPxR[8][4];
    /** Maple CGPE BD Ring High Priority A/B x Parameter                - 0x480 to 0x4bf **/
    unsigned short int usiMCGBDRHPxR[8][4];
    /** Maple CGPE BD Ring Low Priority A/B x Parameter                 - 0x4c0 to 0x4ff **/
    unsigned short int usiMCGBDRLPxR[8][4];
    /** Maple Turbo Viterbi Puncturing Vector x
        High/Low Configuration Parameter                                  - 0x500 to 0x54f **/
    unsigned int       uliMTVPVxCP[10][2];
    /** Maple Turbo Viterbi Puncturing Period Configuration y Parameter   - 0x550 to 0x55f **/
    unsigned char      ucMTVPPCyP[16];
    /** Maple Turbo Viterbi Polynomial Vector Set Configuration Parameter - 0x560 to 0x57f **/
    unsigned short int usiMTVPVSxCP[4][4];
    /** Reserved 1                                                        - 0x580 to 0x59f **/
    unsigned char      ucReserve1[32];
    /** MAPLE AIF DL AntX control adress register                         - 0x5a0 to 0x5a7 **/
    unsigned int auliMPAIFDLADDRxR[2];
    /** MAPLE AIF DL AntX control data register                           - 0x5a8 to 0x5af **/
    unsigned  int auliMPAIFDLDATAxR[2];
    /** MAPLE AIF UL AntX control address register                        - 0x5b0 to 0x5b7 **/
    unsigned  int auliMPAIFULADDRxR[2];
    /** MAPLE AIF UL AntX control data register                           - 0x5b8 to 0x5bf **/
    unsigned  int auliMPAIFULDATAxR[2];
    /** MAPLE Trace Enable Configuration Parameter                        - 0x5c0 to 0x5c3 **/
    unsigned int uliMTRCECP;
    /** MAPLE Trace Write Pointer Parameter                               - 0x5c4 to 0x5c7 **/
    unsigned short int usiMTRCWPP[2];
    /** Reserved 2                                                        - 0x5c8 to 0x5ff **/
    unsigned char      ucReserve2[56];

    /** FTPE Data Size Set x Parameter y                                  - 0x600 to 0x66f **/
    unsigned short int usiFTPEDSSxPyR[7][8];
    /** FTPExUBPP eFTPE x Update Pre/Post Mult Buffer Pointer and Size    - 0x670 to 0x67b  **/
    unsigned int  uliFTPExUPPMy[1][3];
    /** FTPECUBRP eFTPE Complete Update Buffers Routine Parameter         - 0x67c **/
    unsigned int  uliFTPECUBRP;
	/** Maple PDSCHPE Reserved                                            - 0x680 **/
    unsigned  int uliPDSCHPEReserved;
     /** Maple PDSCHPE Cell ID set 0                                      - 0x684 **/
    unsigned short int usiPDSCHPECellID0;
     /** Maple PDSCHPE MBSFN ID set 0                                     - 0x686 **/
    unsigned short int usiPDSCHPEMBSFNID0;
     /** Maple PDSCHPE Cell ID set 1                                      - 0x688 **/
    unsigned short int usiPDSCHPECellID1;
     /** Maple PDSCHPE MBSFN ID set 1                                     - 0x68a **/
    unsigned short int usiPDSCHPEMBSFNID1;
     /** Maple Tx Ant 0 base address                                      - 0x68c to 0x697 **/
    unsigned int  auliPDSCHReserved[3];
    /** Reserved 3                                                        - 0x698 to 0x62f **/
    unsigned char      ucReserve3[40];
    /** Maple EQPE Y Buffer Mode Parameter                                - 0x6c0 **/
    unsigned int  uliMEQYBMP;
    /** Reserved 4                                                        - 0x6c4 to 0x6ff **/
    unsigned char      ucReserve4[60];
    /** SRIO Outbound RapidIO Doorbell Port Base Address Parameter        - 0x700 **/
    unsigned int       uliSORDPBAP;
    /** Hardware Semaphore Port Base Address Parameter                    - 0x704 **/
    unsigned int       uliHSPBAP;
    /** MAPLE-B Doorbell Hardware Semaphore ID Configuration Parameter    - 0x708 **/
    unsigned int       uliMDHSIDCP;
    /** MAPLE-B Doorbell General Configuration Parameter                  - 0x70c **/
    unsigned int       uliMDGCP;
    /** Reserved 8                                                        - 0x710 to 0x7fb **/
    unsigned char      ucReserved8[236];
    /** flag to mark end of init omrMapleInit  --   for internal use only 0x7fc  **/
    unsigned int uliEndOfInitRoutine;
} t_MaplePRAM;
typedef struct TVPE {
    /** TVPE Configuartion 0 Register                                  - 0x0 **/
    unsigned int  uliTVPEC0R;
    /** TVPE Configuartion 1 Register                                  - 0x4 **/
    unsigned int  uliTVPEC1R;
    /** TVPE CDL Symbol Identification 0 Configuration Register        - 0x8 **/
    unsigned int  uliTVCSI0CR;
    /** TVPE CDL Symbol Identification 1 Configuration Register        - 0xc **/
    unsigned int  uliTVCSI1CR;
    /** TVPE CDL Tail Symbol Identification X Configuration Register   - 0x10 to 0x23 **/
    unsigned int  uliTVCTSIxCR[5];
    /** ETVPE CTL Setting Configuration                                - 0x24 **/
    unsigned int  uliETVCSCR;
    /** Reserved 1                                                     - 0x28 to 0x37 **/
    unsigned char ucReserved1[16];
    /** TVPE Event Mask Configuration Register                         - 0x38 **/
    unsigned int  uliTVPEEMCR;
    /** TVPE Aposteriori Quality Configuration Register                - 0x3c **/
    unsigned int  uliTVAQCR;
    /** Reserved 2                                                     - 0x40 to 0x14f **/
    unsigned char ucReserved2[272];
    /** TVPE HARQ Event Mask Configuration Register                    - 0x150 **/
    unsigned int  uliTVHEMCR;
    /** TVPE CDL Event Mask Configuration Register                     - 0x154 **/
    unsigned int  uliTVCEMCR;
    /** TVPE CDL Interleaver Delta Configuration Register              - 0x158 **/
    unsigned int  uliTVCINTDCR;
    /** TVPE  Randomizer Configuration Register                        - 0x15c **/
    unsigned int  uliTVRANCR;
    /** TVPE  Auto NIIL Init Value Configuration Register              - 0x160 **/
    unsigned int  uliTVANIVCR;
    /** Reserved 3                                                     - 0x164 to 0x1ff **/
    unsigned char ucReserved3[156];
    /** TVPE Private Initial Intra Row Pointer x Status Register       - 0x200 to 0x53f **/
    unsigned int  uliTVPIRPxSR[208];
    /** Reserved 4                                                     - 0x540 to 0x7ff **/
    unsigned char ucReserved4[704];
    /** TVPE Command Control High Register                             - 0x800 **/
    unsigned short int usiTVPECMDLRH;
    /** TVPE Command Control Low Register                               - 0x802 **/
    unsigned short int usiTVPECMDLRL;
    /** TVPE Interleaver Rows Control Register                         - 0x804 **/
    unsigned int  uliTVINTRLR;
    /** TVPE Interleaver Columns Control Register                      - 0x808 **/
    unsigned int  uliTVINTCLR;
    /** Reserved 5                                                     - 0x80c **/
    unsigned char ucReserved5[4];
    /** TVPE Viterbi Polynomial Vector Generation Configuration 0 Register - 0x810 **/
    unsigned int  uliTVVPVG0CR;
    /** TVPE Viterbi Polynomial Vector Generation Configuration 1 Register - 0x814 **/
    unsigned int  uliTVVPVG1CR;
    /** TVPE Viterbi Polynomial Vector Generation Configuration 0 HARQ Register - 0x818 **/
    unsigned int  uliTVVPVG0HCR;
    /** TVPE Viterbi Polynomial Vector Generation Configuration 1 HARQ Register - 0x81c **/
    unsigned int  uliTVVPVG1HCR;
    /** TVPE Viterbi Polynomial Vector Generation Configuration 0 CDL Register - 0x820 **/
    unsigned int  uliTVVPVG0CCR;
    /** TVPE Viterbi Polynomial Vector Generation Configuration 1 CDL Register - 0x824 **/
    unsigned int  uliTVVPVG1CCR;
    /** Reserved 6                                                     - 0x828 to 0x83f **/
    unsigned char ucReserved6[24];
    /** TVPE Interleaver Intra Row Step x Control Register             - 0x840 to 0x877 **/
    unsigned int  uliTVIIRSxLR[14];
    /** Reserved 7                                                     - 0x878 to 0x87f **/
    unsigned char ucReserved7[8];
    /** TVPE Interleaver Initial Intra Row Pointer x Control Register  - 0x880 to 0x8b7 **/
    unsigned int  uliTVIIRPxLR[14];
    /** Reserved 8                                                     - 0x8b8 to 0x8bf **/
    unsigned char ucReserved8[8];
    /** TVPE Iteration Control High Register                           - 0x8c0 **/
    unsigned short int usiTVILRH;
    /** TVPE Iteration Control Low Register                            - 0x8c2 **/
    unsigned short int usiTVILRL;
    /** TVPE Trace Back Control Register                               - 0x8c4 **/
    unsigned int  uliTVTBLR;
    /** TVPE Config Wakeup Control Register                            - 0x8c8 **/
    unsigned int  uliTVCWLR;
    /** Reserved 9                                                     - 0x8cc to 0x8cf **/
    unsigned char ucReserved9[4];
    /** TVPE General Interleaver Control Register                      - 0x8d0 **/
    unsigned int  uliTVGILR;
    /** Reserved 10                                                    - 0x8d4 to 0x8ff **/
    unsigned char ucReserved10[44];
    /** TVPE Interleaver Intra Row Lookup Table x Control Register     - 0x900 to 0x9ff **/
    unsigned int  uliTVIIRLTxLR[64];
    /** TVPE Reset Path Metric Control Register                        - 0xa00 **/
    unsigned int  uliTVRPMLR;
    /** TVPE Reset Yamamoto Control Register                           - 0xa04 **/
    unsigned int  uliTVRYMLR;
    /** TVPE Stop Control Register                                     - 0xa08 **/
    unsigned int  uliTVSTOPLR;
    /** Reserved 11                                                    - 0xa0c to 0xa0f **/
    unsigned char ucReserved11[4];
    /** TVPE Log MAP Algorithm Control Register                        - 0xa10 **/
    unsigned int  uliTVLMALR;
    /** TV Task ID Control Register                                    - 0xa14 **/
    unsigned int  uliTVTIDLR;
    /** TVPE Interleaver Inter Row Permutation x Control Register      - 0xa18 to 0xa47 **/
    unsigned int  uliTVINTIRPxLR[12];
    /** Reserved 12                                                    - 0xa48 to 0xa5f **/
    unsigned char ucReserved12[24];
    /** TVPE CDL Puncturing Vector High Control Register               - 0xa60 **/
    unsigned int  uliTVCPVHLR;
    /** TVPE CDL Puncturing Vector Low Control Register                - 0xa64 **/
    unsigned int  uliTVCPVLLR;
    /** TVPE CDL Puncturing Period Control Register                    - 0xa68 **/
    unsigned int  uliTVCPPLR;
    /** TVPE CDL Data Extraction Control Register                      - 0xa6c **/
    unsigned int  uliTVCDELR;
    /** TVPE CDL Systematic Rate De-Matching 0 Control Register        - 0xa70 **/
    unsigned int  uliTVCSRDM0LR;
    /** TVPE CDL Systematic Rate De-Matching 1 Control Register        - 0xa74 **/
    unsigned int  uliTVCSRDM1LR;
    /** TVPE CDL Parity-First-0(PF0) Rate De-Matching 0 Control Register - 0xa78 **/
    unsigned int  uliTVCPF0RDM0LR;
    /** TVPE CDL Parity-First-0(PF0) Rate De-Matching 1 Control Register - 0xa7c **/
    unsigned int  uliTVCPF0RDM1LR;
    /** TVPE Parity-Second-0(PS0) Rate De-Matching 0 Control Register  - 0xa80 **/
    unsigned int  uliTVPS0RDM0LR;
    /** TVPE Parity-Second-0(PS0) Rate De-Matching 1 Control Register  - 0xa84 **/
    unsigned int  uliTVPS0RDM1LR;
    /** Reserved 13                                                    - 0xa88 to 0xa8b **/
    unsigned char ucReserved13[4];
    /** TVPE Start Control Register                                    - 0xa8c **/
    unsigned int  uliTVSTARTLR;
    /** TVPE Reset Super-Parent bits Control Register                  - 0xa90 **/
    unsigned int  uliTVRSPLR;
    /** TVPE CDL Channel Data Interleaver Control Register             - 0xa94 **/
    unsigned int  uliTVCCDLILR;
    /** Reserved 14                                                    - 0xa98 to 0xa9b **/
    unsigned char ucReserved14[4];
    /** TVPE Interleaver Init Enable Control Register                  - 0xa9c **/
    unsigned int  uliTVIIELR;
    /** Reserved 15                                                    - 0xaa0 to 0xaff **/
    unsigned char ucReserved15[96];
    /** TVPE Interleaver Specific Control Register                     - 0xb00 **/
    unsigned int  uliTVISLR;
    /** TVPE Hard Outpu Offset Control Register                        - 0xb04 **/
    unsigned int  uliTVHOOLR;
    /** TVPE Interleaver Fast Forward Control Register                 - 0xb08 **/
    unsigned int  uliTVIFFLR;
    /** TVPE NII Data Prefetch Control Register                        - 0xb0c **/
    unsigned int  uliTVNDPLR;
    /** Reserved 16                                                    - 0xb10 to 0xb13 **/
    unsigned char ucReserved16[4];
    /** TVPE Interleaver Auto Set Control Register                     - 0xb14 **/
    unsigned int  uliTVIASLR;
    /** TVPE CRC Initial Value Control Register                        - 0xb18 **/
    unsigned int  uliTVCIVLR;
    /** Reserved 17                                                    - 0xb1c to 0xb1f **/
    unsigned char ucReserved17[4];
    /** TVPE HARQ Task ID Control Register                             - 0xb20 **/
    unsigned int  uliTVHTIDLR;
    /** TVPE HARQ Input Control Register                               - 0xb24 **/
    unsigned int  uliTVHILR;
    /** TVPE HARQ Weights Control Register                             - 0xb28 **/
    unsigned int  uliTVHWLR;
    /** TVPE HARQ Data Ready Control Register                          - 0xb2c **/
    unsigned int  uliTVHDRLR;
    /** TVPE CDL Size 1 HARQ Stage Control Register                    - 0xb30 **/
    unsigned int  uliTVCS1HLR;
    /** TVPE CDL Size 2 HARQ Stage Control Register                    - 0xb34 **/
    unsigned int  uliTVCS2HLR;
    /** TVPE Command HARQ Stage Control Register                       - 0xb38 **/
    unsigned int  uliTVPECMDHLR;
    /** TVPE CDL Task ID Control Register                              - 0xb3c **/
    unsigned int  uliTVCTIDLR;
    /** TVPE CDL Size 1 Control Register                               - 0xb40 **/
    unsigned int  uliTVCS1LR;
    /** TVPE CDL Size 2 Control Register                               - 0xb44 **/
    unsigned int  uliTVCS2LR;
    /** TVPE CDL Interleaver Rows Control Register                     - 0xb48 **/
    unsigned int  uliTVCINTRLR;
    /** TVPE CDL Interleaver Columns Control Register                  - 0xb4c **/
    unsigned int  uliTVCINTLR;
    /** TVPE CDL Interleaver Block Control Register                    - 0xb50 **/
    unsigned int  uliTVCINTBLR;
    /** Reserved 18                                                    - 0xb54 to 0xb5f **/
    unsigned char ucReserved18[12];
    /** TVPE CDL Interleaver Initial Intra Row Pointer x Control Register - 0xb60 to 0xb97 **/
    unsigned int  uliTVCDLIIRPxLR[14];
    /** TVPE CDL General Interleaver Control Register                  - 0xb98 **/
    unsigned int  uliTVCGILR;
    /** Reserved 19                                                    - 0xb9c to 0xbcb **/
    unsigned char ucReserved19[48];
    /** TVPE CDL Interleaver Inter Row Permutation x Control Register  - 0xbcc to 0xbd3 **/
    unsigned int  uliTVCIIRLTxLR[2];
    /** TVPE CDL Interleaver Auto Set Control Register                 - 0xbd4 **/
    unsigned int  uliTVCIASLR;
    /** TVPE Command CDL Stage Control Register                        - 0xbd8 **/
    unsigned int  uliTVPECMDCLR;
    /** TVPE Log MAP Algorithm HARQ stage Control Register             - 0xbdc **/
    unsigned int  uliTVLMAHLR;
    /** TVPE Log MAP Algorithm CDL stage Control Register              - 0xbe0 **/
    unsigned int  uliTVLMACLR;
    /** Reserved 20                                                    - 0xbe4 to 0xbe7 **/
    unsigned char ucReserved20[4];
    /** TVPE Iteration HARQ stage Control Register                     - 0xbe8 **/
    unsigned int  uliTVIHLR;
    /** TVPE Iteration CDL stage Control Register                      - 0xbec **/
    unsigned int  uliTVICLR;
    /** TVPE HARQ Control Register                                     - 0xbf0 **/
    unsigned int  uliTVHLR;
    /** TVPE Control Register                                          - 0xbf4 **/
    unsigned int  uliTVIBLR;
    /** TVPE Control Register                                          - 0xbf8 **/
    unsigned int  uliTVOBLR;
    /** TVPE Control Register                                          - 0xbfc **/
    unsigned int  uliTVCIBLR;
    /** TVPE Control Register                                          - 0xc00 **/
    unsigned int  uliTVCOBLR;
    /** TVPE HOL Buffer Read Control Register                          - 0xc04 **/
    unsigned int  uliTVHBRLR;
    /** TVPE CDL Vector Streaming Batch Start Control Register         - 0xc08 **/
    unsigned int  uliTVCVSBSLR;
    /** TVPE CDL Size 3 Control Register                               - 0xc0c **/
    unsigned int  uliTVCS3LR;
    /** TVPE Command 2 Control Register                                - 0xc10 **/
    unsigned int  uliTVPECMD2LR;
    /** TVPE Command 2 HARQ Stage Control Register                     - 0xc14 **/
    unsigned int  uliTVPECMD2HLR;
    /** TVPE Command 2 CDL Stage Control Register                      - 0xc18 **/
    unsigned int  uliTVPECMD2CLR;
    /** TVPE CDL Interleaver Specific Control Register                 - 0xc1c **/
    unsigned int  uliTVCISLR;
    /** TVPE CDL Puncturing Vector High HARQ Stage Control Register    - 0xc20 **/
    unsigned int  uliTVCPVHHLR;
    /** TVPE CDL Puncturing Vector Low HARQ Stage Control Register     - 0xc24 **/
    unsigned int  uliTVCPVLHLR;
    /** TVPE CDL Puncturing Period HARQ Stage Control Register         - 0xc28 **/
    unsigned int  uliTVCPPHLR;
    /** TVPE Systematic Rate De-Matching 2 Control Register            - 0xc2c **/
    unsigned int  uliTVCSRDM2LR;
    /** TVPE HARQ Task ID 0 Control Register                           - 0xc30 **/
    unsigned int  uliTVHTID0LR;
    /** Reserved 21                                                    - 0xc34 to 0xc37 **/
    unsigned char      ucReserved21[4];
    /** TVPE HARQ Task ID 1 Control Register                           - 0xc38 **/
    unsigned int  uliTVHTID1LR;
    /** Reserved 22                                                    - 0xc3c to 0xc5b **/
    unsigned char ucReserved22[32];
    /** TVPE Hard Output Offset HARQ Stage Control Register            - 0xc5c **/
    unsigned int  uliTVHOOHLR;
    /** TVPE Hard Output Offset CDL Control Register                   - 0xc60 **/
    unsigned int  uliTVHOOCLR;
    /** TVPE CRC Init Value HARQ Stage Control Register                - 0xc64 **/
    unsigned int  uliTVCIVHLR;
    /** TVPE CRC Init Value CDL Stage Control Register                 - 0xc68 **/
    unsigned int  uliTVCIVCLR;
     /** TVPE CDL DMA Start Control Register                           - 0xc6c **/
    unsigned int  uliTVCSDLR;
    /** TVPE CDL Size 3 HARQ Stage Control Register                    - 0xc70 **/
    unsigned int  uliTVCS3HLR;
    /** Reserved 23                                                    - 0xc74 to 0xc77 **/
    unsigned char ucReserved23[4];
    /** TVPE HARQ Buffer Read Control Register                         - 0xc78 **/
    unsigned int  uliTVHRBRLR;
    /** TVPE HARQ EDCH Active Control Register                         - 0xc7c **/
    unsigned int  uliTVHELVLR;
    /** TVPE CDL Interleaver Intra Row Step x Control Register         - 0xc80 to 0xcaf **/
    unsigned int  uliTVCIIRSxLR[12];
    /** TVPE HARQ Systematic Rate De-Matching 0 Control Register       - 0xcb0 **/
    unsigned int  uliTVHSRDM0LR;
    /** TVPE HARQ Systematic Rate De-Matching 1 Control Register       - 0xcb4 **/
    unsigned int  uliTVHSRDM1LR;
    /** TVPE HARQ Systematic Rate De-Matching 2 Control Register       - 0xcb8 **/
    unsigned int  uliTVHSRDM2LR;
    /** TVPE HARQ Parity-First-0 PF0 Rate De-Matching 0 Control Register - 0xcbc **/
    unsigned int  uliTVHPF0RDM0LR;
    /** TVPE HARQ Parity-First-0 PF0 Rate De-Matching 1 Control Register - 0xcc0 **/
    unsigned int  uliTVHPF0RDM1LR;
    /** TVPE HARQ Parity-First-0 PF0 Rate De-Matching 2 Control Register - 0xcc4 **/
    unsigned int  uliTVHPF0RDM2LR;
    /** TVPE HARQ Parity-Second-0 PS0 Rate De-Matching 0 Control Register - 0xcc8 **/
    unsigned int  uliTVHPS0RDM0LR;
    /** TVPE HARQ Parity-Second-0 PS0 Rate De-Matching 1 Control Register - 0xccc **/
    unsigned int  uliTVHPS0RDM1LR;
    /** TVPE HARQ Parity-Second-0 PS0 Rate De-Matching 2 Control Register - 0xcd0 **/
    unsigned int  uliTVHPS0RDM2LR;
    /** TVPE HARQ EDCH Finish Control Register                         - 0xcd4 **/
    unsigned int  uliTVHFDRLR;
    /** Reserved 24                                                    - 0xcd8 to 0xcff **/
    unsigned char ucReserved24[40];
    /** TVPE Decoder Status Register                                   - 0xd00 **/
    unsigned int  uliTVPESR;
    /** TVPE Event Status Register                                     - 0xd04 **/
    unsigned int  uliTVPEESR;
    /** TVPE Internal Configuration Status Register                    - 0xd08 **/
    unsigned int  uliTVPEICSR;
    /** Reserved 25                                                    - 0xd0c to 0xd0f **/
    unsigned char      ucReserved25[4];
    /** TVPE Aposterior Quality Counter x Status Register              - 0xd10 to 0xd1f **/
    unsigned int  uliTVAQCxSR[4];
    /** TVPE Revision ID Status Register                               - 0xd20 **/
    unsigned int  uliTVREVIDSR;
    /** TVPE HARQ Event Status Register                                - 0xd24 **/
    unsigned int  uliTVHESR;
    /** TVPE CDL Event Status Register                                 - 0xd28 **/
    unsigned int  uliTVCESR;
    /** TVPE CRC Result Value Status Register                          - 0xd2c **/
    unsigned int  uliTVCRVSR;
    /** Reserved 26                                                    - 0xd30 to 0xd33 **/
    unsigned char ucReserved26[4];
    /** TVPE HARQ Saturation Status Register                           - 0xd34 **/
    unsigned int  uliTVHSSR;
    /** Reserved 27                                                    - 0xd38 to 0xd3f **/
    unsigned char ucReserved27[8];
    /** TVPE Tail Systematic Non Interleaved Status Register           - 0xd40 **/
    unsigned int  uliTVTAILSNSR;
    /** TVPE Tail Parity Non Interleaved Status Register               - 0xd44 **/
    unsigned int  uliTVTAILPNSR;
    /** TVPE Tail Systematic Interleaved Status Register               - 0xd48 **/
    unsigned int  uliTVTAILSISR;
    /** TVPE Tail Parity Interleaved Status Register                   - 0xd4c **/
    unsigned int  uliTVTAILPISR;
    /** TV Private Rows and Columns x Status Register                  - 0xd50 to 0xd63 **/
    unsigned int  uliTVPRCxSR[5];
    /** TV Write Queues x Status Register                              - 0xd64 to 0xd6b **/
    unsigned int  uliTVWQxSR[2];
 } t_TVPE;
typedef struct DEPE {
    /** DEPE Configuration Register            - 0x0 **/
    unsigned int  uliDECR;
    /** DEPE Input Buffer Status Register      - 0x4 **/
    unsigned int  uliDEIBSR;
    /** DEPE Output Buffer Status Register     - 0x8 **/
    unsigned int  uliDEOBSR;
    /** DEPE Interrupt Counter Status Register - 0xc **/
    unsigned int  uliDESR;
    /** DEPE Pre Fetch Status Register #0      - 0x10**/
    unsigned int  uliDEPFSR0;
    /** DEPE Pre Fetch Status Register #1      - 0x14**/
    unsigned int  uliDEPFSR1;
    /** DEPE Pre Fetch Status Register #2      - 0x18**/
    unsigned int  uliDEPFSR2;
    /** DEPE Pre Fetch Status Register #3      - 0x1c**/
    unsigned int  uliDEPFSR3;
    /** Reserved 1                             - 0x20 to 0x7ff **/
    unsigned char     ucReserved1[2016];
    /** DEPE RM FIFO Status Register           - 0x800 **/
    unsigned int  uliDERMFSR;
 } t_DEPE;
typedef struct FTPE {
    /** FTPE Partitioning Register                                     - 0x0 to 0x3f/0xff **/
    unsigned int  uliPRTxR[64];
    /** Reserved1                                                      - 0x100 to 0x13f **/
    unsigned char      ucReserved1[64];
    /** FTPE Data Size Register                                        - 0x140 to 0x14b **/
    unsigned short int usiDSxR[6];
    /** Reserved2                                                      - 0x14c to 0x16f **/
    unsigned char      ucReserved2[36];
    /** FTPE Data Read Size Register                                   - 0x170 **/
    unsigned int usiRSR;
     /** Reserved3                                                     - 0x174 to 0x18f **/
    unsigned char      ucReserved3[28];
    /** FTPE Interrupt Type Register1                                  - 0x190 **/
    unsigned int  uliITR1;
    /** FTPE Interrupt Type Register2                                  - 0x190 **/
    unsigned int  uliITR2;
    /** FTPE Interrupt Enable Register                                 - 0x198 **/
    unsigned int  uliIER;
    /** FTPE ECC enable Register                                       - 0x19c **/
    unsigned int  uliEER;
    /** FTPE Interrupt configuration Register                          - 0x1a0 **/
    unsigned int  uliCR;
    /** Reserved4                                                      - 0x1a4 to 0x1ff **/
    unsigned char      ucReserved4[92];
    /** FTPE Command Register                                          - 0x200 **/
    unsigned int  uliCMDR;
    /** FTPE Attributes Register                                       - 0x204 **/
    unsigned int  uliAR;
    /** FTPE Scaling Register1                                         - 0x208 **/
    unsigned int  uliSCLR1;
    /** FTPE Scaling Register2                                         - 0x20c **/
    unsigned int  uliSCLR2;
    /** FTPE Task ID Register                                          - 0x210 **/
    unsigned int  uliTIDR;
    /** FTPE PoST Multiply Register                                    - 0x214 **/
    unsigned int  uliPSTMR;
    /** FTPE TMem Address Register                                     - 0x218 **/
    unsigned int  uliTMAR;
    /** FTPE Frequency Correction Base Register                        - 0x21c **/
    unsigned int  uliFCBR;
    /** FTPE Frequency Correction Control Register                     - 0x220 **/
    unsigned int  uliFCCR;
    /** FTPE Frequency Correction step counter input state Register    - 0x224 **/
    unsigned int  uliFCSCISR;
    /** FTPE Frequency Correction shift Register                       - 0x228 **/
    unsigned int  uliFCSR;
    /** FTPE  Code Generation Register1                                - 0x22c **/
    unsigned int  uliCGR1;
    /** FTPE  Code Generation Register2                                - 0x230 **/
    unsigned int  uliCGR2;
    /** FTPE  Code Generation Register3                                - 0x234 **/
    unsigned int  uliCGR3;
    /** FTPE  Zero Padding  Register                                   - 0x238 **/
    unsigned int  uliZPR;
    /** FTPE  Scaling input Register                                   - 0x23c -0x244 **/
    unsigned int  auliSCLIOR[3];
    /** FTPE Interrupt Register                                        - 0x248 **/
    unsigned int  uliIR;
    /** FTPE Status Register                                           - 0x24c **/
    unsigned int  uliSTR;
    /** FTPE Scaling Status Register1                                  - 0x250 **/
    unsigned int  uliSCLSTR1;
    /** FTPE Scaling Status Register2                                  - 0x254 **/
    unsigned int  uliSCLSTR2;
    /** FTPE Saturation Status Register x                              - 0x258 to 0x268 **/
    unsigned int  uliSSTRx[5];
    /** FFTPE TMem Data Register                                       - 0x26c **/
    unsigned int  uliTMDR;
    /** DFTPE Overall Scalling Status Registerx                        - 0x270 to 0x278 **/
    unsigned int  auliOSCLSTR[3];
    /** FTPE iteration saturation status Register                     - 0x27c **/
    unsigned int  uliITSTSR;
    /** FTPE Frequency Correction State Registerx                     - 0x280 to 0x28c **/
    unsigned int  auliFCSTR[4];
    MAPLERESERVED(0x290,0x2b0);
    /** FTPE Frequency Correction Step Counter State Registerx        - 0x2b0 to 0x2bc **/
    unsigned int  auliFCSCSR[4];
    MAPLERESERVED(0x2c0,0x2e0);
    /** FTPE Code Generation State Registerx                          - 0x2e0 to 0x2ec **/
    unsigned int  auliECGSR[4];
     MAPLERESERVED(0x2f0,0x310);
    /** EFTPE stage status register     - 0x310 **/
    unsigned int uliSTGSTR;
} t_FTPE;
typedef struct EQPE {
    /** EQPE Command Register                 - 0x0 **/
    unsigned int  uliEQCMD;
    /** EQPE Parameter Register               - 0x4 **/
    unsigned int  uliEQPARAM;
    /** EQPE Alignment Value Register         - 0x8 **/
    unsigned int  uliEQALIGNVAL;
    /** EQPE Data Configuration Register      - 0xc **/
    unsigned int  uliEQDATACFG;
    /** EQPE Scaling Register                 - 0x10 **/
    unsigned int  uliEQSCL;
    /** EQPE H Buffer Pointer Register        - 0x14 **/
    unsigned int  uliEQHPTR;
    /** EQPE Output Buffer Pointer 0 Register - 0x18 **/
    unsigned short int usiEQOUTPTR0;
    /** EQPE Output Buffer Pointer 1 Register - 0x1a **/
    unsigned short int usiEQOUTPTR1;
    /** EQPE Output Buffer Pointer 2 Register - 0x1c **/
    unsigned short int usiEQOUTPTR2;
    /** EQPE Output Buffer Pointer 3 Register - 0x1d **/
    unsigned short int usiEQOUTPTR3;
    /** EQPE Y Buffer Pointer Register        - 0x20 **/
    unsigned int  uliEQYPTR;
    /** EQPE Task ID Register                 - 0x24 **/
    unsigned int  uliEQTASKID;
    /** EQPE Rank Reduction 0 Register        - 0x28 **/
    unsigned int  uliEQRANK0;
    /** EQPE Rank Reduction 1 Register        - 0x2c **/
    unsigned int  uliEQRANK1;
    /** EQPE Feedback Scale x Register        - 0x30 to 0x3f **/
    unsigned int  uliEQFSCL[4];
    /** Reserved 1                            - 0x40 to 0x1ff **/
    unsigned char      ucReserved1[448];
    /** EQPE Interrupt Configuration Register - 0x200 **/
    unsigned int  uliEQINTCFG;
    /** EQPE Interrupt Parameter Register     - 0x204 **/
    unsigned int  uliEQINTPARAM;
    /** EQPE Threshold Register               - 0x208 **/
    unsigned int  uliEQTHRESH;
    /** EQPE ECC Enable Register              - 0x20c **/
    unsigned int  uliEQECC;
    /** Reserved 2                            - 0x210 to 0x21f **/
    unsigned char      ucReserved2[16];
    /** EQPE QPSK Factor Register             - 0x220 **/
    unsigned int  uliEQFACTORQPSK;
    /** EQPE 16QAM Factor Register            - 0x224 **/
    unsigned int  uliEQFACTOR16QAM;
    /** EQPE 64QAM Factor Register            - 0x228 **/
    unsigned int  uliEQFACTOR64QAM;
    /** Reserved 3                            - 0x22c to 0x22f **/
    unsigned char      ucReserved3[4];
    /** EQPE QPSK Mapping Register            - 0x230 **/
    unsigned int  uliEQMAPQPSK;
    /** Reserved 4                            - 0x234 to 0x23f **/
    unsigned char      ucReserved4[12];
    /** EQPE 16QAM Mapping x Register         - 0x240 to 0x24f **/
    unsigned int  uliEQMAP16QAM[4];
    /** EQPE 64QAM Mapping x Register         - 0x250 to 0x28f **/
    unsigned int  uliEQMAP64QAM[16];
    /** Reserved 5                            - 0x290 to 0x2ff **/
    unsigned char      ucReserved5[112];
    /** EQPE Event Register                   - 0x300 **/
    unsigned int  uliEQEVENT;
    /** EQPE Buffer Control Register          - 0x304 **/
    unsigned int  uliEQBUFCTRL;
    /** EQPE ECC Event Register               - 0x308 **/
    unsigned int  uliEQECCEVENT;
    /** EQPE SING                             - 0x30c **/
    unsigned int  uliEQSING;
    /** EQPE Status Register                  - 0x310 **/
    unsigned int  uliEQSTATUS;
    /** Reserved 6                            - 0x314 to 0x31f **/
    unsigned char      ucReserved6[12];
    /** EQPE Outpt Scale Register             - 0x320 **/
    unsigned int  uliEQOUTSCL[16];
} t_EQPE;
typedef struct PUPE {
    /** PUPE Command Register                                - 0x0 **/
    unsigned int uliCMDR;
    /** PUPE Control Parameters Register                     - 0x4 **/
    unsigned int uliCPR;
    /** PUPE Control Size Register                           - 0x8 **/
    unsigned int uliCSR;
    /** PUPE DTX Threshold Register                          - 0xc **/
    unsigned int uliDTR;
    /** PUPE Received Signal Energy Factor Register x        - 0x10 to 0x27 **/
    unsigned int auliRSEFR[6];
    /** Reserved1                                            - 0x28 to 0x2f **/
    unsigned char ucReserved1[8];
    /** PUPE Received Signal Energy Factor Scale Register x  - 0x30 to 0x3b **/
    unsigned int auliRSEFSR[3];
    /** Reserved2                                            - 0x3c to 0x3f**/
    unsigned char ucReserved2[4];
    /** PUPE Noise Factor Register x                         - 0x40 to 0x57 **/
    unsigned int auliSFR[6];
    /** Reserved3                                            - 0x58 to 0x5f **/
    unsigned char ucReserved3[8];
    /** PUPE Noise Factor Scale Register x                   - 0x60 to 0x6b **/
    unsigned int auliSFSR[3];
    /** Reserved4                                            - 0x6c to 0x77 **/
    unsigned char ucReserved4[12];
    /** PUPE DeMapper Control Register                       - 0x78 **/
    unsigned int uli;
    /** PUPE DeScrambler Control Register                    - 0x7c **/
    unsigned int uliDSR;
    /** PUPE Output Control Register                         - 0x80 **/
    unsigned int uliOCR;
    /** Reserved5                                            - 0x84 to 0x93 **/
    unsigned char ucReserved5[16];
    /** PUPE Task ID Register                                - 0x94 **/
    unsigned int uliTIDR;
    /** PUPE ECC Error Generation Register                   - 0x98 **/
    unsigned int uliEEGR;
    /** Reserved6                                            - 0x9c to 0x9f **/
    unsigned char ucReserved6[4];
    /** PUPE Event Register                                  - 0xa0 **/
    unsigned int uliER;
    /** PUPE Status Register                                 - 0xa4 **/
    unsigned int uliSR;
    /** PUPE ECC Status Register                             - 0xa8 **/
    unsigned int uliESR;
    /** PUPE RI Status Register x                            - 0xac to 0xbb **/
    unsigned int auliRSR[4];
    /** PUPE ACK Status Register                             - 0xbc **/
    unsigned int uliASR;
    /** Reserved7                                            - 0xc0 to 0xd7 **/
    unsigned char ucReserved7[24];
    /** PUPE Event Control Register                          - 0xd8 **/
    unsigned int uliECR;
    /** PUPE DeMapper Configuration Register                 - 0xdc **/
    unsigned int uliDMCR;
    /** Reserved8                                            - 0xe0 **/
    unsigned char ucReserved8[4];
    /** PUPE General Configuration Register                  - 0xe4 **/
    unsigned int uliGCR;
    /** PUPE QPSK Factor Register                            - 0xe8 **/
    unsigned int uliQPSKFR;
    /** PUPE 16QAM Factor Register                           - 0xec **/
    unsigned int uli16QAMFR;
    /** PUPE 64QAM Factor Register                           - 0xf0 **/
    unsigned int uli64QAMFR;
} t_PUPE;
typedef struct PDPE {
    /** PDPE CW SF0 parameters                               - 0x10000 to 0x10fff **/
    unsigned int uliCWxSF0P[PDPE_NUM_CW][8];
    /** PDPE CW SF1 parameters                               - 0x11000 to 0x11fff **/
    unsigned int uliCWxSF1P[PDPE_NUM_CW][8];
    /** PDPE Symbol CRE parameters Even Symbol               - 0x12000 to 0x127ff **/
    unsigned int uliCRE0T0CP[PDPE_NUM_CRE][4];
    /** PDPE Symbol CRE parameters Odd Symbol                - 0x12800 to 0x12fff **/
    unsigned int uliCRE0T1CP[PDPE_NUM_CRE][4];
    /** PDPE Slot CRE parameters Slot 0                      - 0x13000 to 0x137ff **/
    unsigned int uliCRE0T0REP[PDPE_NUM_CRE][4];
    /** PDPE Slot CRE parameters Slot 1                      - 0x13800 to 0x13fff **/
    unsigned int uliCRE0T1REP[PDPE_NUM_CRE][4];
    /** PDPE Mapping parameter table Slot 0                  - 0x14000 to 0x1407f **/
    unsigned int uliRB0MPT[32];
    /** PDPE Mapping parameter table Slot 1                  - 0x14080 to 0x140ff **/
    unsigned int uliRB1MPT[32];
} t_PDPE;
typedef struct PDPEControl {
    /** Reserved                                             - 0x1e400 to 0x1ebff **/
    unsigned char     ucReserved1[2048];
    /** PDPE Sub-Frame 0 Control Register                    - 0x1ec00 **/
    unsigned int uliPSF0FLR;
    /** PDPE Sub-Frame 1 Control Register                    - 0x1ec04 **/
    unsigned int uliPSF1FLR;
    /** PDPE Command Entry Control Register0                 - 0x1ec08 **/
    unsigned int uliPCE0LR;
    /** PDPE Command Entry Control Register1                 - 0x1ec0c **/
    unsigned int uliPCE1LR;
    /** Reserved                                             - 0x1ec10 **/
    unsigned int uliReserved2;
    /** PDPE Slot Trigger Control Register                   - 0x1ec14 **/
    unsigned int uliPSLTLR;
    /** PDPE OFDM Symbol Trigger Control Register            - 0x1ec18 **/
    unsigned int uliPOSTLR;
    /** Reserved                                             - 0x1ec1c to 0x1eeff **/
    unsigned char     ucReserved3[996];
    /** PDPE Pointer Status register                         - 0x1f000 **/
    unsigned int uliPPSR;
    /** PDPE Event Status register                           - 0x1f004 **/
    unsigned int uliPESR;
    /** PDPE Event Scenario Status register                  - 0x1f008 **/
    unsigned int uliPESSR;
    /** PDPE Status register                                 - 0x1f00a **/
    unsigned int uliPSR;
} t_PDPEControl;
typedef struct CRPEULB {
    /**   General                            - 0x0 **/
    unsigned int uliCRUBGCR;
    /**  Event Mask  0x4 **/
    unsigned int uliCRUBEMCR;
    /**  Mem Clear  0x8 **/
    unsigned int uliCRUBMCCR;
    MAPLERESERVED(0xc,0x10);
    /**  Interpolation weights1 - 0x10 to 0x2c**/
    unsigned int auliCRUBIW1SxCR[8];
     /**  Interpolation weights1 - 0x30 to 0x4c**/
    unsigned int auliCRUBIW2SxCR[8];
    /** Group First Antenna x Configuration Register  0x50 to 0x64 **/
    unsigned char aucCRUBGFAxCR[24];
     /** Group Number Of Antenna  x Configuration Register  0x68 to 0x7c **/
    unsigned char aucCRUBGNOAxCR[24];
    MAPLERESERVED(0x80, 0x400);
    /** start register                                                                 - 0x400 **/
    unsigned int uliCRUBSLR;
    /** General x Control Register                                         - 0x404 to 0x408 **/
    unsigned int auliCRUBGxLR[CRPE_ULB_NUM_OF_IBS];
    MAPLERESERVED(0x40c, 0x410);
    /** PCH Commands Queue Full Control Register               - 0x410 **/
    unsigned int uliCRUBPCQFLR;
    MAPLERESERVED(0x414,0x800);
    /**   Event Status Register                  - 0x800 **/
    unsigned int uliCRUBESR;
    /**    FCQ Status Register                                - 0x804 **/
    unsigned int uliCRUBFCQSR;
    MAPLERESERVED(0x808,0x810);
    /** Output Parameters 0 Status Register   - 0x810 - 0x817  **/
    unsigned int auliCRUBOP0SR[2];
    MAPLERESERVED(0x818,0x820);
    /** Output Sat Counter Status Register      - 0x820 **/
    unsigned int  uliCRUBOSCSR;
    /** Interpolation Sat Counter Status Register  - 0x824 **/
    unsigned int  uliCRUBISCSR;
    /** Mem Clear Status Register                     - 0x828 **/
    unsigned int  uliCRUBMCSR;
    MAPLERESERVED(0x82c,0x830);
    /**  Output Parameters 1 Status Register - 0x830 to 0x837**/
    unsigned int auliCRUBOP1SR[2];
} t_CRPEULB;
typedef struct CRPEULF {
    /** General Configuration Register  0x0    **/
    unsigned int uliULFGCR;
    MAPLERESERVED(0x4,0xc);
    /** seconddary configuration register    0xc  **/
    unsigned int uliULFSCR;
    MAPLERESERVED(0x10,0x80);
    /** Interpolation Configuration Register #x  0x80 to 0xbc   **/
    unsigned int auliULFICR0 ;
    unsigned int auliULFICR1 ;
    MAPLERESERVED(0x88,0x90);
    unsigned int auliULFICR2 ;
    unsigned int auliULFICR3 ;
    MAPLERESERVED(0x98,0xa0);
    unsigned int auliULFICR4 ;
    unsigned int auliULFICR5 ;
    MAPLERESERVED(0xa8,0xb0);
    unsigned int auliULFICR6 ;
    unsigned int auliULFICR7 ;
    MAPLERESERVED(0xb8,0x100);
    /** Output Buffer #x Base Configuration & Attributes Configuration  Register  0x100 to 0x18c   **/
    unsigned int auliULFOB0BCRx[12][2];
    MAPLERESERVED(0x160,0x280);
    /** Event Status Register  0x280    **/
    unsigned int uliULFESR;
    MAPLERESERVED(0x284,0x290);
    /** Commands FIFO Status Register  0x290    **/
    unsigned int uliULFCFSR;
    /** IB Status Register  0x294    **/
    unsigned int uliULFIBSRIB;
    /** Time Status Register  0x298    **/
    unsigned int uliULFTSR;
    /** ECCStatus Register  0x29c    **/
    unsigned int uliULFECCSR;
} t_CRPEULF;
typedef struct CRPEDL {
    /**   Beam Forming Coefficients Values Control 0-7  0x0 to 0x7c    **/
    unsigned int auliCRDBFCVx[8][4];
    /**    Start Control Register   0x080      **/
    unsigned int uliCRDSLR;
    /**    TPC Command Control Register 0x084      **/
    unsigned int uliCDTCLR;
    /**    SLOT Command Control Register  0x088      **/
    unsigned int uliCDSCLR;
    MAPLERESERVED(0x8c,0x90);
    /**  Virtual Antennas Gain Command Control  0x090      **/
    unsigned int uliCDVGCLR;
    MAPLERESERVED(0x94,0x98);
    /**  General Command Control Register 0x98 to 0x9c **/
    unsigned int uliCDGCLR0;
    unsigned int uliCDGCLR1;
    /**    Idle Period Control  0xa0 to  0xdc  **/
    unsigned int auliCRDIPCT0[16];
    /**   Beam Forming Coefficients Timing Command  0x0E0 to 0xec      **/
    unsigned int auliCRDBFCTx[4];
    /**     Combined Chips Shift Command 0xf0 to 0xfc **/
    unsigned int auliCRDCSCTx[4];
    /**     Downlink Event Status 0x100      **/
    unsigned int uliCRDESR;
    /** Processing Stage Status Register   0x104  **/
    unsigned int uliCPSSR;
    /** Downlink ECC Status Register 0x108  **/
    unsigned int uliCRDECCSR;
    /** downlink rate control register   0x10c  **/
    unsigned int uliCRDRCR;
    /** CRPE Downlink Processing Stage 1xRTT Status Register 0x110 **/
    unsigned int uliCPSRSR;
    /** CRPE Downlink Processing Stage EVDO Status Register 0x114 **/
    unsigned int uliCPSESR;
    MAPLERESERVED(0x118,0x140);
    /**     CRPE Downlink Event Mask Register 0x140      **/
    unsigned int uliCRDEMCR;
    /**     Output Mode Configuration Register  0x144      **/
    unsigned int uliCRDOMCR;
    /**   Normalization Value Configuration Register   0x148      **/
    unsigned int uliCRDNVCR;
    /** CRPE Downlink 1xRTT short PN code offsets 0x14c-0x14f **/
    unsigned int uliCRDSCOXLR ;
    /** CRPE Downlink EVDO short PN code offsets  0x150-0x153 **/
    unsigned int uliCRDSCOELR ;
    /** CRPE Downlink Beacon 1XRTT short PN code offsets 0x154-0x157 **/
    unsigned int uliCRDSCOBXLR ;
    /** CRPE Downlink Beacon EVDO short PN code offsets  0x158-0x15b **/
    unsigned int uliCRDSCOBELR ;
} t_CRPEDL;
typedef struct CRPEDL_channel_vag {
   unsigned short int usiFRTRG;
   unsigned short int usiTRDRG;
   unsigned short int usiSNDRG;
   unsigned short int usiFSTRG;
   unsigned char ucReserved[7];
   unsigned char ucIQGAB;
} t_CRPEDL_channel_vag;
typedef struct CRPEDL_channel_vags{
   t_CRPEDL_channel_vag CRPEDL_channel_vag[263];
} t_CRPEDL_channel_vags;

typedef struct CGPEREG {
    /**  CG_CFG_1  0x0 **/
    unsigned int uliPN_NUM;
    /**  CG_CFG_2  0x4 **/
    unsigned int uliSF_START;
    /**  CG_CFG_3  0x8 **/
    unsigned int uliCG_CFG_3;
} t_CGPEREG;

typedef struct FTPEContext {
    /** Index to TL **/
    unsigned short int ausiIndexToTL[64];
    /** Index to Offset **/
    unsigned char      aucIndexToOffset[64];
    /** Index to DS register offset **/
    unsigned char      aucIndexToDS[64];
} t_FTPEContext;
typedef struct TVPEBD {
     /** OWNER/WRAP/DC/INT_EN/L_MAP/YAM_EN/VIT_K/TBZE/ZTTB/ENC_RATE/HAOE/ALG **/
     unsigned short int usiCtrl1;
     /** PUNC_SCHM/MAX_ITER/MIN_ITER/IN_D_STRCT **/
     unsigned short int usiCtrl2;
     /** BS **/
     unsigned short int usiBS;
     /** LLMAP_CF/CRCPOLY/NDRE/PRF_TGL/TRB_DBG/MB_PR **/
     unsigned short int usiCtrl3;
     /** HOE/SOE/DEPUNC/TASK_ID/VIT_SET/BUF_SIZE **/
     unsigned int  uliCtrl4;
     /** HRD_RSLT_ADDR **/
     unsigned int  uliHRDRSLTAddr;
     /** BASE_ADDR **/
     unsigned int  uliBaseAddr;
     /** VEC_OFFSET **/
     unsigned short int usiVecOffset;
     /** SFT_RSLT_OFFSET/HOFF/LLR_OUT_SF **/
     unsigned short int usiCtrl5;
     /** IHBSA/FTH/IHBSZ **/
     unsigned int  uliCtrl6;
     /** HUS/W1E/W2E/W3E/W1/W2/W3 **/
     unsigned int  uliCtrl7;
     /** SFT_RSLT_ADDR **/
     unsigned int  uliSFTRSLTAddr;
     /** E_PARAM_PTR **/
     unsigned int  uliEPARAMPTR;
     /** HA_IN_BASE_ADDR **/
     unsigned int  uliHAINBaseAddr;
     /** PF_BUF_SIZE **/
     unsigned short int usiPFBUFSize;
     /** PS_BUF_SIZE **/
     unsigned short int usiPSBUFSize;
     /** HA_OUT_BASE_ADDR **/
     unsigned int  uliHAOUTBaseAddr;
     /** PUSCH BD Pointer **/
     unsigned int  uliPUSCHBDPTR;
     /** Reserved1 **/
     unsigned char      ucReserved1[4];
     /** HSC **/
     unsigned short int usiHSC;
     /** CMP_RSN/ERR_STATUS/ZT_MAX/ITER_CNT **/
     unsigned short int usiStatus;
} t_TVPEBD;

typedef struct EQPEBD {
     /** OWNER/WRAP/INT_EN/FH/ALG/INTRP/RX/LX/COLS/ROWS **/
     unsigned int  uliCtrl1;
     /** MB_PR/F_DFT/C_EN/F_EN/LCNC/LD)/LD1/LD2/LD3/ALIGN0/ALIGN1/ALIGN2/ALIGN3/M/MOD0/MOD1/MOD2/MOD3 **/
     unsigned int  uliCtrl2;
     /** ALIGN_VAL0/ALIGN_VAL1/ALIGN_VAL2/ALIGN_VAL3 **/
     unsigned int  uliCtrl3;
     /** W_ROW/S_TYPE/S_COL/S_ROW **/
     unsigned int  uliCtrl4;
     /** W_SIZE **/
     unsigned short int usiWSIZE;
     /** S_SIZE **/
     unsigned short int usiSSIZE;
     /** TASK_ID **/
     unsigned short int usiTASKID;
     /** Y_SCL_TYPE/RM_SCL_TYPE/S_SCL_TYPE/C_SCL **/
     unsigned short int usiCtrl5;
     /** NVI Factor **/
     unsigned int  uliNVI;
     /** Beta Factor **/
     unsigned int  uliBETA;
     /** EQPE Y Data Pointer **/
     unsigned int  uliEQPEYDATAPTR;
     /** EQPE Y Offset **/
     unsigned short int usiEQPEYOFFSET;
     /** EQPE Y Gap **/
     unsigned short int usiEQPEYGAP;
     /** EQPE H_POINTER0/R_POINTER/M_POINTER **/
     unsigned int  uliEQPEH0RMDATAPTR;
     /** EQPE H_POINTER1 **/
     unsigned int  uliEQPEH1DATAPTR;
     /** Reserved  **/
     unsigned char      ucReserved[8];
     /** EQPE S_POINTER **/
     unsigned int  uliEQPESDATAPTR;
     /** EQPE W_POINTER **/
     unsigned int  uliEQPEWDATAPTR;
     /** EQPE SCL_POINTER **/
     unsigned int  uliEQPESCLPTR;
     /** EQPE S_SCL_OFFSET **/
     unsigned char      ucEQPESSCLOFFSET;
     /** EQPE H_SCL_OFFSET **/
     unsigned char      ucEQPEHSCLOFFSET;
     /** EQPE Y_SCL_OFFSET **/
     unsigned char      ucEQPEYSCLOFFSET;
     /** Reserved **/
     unsigned char      ucReserved0;
     /** EQPE OUT_POINTER0 **/
     unsigned int  uliEQPEOUT0PTR;
     /** EQPE OUT_POINTER1 **/
     unsigned int  uliEQPEOUT1PTR;
     /** EQPE NVIBETA_PTR **/
     unsigned int  uliNVIBETA_PTR;
     /** EQPE Reserved **/
     unsigned int  uliReserved;
     /** EQPE F_POINTER **/
     unsigned int  uliEQPEFDATAPTR;
     /** EQPE C_POINTER **/
     unsigned int  uliEQPECDATAPTR;
     /** EQPE OUT_SCL_OFF0 **/
     unsigned char      ucOUTSCLOFF0;
     /** EQPE OUT_SCL_OFF1 **/
     unsigned char      ucOUTSCLOFF1;
     /** Reserved 1 **/
     unsigned char      ucReserved1[2];
     /** EQPE SING **/
     unsigned int  uliSING;
     /** Reserved 2 **/
     unsigned char      ucReserved2[8];
     /** EQPE H Offset **/
     unsigned short int usiEQPEHOFFSET;
     /** EQPE H Gap **/
     unsigned short int usiEQPEHGAP;
     /** EQPE H Offset2 **/
     unsigned short int usiEQPEHOFFSET2;
     /** EQPE Y Offset2 **/
     unsigned short int usiEQPEYOFFSET2;
     /** Reserved 3 **/
     unsigned char      ucReserved3[8];
} t_EQPEBD;

typedef struct CGPEBD {
     /** OWNER/WRAP/INT_EN/MB_PR/CODE_SIZE 0x0**/
     unsigned int uliCtrl1;
     /** TASK_ID/PN_NUM 0x4*/
     unsigned int uliPN_NUM;
     /** SF/OVFS_NUM/START OFFSET 0x8*/
     unsigned int uliSF_START;
     /** LONG_MODE/UL_CODE/OUT_MODE/CFG_VAL0/CFG_VAL1 0xc*/
     unsigned int uliMODES_VALS;
     /** CODE_RSLT_PTR 0x10*/
     unsigned int uliRSLT_PTR;
} t_CGPEBD;
typedef struct DEPEBD {
     /** OWNER/WRAP/DC/INT_EN/STD/IBS **/
     unsigned short int usiCtrl1;
     /** SVE/NOH/OBS **/
     unsigned short int usiCtrl2;
     /** IBA/PRF_TGL **/
     unsigned int  uliCtrl3;
     /** OBA  **/
     unsigned int  uliCtrl4;
     /** Header Buffer Address **/
     unsigned int  uliHBA;
     /** BD_STS_PTR  **/
     unsigned int  uliSTSPTR;
     /** MB_PR/LTE_OBO/TASK_ID **/
     unsigned short int usiCtrl5;
     /** COE **/
     unsigned short int usiCOE;
     /** ESVO/PFS **/
     unsigned short int usiCtrl6;
     /** SVE/TB/PSS **/
     unsigned short int usiCtrl7;
     /** CMP_RSN **/
     unsigned short int usiCMPRSN;
     /** RMNA **/
     unsigned short int usiRMNA;
     /** Header1 **/
     unsigned int uliHeader1;
     /** Header2 **/
     unsigned int uliHeader2;
     /** Header3 **/
     unsigned int uliHeader3;
     /** Header4 **/
     unsigned int uliHeader4;
     /** Header5 **/
     unsigned int uliHeader5;
     /** Header6 **/
     unsigned int uliHeader6;
     /** Header7 **/
     unsigned int uliHeader7;
     /** Header8 **/
     unsigned int uliHeader8;
} t_DEPEBD;
typedef struct FTBD {
    /** OWNER 31 WRAP 30 bdp 29 INT_EN 28  TASK_ID 27:20  FC_CG 19 DSTZP 18:6 TL_ID 5;0 **/
    unsigned int  uliCtrl1;
    /** AIUS 31 PRMUR 30 PSMUR 29 PSTMV 28:27  ZP 26 SINS 25 CRPE 24 PO_SCL 23:22 flp 21 GRDC 19 EXSF 18 EXS 17:16
           PSTME 15 CPIE 14 GR 13 PME 12:11 OVA_SCL 10 SCL_TYPE 9 ITE 8 GI 7  cnj 4 convPE 3 DSS 2:0  **/
    unsigned int  uliCtrl2;
    /** Input Buffer Address. 31:3  convpe 2  MB_PR 1:0 **/
    unsigned int  uliCtrl3;
    /** Output Buffer Address 31:3  **/
    unsigned int  uliOBA;
    /** PM_SCL 31 USR_SCL0 30:28 USR_SCL1 27:25 USR_SCL2 24:22
          USR_SCL3 21:19 USR_SCL4 18:16  USR_SCL4 15:13  IN_SCL 12:9 ADP_OVA_SCL 7:0 **/
    unsigned int  uliUSRSCL;
    /** Pre-Multiplier Pointer 31:3**/
    unsigned int  uliPRMPTR;
    /** BD_STAT_PTR 31:3**/
    unsigned int  uliBDSTATPTR;
    /** PSTM real 31:16  img 15:0 **/
    unsigned int  uliPSTM;
    /** CPS 31:23 BD_RPT 22:16   **/
    unsigned short int usiCtrl4;
    /** ADP_OVA_SCL_ST 15:8 CMP_RSN 7:6 **/
    unsigned short int usiStatus;
    /** OFF_SCL_PTR 31:3 in repeat  OFF_SCL 31:24 in non repeat**/
    unsigned int  uliOffSclPTR;
    /** Post-Multiplier Pointer 31:3 **/
    unsigned int  uliPOMPTR;
    /** Reserved 1 **/
    unsigned char      ucReserved1[4];
    /** FCG 31 CG 30 FCPRE 16 FCISC 15:0  **/
    unsigned int  uliCtrl5;
    /** FCB_REAL 15:0 FCB_IMAG 15:0 **/
    unsigned int  uliFCB;
    /** FCS 27:16 FCSS 15:0 **/
    unsigned int  uliFCsize;
    /** FCS_REAL 31:15 FCS_IMAG 15:0 **/
    unsigned int  uliFCshift;
    /** FCS_REAL_ST 31:15  FCS_IMAG_ST 15:0 **/
    unsigned int  uliFCst;
     /** SCOS 15:0  **/
    unsigned  int uliFCscos;
    /** NNPI 31:28  CO 15:0 **/
    unsigned  int uliCGctrl2;
    /** CNPI 31:29 SGO 28:25 LSS 24 SSN 23:0 **/
    unsigned  int  uliCGctrl1;
    /** CL 28:16  SN 3:0 **/
    unsigned  int  uliCGctrl3;
    /** NNPIS 26:24 NSGO 23:20 NSN 19:16 CO 16:0  **/
    unsigned  int  uliCGstat;
    /** Reserved 3 **/
    unsigned char   ucReserved3[8];
} t_FTBD;
typedef struct PUSCHBD {
    unsigned       int uliCtrl1;
    unsigned short int usiCtrl2;
    unsigned short int usiTaskID;
    unsigned       int uliInDataPtr;
    unsigned       int uliDecouplePtr;
	unsigned       int uliStatusPtr;
    unsigned       int uliPUPECtrl;
    unsigned       int uliFCPtr;
    unsigned       int uliReserved;
    unsigned       int uliControlAmount;
    unsigned short int usiCQIAmount[2];
    unsigned       int uliCinit;
    unsigned       int uliDMPPtr;
    unsigned       int uliDTXThr;
    unsigned       int uliCQIPtr;
    unsigned short int usiCBSKIP;
    unsigned short int usiTBS;
    unsigned       int uliTVPECtrl1;
    unsigned       int uliTVPECtrl2;
    unsigned       int uliHARQInPtr;
    unsigned       int uliHARQOutPtr;
    unsigned       int uliHardOutPtr;
    unsigned       int uliSoftOutPtr;
	unsigned       int uliPUPEOffset_CRC;
	unsigned short int usiGammaC;
	unsigned short int usiK;
	unsigned       int uliFTColsMoved_E;
} t_PUSCHBD;
typedef struct PUSCHCWParamHeader{
	unsigned int uliCtrl1;
    unsigned int uliFCPtr;
	unsigned char ucFCBPtr;
	unsigned char ucColsMoved;
	unsigned short int usiReserved;
    unsigned int uliControlAmount;
    unsigned short int usiCQIAmount[2];
    unsigned int uliC_Init;
    unsigned int uliDMPPtr;
    unsigned int uliDTXThresh;
    unsigned int uliCQIPtr;
    unsigned short int usiCBSkip;
    unsigned short int usiTBS;
    unsigned int uliTVPECtrl1;
    unsigned int uliTVPECtrl2;
    unsigned int uliHARQInPtr;
    unsigned int uliHARQOutPtr;
    unsigned int uliHardOutPtr;
    unsigned int uliSoftOutPtr;
    unsigned int uliPUPEOffset_CRC;
    unsigned short int usiGammaC;
    unsigned short int usiK;
	unsigned int uliPUPECurrentWritePtr_Eminus;
} t_PUSCHCWParamHeader;
typedef struct PUSCHPICOBD {
	unsigned int uliCtrl1;
    unsigned short int usiCtrl2;
    unsigned short int usiTaskID;
    unsigned int uliLayerBuffPtr;
    unsigned int uliDecouplePtr;
 	unsigned int uliStatusPtr;
    unsigned int uliEQPECtrl1;
    unsigned short int usiWSize;
    unsigned short int usiSSize;
    unsigned int uliYPtr;
    unsigned int uliYOffset;
	unsigned int uliH0Ptr;
	unsigned int uliH1Ptr;
    unsigned int uliHOffset;
    unsigned short int usiYGap;
    unsigned short int usiHGap;
	unsigned int uliSPtr;
	unsigned int uliWPtr;
	unsigned int uliSclBaPtr;
	unsigned char      ucSSclOffset;
    unsigned char      ucHSclOffset;
    unsigned char      ucYSclOffset;
    unsigned char      ucReserved;
	unsigned int uliCPtr;
	unsigned int uliRR;
	unsigned int uliNVIFactor;
	unsigned int uliBETAFactor;
    unsigned int uliCurrentLayerBuffPtr;
    t_PUSCHCWParamHeader astPUSCHCWParam[PUSCHPE_NUM_OF_LAYERS];
} t_PUSCHPICOBD;
typedef struct PDSCHBD {
    /** OWNER[31] WRAP[30] INT_EN[28] N_ALLOC[22:16] N_SPM_ALLOC[13:8] TASK_ID[7:0] **/
    unsigned int uliCtrl1;
    /** N_UE_SPECIFIC_RS[29:24] FS[21] EXTND_CP[20] SF_NUM[19:16] EXT_SYM_IN_SCALE[7:0] **/
    unsigned int uliCtrl2;
    /** UE_EN[31] CS_EN[30] MBSFN_EN[29] POS_EN[28] PSS_EN[27] SSS_EN[26] PBCH_EN[25] CFI[13:12] NUM_SYMS[11:8] ADP_OVA_SCL[7:0] **/
    unsigned int uliCtrl3;
    /** Address of the Tranport Block descriptors structure **/
    unsigned int uliTBDESCAddress;
    /** Address of the Code Word descriptors structure **/
    unsigned int uliCWDESCAddress;
    /** Address of the RB Mapping descriptors structure **/
    unsigned int uliRBMapAddress;
    /** Address of the UE Specific Mapping descriptors structure **/
    unsigned int uliUEParamPointer;
    /** Address of the Cell Specific Mapping descriptors structure **/
    unsigned int uliCSParamPointer;
    /** Address of the MBSFN Mapping descriptors structure **/
    unsigned int uliMBSFNParamPointer;
    /** Address of the Positioning Mapping descriptors structure **/
    unsigned int uliPOSParamPointer;
    /** Address of the External OFDM symbols **/
    unsigned int uliExtOFDMAddress;
    /** Address of the Primary Synchronization Channel **/
    unsigned int uliPSSAddress;
    /** Address of the Secondary Synchronization Channel **/
    unsigned int uliSSSAddress;
    /** Address of the Physical Broadcast Channel **/
    unsigned int uliPBCHAddress;
    /** Address of the ouput for "external ouptut" for Ant 0 **/
    unsigned int uliAnt0Address;
    /** Address of the ouput for "external ouptut" for Ant 1 **/
    unsigned int uliAnt1Address;
    /** Reserved **/
    unsigned int uliReserved[8];
} t_PDSCHBD;
typedef struct PDSCHTBBD {
    /** RV_IDX[30:29] MOD[28:27] NIR[16:0] **/
    unsigned int uliCtrl1;
    /** TB_IDX[26:21] N_OUT_BITS[16:0] **/
    unsigned int uliCtrl2;
    /** TB_BS[15:0] **/
    unsigned int uliCtrl3;
    /** Input Address **/
    unsigned int uliTBIB;
} t_PDSCHTBBD;
typedef struct PDSCHCWBD {
    /** Cinit of the current CW **/
    unsigned int uliCinit;
    /** CWV[16] RBSZ[14:8] RBPTR[14:8] **/
    unsigned int uliCtrl1;
    /** CWCOP[10] TXMOD[9:8] MOD[1:0] **/
    unsigned int uliCtrl2;
    /** CW_WB_EVEN[30:28] CW_WB_ODD[26:24] CW_WA_EVEN[22:20] CW_WB_ODD[18:16] ANTB_EN[1] ANTA_EN[0] **/
    unsigned int uliCtrl3;
    /** Reserved **/
    unsigned int uliReserved1;
    /** OSPMF[15:9] OSPMF[8:2] **/
    unsigned int uliCtrl4;
    /** DGBE[15:8] DGAE[15:8] **/
    unsigned int uliDataGainExp;
    /** DGBM[31:16] DGAM[15:0] **/
    unsigned int uliDataGainMant;
} t_PDSCHCWBD;
typedef struct PUFFTBD {
	/** [31]	- OWNER
		[30] 	- WRAP
		[29]	- BDP (BD in Process) - For Internal Use Only
		[28]	- INT_EN
		[27:20]	- Task ID
        [19:16] - BD_RPT
		[13]	- GR
		[12:11]	- PME
		[10]	- OVA_SCL
		[5:0]	- TL_ID **/
    unsigned int	uliCtrl1;
    /** [31:3]	- PRM_PTR
		[1:0]	- MB_PR **/
	unsigned int	uliCtrl2;
    /** [31:3]	- BD_STAT_PTR **/
    unsigned int	uliBdStatPtr;
    /** [31:3]	- Output Buffer Base Address **/
    unsigned int	uliOBA;
    /** [31:23]	- CP_OFF (CP Offset)
        [16]    - CPE
    	[15:8]	- ADP_OVA_SCL_ST
        [6:4]   - CMP_RSN **/
    unsigned int	uliCtrl3;
	/** [7:0]	- ADP_OVA_SCL **/
    unsigned int	uliCtrl4;
	/** [31:3]	- POM_PTR
	 *  [1:0]       - PSTMV **/
    unsigned int	uliReserved1;
	/** reserved2    **/
    unsigned int	uliReserved2;
} t_PUFFTBD;
typedef struct CRCBD {
    /** OWNER/WRAP/DC/INT_EN/PRF_TGL/MB_PR/CHECK/UPDATE/RVRSIN/RVRSOUT/INVOUT/CRC_POLY **/
    unsigned short int usiCtrl1;
    /** CRC_BS **/
    unsigned short int usiBS;
    /** CRC_IB **/
    unsigned int  uliCRCIB;
    /** CRC_INIT **/
    unsigned int  uliCRCINIT;
    /** CRC_RSLT **/
    unsigned int  uliCRCRSLT;
    /** FAIL/TASKID **/
    unsigned int  uliStatus;
    /** Reserved1 **/
    unsigned char      ucReserved1[12];
} t_CRCBD;
typedef struct CRPEDLChannel {
    /** ch desc 0 - SBBZ[30:16] VALID[12] STARTED[8] ESL[7:4] SSL [3:0] **/
    unsigned int uliMCDSCP0;
    /** ch desc 1 - CMPN[30] CMPC[29] SYNC[28] PCHID[25:16] CONT[12] [ESBS[11:8] SSBS[7:4] SFR[3] SF[2:0] **/
    unsigned int uliMCDSCP1;
    /** ch desc 2 - MSBBA[31:8] **/
    unsigned int uliMCDSCP2;
    /** system buffer read ptr [18:1] **/
    unsigned int uliMCDSCRP;
} t_CRPEDLChannel;
typedef struct CRPEDL_MCDLOB  {
    /** system output buffer base address [31:7] **/
    unsigned int uliMCDOBBA;
    /** system output buffer sz [14:7] **/
    unsigned int uliMCDOBSZ;
    /** system buffer write pointer [14:6] **/
    unsigned int uliMCDOBWP;
    unsigned int uliReserved;
} t_CRPEDL_MCDLOB;
typedef struct CRPEDLParam {
    /** slow channels   - 0x0 **/
    t_CRPEDLChannel  astMCDLSC[CRPE_CRDL_SLOT_CHANNELS_NUM];
    /** fast channels   - 0x1700   **/
    t_CRPEDLChannel  astMCDLFC[CRPE_CRDL_SUB_SLOT_CHANNELS_NUM];
	MAPLERESERVED(0x37c0,0x3800);
    /** antennas BA, SZ, WRPTR   - 0x3800 **/
    t_CRPEDL_MCDLOB astMCDLOB[CRPE_CRDL_MAX_ANTENNA];
    /** number of channels MNOFCL[22:16] MNORCL[10:0]  - 0x2030 **/
    unsigned int uliMCDLNOCL;
    /**  (Maple CRPE-DL General Configuration Parameter - 0x2034 **/
    unsigned int uliMCDLGCP;
} t_CRPEDLParam;

typedef struct CRPEULBPCH
{
    unsigned int uliBA;
    unsigned int uliSize;
    unsigned int uliWrptr;
    unsigned int uliObintcfg;
} t_CRPEULBPCH;
typedef struct CRPEULBGCP
{
    unsigned int uliGcp1;
    unsigned int uliGcp2;
    unsigned int uliGcp3;
    unsigned int uliGcp4;
} t_CRPEULBGCP;
typedef struct CRPEULBCOREDESC
{
    unsigned int  uliCPBA;
    unsigned int  uliCFBA;
    unsigned short int usiCMAXP;
    unsigned short int usiCMAXF;
    unsigned int  uliCNCMDTBA;
    unsigned int  uliCSWP;
} t_CRPEULBCOREDESC;
typedef struct CRPEULFParam {
    /** command buffer BA[31:8] SZ[7:0]      0x0 to 0x7 **/
    unsigned int auliMCUFCIBA[CRPE_CRULF_NUM_OF_CMD_CORE];
    MAPLERESERVED(0x8,0x20);
    /** command buffer WR_PTR[15:3]         0x20 to 0x27 **/
    unsigned int auliMCUFCIBWP[CRPE_CRULF_NUM_OF_CMD_CORE];
    MAPLERESERVED(0x28,0x40);
    /** command buffer RD_PTR[15:3]          0x40 to 0x47  **/
    unsigned int auliMCUFCIBRP[CRPE_CRULF_NUM_OF_CMD_CORE];
    MAPLERESERVED(0x48,0x70);
    /** interpolation bypass NOG[26:24], ASZ[23:16], RD_PTR[15:6]    0x70**/
    unsigned int uliMCUFIBGCP;
    /** Maple CRPE-ULF General Configuration Parameter - 0x74 **/
    unsigned int uliMCUFGCP ;
    MAPLERESERVED(0x78,0x80);
    /** interpolation bypass group attributes FIA[28:24], NOA[20:16], WR_PTR[15:6]       0x80 to 0x88 **/
    unsigned int auliMCUFIBGA[CRPE_CRULF_NUM_OF_IB_GROUP];
    MAPLERESERVED(0x88,0xa0);
    /** interpolation bypass antenna address [31:8]                                                             0xa0 to 0x100 **/
    unsigned int auliMCRUFIBAA[CRPE_CRULF_NUM_OF_IB_ANTENNA];
  } t_CRPEULFParam;

typedef struct CRPEULBParam {
	/** physical channel parameters - 0x0 **/
	t_CRPEULBPCH astCRPEULBPCH[CRPE_ULB_NUM_OF_PCH];
	/** group parameters - 0x800 **/
	t_CRPEULBGCP astCRPEULBGCP[CRPE_ULB_NUM_OF_GROUP];
	MAPLERESERVED(0x840,0x860);
	/** antenna addresses - 0x860 **/
	unsigned int auliMCUBANTP[CRPE_ULB_NUM_OF_ANT];
	/** configuration parameter  - 0x8c0  **/
	unsigned int uliMCUBCP;
	MAPLERESERVED(0x8c4,0x8d0);
	/** MAPLE CRPE ULB Output Buffer Interrupt configuration - 0x8d0 **/
	unsigned int uliMCUBOBICP;
	MAPLERESERVED(0x8d4,0x900);
	/**  MAPLE CRPE ULB Finished Channels Buffer 0x900 **/
	unsigned short int ausiMCUBFCB[CRPE_ULB_SIZE_OF_FCB];
	MAPLERESERVED(0xa00,0xd00);
	/** core descriptors area - 0xd00 **/
	unsigned char acCoreDescriptorsArea[0x300];
} t_CRPEULBParam;


/*----------------------------- PUSCHPE Definitions -------------------------*/
/* LTE related definitions */
#define PUSCHPE_CRC_SIZE    24
#define PUSCHPE_MAX_CB_SIZE 6144
#define PUSCH_FTPE_QUEUE_SIZE 8
#define FTPE_BD_EXT_SIZE      96
// PUSCH
#define MAPLE_CONTEXTPUSCHPE_FTPE_RING_BA     MAPLE_CONTEXTPUSCHPE_BASE_ADDR
#define MAPLE_CONTEXTPUSCHPE_TVPE_RING_BA    (MAPLE_CONTEXTPUSCHPE_FTPE_RING_BA + (PUSCH_FTPE_QUEUE_SIZE*FTPE_BD_EXT_SIZE))
#define PUSCH_FTPE_RING_BA (MAPLE_CONTEXTPUSCHPE_FTPE_RING_BA - MP_BD_BASE_ADDR)
#define PUSCH_TVPE_RING_BA (MAPLE_CONTEXTPUSCHPE_TVPE_RING_BA - MP_BD_BASE_ADDR)

typedef struct MapleContextPUSCHPE {
	/** eFTPE BD's Memory **/
    t_FTBD stFTPEBD[16];
    /** Status of PUPE Input Buffer**/
    unsigned int uliPUPEINBUFF;

} t_PUSCHPEDATA;

/*--------------------------- End PUSCHPE Definitions ------------------------*/

/* -------------------------- Start CONVPE Definitions ------------------------*/
typedef struct CONVPEBD {
    /** OWNER/WRAP/INT_EN/TASK_ID/TASK_TYPE/TASK_DESCR_SIZE **/
    unsigned int uliCtrl;
    /** TASK_ADDR **/
    unsigned int uliTASKADDR;
} t_CONVPEBD;
typedef struct CONVPERACHBD {
    /** ANT_DIV_BCH/RSLT_OVA_SCL/ANT_FFT_WE/ANT_FD/CCW/SIGN_INDX **/
    unsigned int       uliCtrl1;
    /** RSLT_ADP_OVA_SCL/TL_ID/OVRLP_SZ **/
    unsigned int       uliCtrl2;
    /** ANT_DATA_FD_ADDR **/
    unsigned int       uliANTDATAFDADDR;
    /** AD_FD_SCALE_ADDR **/
    unsigned int       uliADFDSCALEADDR;
    /** AD_OFFSET **/
    unsigned int       uliADOFFSET;
    /** AD_BUFFER_SIZE **/
    unsigned int       uliADBUFFERSIZE;
    /** SIGN_ADDR **/
    unsigned int       uliSIGNADDR;
    /** SIGN_OFFSET**/
    unsigned short int usiSIGNOFFSET;
    /** CORR_RSLT_OFFSET **/
    unsigned short int usiCORRRSLTOFFSET;
    /** CORR_RSLT_ADDR **/
    unsigned int       uliCORRRSLTADDR;
    /** CORR_RSLT_SCALE_ADDR **/
    unsigned int       uliCORRRSLTSCALEADDR;
    /** AD_BASE **/
    unsigned int       uliADBASE[16];
} t_CONVPERACHBD;
typedef struct CONVPEPSBD {
    /** ANT_DIV_BCH/RSLT_OVA_SCL/ANT_FFT_WE/ANT_FD/CCW/NUSER **/
    unsigned int uliCtrl1;
    /** TL_ID/OVRLP_SZ **/
    unsigned int uliCtrl2;
    /** ANT_DATA_FD_ADDR **/
    unsigned int uliFCSTRUCTADDER;
    /** AD_FD_SCALE_ADDR **/
    unsigned int uliADFDSCALEADDR;
    /** AD_OFFSET **/
    unsigned int uliADOFFSET;
    /** AD_BUFFER_SIZE **/
    unsigned int uliADBUFFERSIZE;
   /** FC_STRUCT_SIZE **/
    unsigned short int usiFCSTRCUTSIZE;
    /** U_CORR_RSLT_OFFSET **/
    unsigned short int usiUCORRRSLTOFFSET;
} t_CONVPEPSBD;
typedef struct CONVPEUSER {
    /** SCRM_SEQ/CNPI/FC_EN/LS **/
    unsigned  int  uliCtrl1;
    /** SOIF/RSLT_ADP_OVA_SCL **/
    unsigned short int usiCtrl2;
    /** COIF **/
    unsigned short int usiCOIF;
    /**Reserverd  **/
    unsigned short int usiReserverd1;
    /**Reserverd  **/
    unsigned short int usiReserverd2;
    /** Reserverd **/
    unsigned short int usiReserverd3;
    /** FCSS **/
    unsigned short int usiFCSS;
    /** FCSR **/
    unsigned short int usiFCSR;
    /** FCS_IMAG **/
    unsigned short int usiFCSIMAG;
    /** U_CORR_RSLT_ADDR **/
    unsigned  int  uliUCORRRSLTADDR;
    /** U_CORR_RSLT_SCALE_ADDR **/
    unsigned  int  uliUCORRRSLTSCALEADDR;
} t_CONVPEUSER;
typedef struct CONVPEUSERFC{
    /** FCBR **/
    unsigned short int usiFCBR;
    /** FCB_IMAG **/
    unsigned short int usiFCBIMAG;
    /** FCISC **/
    unsigned short int usiFCISC;
   /** Reserverd **/
    unsigned short int usiReserverd1;

   } t_CONVPEUSERFC;
typedef struct CONVPEFDEBD {
    /** INT1_T/INT2_T/RSLT_OVA_SCL/CCW/IN1_NUM/IN2_NUM **/
    unsigned int uliCtrl1;
    /** RSLT_ADP_OVA_SCL/TL_ID/OVRLP_SZ_IN1 **/
    unsigned int uliCtrl2;
    /** OVRLP_SZ_IN2 **/
    unsigned int uliOVRLPSZIN2;
    /** DATA_FD_ADDR **/
    unsigned int uliDATAFDADDR;
    /** DATA_FD_SCALE_ADDR **/
    unsigned int uliDATAFDSCALEADDR;
} t_CONVPEFDEBD;
typedef struct CONVPEFDEIN1BD {
    /** IN1_BASE **/
    unsigned int       uliIN1BASE;
    /** IN1_OFFSET **/
    unsigned short int usiIN1OFFSET;
    /** IN1_BUFFER_SIZE **/
    unsigned short int usiIN1BUFFERSIZE;
} t_CONVPEFDEIN1BD;
typedef struct CONVPEFDEIN2BD {
    /** IN2_BASE **/
    unsigned int       uliIN2BASE;
    /** IN2_OFFSET **/
    unsigned short int usiIN2OFFSET;
    /** IN2_BUFFER_SIZE **/
    unsigned short int usiIN2BUFFERSIZE;
    /** SCRM_SEQ/CNPI/ZP_IN_EN/FC_EN/LS **/
    unsigned int       uliIN2Ctrl1;
    /** DSTZP **/
    unsigned short int usiIN2DSTZP;
    /** FCISC **/
    unsigned short int usiIN2FCISC;
    /** FCBR **/
    unsigned short int usiIN2FCBR;
    /** FCB_IMAG **/
    unsigned short int usiIN2FCBIMAG;
    /** FCS **/
    unsigned short int usiIN2FCS;
    /** FCSS **/
    unsigned short int usiIN2FCSS;
    /** FCSR **/
    unsigned short int usiIN2FCSR;
    /** FCS_IMAG **/
    unsigned short int usiIN2FCSIMAG;
    /** U_CORR_RSLT_ADDR **/
    unsigned int       uliIN2UCORRRSLTADDR;
} t_CONVPEFDEIN2BD;
/* -------------------------- END CONVPE Definitions ------------------------*/

#endif

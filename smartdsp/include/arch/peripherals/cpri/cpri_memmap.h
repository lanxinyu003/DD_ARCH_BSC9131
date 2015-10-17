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
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not
 evidence any actual or intended publication of such source code.
 
*******************************************************************************/

/******************************************************************************
 $Date: 2014/09/30 13:35:17 $
 $Id: cpri_memmap.h,v 1.29 2014/09/30 13:35:17 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/cpri/cpri_memmap.h,v $
 $Revision: 1.29 $
******************************************************************************/

/**************************************************************************//**

 @File          cpri_memmap.h

 @Description   CPRI Internal Memory Map definitions and structures.

 @Cautions      None.

*//***************************************************************************/

#ifndef __CPRI_MEMMAP_H
#define __CPRI_MEMMAP_H


#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#include "os_datatypes.h"
#include "smartdsp_os_device.h"

#if defined(MSC8157_FAMILY)
#include "msc8157_family_cpri.h"
#elif defined(PSC9132)
#include "psc9x32_family_cpri.h"
#elif defined(B4860_FAMILY)
#include "b4860_family_cpri.h"
#elif defined(B3421)
#include "b3421_family_cpri.h"
#endif




/**************************************************************************//**
 @Group         arch_common_id  Peripheral Memory Maps
 @{
*//***************************************************************************/

#define CPRI_MAX_NUM_OF_ANTENNA_CARRIERS            24
/**<The maximum number of CPRI antenna carriers*/

#ifdef CPRI_BLOCK_VER_4_1
#define CPRI_NUM_OF_AUXILIARY_MASKING_REGISTERS     40
#define CPRI_NUM_OF_INTERRUPT_CONTROL_REGISTERS     12
#define CPRI_NUM_OF_CONFIGURATION_MEMORY_REGISTERS  960
#define CPRI_CONTROL_WORD_MAX_LENGTH_IN_UINT32      3
#elif defined(CPRI_BLOCK_VER_4_2)
#define CPRI_NUM_OF_CONFIGURATION_MEMORY_REGISTERS  3072
#define CPRI_NUM_OF_AUXILIARY_MASKING_REGISTERS     64
#define CPRI_NUM_OF_INTERRUPT_CONTROL_REGISTERS     16
#define CPRI_CONTROL_WORD_MAX_LENGTH_IN_UINT32      4
#else
#error "CPRI block version must be specified"
#endif //MSC8157_FAMILY

#if defined(CPRI_BLOCK_VER_4_1) && defined(CPRI_BLOCK_VER_4_2)
#error  "Multiple CPRI block versions are specified!"
#endif //(CPRI_BLOCK_VER_4_1) && defined (CPRI_BLOCK_VER_4_2)


/**************************************************************************//**
 @Group         cpri_map_group   CPRI Memory Map

 @Description   Definition of CPRI registers and memory map

 @{
*//***************************************************************************/


/**************************************************************************//**
 @Description   CPRI framer registers.

*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0x0, 0x4);
    volatile uint32_t cpri_status;             /**< CPRI status register */
    volatile uint32_t cpri_config;             /**< CPRI configuration register*/
    OS_MEM_RESERVED(0xC, 0x1C);
    volatile uint32_t cpri_lcv;                /**< CPRI Receive Line Coding Violation Counter */
    volatile uint32_t cpri_bfn;                /**< CPRI Recovered BFN Counter*/
    volatile uint32_t cpri_hfn;                /**< CPRI Recovered HFN Counter*/
    OS_MEM_RESERVED(0x28, 0x2C);
    volatile uint32_t cpri_hw_reset;           /**< CPRI Hardware Reset from Control Word */
    OS_MEM_RESERVED(0x30, 0x3C);
    volatile uint32_t cpri_cm_config;          /**< CPRI Control and Management Configuration*/
    volatile uint32_t cpri_cm_status;          /**< CPRI Control and Management Status */
    OS_MEM_RESERVED(0x44, 0x48);
    volatile uint32_t cpri_rx_delay;           /**< CPRI Receive Delay */
    volatile uint32_t cpri_round_delay;        /**< CPRI Round Trip Delay */
    volatile uint32_t cpri_ex_delay_config;    /**< CPRI Extended Delay Measurement Configuration */
    volatile uint32_t cpri_ex_delay_status;    /**< CPRI Extended Delay Measurement Status */
    volatile uint32_t cpri_tx_prot_ver;        /**< CPRI Transmit Protocol Version */
    volatile uint32_t cpri_tx_scr_seed;        /**< CPRI Transmit Scrambler Seed */
    volatile uint32_t cpri_rx_scr_seed;        /**< CPRI Receive Scrambler Seed */
#ifdef CPRI_BLOCK_VER_4_1
    OS_MEM_RESERVED(0x64, 0x80);
#elif defined(CPRI_BLOCK_VER_4_2)
    OS_MEM_RESERVED(0x64, 0x68);
    volatile uint32_t cpri_tx_sync_delay;      /**< CPRI Transmit Sycnhronization Pulse Delay */
    volatile uint32_t cpri_tx_control;         /**< CPRI Transmit Control */
    OS_MEM_RESERVED(0x70, 0x80);
#endif //CPRI_BLOCK_VER_4_1
    volatile uint32_t cpri_serdes_config;      /**< CPRI SerDes Interface Configuration */
    OS_MEM_RESERVED(0x84, 0xC0);
    volatile uint32_t cpri_map_config;         /**< CPRI Mapping Configuration */
    volatile uint32_t cpri_map_cnt_config;     /**< CPRI Mapping Counter Configuration */
    OS_MEM_RESERVED(0xC8, 0xD0);
    volatile uint32_t cpri_map_tbl_config;     /**< CPRI Mapping Table Configuration */
    OS_MEM_RESERVED(0xD4, 0xE4);
    volatile uint32_t cpri_map_offset_rx;      /**< CPRI Mapping RX AxC Container Block Offset */
    volatile uint32_t cpri_map_offset_tx;      /**< CPRI Mapping TX AxC Container Block Offset */
    OS_MEM_RESERVED(0xEC, 0xF0);
    volatile uint32_t cpri_start_offset_tx;    /**< Offset for CPRI_TX_START Synchronization Output */
    OS_MEM_RESERVED(0xF4, 0x100);
    volatile uint32_t cpri_iq_rx_buf_status1;   /**< CPRI Mapping Buffer RX Status Register */
    volatile uint32_t cpri_iq_rx_buf_status2;   /**< CPRI Mapping Buffer RX Status Register */
    OS_MEM_RESERVED(0x108, 0x120);
    volatile uint32_t cpri_iq_tx_buf_status1;   /**< CPRI Mapping Buffer TX Status Register */
    volatile uint32_t cpri_iq_tx_buf_status2;   /**< CPRI Mapping Buffer TX Status Register */
    OS_MEM_RESERVED(0x128, 0x200);
    volatile uint32_t eth_rx_status;            /**< Ethernet Receive Status */
    OS_MEM_RESERVED(0x204, 0x208);
    volatile uint32_t eth_config_1;             /**< Ethernet Feature Enable/Disable and Interrupt Enable Bits */
    volatile uint32_t eth_config_2;             /**< Ethernet Miscellaneous Configuration */
    volatile uint32_t eth_rx_control;           /**< Ethernet RX Packet Discard */
    OS_MEM_RESERVED(0x214, 0x228);
    volatile uint32_t eth_rx_ex_status;         /**< Ethernet RX Additional Status */
    volatile uint32_t eth_addr_msb;             /**< Ethernet MSB of MAC Address (16 bits) */
    volatile uint32_t eth_addr_lsb;             /**< Ethernet LSB of MAC Address (32 bits) */
    volatile uint32_t eth_hash_table;           /**< Ethernet Small 32 Entries Hash Table to Filter Multicast Traffic */
    OS_MEM_RESERVED(0x238, 0x244);
    volatile uint32_t eth_config_3;             /**< Ethernet Configuration 3 */
    volatile uint32_t eth_cnt_rx_frame;         /**< Ethernet Receive Frame Counter */
    volatile uint32_t eth_cnt_tx_frame;         /**< Ethernet Transmit Frame Counter */
#ifdef CPRI_BLOCK_VER_4_1
    OS_MEM_RESERVED(0x250, 0x300);
#elif defined(CPRI_BLOCK_VER_4_2)
    volatile uint32_t eth_cnt_dmac_mism;        /**< Ethernet Received DMAC Mismatch Frame Counter */
    OS_MEM_RESERVED(0x254, 0x300);
#endif //CPRI_BLOCK_VER_4_1
    volatile uint32_t hdlc_rx_status;           /**< HDLC Receive Status */
    OS_MEM_RESERVED(0x304, 0x308);
    volatile uint32_t hdlc_config_1;            /**< HDLC Feature Enable/Disable and Interrupt Enable Bits */
    volatile uint32_t hdlc_config_2;            /**< HDLC Miscellaneous Configuration */
    volatile uint32_t hdlc_rx_control;          /**< HDLC RX Packet Discard */
    OS_MEM_RESERVED(0x314, 0x328);
    volatile uint32_t hdlc_rx_ex_status;        /**< HDLC RX External Status */
    OS_MEM_RESERVED(0x32C, 0x344);
    volatile uint32_t hdlc_config_3;            /**< HDLC Configuration 3 */
    volatile uint32_t hdlc_cnt_rx_frame;        /**< HDLC Receive Frame Counter */
    volatile uint32_t hdlc_cnt_tx_frame;        /**< HDLC Transmit Frame Counter */
    OS_MEM_RESERVED(0x350, 0x400);
}cpri_framer_registers_t;

/**************************************************************************//**
 @Description   CPRI complex/DMA registers.

                In B4860 the MBUS is replaced by AXI.
*//***************************************************************************/
typedef struct
{
    volatile uint32_t riqmts;                   /**< Receive IQ MBus Transaction Size */
    volatile uint32_t riqsdmts;                 /**< Receive IQ Second Destination Mbus Transaction Size */
    volatile uint32_t tiqmts;                   /**< Transmit IQ MBus Transaction Size */
    volatile uint32_t rvssmts;                  /**< Receive VSS MBus Transaction Size */
    volatile uint32_t tvssmts;                  /**< Transmit VSS MBus Transaction Size */
#ifdef CPRI_BLOCK_VER_4_1
    OS_MEM_RESERVED(0x414, 0x45C);
    volatile uint32_t riqsdba;                  /**< Receive IQ Second Dest Base Address */
#elif defined(CPRI_BLOCK_VER_4_2)
    OS_MEM_RESERVED(0x414, 0x460);
#endif //CPRI_BLOCK_VER_4_1
    volatile uint32_t riqbs;                    /**< Receive IQ Buffer Size */
    volatile uint32_t riqsdbs;                  /**< Receive IQ Second Destination Buffer Size */
    volatile uint32_t tiqbs;                    /**< Transmit IQ Buffer Size */
    volatile uint32_t rvssbs;                   /**< Receive VSS Buffer Size */
    volatile uint32_t tvssbs;                   /**< Transmit VSS Buffer Size */
    volatile uint32_t rethbs;                   /**< Receive ETH Buffer Size */
    volatile uint32_t rhdlcbs;                  /**< Receive HDLC Buffer Size */
#ifdef CPRI_BLOCK_VER_4_1
    volatile uint32_t rvssba;                   /**< Receive VSS Buffer Base Address */
    volatile uint32_t tvssba;                   /**< Transmit VSS Buffer Base Address */
    volatile uint32_t rebdrba;                  /**< Receive Ethernet BD Ring Base Address */
    volatile uint32_t tebdrba;                  /**< Transmit Ethernet BD Ring Base Address */
    volatile uint32_t rhbdrba;                  /**< Receive HDLC BD Ring Base Address */
    volatile uint32_t thbdrba;                  /**< Transmit HDLC BD Ring Base Address */
#elif defined(CPRI_BLOCK_VER_4_2)
    OS_MEM_RESERVED(0x47C, 0x494);
#endif //CPRI_BLOCK_VER_4_1
    volatile uint32_t rebdrs;                   /**< Receive Ethernet Buffer Descriptor Ring Size */
    volatile uint32_t tebdrs;                   /**< Transmit Ethernet Buffer Descriptor Ring Size */
    volatile uint32_t rhbdrs;                   /**< Receive HDLC Buffer Descriptor Ring Size */
    volatile uint32_t thbdrs;                   /**< Transmit HDLC Buffer Descriptor Ring Size */
    OS_MEM_RESERVED(0x4A4, 0x4A8);
    volatile uint32_t rgcm;                     /**< Receive General CPRI Mode */
    volatile uint32_t tgcm;                     /**< Transmit General CPRI Mode */
#ifdef CPRI_BLOCK_VER_4_1
    OS_MEM_RESERVED(0x4B0, 0x4B4);
    volatile uint32_t tscr;                     /**< Transmit Synchronization Configuration Register */
#elif defined(CPRI_BLOCK_VER_4_2)
    OS_MEM_RESERVED(0x4B0, 0x4B8);
#endif //CPRI_BLOCK_VER_4_1
    volatile uint32_t tcfbs;                    /**< Transmit Cpri Framer Buffer Size */
    volatile uint32_t tctie1;                   /**< Tx control table insert enable 1 */
    volatile uint32_t tctie2;                   /**< Tx control table insert enable 2 */
    OS_MEM_RESERVED(0x4C4, 0x4C8);
    volatile uint32_t tmrc;                     /**< Timer Configuration */
    volatile uint32_t rfpw;                     /**< Receive Frame Pulse Width */
    volatile uint32_t tfpw;                     /**< Transmit Frame Pulse Width */
#ifdef CPRI_BLOCK_VER_4_1
    OS_MEM_RESERVED(0x4D4, 0x500);
#elif defined(CPRI_BLOCK_VER_4_2)
    volatile uint32_t tiqsc;                    /**< Transmit IQ Sample Count */
    volatile uint32_t riqst[CPRI_TOTAL_NUM_OF_STEERING_BITS];   /**< Receive IQ Steering Bits 0...7 */
    OS_MEM_RESERVED(0x4F8, 0x500);
    volatile uint32_t riqsdba;                  /**< Receive IQ Second Dest Base Address */
    volatile uint32_t riqsdbamsb;               /**< Receive IQ Second Dest Base Address MSB */
    volatile uint32_t riqsdbaattr;              /**< Receive IQ Second Dest Base Address Attributes */
    OS_MEM_RESERVED(0x50C, 0x510);
    volatile uint32_t rvssba;                   /**< Receive VSS Buffer Base Address */
    volatile uint32_t rvssbamsb;                /**< Receive VSS Buffer Base Address MSB */
    volatile uint32_t rvssbaattr;               /**< Receive VSS Buffer Base Address Attributes */
    OS_MEM_RESERVED(0x51C, 0x520);
    volatile uint32_t tvssba;                   /**< Transmit VSS Buffer Base Address */
    volatile uint32_t tvssbamsb;                /**< Transmit VSS Buffer Base Address MSB */
    volatile uint32_t tvssbaattr;               /**< Transmit VSS Buffer Base Address Attributes */
    OS_MEM_RESERVED(0x52C, 0x530);
    volatile uint32_t rebdrba;                  /**< Receive Ethernet BD Ring Base Address */
    volatile uint32_t rebdrbamsb;               /**< Receive Ethernet BD Ring Base Address MAB */
    volatile uint32_t rebdrbaattr;              /**< Receive Ethernet BD Ring Base Address Attributes */
    volatile uint32_t rebdattr;                 /**< Receive Ethernet BD Attributes */
    volatile uint32_t tebdrba;                  /**< Transmit Ethernet BD Ring Base Address */
    volatile uint32_t tebdrbamsb;               /**< Transmit Ethernet BD Ring Base Address MAB */
    volatile uint32_t tebdrbaattr;              /**< Transmit Ethernet BD Ring Base Address Attributes */
    volatile uint32_t tebdattr;                 /**< Transmit Ethernet BD Attributes */
    volatile uint32_t rhbdrba;                  /**< Receive HDLC BD Ring Base Address */
    volatile uint32_t rhbdrbamsb;               /**< Receive HDLC BD Ring Base Address MAB */
    volatile uint32_t rhbdrbaattr;              /**< Receive HDLC BD Ring Base Address Attributes */
    volatile uint32_t rhbdattr;                 /**< Receive HDLC BD Attributes */
    volatile uint32_t thbdrba;                  /**< Transmit HDLC BD Ring Base Address */
    volatile uint32_t thbdrbamsb;               /**< Transmit HDLC BD Ring Base Address MAB */
    volatile uint32_t thbdrbaattr;              /**< Transmit HDLC BD Ring Base Address Attributes */
    volatile uint32_t thbdattr;                 /**< Transmit HDLC BD Attributes */
    OS_MEM_RESERVED(0x570, 0x5F0);
    volatile uint32_t auxcr;                    /**< Auxiliary Control Register */
    OS_MEM_RESERVED(0x5F4, 0x600);
#endif //CPRI_BLOCK_VER_4_1
}cpri_complex_registers_t;
 
/**************************************************************************//**
 @Description   CPRI control registers.

*//***************************************************************************/
typedef struct
{
    volatile uint32_t rcr;                      /**< Receive Control Register */
    volatile uint32_t tcr;                      /**< Transmit Control Register */
    volatile uint32_t raccr;                    /**< Receive AxC Control Register */
    OS_MEM_RESERVED(0x60C, 0x610);              // In MSC8157 (0x60C, 0x610)
    volatile uint32_t taccr;                    /**< Transmit AxC Control Register */
#ifdef CPRI_BLOCK_VER_4_1
    volatile uint32_t rca;                      /**< Receive Control Attribute */
    volatile uint32_t rcd[CPRI_CONTROL_WORD_MAX_LENGTH_IN_UINT32];  /**< Receive Control Data 0 - 2 */
    volatile uint32_t tca;                      /**< Transmit Control Attribute */
    volatile uint32_t tcd[CPRI_CONTROL_WORD_MAX_LENGTH_IN_UINT32];  /**< Transmit Control Data 0 - 2 */
#elif defined(CPRI_BLOCK_VER_4_2)
    volatile uint32_t ttv;                      /**< Timer Tolerance Value */
    volatile uint32_t rxsaicr;                  /**< Receive Symbol Awareness Interrupt Control Register */
    volatile uint32_t rxsdsaicr;                /**< Receive Second Destination Symbol Awareness Interrupt Control Register */
    volatile uint32_t txsaicr;                  /**< Transmit Symbol Awareness Interrupt Control Register */
    OS_MEM_RESERVED(0x624, 0x634);
#endif //CPRI_BLOCK_VER_4_1
    volatile uint32_t riqft;                    /**< Receive IQ First Threshold */
    volatile uint32_t riqst;                    /**< Receive IQ Second Threshold */
    volatile uint32_t riqt;                     /**< Receive IQ Threshold */
    volatile uint32_t tiqft;                    /**< Transmit IQ First Threshold */
    volatile uint32_t tiqst;                    /**< Transmit IQ Second Threshold */
    volatile uint32_t tiqt;                     /**< Transmit IQ Threshold */
    volatile uint32_t rvsst;                    /**< Receive VSS Threshold */
    volatile uint32_t tvsst;                    /**< Transmit VSS Threshold */
    volatile uint32_t rethct;                   /**< Receive Ethernet Coalescing Threshold */
    volatile uint32_t tethct;                   /**< Transmit Ethernet Coalescing Threshold */
    OS_MEM_RESERVED(0x65C, 0x660);              // In MSC8157 (0x55C, 0x560)
    volatile uint32_t rcier;                    /**< CPRI Receive Control and Timing interrupt Enable Register */
    volatile uint32_t tcier;                    /**< CPRI transmit Control and Timing Interrupt Enable Register */
    volatile uint32_t riqtsd;                   /**< Receive IQ Threshold Second Destination */
    OS_MEM_RESERVED(0x56C, 0x570);
    volatile uint32_t eier;                     /**< CPRI Error Interrupt Enable Register */
    volatile uint32_t tmre;                     /**< Timer Enable Register */
    volatile uint32_t rewpr;                    /**< Receive Ethernet Write Pointer Ring */
    volatile uint32_t tewpr;                    /**< Transmit Ethernet Write Pointer Ring */
    volatile uint32_t rhwpr;                    /**< Receive HDLC Write Pointer Ring */
    volatile uint32_t thwpr;                    /**< Transmit HDLC Write Pointer Ring */
#ifdef CPRI_BLOCK_VER_4_1
    OS_MEM_RESERVED(0x588, 0x5B0);
    volatile uint32_t racpr[CPRI_MAX_NUM_OF_ANTENNA_CARRIERS];  /**< Receive AxCn Parameter Register */
    OS_MEM_RESERVED(0x610, 0x640);
    volatile uint32_t tacpr[CPRI_MAX_NUM_OF_ANTENNA_CARRIERS];  /**< Transmit AxCn Parameter Register */
    OS_MEM_RESERVED(0x6A0, 0x700);
    volatile uint32_t maskr[CPRI_NUM_OF_AUXILIARY_MASKING_REGISTERS];   /**< cpri auxillary intetbface mask registers */
    volatile uint32_t auxcr;                    /**< CPRI Auxillary control register */
    OS_MEM_RESERVED(0x7A4, 0x800);
#elif defined(CPRI_BLOCK_VER_4_2)
    OS_MEM_RESERVED(0x688, 0x68C);
    volatile uint32_t raccrdma;                 /**< Receive AxC Control for DMA */
    volatile uint32_t taccrdma;                 /**< Transmit AxC Control for DMA */
    OS_MEM_RESERVED(0x694, 0x6A0);
    volatile uint32_t rca;                      /**< Receive Control Attribute */
    volatile uint32_t rcd[CPRI_CONTROL_WORD_MAX_LENGTH_IN_UINT32];  /**< Receive Control Data 0 - 3 */
    volatile uint32_t tca;                      /**< Transmit Control Attribute */
    volatile uint32_t tcd[CPRI_CONTROL_WORD_MAX_LENGTH_IN_UINT32];  /**< Transmit Control Data 0 - 3 */
    volatile uint32_t raccrfwd;                 /**< Receive Forwarding AxC Control Register */
    volatile uint32_t taccrsum;                 /**< Transmit Summing AxC Control Register */
    OS_MEM_RESERVED(0x6D0, 0x700);
    volatile uint32_t racpr[CPRI_MAX_NUM_OF_ANTENNA_CARRIERS];      /**< Receive AxCn Parameter Register */
    OS_MEM_RESERVED(0x760, 0x790);
    volatile uint32_t racprmsb[CPRI_MAX_NUM_OF_ANTENNA_CARRIERS];   /**< Receive AxCn Parameter Register MSB */
    OS_MEM_RESERVED(0x7F0, 0x820);
    volatile uint32_t racprattr[CPRI_MAX_NUM_OF_ANTENNA_CARRIERS];  /**< Receive AxCn Parameter Register Attributes */
    OS_MEM_RESERVED(0x880, 0x900);
    volatile uint32_t tacpr[CPRI_MAX_NUM_OF_ANTENNA_CARRIERS];      /**< Transmit AxCn Parameter Register */
    OS_MEM_RESERVED(0x960, 0x990);
    volatile uint32_t tacprmsb[CPRI_MAX_NUM_OF_ANTENNA_CARRIERS];   /**< Transmit AxCn Parameter Register MSB */
    OS_MEM_RESERVED(0x9F0, 0xA20);
    volatile uint32_t tacprattr[CPRI_MAX_NUM_OF_ANTENNA_CARRIERS];  /**< Transmit AxCn Parameter Register Attributes */
    OS_MEM_RESERVED(0xA80, 0xB00);
    volatile uint32_t maskr[CPRI_NUM_OF_AUXILIARY_MASKING_REGISTERS];   /**< cpri auxillary interface mask registers */
    OS_MEM_RESERVED(0xC00, 0xD00);
#endif //CPRI_BLOCK_VER_4_1
}cpri_control_registers_t;
 
/**************************************************************************//**
 @Description   CPRI status registers.

*//***************************************************************************/
typedef struct
{
    volatile uint32_t riqbdr;                   /**< Receive IQ Buffer Displacement Register */
    volatile uint32_t riqsdbdr;                 /**< Receive IQ Buffer Second Destination Displacement Register */
    volatile uint32_t tiqbdr;                   /**< Transmit IQ Buffer Displacement Register */
    volatile uint32_t rccr;                     /**< receive Chips Counter Register */
    volatile uint32_t rvssbdr;                  /**< Receive VSS Buffer Displacement Register */
    volatile uint32_t tvssbdr;                  /**< Transmit VSS Buffer Displacement Register */
    volatile uint32_t rethbd1;                  /**< Receive Ethernet Buffer Descriptor */
    volatile uint32_t rethbd2;                  /**< Receive Ethernet Buffer Descriptor */
    volatile uint32_t tethbd1;                  /**< Transmit Ethernet Buffer Descriptor */
    volatile uint32_t tethbd2;                  /**< Transmit Ethernet Buffer Descriptor */
    volatile uint32_t rerpr;                    /**< Receive Ethernet Read Pointer Ring */
    volatile uint32_t terpr;                    /**< Transmit Ethernet Read Pointer Ring */
    volatile uint32_t rhdlcbd1;                 /**< Receive HDLC Buffer Descriptor */
    volatile uint32_t rhdlcbd2;                 /**< Receive HDLC Buffer Descriptor */
    volatile uint32_t thdlcbd1;                 /**< Transmit HDLC Buffer Descriptor */
    volatile uint32_t thdlcbd2;                 /**< Transmit HDLC Buffer Descriptor */
    volatile uint32_t rhrpr;                    /**< Receive HDLC Read Pointer Ring */
    volatile uint32_t thrpr;                    /**< Transmit HDLC Read Pointer Ring */
    volatile uint32_t rer;                      /**< Receive Event Register */
    volatile uint32_t ter;                      /**< Transmit Event Register */
    volatile uint32_t eer;                      /**< Error Event Register */
    volatile uint32_t rethcs;                   /**< Receive Ethernet Coalescing Status */
    volatile uint32_t tethcs;                   /**< Transmit Ethernet Coalescing Status */
    volatile uint32_t tmrsr;                    /**< Timer Status Register */
    volatile uint32_t rsr;                      /**< Receive Status Register */
    volatile uint32_t tsr;                      /**< Transmit Status Register */
#ifdef CPRI_BLOCK_VER_4_1
    OS_MEM_RESERVED(0x868, 0x900);
#elif defined(CPRI_BLOCK_VER_4_2)
    volatile uint32_t mbeesr;                   /**< Multi Bit Error Status Register */
    OS_MEM_RESERVED(0xD6C, 0xD70);
    volatile uint32_t txhfn;                    /**< CPRI Transmit HFN Counter */
    volatile uint32_t txbfn;                    /**< CPRI Transmit BFN Counter */
    volatile uint32_t rersai;                   /**< receive Event Register for Symbol Awareness Interrupt*/
    volatile uint32_t rersdsai;                 /**< receive Event Register Second Destination for Symbol Awareness Interrupt*/
    volatile uint32_t tersai;                   /**< Transmit Event Register for Symbol Awareness Interrupt*/
    OS_MEM_RESERVED(0xD84, 0x1000);
#endif //CPRI_BLOCK_VER_4_1
}cpri_status_registers_t;

/**************************************************************************//**
 @Description   CPRI configuration memories registers.

*//***************************************************************************/
typedef struct
{
    volatile uint32_t rcm[CPRI_NUM_OF_CONFIGURATION_MEMORY_REGISTERS];  /**< Receive Configuration Memory */
    volatile uint32_t tcm[CPRI_NUM_OF_CONFIGURATION_MEMORY_REGISTERS];  /**< Transmit Configuration Memory */
#ifdef CPRI_BLOCK_VER_4_1
    OS_MEM_RESERVED(0x2700, 0x3000);
#endif //CPRI_BLOCK_VER_4_1
}cpri_configuration_memories_t;

/**************************************************************************//**
 @Description   CPRI general registers.

*//***************************************************************************/
typedef struct
{
#ifdef CPRI_BLOCK_VER_4_2
    OS_MEM_RESERVED(0x7000, 0x7E00);
#endif //CPRI_BLOCK_VER_4_2
    volatile uint32_t iccr;                     /**< CPRI Clocks Control Register */
    OS_MEM_RESERVED(0x7E04, 0x7E10);            // In MSC8157 (0X18004, 0X18010)
    volatile uint32_t ICR[CPRI_NUM_OF_INTERRUPT_CONTROL_REGISTERS]; /**< CPRI Interrupt Control Register */
#ifdef CPRI_BLOCK_VER_4_1
    volatile uint8_t  reserved2[16];
#endif //CPRI_BLOCK_VER_4_1
    volatile uint32_t rccier;                   /**< CPRI Receive CPU Control Interrupt Enable Register */
    volatile uint32_t tccier;                   /**< CPRI Transmit CPU Control Interrupt Enable Register */
    volatile uint32_t grsr;                     /**< General Receive Synchronization Register */
    volatile uint32_t gtsr;                     /**< General Transmit Synchronization Register */
    OS_MEM_RESERVED(0x7E60, 0x7E94);            // In MSC8157 (0X1860, 0X1894)
    volatile uint32_t iesr;                     /**< CPRI Error Status Register */
#ifdef CPRI_BLOCK_VER_4_2
    OS_MEM_RESERVED(0x7E98, 0x7EB0);
    volatile uint32_t cpridmastop;              /**< CPRI DMA Stop Register */
    volatile uint32_t cpridmastopst;            /**< CPRI DMA Stop Status Register */
    volatile uint32_t cprirrocr;                /**< CPRI Reset Request Output Control Register */
    OS_MEM_RESERVED(0x7EBC, 0x8000);
#endif //CPRI_BLOCK_VER_4_2
} cpri_general_registers_t;

/**************************************************************************//**
 @Description   CPRI peripheral registers.

*//***************************************************************************/
typedef struct
{
    cpri_framer_registers_t         cpri_framer;                /**< CPRI framer registers */
    cpri_complex_registers_t        cpri_complex;               /**< CPRI complex registers */
    cpri_control_registers_t        cpri_control;               /**< CPRI control registers */
    cpri_status_registers_t         cpri_status;                /**< CPRI status registers */
    cpri_configuration_memories_t   cpri_configuration_memories;/**< CPRI configuration memories registers */
#ifdef CPRI_BLOCK_VER_4_2
    cpri_general_registers_t        cpri_general_registers;     /**< CPRI general registers; valid only in CPRI1 unit */
#endif //CPRI_BLOCK_VER_4_2
}cpri_unit_map_t;

/** @} */ // end of cpri_map_group
/** @} */ // arch_common_id

_OS_END_EXTERN_C

#endif  // __CPRI_MEMMAP_H

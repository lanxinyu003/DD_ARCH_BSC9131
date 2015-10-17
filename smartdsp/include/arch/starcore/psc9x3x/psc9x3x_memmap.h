/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/05/15 10:38:51 $
 $Id: psc9x3x_memmap.h,v 1.30 2014/05/15 10:38:51 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/psc9x3x_memmap.h,v $
 $Revision: 1.30 $
******************************************************************************/

/******************************************************************************
 
 @File          psc9x3x_memmap.h

 @Description   PSC9x3x Internal Memory Map definitions and structures.
 
                 This file defines the register memory map for PSC9x3x.
                 It is divided into three sections:
                 - QBUS definition
                 - QE (Quicc Engine) definition
                 - CCSR (Control, Configuration and Status Registers) definition
 
 @Cautions      None.
 
*//***************************************************************************/

#ifndef __PSC9X3X_MEMMAP_H
#define __PSC9X3X_MEMMAP_H

#include "smartdsp_os_device.h"
#include "memmap.h"
#include "sc3x00_platform_memmap.h"
#include "dma_memmap.h"
#include "timer32b_memmap.h"
#include "wdt16b_memmap.h"
#include "class_memmap.h"
#include "vsg_memmap.h"
#include "tdm_memmap.h"
#ifdef PSC9132
#include "cpri_memmap.h"
#endif // PSC9132
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         arch_dev_map_id  CCSR Memory Maps
 @{
*//***************************************************************************/
/**************************************************************************//**
 @Group         arch_9x3x_dev_map_id  PSC9x3x CCSR Memory Maps
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   MAPLE Registers
*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0xC000, 0x10000);
} psc9x3x_maple_map_t;

/**************************************************************************//**
 @Description   DDR Memory Controller Registers
*//***************************************************************************/
typedef struct
{
    struct
    {
        volatile uint32_t       cs_bnds;    /**< Chip select x memory bounds */
        volatile uint32_t       reserved0;
    }cs_bnds[NUM_DDRC_CHIP_SELECT];     /**< All chip select memory bounds array */
    OS_MEM_RESERVED(0x20, 0x80);
    volatile uint32_t       cs_config[NUM_DDRC_CHIP_SELECT];    /**< Chip select x configuration */
    OS_MEM_RESERVED(0x90, 0xC0);
    volatile uint32_t       cs_config_2[NUM_DDRC_CHIP_SELECT];  /**< Chip select x configuration 2 */
    OS_MEM_RESERVED(0xd0, 0x100);
    volatile uint32_t       timing_cfg_3;   /**< DDR SDRAM timing configuration 3 */
    volatile uint32_t       timing_cfg_0;   /**< DDR SDRAM timing configuration 0 */
    volatile uint32_t       timing_cfg_1;   /**< DDR SDRAM timing configuration 1 */
    volatile uint32_t       timing_cfg_2;   /**< DDR SDRAM timing configuration 2 */
    volatile uint32_t       sdram_cfg;      /**< DDR SDRAM control configuration */
    volatile uint32_t       sdram_cfg_2;    /**< DDR SDRAM control configuration 2 */
    volatile uint32_t       sdram_mode;     /**< DDR SDRAM mode configuration */
    volatile uint32_t       sdram_mode_2;   /**< DDR SDRAM mode configuration 2 */
    volatile uint32_t       sdram_md_cntl;  /**< DDR SDRAM mode control */
    volatile uint32_t       sdram_interval; /**< DDR SDRAM interval configuration */
    volatile uint32_t       data_init;      /**< DDR SDRAM data initialization */
    OS_MEM_RESERVED(0x12C, 0x130);
    volatile uint32_t       sdram_clk_cntl; /**< DDR SDRAM clock control */
    OS_MEM_RESERVED(0x134, 0x148);
    volatile uint32_t       init_address;   /**< DDR training initialization address */
    volatile uint32_t       init_enxt_addr; /**< DDR training initialization extended address */
    OS_MEM_RESERVED(0x150, 0x160);
    volatile uint32_t       timing_cfg_4;   /**< DDR SDRAM timing configuration 4 */
    volatile uint32_t       timing_cfg_5;   /**< DDR SDRAM timing configuration 5 */
    OS_MEM_RESERVED(0x168, 0x170);
    volatile uint32_t       zq_cntl;        /**< DDR ZQ calibration control */
    volatile uint32_t       wrlvl_cntl;     /**< DDR write leveling control */
    OS_MEM_RESERVED(0x178, 0x17C);
    volatile uint32_t       sr_cntr;        /**< DDR Self Refresh Counter */
    volatile uint32_t       sdram_rcw_1;    /**< DDR Register Control Words 1 */
    volatile uint32_t       sdram_rcw_2;    /**< DDR Register Control Words 2 */
    OS_MEM_RESERVED(0x188, 0x190);
    volatile uint32_t       wrlvl_cntl_2;   /**< DDR write leveling control 2 */
    volatile uint32_t       wrlvl_cntl_3;   /**< DDR write leveling control 3 */
    OS_MEM_RESERVED(0x198, 0xB20);
    volatile uint32_t       ddrdsr_1;       /**< DDR Debug Status Register 1 */
    volatile uint32_t       ddrdsr_2;       /**< DDR Debug Status Register 2 */
    volatile uint32_t       ddrcdr_1;       /**< DDR Control Driver Register 1 */
    volatile uint32_t       ddrcdr_2;       /**< DDR Control Driver Register 2 */
    OS_MEM_RESERVED(0xB30, 0xBF8);
    volatile uint32_t       ip_rev1;        /**< DDR IP block revision 1 */
    volatile uint32_t       ip_rev2;        /**< DDR IP block revision 2 */
    volatile uint32_t       eor;            /**< DDR Enhanced Optimization Register */
    OS_MEM_RESERVED(0xC04, 0xD00);
    volatile uint32_t       mtcr;           /**< DDR Memory Test Control Register */
    OS_MEM_RESERVED(0xD04, 0xE00);
    volatile uint32_t       err_inject_hi;  /**< Memory data path error injection mask high */
    volatile uint32_t       err_inject_lo;  /**< Memory data path error injection mask low */
    volatile uint32_t       err_inject;     /**< Memory data path error injection mask ECC */
    OS_MEM_RESERVED(0xE0C, 0xE20);
    volatile uint32_t       capture_data_hi;    /**< Memory data path read capture high */
    volatile uint32_t       capture_data_lo;    /**< Memory data path read capture low */
    volatile uint32_t       capture_ecc;        /**< Memory data path read capture ECC */
    OS_MEM_RESERVED(0xE2C, 0xE40);
    volatile uint32_t       err_detect;         /**< Memory error detect */
    volatile uint32_t       err_disable;        /**< Memory error disable */
    volatile uint32_t       err_int_en;         /**< Memory error interrupt enable */
    volatile uint32_t       capture_attributes; /**< Memory error attributes capture */
    volatile uint32_t       capture_address;    /**< Memory error address capture */
    volatile uint32_t       capture_ext_address;/**< Memory error extended address capture */
    volatile uint32_t       err_sbe;            /**< Single-Bit ECC memory error management */
    OS_MEM_RESERVED(0xE5C, 0x1000);
} psc9x3x_ddr_map_t;

/**************************************************************************//**
 @Description   PA PIC Controller Registers
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       brr1;           /**< Block revision register 1 */
    OS_MEM_RESERVED(0x4, 0x40000);
}pic_map_t;

/**************************************************************************//**
 @Description   PA PIC Controller Registers
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       porpllsr;      /**< POR PLL Status Register */
    volatile uint32_t       porbmsr;       /**< POR Boot Mode Status Register */
    OS_MEM_RESERVED(0x8, 0xC);
    volatile uint32_t       pordevsr;      /**< POR Device Status Register */
    OS_MEM_RESERVED(0x10, 0x14);
    volatile uint32_t       pordevsr2;      /**< POR Device Status Register 2 */
    volatile uint32_t       pordspdevsr;    /**< POR DSP Device Status Register */
    volatile uint32_t       pordevsr3;      /**< POR Device Status Register 3 */
    OS_MEM_RESERVED(0x20, 0x60);
    volatile uint32_t       pmuxcr1;        /**< Alternate Function Signal Multiplex Control Register1 */
    volatile uint32_t       pmuxcr2;        /**< Alternate Function Signal Multiplex Control Register2 */
    volatile uint32_t       pmuxcr3;        /**< Alternate Function Signal Multiplex Control Register3 */
    volatile uint32_t       pmuxcr4;        /**< Alternate Function Signal Multiplex Control Register4 */
    volatile uint32_t       devdisr;		/**< Device Disable Control Register*/
    volatile uint32_t       devdisr2;		/**< Device disable control register2*/
    OS_MEM_RESERVED(0x78, 0x1000);
}device_config_t;

/**************************************************************************//**
 @Description   Lane control Registers
*//***************************************************************************/
typedef struct
{
    volatile uint32_t lgcr0;                /**< General Control Register */
    volatile uint32_t lgcr1;                /**< General Control Register */
    OS_MEM_RESERVED(0x208, 0x210);
    volatile uint32_t lrecr0;               /**< Receive Equalization Control Register */
    OS_MEM_RESERVED(0x214, 0x218);
    volatile uint32_t ltecr0;               /**< Transmit Equalization Control Register */
    OS_MEM_RESERVED(0x21C, 0x220);
    volatile uint32_t lttlcr0;              /**< Transition Tracking Loop Control Register 0*/
    volatile uint32_t lttlcr1;              /**< Transition Tracking Loop Control Register 1*/
    volatile uint32_t lttlcr2;              /**< Transition Tracking Loop Control Register 2*/
    OS_MEM_RESERVED(0x22C, 0x230);
    volatile uint32_t tcsr0;                /**< Test Control/Status Register 0*/
    volatile uint32_t tcsr1;                /**< Test Control/Status Register 1*/
    volatile uint32_t tcsr2;                /**< Test Control/Status Register 2*/
    volatile uint32_t tcsr3;                /**< Test Control/Status Register 3*/
}lane_control_t;

/**************************************************************************//**
 @Description   PA serdes control Controller Registers
*//***************************************************************************/
typedef struct
{
    volatile uint32_t srdsb1_rstctl;        /**< SRDS bank reset and control register */
    volatile uint32_t srdsb1_pllcr0;        /**< SRDS bank test/control register      */
    volatile uint32_t srdsb1_pllcr1;        /**< SRDS bank test/control register      */
    OS_MEM_RESERVED(0xC, 0x20);
    volatile uint32_t srdsb2_rstctl;        /**< SRDS bank reset and control register */
    volatile uint32_t srdsb2_pllcr0;        /**< SRDS bank test/control register      */
    volatile uint32_t srdsb2_pllsr1;        /**< SRDS bank status register            */
    OS_MEM_RESERVED(0x2C, 0x70);
    volatile uint32_t srdstcalcr;           /**< SerDes Transmit Calibration Control Register */
    OS_MEM_RESERVED(0x74, 0x80);
    volatile uint32_t srdsrcalcr;           /**< SerDes Receive Calibration Control Register */
    OS_MEM_RESERVED(0x84, 0x90);
    volatile uint32_t srdsgr0;              /**< SerDes General Register 0 */
    OS_MEM_RESERVED(0x94, 0xB0);
    volatile uint32_t srdsmemr0;            /**< SerDes Mode Error Monitor Register 0 */
    OS_MEM_RESERVED(0xB4, 0xC0);
    volatile uint32_t srdspccr0;            /**< SerDes Protocol Converter Configuration Register 0 */
    volatile uint32_t srdspccr1;            /**< SerDes Protocol Converter Configuration Register 1 */
    OS_MEM_RESERVED(0xC8, 0x150);
    volatile uint32_t pcvtrcpricr0;         /**< CPRI PCVTR Control Register 0 */
    volatile uint32_t pcvtrcpricr1;         /**< CPRI PCVTR Control Register 1 */
    OS_MEM_RESERVED(0x158, 0x200);
    lane_control_t  lane_control[4];
}serdes_control_t;

/**************************************************************************//**
 @Description   Clock Registers
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       sccr;       /**< System Clock Control Register */
    volatile uint32_t       gpr;        /**< General Purpose Register */
    volatile uint32_t       pllsr[0x2]; /**< PLLs Status Registers */
    OS_MEM_RESERVED(0x10, 0x20);
    volatile uint32_t       pcmr[0x4];  /**< PLL Clock Mode Registers */
    OS_MEM_RESERVED(0x30, 0x40);
    volatile uint32_t       dcmr[0x2];  /**< Dividers Clock Mode Registers */
    OS_MEM_RESERVED(0x48, 0x60);
    volatile uint32_t       pamr[0x4];  /**< PLL Auxiliary Mode Registers */
    OS_MEM_RESERVED(0x70, 0x80);
} psc9x3x_clock_map_t;

/**************************************************************************//**
 @Description   General Configuration (GCR) Interrupt Registers .
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       gir;                /**< General Interrupt Register */
    volatile uint32_t       gier[2];            /**< General Interrupt Enable Register (cores 0-1) */
    volatile uint32_t       gier_mpic;          /**< General Interrupt Enable Register 1 for MPIC */
} gen_cfg_int_t;

/**************************************************************************//**
 @Description   General configuration registers (GCR)
*//***************************************************************************/
typedef struct
{
    volatile uint32_t       gcr1;           /**< General Control Register 1 */
    volatile uint32_t       gcr2;           /**< General Control Register 2 */
    volatile uint32_t       gsr1;           /**< General Status Register 1 */
    volatile uint32_t       hssi_sr;        /**< High Speed Serial Interface Status Register */
    volatile uint32_t       ddr_gcr;        /**< DDR General Configuration Register */
    OS_MEM_RESERVED(0x14, 0x18);
    volatile uint32_t       hssi_cr;        /**< High Speed Serial Interface Control Register  */
    OS_MEM_RESERVED(0x1C, 0x20);
    volatile uint32_t       irq_status;     /**< IRQ Status Register */
    volatile uint32_t       irq_plr;        /**< IRQ Polarity Control Register */
    volatile uint32_t       spridr;         /**< System Part and Revision ID Register */
    OS_MEM_RESERVED(0x2C, 0x34);
    volatile uint32_t       gcr5;           /**< General Control Register 5 */
    volatile uint32_t       gsr2;           /**< General Status Register 2 */
    volatile uint32_t       dsppcr;         /**< DSP Slave Port Priority Control Register */
    OS_MEM_RESERVED(0x40, 0x50);
    volatile uint32_t       gsr3;           /**< General Status Register 3 */
    OS_MEM_RESERVED(0x54, 0x68);
    volatile uint32_t       gcr7;           /**< General Control Register 7 */
    volatile uint32_t       gcr8;           /**< General Control Register 8 */
    OS_MEM_RESERVED(0x70, 0x74);
    volatile uint32_t       gcr10;          /**< General Control Register 10 */
    OS_MEM_RESERVED(0x78, 0x80);
    gen_cfg_int_t           gir1;           /**< General interrupts  1 */
    volatile uint32_t       gir2;           /**< General interrupts  Register 2 */
    volatile uint32_t       gier2_mpic;     /**< General Interrupt Enable Register 2 for MPIC */
    OS_MEM_RESERVED(0x98, 0xA4);
    gen_cfg_int_t           gir3;           /**< General interrupts 3 */
    OS_MEM_RESERVED(0xB4, 0xCC);
    volatile uint32_t       gier4[2];       /**< General Interrupt Enable Register 4 (cores 0-1) */
    volatile uint32_t       gier4_mpic;     /**< General Interrupt Enable Register 2 for MPIC */
    OS_MEM_RESERVED(0xD8, 0xEC);
    gen_cfg_int_t           gir5;           /**< General interrupts 5 */
    OS_MEM_RESERVED(0xFC, 0x100);
    volatile uint32_t       dsp_state;          /**< DSP State Register */
    volatile uint32_t       pa_state;           /**< Power Architecture State Register */
    volatile uint32_t       boot_jmp_addr[2];   /**< Boot Jump Address 0/1 Register */
    volatile uint32_t       boot_patch_addr;    /**< Boot Patch Address Register */
    OS_MEM_RESERVED(0x114, 0x120);
    volatile uint32_t       gcr_dreq0;          /**< DMA REQUEST 0 Control Register */
    volatile uint32_t       gcr_dreq1;          /**< DMA REQUEST 1 Control Register */
    volatile uint32_t       gcr_ddone;          /**< DMA Done Control Register */
    volatile uint32_t       ddr1_gcr;           /**< DDR General Configuration Register */
    OS_MEM_RESERVED(0x130, 0x140);
    volatile uint32_t       gir_ant;            /**< General Interrupt Register Antenna */
    volatile uint32_t       gir_ant_en_mpic;    /**< General Interrupt Enable Register Antenna for MPIC */
    volatile uint32_t       gir_dma;            /**< General Interrupt Register DMA */
    volatile uint32_t       gir_dma_en_mpic;    /**< General Interrupt Enable Register Antenna for MPIC */
    volatile uint32_t       gir_tmr;            /**< General Interrupt Register Timers */
    volatile uint32_t       gir_tmr_en_mpic;    /**< General Interrupt Enable Register Timers for MPIC */
    volatile uint32_t       gir_mpl;            /**< General Interrupt Register MAPLE */
    volatile uint32_t       gir_mpl_en_mpic;    /**< General Interrupt Enable Register MAPLE for MPIC */
    OS_MEM_RESERVED(0x160, 0x170);
    gen_cfg_int_t           gir6;               /**< General interrupts 6 */
    OS_MEM_RESERVED(0x180, 0x194);
    gen_cfg_int_t           gir7;               /**< General interrupts 7 */
    OS_MEM_RESERVED(0x1A4, 0x1BC);
    volatile uint32_t       non_snoop_map;      /**< Mapping the Non-Snoopable Accesses */
    OS_MEM_RESERVED(0x1C0, 0x1E8);
    volatile uint32_t       gir8;               /**< General interrupts  Register 2 */
    volatile uint32_t       cpri_int_en_mpic;   /**< CPRI to MPIC External Request Enable */
    volatile uint32_t       maple_ext_req_en;   /**< MPIC/CPRI to MAPLE External Request Enable */
    OS_MEM_RESERVED(0x1F4, 0x200);
} psc9x3x_gcr_map_t;

/**************************************************************************//**
 @Description   DSP CCSR Access Window
*//***************************************************************************/
typedef struct
{
    volatile uint32_t abw;  /**< Access base window */
    volatile uint32_t aaw;  /**< Access attributes window */
}ab_window_t;

/**************************************************************************//**
 @Description   DSP CCSR Access Control Unit (ACU)
*//***************************************************************************/
typedef struct
{
    volatile uint32_t   com_ctl;            /**< Complete CCSR space access control register */
    OS_MEM_RESERVED(0x4, 0x20);
    ab_window_t acc_win[NUM_CCSR_ACC_WINDOWS];  /**< Access Windows */
    OS_MEM_RESERVED(0xA0, 0x130);
    volatile uint32_t       err_add_st;     /**< Status register for address of illegal access */
    volatile uint32_t       err_att_st;     /**< Status register for attributes of illegal access */
    volatile uint32_t       ctl_err_rpt;    /**< Control for error reporting */
    OS_MEM_RESERVED(0x13C, 0x1000);
} psc9x3x_acu_map_t;

/**************************************************************************//**
 @Description  SPI Registers.
*//***************************************************************************/
typedef struct
{
    /* The total size of the SPI structure is 0x30 */
	volatile uint32_t       spmode;         /**< SPI mode register     */
    volatile uint32_t       spie;           /**< SPI event register    */
    volatile uint32_t       spim;           /**< SPI mask register     */
    volatile uint32_t       spcom;          /**< SPI command register  */
    volatile uint32_t       spitf;          /**< SPI transmit register */
    volatile uint32_t       spirf;          /**< SPI receive register  */
    OS_MEM_RESERVED(0x6018, 0x6020);
    volatile uint32_t       spmode_cs[4];   /**< SPI mode register for CS 0-3 */
    OS_MEM_RESERVED(0x6030, 0x7000);
} spi_map_t;


#ifdef PSC9132
/**************************************************************************//**
 @Description   CPRI Registers
*//***************************************************************************/
typedef struct
{
    volatile cpri_unit_map_t            cpri_unit[2];   /**< CPRI unit */
    OS_MEM_RESERVED(0x6000, 0x18000);
    volatile cpri_general_registers_t   cpri_general;   /**< CPRI general registers */
    OS_MEM_RESERVED(0x18098, 0x20000);
} cpri_map_t;
#endif // PSC9132

/**************************************************************************//**
 @Description   AIC Registers
*//***************************************************************************/
typedef struct
{
    OS_MEM_RESERVED(0x50000, 0x70000);
} psc9x3x_aic_map_t;

/**************************************************************************//**
 @Description   PSC9x3x CCSR Memory Map
*//***************************************************************************/
typedef struct
{
    dma_memmap_t        dma;
    /**< DMA CCSR registers */
    OS_MEM_RESERVED(0x400, 0x8000);
    class_map_t         class0;
    /**< CLASS0 CCSR registers */
    OS_MEM_RESERVED(0x9000, 0xC000);
    psc9x3x_maple_map_t maple;
    /**< MAPLE CCSR registers placeholder */
    psc9x3x_ddr_map_t   ddrc;
    /**< DDRC CCSR registers */
    OS_MEM_RESERVED(0x11000, 0x14000);
    psc9x3x_clock_map_t clock;
    /**< Clock CCSR registers */
    OS_MEM_RESERVED(0x14080, 0x15000);
    wdt_map_t           wdt[NUM_OF_HW_WATCHDOGS];
    /**< HW WATCHDOGS CCSR registers */
#if defined(PSC9131)
    OS_MEM_RESERVED(0x15100, 0x15200);
#endif //PSC9131
    OS_MEM_RESERVED(0x15200, 0x16400);
    tmr32b_map_t        timer_32b_module[NUM_OF_HW_TIMER_32b_MODULES];
    /**< Timer32b CCSR registers */
    OS_MEM_RESERVED(0x16800, 0x17000);
    vint_map_t          gic;
    /**< General Interrupt Control CCSR registers */
    hs_map_t            hs[NUMBER_OF_HW_SEMAPHORES];
    /**< HW Semaphores CCSR registers */
    OS_MEM_RESERVED(0x17140, 0x18000);
    psc9x3x_gcr_map_t   general_config;
    /**< General Configuration CCSR registers */
    OS_MEM_RESERVED(0x18200, 0x19000);
    psc9x3x_acu_map_t   acu;
    /**< ACU CCSR registers placeholder */
    OS_MEM_RESERVED(0x1A000, 0x20000);
    tdm_map_t   tdm[MAX_NUM_OF_TDMS];
    /**< TDM CCSR registers placeholder */
#if defined(PSC9131)
    OS_MEM_RESERVED(0x24000, 0x50000);
#elif defined(PSC9132)
    OS_MEM_RESERVED(0x28000, 0x30000);
    cpri_map_t  cpri;
    /**< CPRI CCSR registers placeholder */
#endif // PSC9131
    psc9x3x_aic_map_t   aic;
    /**< AIC CCSR registers placeholder */
    OS_MEM_RESERVED(0x70000, 0x100000);
}psc9x3x_ccsr_t;

/**************************************************************************//**
 @Description   PSC9x3x Power Architecture CCSR Memory Map
*//***************************************************************************/
typedef struct
{
	OS_MEM_RESERVED(0x0, 0x06000);

	/**< eSPI registers */
	spi_map_t			espi[2];    //eSI1 and eSPI2 modules
	
	OS_MEM_RESERVED(0x08000, 0x0D000);
#if defined(PSC9131)
    /**< eSPI registers */
    spi_map_t           espi3_4[2]; //eSI3 and eSPI4 modules
#else
    OS_MEM_RESERVED(0x0D000, 0x0F000);
#endif
	gpio_map_t           gpio[3];
    OS_MEM_RESERVED(0x0F300, 0x40000);
    pic_map_t           pic;
    OS_MEM_RESERVED(0x80000, 0xE0000);
    device_config_t     device_config;
    OS_MEM_RESERVED(0xE1000, 0xE3000);
    serdes_control_t    serdes_control;
    OS_MEM_RESERVED(0xE4000, 0x100000);
}psc9x3x_pa_ccsr_t;


/*  Global variables for accessing the device memory space. */
extern dsp_plat_sc3850_map_t       *g_dsp_plat_map;        /**< Global variable for accessing the DSP Subsystem map */
extern psc9x3x_ccsr_t              *g_dsp_ccsr_map;        /**< Global variable for accessing the Control, Configuration and Status Registers */
extern psc9x3x_pa_ccsr_t           *g_dsp_pa_ccsr_map;        /**< Global variable for accessing the Control, Configuration and Status Registers */

/** @} */ // end of 9x3x
/** @} */ // end of ccsr

_OS_END_EXTERN_C

#endif  // __PSC9X3X_MEMMAP_H


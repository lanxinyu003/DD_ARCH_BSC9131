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
   $RCSfile: maple_api.h,v $
   Tag $Name:  $
   $Revision: 1.27 $
   $Date: 2014/05/28 07:20:11 $
   Target Processor: MAPLE RISC
   Target Device: PSC9131
   *******************************************************************************/
//----------------------------------------------------------------------------
/**
 @file   maple_api.h
 @brief  MAPLE API file
*/
//----------------------------------------------------------------------------
#ifndef _MAPLEAPI_H
#define _MAPLEAPI_H
/**< This function type is used for accessing MAPLE memory space in a platform
     independent way.MUST provide big-endian accesses towards MAPLE. */
typedef long (*maple_config_func)(
                                   void *addr,
                                   void *data,
                                   unsigned long int num_bytes,
                                   unsigned long int dev_id,
                                   unsigned long int config_param
                                  );
typedef struct
{
    /**< Timer Period value which determine
            the MAPLE PEs BD parse interval time .
          timer_period range is between 0 to 63 .
          The timer period by MAPLE clock equal to (timer_period + 1)*1024 .
          timer_period of 0 result period of 1024 MAPLE clock .
          timer_period of 63 result period of 65536 MAPLE clock . **/
    unsigned long     timer_period;
 
    /**< MAPLE mode1 set 0x1 for WIMAX standard
                     set 0x10 for ECC memory protection Enable indication
                     set 0x300 for TVPE Power control Mode
                     set 0x30000 for MBus1 default Priority
                     set 0xc0000 for MBus2 default Priority
                     set 0x3000000 for MBus Priority Scheme
                     set 0x10000000 for MBus Allocation **/
    unsigned long     mode1;

    /**< MAPLE mode2 set 0x3 for CRCPE Read Bus
                     set 0xc for CRCPE Write Bus
                     set 0x30 for CRPE Read Bus
                     set 0xc0 for CRPE Write Bus
                     set 0x3000 for DEPE Read Bus
                     set 0xc000 for DEPE Write Bus
                     set 0x30000 for PDPE Read Bus
                     set 0xc0000 for PDPE Write Bus
                     set 0x300000 for PUPE Read Bus
                     set 0xc00000 for PUPE Write Bus
                     set 0x3000000 for FTPE Read Bus
                     set 0xc000000 for FTPE Write Bus
                     set 0x30000000 for TVPE Read Bus
                     set 0xc0000000 for TVPE Write Bus **/
    unsigned long     mode2;


    /**< MAPLE PDSCH Configuration set 0x1 for PDSCH 3 MHz case
                                   set 0x2 for PDSCH 5 MHz case
                                   set 0x3 for PDSCH 10 MHz case
                                   set 0x4 for PDSCH 15 MHz case
                                   set 0x5 for PDSCH 20 MHz case
                                   set 0x100 for system with 2 Tx antennas (0 otherwise)
                                   set 0x10000 for PSS targeting Tx antenna 0
                                   set 0x20000 for PSS targeting Tx antenna 1
                                   set 0x30000 for PSS targeting both Tx antenna 0 and 1
                                   set 0x100000 for SSS targeting Tx antenna 0
                                   set 0x200000 for SSS targeting Tx antenna 1
                                   set 0x300000 for SSS targeting both Tx antenna 0 and 1
                                   set 0x1000000 for AIC operation mode **/
    unsigned long     pdsch_mode;
 
    /**< MAPLE PUSCH Configuration set 0x1 to Enable Decoupled Mode
                                   set 0x20 for PUSCH eFTPE served in high priority
                                   set 0x40 for PUSCH eTVPE served in high priority
                                   set 0x80000000 to Enable PUSCH **/
    unsigned long     pusch_mode;

    /**< MAPLE TVPE mode set 0x10 for Polarity
                         set 0x40 for Aposteriori Quality stop criteria indication
                         set 0x80 for CRC check stop criteria indication
                         set 0x2000 for Automatic NDRE
                         set 0x100000 for HARQ Enable
                         set 0x200000 for Separate Vectors indication
                         set 0x800000 for DeRandomizer Enable
                         set 0x1000000 for Steady CRC**/
    unsigned long     tvpe_mode;
 
    /**< MAPLE CRPE ULB mode
                         bit 0 is INT_MODE
						 bit 1 is CDMA2K
                         bit 7 is DLY_SPREAD
                         bits 19:16 are ISHA
    **/
    unsigned long     crpeulb_mode;
 
    /**< MAPLE CRPE DL mode
         bit 0 MOM
         bit 1 RCEL
         bit 2 RCS
         bit 4 CDMA2K ENABLE
         bits 18:16 are antenna enables
	 **/
    unsigned long crpedl_mode;
 
    /**< Pointer to micro-code to be uploaded to MAPLE **/
    void              *ucode;

    /**< Base address of MAPLE-B on system address space (Magenta) **/
    void              *mbus_base;

    /**< Base address of MAPLE-B on CCSR address space (Skyblue) **/
    void              *sbus_base;

    /**< Boolean flag - should MAPLE-B be ECC protected **/
    int                ecc_protect;

    /**< Function for writing data to the MAPLE-B during initialization **/
    maple_config_func  config_write;

    /**< Function for reading data from the MAPLE-B during initialization **/
    maple_config_func  config_read;

    /**< Device ID of MAPLE-B (for relevant protocol) if not on current SoC **/
    unsigned long      dev_id;
 
    /**< Configuration parameter **/
    unsigned long      config_param;
} maple_dev_init_params_t;
enum maplestd
{
	MAPLE_STD_3GLTE = 1,
	MAPLE_STD_WIMAX = 2,
	MAPLE_STD_3GPP = 3
};
/**< This function returns the standad supported by the MAPLE code.
      It takes, as an input, the same ucode pointer as passed in maple_dev_init_params_t **/
#define maple_standard(ucode) (*(unsigned long int *)ucode)
#define maple_srio_doorbell_enable(ucode) 0x1
#define maple_bd_base(ucode) 0x4000
#ifdef MAPLEAPI_GLOBALS
#define MAPLEAPI_EXT
#else
#define MAPLEAPI_EXT extern
#endif
MAPLEAPI_EXT void maple_init(maple_dev_init_params_t *maple_param);
MAPLEAPI_EXT void maple_soft_reset(maple_dev_init_params_t *maple_param);
#endif




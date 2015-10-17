/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:01 $
 $Id: class_profiling.h,v 1.13 2012/11/22 16:28:01 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/class/class_profiling.h,v $
 $Revision: 1.13 $
******************************************************************************/

/**************************************************************************//**
 
 @File          class_profiling.h

 @Description   CLASS debug and profiling Monitor API.

 @Cautions      User MUST include smartdsp_os_device.h prior to including this file
                In order to have the proper set of CLASS ports enumerated.
 
*//***************************************************************************/

#ifndef __CLASS_PROFILING_H
#define __CLASS_PROFILING_H


#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/***********************************************************//**
 @Group         dbg_class_id CLASS Profiling API

 @Description   CLASS debug and profiling Monitor API.

 @{
*//************************************************************/
/***********************************************************//**
 @Group         dbg_common_class_id Common CLASS Profiling API
 @{
*//************************************************************/

#define NO_TIME_OUT_PERIOD                  0    /**< No time out period on profiling */

/*************************************************************//**
 @Collection    block definitions.
 
 @{
*//**************************************************************/
#define BLOCK_OFFSET                        28
#define BLOCK_MASK                          (0xF << BLOCK_OFFSET)
/* @} */

/* Do not change these enumerations!!! */
enum
{
    CLASS0_BLK_NUM             = 1,
    CLASS0                     = (CLASS0_BLK_NUM << BLOCK_OFFSET)  /**< CLASS0 - generic */
} ;

/*************************************************************//**
 @Collection    CLASS initiator and target definitions.
 
 @{
*//**************************************************************/
#define CLASS_INIT                          0x00000000
#define CLASS_TRGT                          0x00010000
#define CLASS_PORT_MASK                     0x0000FFFF

#define CLASS_INIT0                         (CLASS_INIT | 0x00000001)
#define CLASS_INIT1                         (CLASS_INIT | 0x00000002)
#define CLASS_INIT2                         (CLASS_INIT | 0x00000004)
#define CLASS_INIT3                         (CLASS_INIT | 0x00000008)
#define CLASS_INIT4                         (CLASS_INIT | 0x00000010)
#define CLASS_INIT5                         (CLASS_INIT | 0x00000020)
#define CLASS_INIT6                         (CLASS_INIT | 0x00000040)
#define CLASS_INIT7                         (CLASS_INIT | 0x00000080)
#define CLASS_INIT8                         (CLASS_INIT | 0x00000100)
#define CLASS_INIT9                         (CLASS_INIT | 0x00000200)
#define CLASS_INIT10                        (CLASS_INIT | 0x00000400)
#define CLASS_INIT11                        (CLASS_INIT | 0x00000800)
#define CLASS_INIT12                        (CLASS_INIT | 0x00001000)
#define CLASS_INIT13                        (CLASS_INIT | 0x00002000)
#define CLASS_INIT14                        (CLASS_INIT | 0x00004000)
#define CLASS_INIT15                        (CLASS_INIT | 0x00008000)

#define CLASS_TRGT0                         (CLASS_TRGT | 0x00000001)
#define CLASS_TRGT1                         (CLASS_TRGT | 0x00000002)
#define CLASS_TRGT2                         (CLASS_TRGT | 0x00000004)
#define CLASS_TRGT3                         (CLASS_TRGT | 0x00000008)
#define CLASS_TRGT4                         (CLASS_TRGT | 0x00000010)
#define CLASS_TRGT5                         (CLASS_TRGT | 0x00000020)
#define CLASS_TRGT6                         (CLASS_TRGT | 0x00000040)
#define CLASS_TRGT7                         (CLASS_TRGT | 0x00000080)
#define CLASS_TRGT8                         (CLASS_TRGT | 0x00000100)
#define CLASS_TRGT9                         (CLASS_TRGT | 0x00000200)
#define CLASS_TRGT10                        (CLASS_TRGT | 0x00000400)
#define CLASS_TRGT11                        (CLASS_TRGT | 0x00000800)
#define CLASS_TRGT12                        (CLASS_TRGT | 0x00001000)
#define CLASS_TRGT13                        (CLASS_TRGT | 0x00002000)
#define CLASS_TRGT14                        (CLASS_TRGT | 0x00004000)
#define CLASS_TRGT15                        (CLASS_TRGT | 0x00008000)
/* @} */

#define PORT_NUM(block) (31 + neg_norm_l(block & CLASS_PORT_MASK) - 1)  /**< returns value between 1 and NOS-1 */

#if defined(MSC8156_FAMILY) || defined(MSC8157_FAMILY)
#include "msc815x_class_ports.h"
#elif defined(PSC9132) || defined(PSC9131)
#include "psc9x3x_class_ports.h"
#else
#error "please include smartdsp_os_device.h prior to including this file"
#endif



/**************************************************************************//**
 @Group         class_wm_id CLASS watchpoint Runtime API
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   CLASS watchpoint enables.
 
*//***************************************************************************/
typedef enum soc_class_wp_enable_t
{
    CLASS_WP_UPGRADEABLE_EN    = 0x00010000,    /**< Enable watchpoint on ipms_upgradeable */
    CLASS_WP_WR_CONFIRM_EN     = 0x00008000,    /**< Enable watchpoint on ipms_wr_confirm */
    CLASS_WP_EOT_ATTR_EN       = 0x00004000,    /**< Enable watchpoint on ipm_eot_attr */
    CLASS_WP_ATTR_EN           = 0x00002000,    /**< Enable watchpoint on ipm_attr */
    CLASS_WP_SRC_ID_EN         = 0x00001000,    /**< Enable watchpoint on ipms_src_id */
    CLASS_WP_PRIORITY_EN       = 0x00000800,    /**< Enable watchpoint on ipm_priority */
    CLASS_WP_BYTE_CNT_EN       = 0x00000400,    /**< Enable watchpoint on ipm_byte_cnt */
    CLASS_WP_ATOMIC_RSLT_EN    = 0x00000200,    /**< Enable watchpoint on ipms_atomic_result */
    CLASS_WP_ATOMIC_ACCESS_EN  = 0x00000100,    /**< Enable watchpoint on ipms_atomic_access */
    CLASS_WP_READ_SAFE_EN      = 0x00000080,    /**< Enable watchpoint on ipm_rsafe */
    CLASS_WP_SNOOP_EN          = 0x00000040,    /**< Enable watchpoint on ipm_snoop */
    CLASS_WP_OPTIMIZE_EN       = 0x00000020,    /**< Enable watchpoint on ipm_optimize */
    CLASS_WP_TEST_ACCESS_EN    = 0x00000010,    /**< Enable watchpoint on ipm_test_access */
    CLASS_WP_SUPERVISOR_EN     = 0x00000008,    /**< Enable watchpoint on ipm_supervisor_access */
    CLASS_WP_RW_EN             = 0x00000004,    /**< Enable watchpoint on ipm_rwb */
    CLASS_WP_ADDR_EN           = 0x00000002,    /**< Enable watchpoint on ipm_addr */
    CLASS_WP_CNT_ENABLE_EN     = 0x00000001,
    CLASS_WP_NONE_EN           = 0x00000000
} soc_class_wp_enable_t;

/**************************************************************************//**
 @Description   Size of address masked by class_watchpoint_t.addr_mask
 
*//***************************************************************************/
typedef enum soc_class_wp_addr_mask_t
{
    CLASS_WP_ADDR_MASK_4K      = 0xFF,        /**< ipm_addr aligned with a range of 4K */
    CLASS_WP_ADDR_MASK_8K      = 0xFE,        /**< ipm_addr aligned with a range of 8K */
    CLASS_WP_ADDR_MASK_16K     = 0xFC,        /**< ipm_addr aligned with a range of 16K */
    CLASS_WP_ADDR_MASK_32K     = 0xF8,        /**< ipm_addr aligned with a range of 32K */
    CLASS_WP_ADDR_MASK_64K     = 0xF0,        /**< ipm_addr aligned with a range of 64K */
    CLASS_WP_ADDR_MASK_128K    = 0xE0,        /**< ipm_addr aligned with a range of 128K */
    CLASS_WP_ADDR_MASK_256K    = 0xC0,        /**< ipm_addr aligned with a range of 256K */
    CLASS_WP_ADDR_MASK_512K    = 0x80,        /**< ipm_addr aligned with a range of 512K */
    CLASS_WP_ADDR_MASK_1024K   = 0x00         /**< ipm_addr aligned with a range of 1024K */
} soc_class_wp_addr_mask_t;

/**************************************************************************//**
 @Description   CLASS watchpoint access parameters.
 
*//***************************************************************************/
typedef enum soc_class_wp_param_t
{
    CLASS_WP_NONE              = 0x00000000,    /**< default value */
    CLASS_WP_ATOMIC_SUCCESS    = 0x80000000,    /**< Atomic access succeeded */
    CLASS_WP_ATOMIC_ACCESS     = 0x40000000,    /**< Atomic access */
    CLASS_WP_READ_SAFE         = 0x20000000,    /**< Read safe access */
    CLASS_WP_SNOOPABLE         = 0x10000000,    /**< Snoopable access */
    CLASS_WP_OPTIMIZED         = 0x08000000,    /**< Optimized access */
    CLASS_WP_TEST_ACCESS       = 0x04000000,    /**< Test access */
    CLASS_WP_SUPERVISOR        = 0x02000000,    /**< Supervisor access */
    CLASS_WP_READ              = 0x01000000     /**< Read access */
} soc_class_wp_param_t;

/**************************************************************************//**
 @Description   extended CLASS watchpoint access parameters.
 
*//***************************************************************************/
typedef enum soc_class_wp_ext_param_t
{
    CLASS_WP_EXT_NONE          = 0x00000000,    /**< default value */
    CLASS_WP_UPGRADEABLE       = 0x80000000,    /**< Upgradeable access */
    CLASS_WP_WR_CONFIRM        = 0x40000000,    /**< Real confirmation access */

    CLASS_WP_UPGRADEBALE       = CLASS_WP_UPGRADEABLE,
    CLASS_WP_WR_CONFIM         = CLASS_WP_WR_CONFIRM
} soc_class_wp_ext_param_t;

/**************************************************************************//**
 @Description   CLASS watchpoint priorities.
 
*//***************************************************************************/
typedef enum soc_class_wp_priority_t
{
    PRIORITY_0     = 0x00000000,    /**< Priority 0 access */
    PRIORITY_1     = 0x00000200,    /**< Priority 1 access */
    PRIORITY_2     = 0x00000400,    /**< Priority 2 access */
    PRIORITY_3     = 0x00000600     /**< Priority 3 access */
} soc_class_wp_priority_t;
/**************************************************************************//**
 @Description   CLASS Watchpoint configuration Structure.
 
*//***************************************************************************/
typedef struct soc_class_watchpoint_param_t
{
    soc_class_ports_t          watchpoint_port;        /**< Port on which to perform watchpoint (from soc_class_ports_t) */
    soc_class_wp_enable_t      enable_mask;            /**< What criteria define the watchpoint (from soc_class_wp_enable_t) */
    soc_class_wp_addr_mask_t   addr_mask;              /**< Address mask (from soc_class_wp_addr_mask_t) */
    soc_class_wp_param_t       watchpoint_param;       /**< Parameters for configuring CWPACR */
    soc_class_wp_ext_param_t   watchpoint_ext_param;   /**< Parameters fof configuring CWPEACR */
    soc_class_wp_priority_t    priority;               /**< Priority of access */
    uint32_t                       base_addr;              /**< Base address  */
    uint16_t                       byte_cnt;               /**< Number of bytes in transaction */
    uint8_t                        src_id;                 /**< Source ID - See RM for values */
    uint8_t                        eot_attr;               /**< EOT attributes - See RM for values */
    bool                           generate_wp_event;      /**< Generate interrupt if watchpoint is met */
    bool                           overflow_int;           /**< Generate interrupt if counter overflows */
} soc_class_watchpoint_param_t;


/**************************************************************************//**
 @Description   Watchpoint initializtion default parameters.
 
*//***************************************************************************/
#define CLASS_WATCHPOINT_INIT (soc_class_watchpoint_param_t)        \
{                                                          \
    NO_PORT_SELECTED,           /* watchpoint_port */      \
    CLASS_WP_NONE,              /* enable_mask */          \
    CLASS_WP_ADDR_MASK_1024K,   /* addr_mask */            \
    CLASS_WP_NONE,              /* watchpoint_param*/      \
    CLASS_WP_EXT_NONE,          /* watchpoint_ext_param */ \
    PRIORITY_0,                 /* priority */             \
    0 ,                         /*base_addr */             \
    0 ,                         /*byte_cnt */              \
    0 ,                         /*src_id */                \
    0 ,                         /*eot_attr */              \
    FALSE,                      /*generate_wp_event */     \
    FALSE                       /*overflow_int */          \
}

/** @} */ // end of class_watchpoint_configurations

/**************************************************************************//**
 @Group         class_pm_id  CLASS profiling Runtime API

 @{
*//***************************************************************************/
/**************************************************************************//**
 @Description   Supported CLASS initiator configurations.
 
*//***************************************************************************/
typedef enum class_profile_type_t
{
    /* Supported CLASS initiator configurations */
    NO_INIT_PROFILE = (CLASS_INIT | 0),
    /**< No profiling */
    PRIORITY_AND_AUTO_UPGRADE,
    /**< CPGCR0: ipm_priority == 1. CPGCR: ipm_priority == 2. CPGCR2: ipm_priority == 3. CPGCR3: number of priority auto-upgrades.  */
    ACCESS_TYPE,
    /**< CPGCR0: ipm_req == 1 and ipm_req_ack == 0. CPGCR1: number of read access. CPGCR2: number of write accesses (real confirm). CPGCR3: number of write accesses (fast confirm). */
    INIT_STALL,
    /**< CPGCR0: number of WAR. CPGCR1: number of stalls due to WAR. CPGCR2: number of target switches. CPGCR3: number of stalls due to target switches */
    PRIORITY_UPGRADE,
    /**< CPGCR0: .CPGCR1: .CPGCR2: .CPGCR3:  */
    PRIORITY_NON_UPGRADE,
    /**< CPGCR0: .CPGCR1: .CPGCR2: .CPGCR3:  */
    SUPERVISOR,
    /**< CPGCR0: ipm_req == 1 and ipm_req_ack == 0. CPGCR1: number supervisor access. CPGCR2: number non-supervisor access. CPGCR3: NA */
    INITATOR_BW,
    /**< CPGCR0: number ipm_data_ack for read datum. CPGCR1: number ipm_data_ack for write datum. CPGCR2: NA. CPGCR3: NA */
    TARGET_BW = 16,
    /**< CPGCR0: number ipm_data_ack for read datum from specified target .CPGCR1: number ipm_data_ack for write datum to specified target. CPGCR2: NA .CPGCR3: NA */

    /* Supported CLASS target configurations */
    NO_TRGT_PROFILE = (CLASS_TRGT | 0),
    /**< No profiling:  */
    WINNER_BY_PRIORITY,
    /**< CPGCR0: number transactions where ipm_priority == 0. CPGCR1: number transactions where ipm_priority == 1. CPGCR2: number transactions where ipm_priority == 2. CPGCR3: number transactions where ipm_priority == 3. */
    COLLISION,
    /**< CPGCR0: number cycles where arbiter didn't assert ipm_req_ack due to collisions. CPGCR1: NA. CPGCR2: NA. CPGCR3: NA */
    TRANACTION_SPLITTING = (CLASS_TRGT | 1 | (1<<12)),
    /**< CPGCR0: number cycles where transaction acknowledged to initiator. CPGCR1:  number cycles where transaction acknowledged by target. CPGCR2: NA. CPGCR3: NA */
    BANDWIDTH,
    /**< CPGCR0: number ipm_data_ack for read datum. CPGCR1: number ipm_data_ack for write datum. CPGCR2: NA. CPGCR3: NA */
    TRGT_STALL
    /**< CPGCR0: number of WAR. CPGCR1: number of stalls due to WAR. CPGCR2: NA. CPGCR3: NA */
} class_profile_type_t;


/**************************************************************************//**
 @Description   Options for watchpoint to affect profiling.
 
*//***************************************************************************/
typedef enum class_wp_affect_profile_t
{
    NO_AFFECT = 0,                             /**< Watchpoint doesn't affect profiling */
    PROFILE_AT_WATCHPOINT = (1<<8),            /**< Profiling starts when watchpoint asserts */
    STOP_PROFILE_AT_WATCHPOINT = (2<<8),       /**< Profiling stops when watchpoint asserts */
    TOGGLE_PROFILE_AT_WATCHPOINT = (3<<8)      /**< Profiling toggles watchpoint asserts */
} class_wp_affect_profile_t;

/**************************************************************************//**
 @Description   CLASS Profile configuration Structure.
 
*//***************************************************************************/
typedef struct soc_class_profiling_param_t
{
    soc_class_ports_t        profile_port;
    /**< Port on which to perform watchpoint (from class_ports) */
    class_profile_type_t        profiling_type;
    /**< What to profile (from class_initiator_profile_enum or class_target_profile_enum) */
    uint8_t                    trgt_profiled_by_init;
    /**< Target profiled by initiator if profiling_type equals TARGET_BW */
    uint32_t                    time_out_period;
    /**< Time out period for profiling. If 0, no time out */
    class_wp_affect_profile_t    watchpoint_enable;
    /**< How does watchpoint affect profiling (from class_wp_affects_profile_enum) */
    soc_class_watchpoint_param_t*    watchpoint_config;
    /**< Pointer to class_watchpoint_t. May be NULL if watchpoint_enable = NO_AFFECT */
    bool                    overflow_int;
    /**< Generate interrupt if there is an overflow in the counters. If watchpoint_config is not NULL, the value from watchpoint_config will be used */
} soc_class_profiling_param_t;

/**************************************************************************//**
 @Description  Class Profiling initializtion default parameters
 
*//***************************************************************************/
#define CLASS_PROFILE_INIT (soc_class_profiling_param_t)  \
{                                                         \
    NO_PORT_SELECTED,     /*profile_port */               \
    NO_INIT_PROFILE,      /* profiling_type */            \
    0,                    /* trgt_profiled_by_init */     \
    NO_TIME_OUT_PERIOD,   /* time_out_period */           \
    NO_AFFECT,            /* watchpoint_enable */         \
    NULL,                 /* watchpoint_config */         \
    FALSE                 /* overflow_int */              \
}

/**************************************************************************//**
 @Description   CLASS Profile Results Structure.
 
*//***************************************************************************/
typedef struct class_profile_results_t
{
    uint32_t    class_ref_counter;    /**< Value of reference counter (CPIER) */
    uint32_t    class_counters[4];    /**< Value of general counter x (CPGCRx) */
} class_profile_results_t;

/** @} */ // end of class_pm_id


/**************************************************************************//**
 @Group         class_wm_id  CLASS watchpoint Runtime API
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      classConfigureWatchpoint

 @Description   Configures a CLASS watchpoint.
 
 @Param[in]     watchpoint_config -  Pointer to the configuration structure.

 @Retval        SUCCESS
*//***************************************************************************/
os_status classConfigureWatchpoint(soc_class_watchpoint_param_t *watchpoint_config);


/**************************************************************************//**
 @Function      classEnableWatchpoint

 @Description   Enables a CLASS watchpoint.
 
 @Param[in]     port -  A soc_class_ports_t specifiying where the
                        watchpoint should be enabled.
 @Retval        SUCCESS     - The watchpoint was succesfully enabled.
 @Retval        FAIL        - The watchpoint was not succesfully enabled.

*//***************************************************************************/
os_status classEnableWatchpoint(soc_class_ports_t port);


/**************************************************************************//**
 @Function      classDisableWatchpoint

 @Description   Disables a CLASS watchpoint.
 
 @Param[in]     port -  A soc_class_ports_t specifiying where the
                        watchpoint should be disabled.
 @Retval        SUCCESS     - The watchpoint was succesfully disabled.
 @Retval        FAIL        - The watchpoint was not succesfully disabled.

*//***************************************************************************/
os_status classDisableWatchpoint(soc_class_ports_t port);

/** @} */ // end of class_wm_id
 
/**************************************************************************//**
 @Group         class_pm_id  CLASS profiling Runtime API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      classConfigureProfiling

 @Description   Configures CLASS profiling.
 
 @Param[in]     profiling_config -  Pointer to the configuration structure (soc_class_profiling_param_t).

 @Retval        SUCCESS     - The profiling was succesfully configured.
 @Retval        FAIL        - The profiling was not succesfully configured.

*//***************************************************************************/
os_status classConfigureProfiling(soc_class_profiling_param_t *profiling_config);


/**************************************************************************//**
 @Function      classStartProfiling

 @Description   Starts CLASS profiling.
 
 @Param[in]     port -  A soc_class_ports_t specifiying what should
                                     be profiled.

 @Retval        SUCCESS     - The profiling was succesfully started.
 @Retval        FAIL        - The profiling was not succesfully started.

*//***************************************************************************/
os_status classStartProfiling(soc_class_ports_t port);


/**************************************************************************//**
 @Function      classRestartProfiling

 @Description   Restarts the current CLASS profiling configuration.
                Should be called after CLASS counters overflow.
 
 @Param[in]     port -  A soc_class_ports_t specifiying which port/CLASS
                                     should be restarted.
 @Retval        SUCCESS     - The profiling was succesfully restarted.
 @Retval        FAIL        - The profiling was not succesfully restarted.
*//***************************************************************************/
os_status classRestartProfiling(soc_class_ports_t port);


/**************************************************************************//**
 @Function      classStopProfiling

 @Description   Stop CLASS profiling (without changing the configuration).
 
 @Param[in]     port -  A soc_class_ports_t specifiying what should
                                     no longer be profiled.

 @Retval        SUCCESS     - The profiling was succesfully stopped.
 @Retval        FAIL        - The profiling was not succesfully stopped.

*//***************************************************************************/
os_status classStopProfiling(soc_class_ports_t port);


/**************************************************************************//**
 @Function     classDisableProfiling

 @Description  Disable CLASS profiling and erase the profiling configuration.
 
 @Param[in]    port -  A soc_class_ports_t specifiying what should
                                     be disabled from profiling.

 @Retval       SUCCESS     - The profiling was succesfully disabled.
 @Retval       FAIL        - The profiling was not succesfully disabled.
 
@Cautions      If current profiling is on CLASS initiator, must be called with correct
               port from soc_class_ports_t; Calls classStopProfiling()
*//***************************************************************************/
os_status classDisableProfiling(soc_class_ports_t port);


/**************************************************************************//**
 @Function     classGetProfilingResults

 @Description  Retrieves the CLASS profiling results.
 
 @Param[in]    port -  A soc_class_ports_t specifiying where the results
                                     should be brought from.
 @Param        results -  Pointer to the profiling results structure
                                     (class_profile_results_t).

 @Retval       SUCCESS     - The results are valid.
 @Retval       FAIL        - The results are invalid.

*//***************************************************************************/
os_status classGetProfilingResults(soc_class_ports_t port, class_profile_results_t *results);


/** @} */ // end of class_pm_id
/** @} */ // end of dbg_common_class_id
/** @} */ // end of dbg_class_id


_OS_END_EXTERN_C

#endif // __CLASS_PROFILING_H

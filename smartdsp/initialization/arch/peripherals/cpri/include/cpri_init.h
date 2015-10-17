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
 $Date: 2014/11/23 16:04:57 $
 $Id: cpri_init.h,v 1.124 2014/11/23 16:04:57 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/cpri/include/cpri_init.h,v $
 $Revision: 1.124 $
******************************************************************************/

/******************************************************************************

 @File          cpri_init.h

 @Description   CPRI Initialization header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __CPRI_INIT_H
#define __CPRI_INIT_H

#include "smartdsp_os_device.h"

#include "os_sio_lld_.h"
#include "os_bio_lld_.h"
#if defined(MSC8157_FAMILY)
#include "msc815x_clocks.h"
#include "msc8157_family_cpri.h"
#include "msc815x_hssi_init.h"
#elif defined(PSC9132_FAMILY)
#include "psc9x3x_clocks.h"
#include "psc9x32_family_cpri.h"
#include "psc9x32_hssi_init.h"
#elif defined(B4860_FAMILY)
#include "b486x_clocks.h"
#include "b4860_family_cpri.h"
#include "b4860_hssi_init.h"
#endif

#include "os_cpp.h"

_OS_BEGIN_EXTERN_C


/**************************************************************************//**
 @Group        cpri_id CPRI Module API
 
 @Description   CPRI initialize.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         cpri_init_id CPRI initialization
 
 @Description   CPRI initialization API.
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Description   CPRI global control flags
*//***************************************************************************/


/**************************************************************************//**
 @Description   CPRI naming defines
*//***************************************************************************/
#define CPRI_IQ_DEV_NAME0       "c_i0"  /**< CPRI1 IQ device name for SIO */
#define CPRI_IQ_DEV_NAME1       "c_i1"  /**< CPRI2 IQ device name for SIO */
#define CPRI_IQ_DEV_NAME2       "c_i2"  /**< CPRI3 IQ device name for SIO */
#define CPRI_IQ_DEV_NAME3       "c_i3"  /**< CPRI4 IQ device name for SIO */
#define CPRI_IQ_DEV_NAME4       "c_i4"  /**< CPRI5 IQ device name for SIO */
#define CPRI_IQ_DEV_NAME5       "c_i5"  /**< CPRI6 IQ device name for SIO */
#define CPRI_VSS_DEV_NAME0      "c_v0"  /**< CPRI1 VSS device name for SIO */
#define CPRI_VSS_DEV_NAME1      "c_v1"  /**< CPRI2 VSS device name for SIO */
#define CPRI_VSS_DEV_NAME2      "c_v2"  /**< CPRI3 VSS device name for SIO */
#define CPRI_VSS_DEV_NAME3      "c_v3"  /**< CPRI4 VSS device name for SIO */
#define CPRI_VSS_DEV_NAME4      "c_v4"  /**< CPRI5 VSS device name for SIO */
#define CPRI_VSS_DEV_NAME5      "c_v5"  /**< CPRI6 VSS device name for SIO */
#define CPRI_ETHERNET_DEV_NAME0 "c_e0"  /**< CPRI1 Ethernet device name for BIO */
#define CPRI_ETHERNET_DEV_NAME1 "c_e1"  /**< CPRI2 Ethernet device name for BIO */
#define CPRI_ETHERNET_DEV_NAME2 "c_e2"  /**< CPRI3 Ethernet device name for BIO */
#define CPRI_ETHERNET_DEV_NAME3 "c_e3"  /**< CPRI4 Ethernet device name for BIO */
#define CPRI_ETHERNET_DEV_NAME4 "c_e4"  /**< CPRI5 Ethernet device name for BIO */
#define CPRI_ETHERNET_DEV_NAME5 "c_e5"  /**< CPRI6 Ethernet device name for BIO */
#define CPRI_HDLC_DEV_NAME0     "c_h0"  /**< CPRI1 HDLC device name for BIO */
#define CPRI_HDLC_DEV_NAME1     "c_h1"  /**< CPRI2 HDLC device name for BIO */
#define CPRI_HDLC_DEV_NAME2     "c_h2"  /**< CPRI3 HDLC device name for BIO */
#define CPRI_HDLC_DEV_NAME3     "c_h3"  /**< CPRI4 HDLC device name for BIO */
#define CPRI_HDLC_DEV_NAME4     "c_h4"  /**< CPRI5 HDLC device name for BIO */
#define CPRI_HDLC_DEV_NAME5     "c_h5"  /**< CPRI6 HDLC device name for BIO */

#define CPRI_IQ_ENABLED        &cpriIqInitialize        /**< CPRI IQ device enabled */
#define CPRI_VSS_ENABLED       &cpriVssInitialize       /**< CPRI VSS device enabled */
#define CPRI_ETHERNET_ENABLED  &cpriEthernetInitialize  /**< CPRI Ethernet device enabled */
#define CPRI_HDLC_ENABLED      &cpriHdlcInitialize      /**< CPRI HDLC device enabled */
#define CPRI_IQ_DISABLED       NULL                     /**< CPRI IQ device disabled */
#define CPRI_VSS_DISABLED      NULL                     /**< CPRI VSS device disabled */
#define CPRI_ETHERNET_DISABLED NULL                     /**< CPRI Ethernet device disabled */
#define CPRI_HDLC_DISABLED     NULL                     /**< CPRI HDLC device disabled */

#define CPRI_NOT_ACTIVE         0                       /**< CPRI feature is not active */
#define CPRI_ACTIVE             1                       /**< CPRI feature is active */
#define CPRI_NOT_SYNCHONIZED    0                       /**< CPRI is not synchronized */
#define CPRI_SYNCHONIZED        1                       /**< CPRI is synchronized */

/*************************************************************//**
 @Collection    CPRI active flags for RCR and TCR registers.
 @{
*//**************************************************************/
#define CPRI_IQ_ACTIVE              0x00000001
#define CPRI_ETHERNET_ACTIVE        0x00000002
#define CPRI_HDLC_ACTIVE            0x00000004
#define CPRI_VSS_ACTIVE             0x00000008
#define CPRI_ALL_DATA_TYPES_ACTIVE  (CPRI_IQ_ACTIVE | CPRI_ETHERNET_ACTIVE | CPRI_HDLC_ACTIVE | CPRI_VSS_ACTIVE)
/* @} */

/*************************************************************//**
 @Collection    Defines of the cpri device control commands
 
 @{
*//**************************************************************/
#define CPRI_DEVICE_DELAYS_CALCULATE          0x00010000    /**< calculate delays of the cpri device */
#ifdef CPRI_BLOCK_VER_4_1
#define CPRI_DEVICE_STATISTICS_SET            0x00020000    /**< enable cpri events to be used for statistics in sys timers (external to cpri unit) */
#endif //CPRI_BLOCK_VER_4_1
#define CPRI_DEVICE_TX_CONTROL_TABLE_WRITE    0x00040000    /**< write a control table TX entrance */
#define CPRI_DEVICE_TX_CONTROL_TABLE_READ     0x00080000    /**< read an entrance from TX control table */
#define CPRI_DEVICE_RX_CONTROL_TABLE_READ     0x00100000    /**< read an entrance from RX control table */
#define CPRI_DEVICE_RX_BFN_COUNTER_READ       0x00200000    /**< read BFN (radio frame) counter from cpri internal counters */
#define CPRI_DEVICE_RX_HFN_COUNTER_READ       0x00400000    /**< read HFN (hyper frame) counter from cpri internal counters */
#ifdef CPRI_BLOCK_VER_4_2
#define CPRI_DEVICE_TX_BFN_COUNTER_READ       0x00800000    /**< read BFN (radio frame) counter from cpri internal counters */
#define CPRI_DEVICE_TX_HFN_COUNTER_READ       0x01000000    /**< read HFN (hyper frame) counter from cpri internal counters */
#endif //CPRI_BLOCK_VER_4_2
#define CPRI_DEVICE_RESET_REQUEST             0x02000000    /**< Force reset request (master) or acknowledge (slave) */
#define CPRI_DEVICE_RESET_REQUEST_DISABLE     0x04000000    /**< Stop forcing reset request (master) or acknowledge (slave) */
#define CPRI_DEVICE_RESET_ENABLE              0x08000000    /**< enable master cpri unit to generate HW reset requests and slave to receive acknowledges */
#define CPRI_DEVICE_RESET_DISABLE             0x10000000	/**< disable master cpri unit to generate HW reset requests and slave to receive acknowledge */
#define CPRI_DEVICE_CHECK_RESET_DETECTED      0x20000000    /**< check if reset request or acknowledge was received */
#ifdef CPRI_BLOCK_VER_4_2
#define CPRI_DEVICE_TX_BFN_RESET              0x40000000    /**< check if reset request or acknowledge was received */
#define CPRI_DEVICE_DMA_RESTART               0x80000000    /**< restart cpri after it was stopped by warm reset */
#define CPRI_DEVICE_RECONFIGURATION_LEVEL0    0x80010000    /**< Reconfigure the CPRI unit with a new CPRI parameters (see in CPRI block RM); #cpri_reconfiguration_level0_param_t is supplied as an input*/
#define CPRI_DEVICE_RECONFIGURATION_LEVEL1    0x80020000    /**< Reconfigure the CPRI unit with a new CPRI parameters but same link rate and loopback setting(see in CPRI block RM); #cpri_init_params_t is supplied as an input*/
#define CPRI_DEVICE_RECONFIGURATION_LEVEL2    0x80040000    /**< Reconfigure the CPRI unit with a new IQ data type setting (see in CPRI block RM); #cpri_iq_init_params_t is supplied as input*/
#endif //CPRI_BLOCK_VER_4_2
/* @} */

/*************************************************************//**
 @Collection    defines of CPRI AXC (channels) activation
 
 @{
*//**************************************************************/
#define CPRI_AXC_0_ACTIVE      0x00000001
#define CPRI_AXC_1_ACTIVE      0x00000002
#define CPRI_AXC_2_ACTIVE      0x00000004
#define CPRI_AXC_3_ACTIVE      0x00000008
#define CPRI_AXC_4_ACTIVE      0x00000010
#define CPRI_AXC_5_ACTIVE      0x00000020
#define CPRI_AXC_6_ACTIVE      0x00000040
#define CPRI_AXC_7_ACTIVE      0x00000080
#define CPRI_AXC_8_ACTIVE      0x00000100
#define CPRI_AXC_9_ACTIVE      0x00000200
#define CPRI_AXC_10_ACTIVE     0x00000400
#define CPRI_AXC_11_ACTIVE     0x00000800
#define CPRI_AXC_12_ACTIVE     0x00001000
#define CPRI_AXC_13_ACTIVE     0x00002000
#define CPRI_AXC_14_ACTIVE     0x00004000
#define CPRI_AXC_15_ACTIVE     0x00008000
#define CPRI_AXC_16_ACTIVE     0x00010000
#define CPRI_AXC_17_ACTIVE     0x00020000
#define CPRI_AXC_18_ACTIVE     0x00040000
#define CPRI_AXC_19_ACTIVE     0x00080000
#define CPRI_AXC_20_ACTIVE     0x00100000
#define CPRI_AXC_21_ACTIVE     0x00200000
#define CPRI_AXC_22_ACTIVE     0x00400000
#define CPRI_AXC_23_ACTIVE     0x00800000
/* @} */

#define MAXIMUM_NUMBER_OF_AXCS  24          /**< maximum number of CPRI channels (AxC's) */

/**************************************************************************//**
 @Description   Enumeration of CPRI loop modes.
*//***************************************************************************/
typedef enum
{
    CPRI_NO_LOOPBACK                                = 0,
    CPRI_DIGITAL_LOOPBACK                           = 1
} cpri_loop_mode_t;

/**************************************************************************//**
 @Description   Enumeration of CPRI working modes.
*//***************************************************************************/
typedef enum
{
    /*numbers must follow the following rules:
     *  (any_slave_mode)&(any_master_mode)   = 0
     *  (any_slave_mode)&(any_slave_mode)   != 0
     *  (any_master_mode)&(any_master_mode) != 0*/
    CPRI_MASTER_MODE                                = 1,
    CPRI_SLAVE_MODE                                 = 2,
    CPRI_END_POINT_MODE_SLAVE_MODE                  = 6
} cpri_sync_mode_t;

/**************************************************************************//**
 @Description   Enumeration of CPRI external sync active modes.
*//***************************************************************************/
typedef enum
{
    SYNC_GENERATED_LOCALLY                          = 0,
    SYNC_GENERATED_EXTERNALLY                       = 1,
    SYNC_GENERATED_FROM_PAIR                        = 2
#if defined(CPRI_BLOCK_VER_4_2)
    ,SYNC_GENERATED_FROM_OWN_FRAMER                 = 3
#endif // CPRI_BLOCK_VER_4_2
} ext_sync_act_t;

#if defined(CPRI_BLOCK_VER_4_2)
/**************************************************************************//**
 @Description   Enumeration shared sync modes (defines which CPRI is the sync "master").
*//***************************************************************************/
typedef enum
{
    CPRI_SHARED_SYNC_MODE_CPRI1               = 0X00,
    CPRI_SHARED_SYNC_MODE_CPRI3               = 0X01,
    CPRI_SHARED_SYNC_MODE_CPRI5               = 0X10,
    CPRI_SHARED_SYNC_MODE_CPRI7               = 0X11
} shared_sync_mode_source_t;

/**************************************************************************//**
 @Description   Enumeration of sync mode pulse options for transmission.
*//***************************************************************************/
typedef enum
{
    CPRI_SYNC_MODE_RECOVERED               = 0X0,
    CPRI_SYNC_MODE_TIMER_GENERATED         = 0X1
} sync_mode_pulse_t;

/**************************************************************************//**
 @Description   Enumeration of reset acknowledge length, counted in number of hyperframes.
*//***************************************************************************/
typedef enum
{
    CPRI_5_HFS_RESET_ACK_ASSERT            = 0X0,
    CPRI_10_HFS_RESET_ACK_ASSERT           = 0X1
} reset_ack_assert_length_t;

/**************************************************************************//**
 @Description   Enumeration of reset acknowledge(master) or request(slave) detection length, counted in number of hyperframes.
*//***************************************************************************/
typedef enum
{
    CPRI_3_HFS_OUT_OF_5                 = 0X0,
    CPRI_6_HFS_OUT_OF_10                = 0X1
} reset_detection_length_t;
#endif // CPRI_BLOCK_VER_4_2

/**************************************************************************//**
 @Description   Enumeration of CPRI available protocol types.
*//***************************************************************************/
typedef enum
{
    CPRI_PROTOCOL_1                                 = 1,
    CPRI_PROTOCOL_2                                 = 2
} cpri_protocol_t;
/**************************************************************************//**
 @Description   Enumeration of CPRI multicast modes.
*//***************************************************************************/
typedef enum
{
    CPRI_NORMAL_MODE                                = 0,
    CPRI_MULTI_CAST                                 = 1,
    CPRI_MULTI_UNI_CAST                             = 2
} cpri_multicast_mode_t;

/**************************************************************************//**
 @Description   Enumeration of CPRI mapping modes.
*//***************************************************************************/
typedef enum
{
    CPRI_BASIC_MODE                                 = 0,
    CPRI_ADVANCED_MODE_1                            = 1,
    CPRI_ADVANCED_MODE_3                            = 2
} mapping_mode_t;

/**************************************************************************//**
 @Description   Enumeration of CPRI available sample rates.
*//***************************************************************************/
/* Note the values are defined precisely such that the MSB nibble represents TX sample width and the LSB nibble represents RX sample width */
typedef enum
{
    CPRI_SAMPLE_16_BIT                              = 0X00,
    CPRI_SAMPLE_15_BIT                              = 0X11,
    CPRI_SAMPLE_8_BIT                               = 0X22,
    CPRI_TX_16_CPRI_RX_8                            = 0X02,
    CPRI_TX_8_CPRI_RX_16                            = 0X20
} sampling_width_select_t;

/**************************************************************************//**
 @Description   Enumeration of CPRI IQ transaction sizes.
*//***************************************************************************/
typedef enum
{
    CPRI_IQ_TRANSACTION_64_BYTES                    = 0x1,
    CPRI_IQ_TRANSACTION_128_BYTES                   = 0x2,
    CPRI_IQ_TRANSACTION_256_BYTES                   = 0x4,
    CPRI_IQ_TRANSACTION_512_BYTES                   = 0x8,
    CPRI_IQ_TRANSACTION_1024_BYTES                  = 0x10,
    CPRI_IQ_TRANSACTION_2048_BYTES                  = 0x20,
    CPRI_IQ_NOT_USED
} iq_bus_transaction_size_t;

/**************************************************************************//**
 @Description   Enumeration of CPRI VSS transaction sizes.
*//***************************************************************************/
typedef enum
{
    CPRI_VSS_TRANSACTION_16_BYTES                   = 0x1,
    CPRI_VSS_TRANSACTION_32_BYTES                   = 0x2,
    CPRI_VSS_TRANSACTION_64_BYTES                   = 0x4,
    CPRI_VSS_TRANSACTION_128_BYTES                  = 0x8
} vss_bus_transaction_size_t;

/**************************************************************************//**
 @Description   Enumeration of CPRI BD oversampling available ratios
*//***************************************************************************/
typedef enum
{
    CPRI_OVERSAMPLING_RATIO_1                        = 0x1,
    CPRI_OVERSAMPLING_RATIO_2                        = 0x2,
    CPRI_OVERSAMPLING_RATIO_3                        = 0x3,
    CPRI_OVERSAMPLING_RATIO_4                        = 0x4,
    CPRI_OVERSAMPLING_RATIO_5                        = 0x5,
    CPRI_OVERSAMPLING_RATIO_6                        = 0x6,
    CPRI_OVERSAMPLING_RATIO_7                        = 0x7,
    CPRI_OVERSAMPLING_RATIO_8                        = 0x8
} oversampling_ratio_t;

/**************************************************************************//**

 @Description   Enumeration of CPRI sync groups
*//***************************************************************************/
typedef enum
{
    CPRI_CLOCK_DOMAIN_GROUP_1                        = 0x0,
    CPRI_CLOCK_DOMAIN_GROUP_2                        = 0x1
} clock_domain_group_t;

/**************************************************************************//**

 @Description   CPRI multicast mode parameters.

                This structure should be used as the CPRI LLD parameters
                for using multicast modes with CPRI .
*//***************************************************************************/
typedef struct
{
    cpri_multicast_mode_t       multicast_mode;                     /**< multicast mode  */
    iq_bus_transaction_size_t   unicast_receive_transaction_size;   /**< only for uni multicast - uni_multicast mode transaction size */
    uint32_t                    rx_buffer_size;                     /**< only for multicast - size of rx buffer for second destination*/
    uint32_t                    threshold;                          /**< the amount of received bytes for a IQ tx event*/
}cpri_multicast_params_t;

/**************************************************************************//**
 @Description   CPRI advanced mapping mode parameters.

                This structure should be used as the CPRI LLD parameters
                for using advanced mapping modes with CPRI .
*//***************************************************************************/
#if defined(CPRI_BLOCK_VER_4_1)
typedef struct
{
    uint32_t                 width0:5,           /**<30 or 32 bit sample. */
                             position0:5,        /**<I/Q Sample Start Bit Position */
                             axc0:5,             /**<AxC Number */
                             enable0:1,          /**<Mapping of I/Q Sample Into Timeslot Enable */
                             width1:5,           /**<30 or 32 bit sample. */
                             position1:5,        /**<I/Q Sample Start Bit Position */
                             axc1:5,             /**<AxC Number */
                             enable1:1;          /**<Mapping of I/Q Sample Into Timeslot Enable */
} mapping_table_entry_t;
#elif defined(CPRI_BLOCK_VER_4_2)
typedef struct
{
    uint32_t                 forward_axc:5,             /**<AxC Number to forward */
                             width:5,                   /**<30 or 32 bit sample. */
                             position:5,                /**<I/Q Sample Start Bit Position */
                             axc:5,                     /**<AxC Number */
                             enable_forwarding_axc:1,   /**<Mapping of Forwarding I/Q Sample Into Timeslot Enable */
                             enable_axc:1;              /**<Mapping of I/Q Sample Into Timeslot Enable */
} receive_mapping_table_entry_t;

typedef struct
{
    uint32_t                 forward_axc:5,             /**<AxC Number to forward */
                             summing_en:1,              /**<Enable Summing; if enabled and #enable_axc is disabled AxC defined in #forward_axc is forwarded.*/
                             width:5,                   /**<30 or 32 bit sample. */
                             position:5,                /**<I/Q Sample Start Bit Position */
                             axc:5,                     /**<AxC Number */
                             enable_axc:1;              /**<Mapping of I/Q Sample Into Timeslot Enable */
} transmit_mapping_table_entry_t;
#endif //CPRI_BLOCK_VER_4_1

/**************************************************************************//**
 @Description   CPRI advanced mapping mode parameters.

                This structure should be used as a mapping entry to the
                CPRI LLD parameters for using advanced mapping modes with CPRI .
*//***************************************************************************/
typedef struct
{
    mapping_mode_t                  mapping_mode;                   /**< Mapping mode used on the link*/
    oversampling_ratio_t            axc_oversampling_factor;        /**< Only for basic mapping - oversampling factor for AxC's on the link*/
    uint32_t                        mapping_table_configuration:7;  /**< Only for advanced mapping - Number of Basic Frames in AxC Container Block*/
#if defined(CPRI_BLOCK_VER_4_1)
    mapping_table_entry_t           *rx_mapping_table;              /**< Only for advanced mapping - pointer to beginning of the rx mapping table array - length up to 1920 */
    mapping_table_entry_t           *tx_mapping_table;              /**< Only for advanced mapping - pointer to beginning of the tx mapping table array - length up to 1920 */
#elif defined(CPRI_BLOCK_VER_4_2)
    receive_mapping_table_entry_t   *rx_mapping_table;              /**< Only for advanced mapping - pointer to beginning of the rx mapping table array - length up to 3072 */
    transmit_mapping_table_entry_t  *tx_mapping_table;              /**< Only for advanced mapping - pointer to beginning of the tx mapping table array - length up to 3072 */
#endif //CPRI_BLOCK_VER_4_1
    uint32_t                        tx_table_size;                  /**< Only for advanced mapping - length of tx table up to 960 */
    uint32_t                        rx_table_size;                  /**< Only for advanced mapping - length of rx table up to 960 */
} mapping_params_t;

/**************************************************************************//**
 @Description   CPRI interrupt types definition.

                This enum should be to define the different interrupt
                types supported by CPRI
 
                RER and TER interrupts can support the following events:
                CPRI_IQ_THRESHOLD_EVENT, CPRI_IQ_FIRST_THRESHOLD_EVENT,
                CPRI_IQ_SECOND_THRESHOLD_EVENT, CPRI_VSS_EVENT, CPRI_HDLC_EVENT, CPRI_HDLC_EVENT,
                CPRI_HFN_TIMING_EVENT, CPRI_BFN_TIMING_EVENT,
                CPRI_MULTICAST_RX_IQ_THRESHOLD_EVENT (RER only), CPRI_IQ_TX_BEGIN_EVENT (TER only)
                CPRI driver will assign RER/TER interrupts in consecutive order up to the number of available lines.
 
                Transmit control and receive control interrupts can support the following events:
                CPRI_VSS_EVENT, CPRI_HDLC_EVENT, CPRI_ETHERENT_EVENT
 
                transmit timing and receive timing interrupts can support the following events:
                CPRI_HFN_TIMING_EVENT, CPRI_BFN_TIMING_EVENT
*//***************************************************************************/
typedef enum
{
    CPRI_INT_RER                        = 0x0,
    CPRI_INT_TER                        = 0x1,
    CPRI_INT_TRANSMIT_CONTROL           = 0x2,
    CPRI_INT_RECEIVE_CONTROL            = 0x3,
    CPRI_INT_TRANSMIT_TIMING            = 0x4,
    CPRI_INT_RECEIVE_TIMING             = 0x5,
    CPRI_INT_ERROR                      = 0x6,
    CPRI_INT_LAST
} cpri_interrupt_type_t;

typedef struct
{
    cpri_interrupt_type_t   interrupt_type;    /**< interrupt type*/
    uint32_t                enabled_events;    /**< the enabled events for this interrupt*/
    os_hwi_priority         interrupt_priority;/**< the priority of the interrupt*/
} cpri_interrupt_t;

/**************************************************************************//**
 @Description   Initialization structure of CPRI IQ data
 
                This structure can be used for 2 purposes:
 
                1 - Given as CPRI IQ initialization parameters in #cpri_init_params_t.
 
                2 - Parameters for calling CPRI level2 reconfiguration.
 
                    The following parameters must not be changed compared to existing configuration:
                    a. Auxiliary configuration fields (auxiliary_mode, auxiliary_mask and auxiliary_mask_size)
                    b. Cores configuration (participating_cores and initializing_core)
                    c. Sync (transmit_iq_sync and receive_iq_sync)
 
                3 - Parameters for calling CPRI level0 reconfiguration.
                    In that case only cores configuration must equal to existing configuration.
 
                NOTE: for reconfiguration usage, participating_cores mask must contain only a single core.
 
*//***************************************************************************/
typedef struct
{
    uint8_t                     tx_axc_number;              /**< number of tx AxC's passing on the link - 1 to 24*/
    uint8_t                     rx_axc_number;              /**< number of rx AxC's passing on the link - 1 to 24*/
    uint8_t                     participating_cores;        /**< cores that will use IQ data type*/
    cpri_cores_t                initializing_core;          /**< core that will initialize the CPRI block*/
    uint32_t                    axc_tx_active;              /**< active tx AxC channels*/
    uint32_t                    axc_rx_active;              /**< active rx AxC channels*/
#if defined(CPRI_BLOCK_VER_4_2)
    uint32_t                    axc_tx_summing_active;      /**< AxC channels for summing */
    uint32_t                    axc_rx_forwarding_active;   /**< Forwarded AxC channels */
    uint32_t                    axc_tx_active_dma;          /**< Only if dual bandwidth mode is active - active tx AxC channels of the DMA, must be a subset of #axc_tx_active. */
    uint32_t                    axc_rx_active_dma;          /**< Only if dual bandwidth mode is active - active rx AxC channels of the DMA, must be a subset of #axc_rx_active. */
#endif //CPRI_BLOCK_VER_4_2
    uint32_t                    auxiliary_mode:1,           /**< auxiliary mode active */
#ifdef CPRI_BLOCK_VER_4_1
                                transmit_sync_active:1,     /**< determines if syncronization is required (to the 10ms frame) */
#endif // CPRI_BLOCK_VER_4_1
                                transmit_iq_sync:1,         /**< determines if the transfer of transmit IQ data should be synchronized to other CPRI modules in the group. */
                                receive_iq_sync:1,          /**< determines if the received IQ data should be synchronized to the other CPRIs in the group. */
#ifdef CPRI_BLOCK_VER_4_1
                                map_tx_hf_resync:1,         /**< TX Resynchronization Every Hyper Frame Enable*/
                                map_rx_hf_resync:1,         /**< RX Resynchronization Every Hyper Frame Enable*/
                                start_tx_hf_resync:1,       /**< enables synchronization every Hyper Frame nstead of every Radio Frame*/
#endif // CPRI_BLOCK_VER_4_1
                                tx_shared_mode:1,           /**< Selects if the CPRI pair operates in tx shared mode*/
                                rx_shared_mode:1            /**< Selects if the CPRI pair operates in rx shared mode*/
#if defined(CPRI_BLOCK_VER_4_2)
                                ,dual_bandwidth_mode:1,      /**< Double Bandwidth Mode active */
                                dual_8_bit_summing:1         /**< perform dual 8 bit summing */
#endif //CPRI_BLOCK_VER_4_1
                                ;
    mapping_params_t            *mapping_params;            /**< parameters for the mapping used by cpri*/
    cpri_multicast_params_t     *multicast_params;          /**< parameters multicast modes cpri*/
    sampling_width_select_t     sample_width;               /**< 15, 16 or 8 bits of i/q sample width*/
    iq_bus_transaction_size_t   tx_transaction_size;        /**< size of iq bus tx transaction*/
    iq_bus_transaction_size_t   rx_transaction_size;        /**< size of iq bus rx transaction*/
    uint32_t                    rx_buffer_size;             /**< size of rx buffer*/
    uint32_t                    tx_buffer_size;             /**< size of tx buffer*/
    uint32_t                    tx_first_threshold;         /**< the location of the first threshold in the AxC tx data buffers*/
    uint32_t                    tx_second_threshold;        /**< the location of the second threshold in the AxC tx data buffers*/
    uint32_t                    rx_first_threshold;         /**< the location of the first threshold in the AxC rx data buffers*/
    uint32_t                    rx_second_threshold;        /**< the location of the second threshold in the AxC rx data buffers*/
    uint32_t                    tx_threshold;               /**< the amount of transmitted bytes for a IQ tx event*/
    uint32_t                    rx_threshold;               /**< the amount of received bytes for a IQ tx event*/
    uint8_t                     map_tx_offset_z;            /**< hyper frame number for the start of TX side AxC Container Block*/
    uint8_t                     map_tx_offset_x;            /**< basic frame number for the start of TX side AxC Container Block*/
    uint8_t                     map_rx_offset_z;            /**< hyper frame number for the start of RX side AxC Container Block*/
    uint8_t                     map_rx_offset_x;            /**< basic frame number for the start of RX side AxC Container Block*/
    uint8_t                     start_tx_offset_z;          /**< Stores the hyper frame number for start of CPRI_TX_START synchronization output*/
    uint8_t                     start_tx_offset_x;          /**< Stores the basic frame number for start of CPRI_TX_START synchronization output*/
    uint32_t                    *auxiliary_mask;            /**< only if auxiliary_mode is active - pointer to the beginning of the mask registers array*/
    uint32_t                    auxiliary_mask_size;        /**< only if auxiliary_mode is active - mask registers array size*/
#if defined(CPRI_BLOCK_VER_4_2)
    uint32_t                    rx_symbol_awareness_int:1,          /**< Generate Receive Symbol Awareness interrupt (expected usage: toward DSP timer) */
                                rx_2nd_dest_symbol_awareness_int:1, /**< Generate 2nd destination receive Symbol Awareness interrupt (expected usage: toward DSP timer) */
                                tx_symbol_awareness_int:1,          /**< Generate transmit Symbol Awareness interrupt (expected usage: toward DSP timer) */
                                warm_reset_enable,                  /**< is warm reset active - must be active if L1 defense mode 1/2 is enabled*/
                                transmit_iq_sample_count:22;        /**< Transmit IQ Sample Count - to be configured in TIQSC register for L1 defense stop-restart usage
                                                                    this value is relevant only warm_reset_enable == CPRI_ACTIVE*/
#endif //CPRI_BLOCK_VER_4_2

} cpri_iq_init_params_t;

/**************************************************************************//**
 @Description   Adittional parameters passed to IQ initialization function.
*//***************************************************************************/
typedef struct
{
    uint32_t             rx_transparent_mode:1,     /**< rx transparent mode active */
                         tx_transparent_mode:1;     /**< tx transparent mode active */
} cpri_iq_additional_params_t;

/**************************************************************************//**
 @Description   Initialization structure of CPRI VSS data.
*//***************************************************************************/
typedef struct
{
    cpri_cores_t                initializing_core;          /**< core that will initialize and use the CPRI block (one only)*/
    vss_bus_transaction_size_t  tx_transaction_size;        /**<size of vss bus tx transaction*/
    vss_bus_transaction_size_t  rx_transaction_size;        /**<size of vss bus rx transaction*/
    uint32_t                    tx_buffer_size;             /**<size of tx buffer*/
    uint32_t                    rx_buffer_size;             /**<size of rx buffer*/
    uint32_t                    tx_threshold;               /**< the amount of transmitted bytes for a VSS tx event*/
    uint32_t                    rx_threshold;               /**< the amount of received bytes for a VSS rx event*/

} cpri_vss_init_params_t;

/**************************************************************************//**
 @Description   Initialization structure of CPRI Ethernet data.
 
                This structure can be used for 2 purposes:
 
                1 - Given as Ethernet initialization parameters in #cpri_init_params_t.
 
                2 - Parameters for calling CPRI level0 reconfiguration.
                    The following parameters must not be changed compared to existing configuration:
                    a. Initializing core
                    b. #tx_bd_ring_size and #rx_bd_ring_size
 
*//***************************************************************************/
typedef struct
{
    cpri_cores_t                initializing_core;             /**< core that will initialize and use the CPRI block (one only)*/
    uint32_t                    tx_crc_enable:1,               /**<Enable insertion of the Ethernet FCS at the end of the frame. */
                                rx_crc_enable:1,               /**<Enable checking of received frames CRC. */
                                rx_long_ethernet_frame_ena:1,  /**<Enables receipt of RX frames >1536 bytes. */
                                rx_illegal_preamble_dis_ena:1, /**<Enables discard of RX frames with illegal preamble nibble before receipt of SFD.. */
                                rx_broadcast_enable:1,         /**<Enables RX of broadcast packets. */
                                rx_multicast_enable:1,         /**<Enables RX of the subset of multicast Ethernet packets enabled by the hash function. */
                                rx_mac_address_check:1,        /**<Enables RX MAC address checking. */
                                tx_store_and_forward_enable:1, /**<Used to select TX store and forward mode - frame length should be less than 512bytes in store and forward mode*/
                                rx_packet_length_chk_enable:1, /**<Enables RX packet length checking */
                                little_endian_selection:1,     /**<Select the endian mode to use for Ethernet RX and TX data (0 - big, 1- little) */
                                tx_interrupt_en:1,             /**< If TRUE, enable transmitted frames interrupt for Tx confirmation for this channel. */
                                tx_coherency_en:1,             /**< If TRUE, flushes frame from cache before transmit (buffers must be in system area only) */
                                rx_interrupt_en:1,             /**< If TRUE, enable transmitted frames interrupt for Rx confirmation for this channel. */
                                rx_coherency_en:1              /**< If TRUE, flushes frame from cache before transmit (buffers must be in system area only) */
                                ;
    uint32_t                    mac_address_lsb;               /**<only if rx_mac_address_check enabled - Ethernet address least significant bits*/
    uint16_t                    mac_address_msb;               /**<only if rx_mac_address_check enabled - Ethernet address most significant bits*/
    uint32_t                    rx_buffer_size;                /**<size of rx buffer*/
    uint32_t                    hash_filter;                   /**<relevant only for multicast - hash table to filter multicast traffic */
    uint8_t                     tx_bd_ring_size;               /**<receive ethernet buffer descriptor ring size */
    uint8_t                     rx_bd_ring_size;               /**<Transmit ethernet buffer descriptor ring size */
    uint8_t                     tx_coalescing_threshold;       /**< tx coalescing - the number of transmitted packets for a ethernet tx event - 1*/
    uint8_t                     rx_coalescing_threshold;       /**< rx coalescing - the number of received packets for a ethernet tx event -1 */
    os_mem_type                 tx_heap;                        /**< Heap to use for a channel's BD ring - NOTE: local heap should be used (cpri ethernet
                                                                can be used by a single core only*/
    os_mem_type                 rx_heap;                        /**< Heap to use for a channel's BD ring - NOTE: local heap should be used (cpri ethernet
                                                                can be used by a single core only*/
} cpri_ethernet_init_params_t;

/**************************************************************************//**
 @Description   Initialization structure of CPRI HDLC data.
 
                This structure can be used for 2 purposes:
 
                1 - Given as HDLC initialization parameters in #cpri_init_params_t.
 
                2 - Parameters for calling CPRI level0 reconfiguration.
                    The following parameters must not be changed compared to existing configuration:
                    a. Initializing core
                    b. #tx_bd_ring_size and #rx_bd_ring_size

*//***************************************************************************/
typedef struct
{
    cpri_cores_t                initializing_core;             /**< core that will initialize and use the CPRI block (one only)*/
    uint32_t                    tx_crc_enable:1,               /**<Enable insertion of the HDLC FCS at the end of the frame. */
                                rx_crc_enable:1,               /**<Enable checking of received frames CRC. */
                                tx_store_and_forward_enable:1, /**<Used to select TX store and forward mode */
                                rx_packet_length_chk_enable:1, /**<Enables RX packet length checking */
                                little_endian_selection:1,     /**<Select the endian mode to use for HDLC RX and TX data (0 - big, 1- little) */
                                tx_interrupt_en:1,             /**< If TRUE, enable transmitted frames interrupt for Tx confirmation for this channel. */
                                tx_coherency_en:1,             /**< If TRUE, flushes frame from cache before transmit (buffers must be in system area only) */
                                rx_interrupt_en:1,             /**< If TRUE, enable transmitted frames interrupt for Rx confirmation for this channel. */
                                rx_coherency_en:1              /**< If TRUE, flushes frame from cache before transmit (buffers must be in system area only) */
                                ;
    uint32_t                    rx_buffer_size;                /**<size of rx buffer*/
    uint8_t                     tx_bd_ring_size;               /**<receive HDLC buffer descriptor ring size */
    uint8_t                     rx_bd_ring_size;               /**<Transmit HDLC buffer descriptor ring size */
    os_mem_type                 tx_heap;                       /**< Heap to use for a channel's BD ring - NOTE: local heap should be used (cpri hdlc
                                                                can be used by a single core only*/
    os_mem_type                 rx_heap;                        /**< Heap to use for a channel's BD ring - NOTE: local heap should be used (cpri hdlc
                                                                can be used by a single core only*/
} cpri_hdlc_init_params_t;

/**************************************************************************//**
 @Description   Initialization structure of CPRI.
 
                This structure can be used for 2 purposes:
 
                1 - CPRI initialization parameters structure in system bringup phase.
 
                2 - Passed as a parameter for calling CPRI level0 reconfiguration.
                    The following parameters must not be changed compared to existing configuration:
                    a.  cpri_iq_init, cpri_vss_init, cpri_ethernet_init and cpri_hdlc_init.
 
                3 - Passed as a parameter for calling CPRI level1 reconfiguration.
                    The following parameters must not be changed compared to existing configuration:
                    a.  cpri_iq_init, cpri_vss_init, cpri_ethernet_init and cpri_hdlc_init.
                    b.  loop_mode.
                    c.  interrupts_table.
 
*//***************************************************************************/
typedef struct
{
    cpri_num_t                  cpri_num;                      /**< number of CPRI block to initialize */
    cpri_loop_mode_t            loop_mode;                     /**< loop mode to run CPRI with */
    cpri_sync_mode_t            sync_mode;                     /**< CPRI master or slave */
    ext_sync_act_t              external_sync_active;          /**< sync pulse source*/
#if defined(CPRI_BLOCK_VER_4_2)
    shared_sync_mode_source_t   shared_sync_mode_source;        /**< shared sync mode, used only if #shared_sync_mode is active. */
    sync_mode_pulse_t           sync_mode_pulse;                /**< relevant only when #sync_mode is slave. set the origin of transmit sync pulse.*/
    reset_ack_assert_length_t   reset_ack_assert_length;        /**< For how many HFs reset ack is asserted? */
    reset_detection_length_t    reset_detection_length;         /**< reset ack/req detection length */
#endif //CPRI_BLOCK_VER_4_2
    uint32_t                     rx_transparent_mode:1,         /**< rx transparent mode active */
                                 tx_transparent_mode:1,         /**< tx transparent mode active */
                                 transmit_sync_output:1,        /**< should transmit sync be an output (only if external_sync_active is SYNC_GENERATED_LOCALLY)*/
                                 sync_state_machine_ena:1,      /**< Sync state machine enable*/
                                 shared_sync_mode:1,            /**< independent(0) / shared(1) sync mode*/
#if defined(CPRI_BLOCK_VER_4_2)
                                 delay_direction:1,             /**< When #external_sync_active is set to SYNC_GENERATED_EXTERNALLY, define sync direction to positive (active) or negative (not active) */
                                 sync1588:1,                    /**< if active the 10ms sync pulse comes from block 1588. */
                                 remote_reset_indication:1,     /**< Active if this unit participates in external remote reset indication; master unit indicates reset acknowledge and slave unit indicates reset request.*/
#endif //CPRI_BLOCK_VER_4_2
                                 number_of_errors:2,            /**< number of sync errors to hunt state */
                                 tx_framer_buffer_size:7,       /**< Transmit CPRI Framer Buffer size. 0 -> default size */
                                 vss_eth_portion_in_frame:6,    /**< Used to select Ethernet/VSS portion in CPRI frame, 0x14 is maximal Ethernet, 0x3f is maximal VSS */
#if defined(CPRI_BLOCK_VER_4_1)
                                 tx_ex_delay:6;                 /**< Not used. Kept for backward compatibility. */
#elif defined(CPRI_BLOCK_VER_4_2)
                                 tx_ex_delay:6;                  /**< additional transmit delay (for delay measurement)*/
#endif //CPRI_BLOCK_VER_4_1
    uint32_t                    scarmbling_seed;                /**< Seed for scrambling. 0 if scrambling is not desired */
#if defined(CPRI_BLOCK_VER_4_2)
    uint32_t                    timer_tolerance_value;          /**< Timer tolerance value in case of external sync pulse */
#endif //CPRI_BLOCK_VER_4_2
    uint8_t                     transmit_delay;                /**< TX delay to configure on the link */
    uint8_t                     sync_delay;                    /**< sync delay to configure on the link */
    cpri_interrupt_t            *interrupts_table;             /**< pointer to beginning of cpri_interrupts array */
    arch_specific               extended_features;             /**< init extended features */
 
    os_status    (*cpri_iq_init)(cpri_num_t cpri_num, cpri_iq_init_params_t *init_params,
                  cpri_iq_additional_params_t *additional_init_params);
                 /**< pointer to cpriIqInitialize function used to enable/disable IQ path*/
 
    os_status    (*cpri_vss_init)(cpri_num_t cpri_num, cpri_vss_init_params_t *init_params
#if defined(CPRI_BLOCK_VER_4_1)
        , void **vss_handle
#endif //CPRI_BLOCK_VER_4_1
);
                 /**< pointer to cpriVssInitialize function used to enable/disable VSS path*/
    os_status    (*cpri_ethernet_init)(cpri_num_t cpri_num, cpri_ethernet_init_params_t *init_params
#if defined(CPRI_BLOCK_VER_4_1)
        , void **ethernet_handle
#endif //CPRI_BLOCK_VER_4_1
);
                 /**< pointer to cpriEthernetInitialize function used to enable/disable Ethernet path*/
    os_status    (*cpri_hdlc_init)(cpri_num_t cpri_num, cpri_hdlc_init_params_t *init_params
#if defined(CPRI_BLOCK_VER_4_1)
        , void **hdlc_handle
#endif //CPRI_BLOCK_VER_4_1
);
                 /**< pointer to cpriHdlcInitialize function used to enable/disable HDLC path*/
 
    cpri_iq_init_params_t       *iq_init_params;        /**< pointer to iq_init_params*/
    cpri_vss_init_params_t      *vss_init_params;       /**< pointer to vss_init_params*/
    cpri_ethernet_init_params_t *ethernet_init_params;  /**< pointer to ethernet_init_params*/
    cpri_hdlc_init_params_t     *hdlc_init_params;      /**< pointer to hdlc_init_params*/
 
} cpri_init_params_t;

/**************************************************************************//**
 @Description   CPRI auxiliary mode parameters.

                This structure should be used as the CPRI LLD parameters
                for using auxiliary mode with CPRI .
*//***************************************************************************/
typedef struct
{
    uint32_t                 *mask;             /**< pointer to the beginning of the mask registers array*/
    uint32_t                 mask_size;         /**< mask registers array size*/
} cpri_auxiliary_params_t;

/**************************************************************************//**
 @Function      cpriInitialize

 @Description   Initializes the CPRI driver's structures

                The CPRI main initialization function
 
 @Param[in]    global_parms - cpri global initialization parameters structure
 @Param[in]    init_params -  cpri Initialization parameters array.
 
 @Return        OS status

*//***************************************************************************/
os_status   cpriInitialize           (cpri_global_init_params_t *global_parms, cpri_init_params_t (*init_params)[]);
/**************************************************************************//**
 @Function      cpriIqInitialize

 @Description   Initializes the CPRI driver's structures

                The CPRI IQ initialization function

 @Param[in]    cpri_num -  cpri CPRI block number.
 @Param[in]    init_params -  cpri IQ Initialization parameters.
 @Param[in]    additional_init_params - additional cpri IQ Initialization parameters.
 
 @Return        OS status

*//***************************************************************************/
os_status   cpriIqInitialize        (cpri_num_t cpri_num, cpri_iq_init_params_t *init_params,
                                     cpri_iq_additional_params_t *additional_init_params);

/**************************************************************************//**
 @Function      cpriVssInitialize

 @Description   Initializes the CPRI driver's structures

                The CPRI VSS initialization function

 @Param[in]    cpri_num -  cpri CPRI block number.
 @Param[in]    init_params -  cpri VSS Initialization parameters.
 @Param[out]   vss_handle (B4 family only) - the returned VSS handler.

 
 @Return        OS status
*//***************************************************************************/
os_status   cpriVssInitialize       (cpri_num_t cpri_num, cpri_vss_init_params_t *init_params
#if defined(CPRI_BLOCK_VER_4_1)
        , void **vss_handle
#endif //CPRI_BLOCK_VER_4_1
);

/**************************************************************************//**
 @Function      cpriEthernetInitialize

 @Description   Initializes the CPRI driver's structures

                The CPRI VSS Ethernet function

 @Param[in]    cpri_num -  cpri CPRI block number.
 @Param[in]    init_params -  cpri Ethernet Initialization parameters.
 @Param[out]   ethernet_handle (B4 family only) - the returned Ethernet handler.
 
 @Return        OS status
*//***************************************************************************/
os_status   cpriEthernetInitialize  (cpri_num_t cpri_num, cpri_ethernet_init_params_t *init_params
#if defined(CPRI_BLOCK_VER_4_1)
        , void **ethernet_handle
#endif //CPRI_BLOCK_VER_4_1
);

/**************************************************************************//**
 @Function      cpriHdlcInitialize

 @Description   Initializes the CPRI driver's structures

                The CPRI HLCD initialization function

 @Param[in]    cpri_num -  cpri CPRI block number.
 @Param[in]    init_params -  cpri HDLC Initialization parameters.
 @Param[out]   hdlc_handle (B4 family only) - the returned HDLC handler.
 
 @Return        OS status
*//***************************************************************************/
os_status   cpriHdlcInitialize      (cpri_num_t cpri_num, cpri_hdlc_init_params_t *init_params
#if defined(CPRI_BLOCK_VER_4_1)
        , void **hdlc_handle
#endif //CPRI_BLOCK_VER_4_1
);


/**************************************************************************//**
 @Description   CPRI interrupt definition data structures.

                This structure should be used to define the interrutps used for
                CPRI and the events represented by each interrupt .
*//***************************************************************************/


/*************************************************************//**
 @Collection    cpri interrupts events - values according to ICR and RER/TER registers
 
 @{
*//**************************************************************/
#define  CPRI_IQ_THRESHOLD_EVENT                0x00000001
#define  CPRI_IQ_FIRST_THRESHOLD_EVENT          0x00000002
#define  CPRI_IQ_SECOND_THRESHOLD_EVENT         0x00000004
#define  CPRI_VSS_EVENT                         0x00000008
#define  CPRI_HDLC_EVENT                        0x00000010
#define  CPRI_ETHERNET_EVENT                    0x00000020
#define  CPRI_HFN_TIMING_EVENT                  0x00000040
#define  CPRI_BFN_TIMING_EVENT                  0x00000080
#define  CPRI_MULTICAST_RX_IQ_THRESHOLD_EVENT   0x00000100
#define  CPRI_IQ_TX_BEGIN_EVENT                 0x00000100
#define  CPRI_ALL_TER_RER_EVENTS    (CPRI_IQ_THRESHOLD_EVENT |\
                                     CPRI_IQ_FIRST_THRESHOLD_EVENT |\
                                     CPRI_IQ_SECOND_THRESHOLD_EVENT | \
                                     CPRI_VSS_EVENT | \
                                     CPRI_HDLC_EVENT | \
                                     CPRI_ETHERNET_EVENT | \
                                     CPRI_HFN_TIMING_EVENT | \
                                     CPRI_BFN_TIMING_EVENT | \
                                     CPRI_MULTICAST_RX_IQ_THRESHOLD_EVENT | \
                                     CPRI_IQ_TX_BEGIN_EVENT)
/* @} */


/*************************************************************//**
 @Collection    cpri error events
 
 @{
*//**************************************************************/
#define CPRI_RECEIVE_IQ_OVERRUN_ERROR                      0x00000001
#define CPRI_TRANSMIT_IQ_UNDERRUN_ERROR                    0x00000002
#define CPRI_RECEIVE_ETH_MEMORY_OVERRUN_ERROR              0x00000004
#define CPRI_TRANSMIT_ETH_UNDERRUN_ERROR                   0x00000008
#define CPRI_RECEIVE_ETH_BUFFER_DESCRIPTOR_UNDERRUN_ERROR  0x00000010
#define CPRI_RECEIVE_HDLC_OVERRUN_ERROR                    0x00000020
#define CPRI_TRANSMIT_HDLC_UNDERRUN_ERROR                  0x00000040
#define CPRI_RECEIVE_HDLC_BUFFER_DESCRIPTOR_UNDERRUN_ERROR 0x00000080
#define CPRI_RECEIVE_VSS_OVERRUN_ERROR                     0x00000100
#define CPRI_TRANSMIT_VSS_UNDERRUN_ERROR                   0x00000200
#define CPRI_EXTERNAL_SYNC_SYNCHRONIZATION_LOST_ERROR      0x00008000
#define CPRI_REMOTE_LOS_ERROR                              0x00010000
#define CPRI_REMOTE_LOF_ERROR                              0x00020000
#define CPRI_REMOTE_RAI_ERROR                              0x00040000
#define CPRI_REMOTE_SDI_ERROR                              0x00080000
#define CPRI_LOCAL_LOS_ERROR                               0x00100000
#define CPRI_LOCAL_LOST_OF_FRAME_ERROR                     0x00200000
#define CPRI_FREQUENCY_ALARM_ERROR                         0x00800000
#define CPRI_REMOTE_RESET_ERROR                            0x00400000
#if defined(CPRI_BLOCK_VER_4_2)
#define CPRI_ECC_CONFIGURATION_MEMORY_ERROR                0x00000400
#define CPRI_ECC_DATA_MEMORY_ERROR                         0x00000800
#define CPRI_REMOTE_RESET_ACKNOWLEDGE                      0x01000000
#else
#define CPRI_ECC_CONFIGURATION_MEMORY_ERROR                0x00000000
#define CPRI_ECC_DATA_MEMORY_ERROR                         0x00000000
#define CPRI_REMOTE_RESET_ACKNOWLEDGE                      0x00000000
#endif //CPRI_BLOCK_VER_4_2
/* @} */

#define CPRI_ALL_ERRORS       (CPRI_RECEIVE_IQ_OVERRUN_ERROR | \
                               CPRI_TRANSMIT_IQ_UNDERRUN_ERROR | \
                               CPRI_RECEIVE_ETH_MEMORY_OVERRUN_ERROR | \
                               CPRI_TRANSMIT_ETH_UNDERRUN_ERROR |  \
                               CPRI_RECEIVE_ETH_BUFFER_DESCRIPTOR_UNDERRUN_ERROR | \
                               CPRI_RECEIVE_HDLC_OVERRUN_ERROR | \
                               CPRI_TRANSMIT_HDLC_UNDERRUN_ERROR | \
                               CPRI_RECEIVE_HDLC_BUFFER_DESCRIPTOR_UNDERRUN_ERROR | \
                               CPRI_RECEIVE_VSS_OVERRUN_ERROR | \
                               CPRI_TRANSMIT_VSS_UNDERRUN_ERROR | \
                               CPRI_EXTERNAL_SYNC_SYNCHRONIZATION_LOST_ERROR | \
                               CPRI_REMOTE_LOS_ERROR | \
                               CPRI_REMOTE_LOF_ERROR | \
                               CPRI_REMOTE_RAI_ERROR | \
                               CPRI_REMOTE_SDI_ERROR | \
                               CPRI_LOCAL_LOS_ERROR | \
                               CPRI_LOCAL_LOST_OF_FRAME_ERROR | \
                               CPRI_FREQUENCY_ALARM_ERROR | \
                               CPRI_ECC_CONFIGURATION_MEMORY_ERROR | \
                               CPRI_ECC_DATA_MEMORY_ERROR | \
                               CPRI_REMOTE_RESET_ERROR | \
                               CPRI_REMOTE_RESET_ACKNOWLEDGE) /**< bitwise-or of all CPRI errors */


/*************************************************************//**
 @Collection    CPRI ECC multibit errors
 
 @{
*//**************************************************************/
#define CPRI_ERROR_MULTIBIT_ECC_SECOND_DESTINATION_DMA_MEMORY       0X00000001
#define CPRI_ERROR_MULTIBIT_ECC_COMBINER_MEMORY                     0X00000002
#define CPRI_ERROR_MULTIBIT_ECC_FRAME_ELASTIC_BUFFER_MEMORY         0X00000004
#define CPRI_ERROR_MULTIBIT_ECC_AUX_MEMORY                          0X00000008
#define CPRI_ERROR_MULTIBIT_ECC_FRAMER_TRANSMIT_HDLC_MEMORY         0X00000010
#define CPRI_ERROR_MULTIBIT_ECC_FRAMER_RECEIVE_HDLC_MEMORY          0X00000020
#define CPRI_ERROR_MULTIBIT_ECC_FRAMER_TRANSMIT_ETH_MEMORY          0X00000040
#define CPRI_ERROR_MULTIBIT_ECC_FRAMER_RECEIVE_ETH_MEMORY           0X00000080
#define CPRI_ERROR_MULTIBIT_ECC_TRANSMIT_CONTROL_WORD_TABLE_MEMORY  0X00000100
#define CPRI_ERROR_MULTIBIT_ECC_RECEIVE_CONTROL_WORD_TABLE_MEMORY   0X00000200
#define CPRI_ERROR_MULTIBIT_ECC_RECEIVE_DMA_MEMORY                  0X00000800
#define CPRI_ERROR_MULTIBIT_ECC_FRAMER_TRANSMIT_IQ_MEMORY           0X00001000
#define CPRI_ERROR_MULTIBIT_ECC_FRAMER_RECEIVE_IQ_MEMORY            0X00002000
#define CPRI_ERROR_MULTIBIT_ECC_TRANSMIT_CONFIGURATION_MEMORY       0X00004000
#define CPRI_ERROR_MULTIBIT_ECC_RECEIVE_CONFIGURATION_MEMORY        0X00008000
/* @} */

/** @} */ // end of cpri_init_id
/** @} */ // end of cpri_id

_OS_END_EXTERN_C

#endif // __CPRI_INIT_H


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
 $Date: 2014/08/21 12:34:45 $
 $Id: cpri.h,v 1.47 2014/08/21 12:34:45 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/cpri/cpri.h,v $
 $Revision: 1.47 $
******************************************************************************/

/**************************************************************************//**

 @File          cpri.h

 @Description   CPRI external header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __CPRI_H
#define __CPRI_H

#include "os_cpp.h"

_OS_BEGIN_EXTERN_C

#include "cpri_init.h"

#define CPRI_DEVICE_NAME        "cpri"


/**************************************************************************//**
 @Group         cpri_id   CPRI Module API

 @Description   CPRI runtime API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         cpri_runtime_id CPRI runtime
 
 @Description   CPRI runtime API.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   CPRI delay calculation structure.

                This structure should be used for the cpriDelaysCalculate to put the
                calculated data in
 
*//***************************************************************************/
typedef struct
{
    double                 t_ext;              /**< input - the sum of RX,TX physical and optical delays on the board in nano seconds.
                                                if the value is not known, 0 can be used and accuracy will degrade accordingly.
                                                (refer to RM for more details on delays - SFP_RXREC, SFP_TXREC, SFP_RXRE, SFP_TXRE)*/
    double                 t_14 ;              /**< calculated delay*/
#if defined(CPRI_BLOCK_VER_4_1)
    double                 t_offset ;          /**< calculated delay - relevant for multi-hop configurations only*/
#elif defined(CPRI_BLOCK_VER_4_2)
    double                  tb_delay_dl;        /**< calculated TB_delay_DL */
    double                  tb_delay_ul;        /**< calculated TB_delay_UL */
#endif //CPRI_BLOCK_VER_4_1
} cpri_delays_t;

/**************************************************************************//**
 @Description   CPRI control table entry data.

                This structure should be used for writing/reading data for a single
                control word entry
 
*//***************************************************************************/
typedef struct
{
    uint8_t                address;            /**< address of the control table entry*/
    uint32_t               data_register_0;    /**< data register*/
    uint32_t               data_register_1;    /**< data register*/
#if defined(CPRI_BLOCK_VER_4_1)
    uint16_t               data_register_2;    /**< data register*/
#elif defined(CPRI_BLOCK_VER_4_2)
    uint32_t               data_register_2;    /**< data register*/
    uint32_t               data_register_3;    /**< data register*/
#endif //CPRI_BLOCK_VER_4_1
} cpri_control_table_entry_t;


/**************************************************************************//**
 @Description   CPRI IQ opening parameters.

                This structure should be used when calling osSioDeviceOpen()
                with the CPRI IQ device handle.

*//***************************************************************************/
typedef struct
{
#if defined(CPRI_BLOCK_VER_4_1)
    uint8_t                     *multicast_buffer_data_base;            /**< to be used only in multicast or multiunicast modes (otherwise can be 0). */
#elif defined(CPRI_BLOCK_VER_4_2)
    uint64_t                    multicast_buffer_data_base;             /**< to be used only in multicast or multiunicast modes (otherwise can be 0). It includes the steering bits field. */
    cpri_buffer_attributes_t    *buffer_attributes;                      /**< 2nd destination buffer attributes */
#endif // CPRI_BLOCK_VER_4_1
    void        (*hfn_tx)(cpri_num_t cpri_num, uint32_t event, void* data);            /**< Callback for handling CPRI_HFN_TIMING_EVENT event (initializing core only). */
    void        (*hfn_rx)(cpri_num_t cpri_num, uint32_t event, void* data);            /**< Callback for handling CPRI_HFN_TIMING_EVENT event (initializing core only). */
    void        (*bfn_tx)(cpri_num_t cpri_num, uint32_t event, void* data);            /**< Callback for handling CPRI_BFN_TIMING_EVENT event (initializing core only). */
    void        (*bfn_rx)(cpri_num_t cpri_num, uint32_t event, void* data);            /**< Callback for handling CPRI_BFN_TIMING_EVENT event (initializing core only). */
    void        (*iq_tx_begin)(cpri_num_t cpri_num, uint32_t event, void* data);       /**< Callback for handling CPRI_IQ_TX_BEGIN_EVENT event (initializing core only). */
    void        (*iq_multicast_rx)(cpri_num_t cpri_num, uint32_t event, void* data);   /**< Callback for handling CPRI_MULTICAST_RX_IQ_THRESHOLD_EVENT event (initializing core only). */
    void        (*error_callback)(void* param);                                         /**< allback for handling general error interrupts (initializing core only) */
} cpri_iq_open_params_t;

/**************************************************************************//**
 @Description   CPRI VSS opening parameters.

                This structure should be used when calling osSioDeviceOpen()
                with the CPRI VSS device handle.

*//***************************************************************************/
typedef struct
{
    void        (*error_callback)(void* param);
    /**< Callback for handling CPRI_MULTICAST_RX_IQ_THRESHOLD_EVENT event (master core only). */
} cpri_vss_open_params_t;


/**************************************************************************//**
 @Description   CPRI Ethernet opening parameters.

                This structure should be used when calling osBioDeviceOpen()
                with the CPRI Ethernet device handle.

*//***************************************************************************/
typedef struct
{
    void        (*error_callback)(void* param);
    /**< Callback for handling general error interrupts (master core only). */
} cpri_ethernet_open_params_t;


#ifdef CPRI_BLOCK_VER_4_2
/**************************************************************************//**
 @Description   CPRI Ethernet channel opening parameters.

                This structure should be used when opening a CPRI Ethernet channel

*//***************************************************************************/
typedef struct
{
    uint32_t                    bd_ring_steering_bits:3,/**< steering bits for the BD ring) */
    buffer_steering_bits:3;              /**< steering bits for the data buffers */
    cpri_buffer_attributes_t    bd_ring_attributes;   /**< BD ring attributes */
    cpri_buffer_attributes_t    buffer_attributes;   /**< buffer attributes */
} cpri_ethernet_channel_params_t;
#endif //CPRI_BLOCK_VER_4_2

/**************************************************************************//**
 @Description   CPRI HDLC opening parameters.

                This structure should be used when calling osBioDeviceOpen()
                with the CPRI HDLC device handle.

*//***************************************************************************/
typedef struct
{
    void        (*error_callback)(void* param);
    /**< Callback for handling general error interrupts (master core only). */
} cpri_hdlc_open_params_t;


#ifdef CPRI_BLOCK_VER_4_2
/**************************************************************************//**
 @Description   CPRI HDLC channel opening parameters.

                This structure should be used when opening a CPRI Ethernet channel

*//***************************************************************************/
typedef struct
{
    uint32_t                    bd_ring_steering_bits:3,/**< steering bits for the BD ring) */
    buffer_steering_bits:3;              /**< steering bits for the data buffers */
    cpri_buffer_attributes_t    bd_ring_attributes;   /**< BD ring attributes */
    cpri_buffer_attributes_t    buffer_attributes;   /**< buffer attributes */
} cpri_hdlc_channel_params_t;
#endif //CPRI_BLOCK_VER_4_2

/**************************************************************************//**
 @Description   CPRI IQ interrupts callback type.

                This structure should be used when supplying a callback for IQ device

*//***************************************************************************/
typedef struct
{
    void*      callback_param;
    /**< Callback for handling IQ interrupts. */
    cpri_num_t cpri_num;
    /**< CPRI block */
    uint32_t   event_type;
    /**< Event description. */
} cpri_iq_int_cb_param_t;


/**************************************************************************//**
 @Description   CPRI IQ channel opening parameters.

                This structure should be used when opening a CPRI IQ channel

*//***************************************************************************/
typedef struct
{
#ifdef CPRI_BLOCK_VER_4_1
    uint8_t     stub;                                   //to be removed
#elif defined(CPRI_BLOCK_VER_4_2)
    uint32_t                    steering_bits:3;
    cpri_buffer_attributes_t    buffer_attributes;
#endif // CPRI_BLOCK_VER_4_1
} cpri_channel_params_t;



/**************************************************************************//**
 @Description   CPRI error parameters.

                This sturcture is passed to the error callback when error
                event occurs. It contains the needed information so that
                the callback can know which CPRI unit and what is the error.

*//***************************************************************************/
typedef struct
{
    cpri_num_t          cpri_num;
    /**< CPRI block */
    uint32_t            error_events;
    /**< Error events description. */
 
#if defined(CPRI_BLOCK_VER_4_2)
    uint32_t            error_ecc_multibit_events;
    /**< ECC multibit error events description. */
#endif // CPRI_BLOCK_VER_4_2
} cpri_error_param_t;



#if defined(CPRI_BLOCK_VER_4_2)
/**************************************************************************//**
 @Description   CPRI reconfiguration level0 parameters.

                This structure is passed to the reconfiguration level0 flow,
                which is invoked via #CPRI_DEVICE_RECONFIGURATION_LEVEL0
                SIO device control command.
*//***************************************************************************/
typedef struct
{
        cpri_link_rate_t    maximal_desired_link_rate;                      /**<maximal link rate. must not be greater then RCWLR defined rate*/
        cpri_link_rate_t    minimal_accepted_link_rate;                     /**<minimal link rate accepted without failure*/
        cpri_init_params_t (*cpri_init_params)[];                           /**<reconfiguration parameters of all the CPRI units */
} cpri_reconfiguration_level0_param_t;
#endif // CPRI_BLOCK_VER_4_2

/**************************************************************************//**
 @Function      cpriRerInterruptRoute

 @Description   route the rer interrupts
 
                This function is calling the appropriate interrupts service routines
 
 @Param[in]     cpri -  cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriRerInterruptRoute(os_hwi_arg cpri);

/**************************************************************************//**
 @Function      cpriRerInterruptRoute

 @Description   route the rer interrupts
 
                This function is calling the appropriate interrupts service routines
 
 @Param[in]     cpri - cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriTerInterruptRoute(os_hwi_arg cpri);
/**************************************************************************//**
 @Function      cpriTxControlInterruptRoute

 @Description   route the tx control interrupts
 
                This function is calling the appropriate interrupts service routines
 
 @Param[in]     cpri - cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriTxControlInterruptRoute(os_hwi_arg cpri);

/**************************************************************************//**
 @Function      cpriRxControlInterruptRoute

 @Description   route the rx control interrupts
 
                This function is calling the appropriate interrupts service routines
 
 @Param[in]     cpri -   cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriRxControlInterruptRoute(os_hwi_arg cpri);

/**************************************************************************//**
 @Function      cpriTxTimingInterruptRoute

 @Description   route the tx timing interrupts
 
                This function is calling the appropriate interrupts service routines
 
 @Param[in]     cpri -   cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriTxTimingInterruptRoute(uint32_t cpri);

/**************************************************************************//**
 @Function      cpriRxTimingInterruptRoute

 @Description   route the tx timing interrupts
 
                This function is calling the appropriate interrupts service routines
 
 @Param[in]     cpri - cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriRxTimingInterruptRoute(os_hwi_arg cpri);

/**************************************************************************//**
 @Function      cpriErrorInterruptHandle

 @Description   handle error interrupt
 
                This function is calling the user error callback function, if any
 
 @Param[in]     cpri -   cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriErrorInterruptHandle(os_hwi_arg cpri);

/**************************************************************************//**
 @Function      cpriIqRxIsr

 @Description   handle iq rx interrupt
 
 @Param[in]     cpri_iq_handle - iq device handle
 
 @Return        none

*//***************************************************************************/
void cpriIqRxIsr(os_hwi_arg cpri_iq_handle);

/**************************************************************************//**
 @Function      cpriIqFirstRxIsr

 @Description   handle iq rx first threshold interrupt
 
 @Param[in]     cpri_iq_handle - iq device handle
 
 @Return        none

*//***************************************************************************/
void cpriIqFirstRxIsr(os_hwi_arg cpri_iq_handle);

/**************************************************************************//**
 @Function      cpriIqSecondRxIsr

 @Description   handle iq rx second threshold interrupt
 
 @Param[in]     cpri_iq_handle - iq device handle
 
 @Return        none

*//***************************************************************************/
void cpriIqSecondRxIsr(os_hwi_arg cpri_iq_handle);

/**************************************************************************//**
 @Function      cpriIqTxIsr

 @Description   handle iq tx interrupt
 
 @Param[in]     cpri_iq_handle - iq device handle
 
 @Return        none

*//***************************************************************************/
void cpriIqTxIsr(os_hwi_arg cpri_iq_handle);

/**************************************************************************//**
 @Function      cpriIqFirstTxIsr

 @Description   handle iq tx first threshold interrupt
 
 @Param[in]     cpri_iq_handle - iq device handle
 
 @Return        none

*//***************************************************************************/
void cpriIqFirstTxIsr(os_hwi_arg cpri_iq_handle);

/**************************************************************************//**
 @Function      cpriIqSecondTxIsr

 @Description   handle iq tx second threshold interrupt
 
 @Param[in]     cpri_iq_handle - iq device handle
 
 @Return        none

*//***************************************************************************/
void cpriIqSecondTxIsr(os_hwi_arg cpri_iq_handle);


/**************************************************************************//**
 @Function      cpriVssTxIsr

 @Description   handle VSS interrupt
 
 @Param[in]     cpri_vss_handle - cpri vss device handle
 
 @Return        none

*//***************************************************************************/
void cpriVssTxIsr(os_hwi_arg cpri_vss_handle);

/**************************************************************************//**
 @Function      cpriVssRxIsr

 @Description   handle VSS interrupt
 
 @Param[in]     cpri_vss_handle -   cpri vss device handle
 
 @Return        none

*//***************************************************************************/
void cpriVssRxIsr(os_hwi_arg cpri_vss_handle);

/**************************************************************************//**
 @Function      cpriEthernetTxIsr

 @Description   handle Ethernet interrupt
 
 @Param[in]     cpri_ethernet_handle -   cpri Ethernet device handle
 
 @Return        none

*//***************************************************************************/
void cpriEthernetTxIsr(os_hwi_arg cpri_ethernet_handle);

/**************************************************************************//**
 @Function      cpriEthernetRxIsr

 @Description   handle Ethernet interrupt
 
 @Param[in]     cpri_ethernet_handle -   cpri Ethernet device handle
 
 @Return        none

*//***************************************************************************/
void cpriEthernetRxIsr(os_hwi_arg cpri_ethernet_handle);

/**************************************************************************//**
 @Function      cpriEthernetChannelTx

 @Description   Transmits a given frame.

 @Param[in]     ch_handle -  Channel handle, returned from cpriEthernetChannelOpen().
 @Param[in]     frame -  The frame to transmit.

 @Return        OS status - OS_SUCCESS for success / OS_FAIL for failure

*//***************************************************************************/
os_status   cpriEthernetChannelTx(void *ch_handle, void *frame);

/**************************************************************************//**
 @Function      cpriHdlcTxIsr

 @Description   handle HDLC interrupt
 
 @Param[in]     cpri_hdlc_handle -   cpri ethernet handle.
 
 @Return        OS status - OS_SUCCESS for success / OS_FAIL for failure

*//***************************************************************************/

void cpriHdlcTxIsr(os_hwi_arg cpri_hdlc_handle);

/**************************************************************************//**
 @Function      cpriHdlcRxIsr

 @Description   handle HDLC interrupt
 
 @Param[in]     cpri_hdlc_handle -   cpri Hdlc handle.
 
 @Return        none

*//***************************************************************************/
void cpriHdlcRxIsr(os_hwi_arg cpri_hdlc_handle);

/**************************************************************************//**
 @Function      cpriHdlcChannelTx

 @Description   Transmits a given frame.

 @Param[in]     ch_handle -  Channel handle, returned from cpriEthernetChannelOpen().
 @Param[in]     frame -  The frame to transmit.

 @Return        OS status - OS_SUCCESS for success / OS_FAIL for failure

*//***************************************************************************/
os_status   cpriHdlcChannelTx(void *ch_handle, void *frame);

/**************************************************************************//**
 @Function      cpriHfnTxIsr

 @Description   handle Hyper frame interrupt
 
 @Param[in]     cpri_handle -   cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriHfnTxIsr(os_hwi_arg cpri_handle);

/**************************************************************************//**
 @Function      cpriHfnRxIsr

 @Description   handle Hyper frame interrupt
 
 @Param[in]     cpri_handle -   cpri device handle.
 
 @Return        none

*//***************************************************************************/
void cpriHfnRxIsr(os_hwi_arg cpri_handle);

/**************************************************************************//**
 @Function      cpriHfnTxIsr

 @Description   handle Hyper frame interrupt
 
 @Param[in]     cpri_handle -   cpri device handle.
 
 @Return        none

*//***************************************************************************/
void cpriBfnTxIsr(os_hwi_arg cpri_handle);

/**************************************************************************//**
 @Function      cpriHfnRxIsr

 @Description   handle Hyper frame interrupt
 
@Param[in]     cpri_handle -   cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriBfnRxIsr(os_hwi_arg cpri_handle);

/**************************************************************************//**
 @Function      cpriIqTxBeginIsr

 @Description   handle IQ TX begin interrupt
 
 @Param[in]     cpri_handle -   cpri device handle
 
 @Return        none

*//***************************************************************************/
void cpriIqTxBeginIsr(os_hwi_arg cpri_handle);

/**************************************************************************//**
 @Function      cpriMulticastIqRxIsr

 @Description   handle multicast IQ rx interrupt
 
@Param[in]     cpri_handle -   os_hwi_arg.
 
 @Return        none

*//***************************************************************************/
void cpriMulticastIqRxIsr(os_hwi_arg cpri_handle);

/**************************************************************************//*
 @Description   CPRI Ethernet Controller channel control commands.

                These commands can be used by cpriEthernetChannelCtrl() function. They are
                dedicated for channel operations.

*//***************************************************************************/
#define CPRI_ETHERNET_CMD_RX_POLL                 (0x00000400 | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Process received frames, when Rx interrupt is disabled (param: NULL). */
#define CPRI_ETHERNET_CMD_TX_CONFIRM              (0x00000800 | BIO_LLD_COMMAND)
/**< Confirm transmitted frames (param: NULL). */

/**************************************************************************//*
 @Description   CPRI HDLC Controller channel control commands.

                These commands can be used by cpriHdlcChannelCtrl() function. They are
                dedicated for channel operations.

*//***************************************************************************/
#define CPRI_HDLC_CMD_RX_POLL                 (0x00000400 | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Process received frames, when Rx interrupt is disabled (param: NULL). */
#define CPRI_HDLC_CMD_TX_CONFIRM              (0x00000800 | BIO_LLD_COMMAND)
/**< Confirm transmitted frames (param: NULL). */

/** @} */ // end of cpri_runtime_id
/** @} */ // end of cpri_id

_OS_END_EXTERN_C

#endif // __CPRI_H


/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/05/15 10:40:54 $
 $Id: aic.h,v 1.35 2014/05/15 10:40:54 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/aic/aic.h,v $
 $Revision: 1.35 $
******************************************************************************/



/**************************************************************************//**

 @File          aic.h

 @Description   AIC external header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __AIC_H
#define __AIC_H

#include "aic_init.h"


#include "os_cpp.h"

_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         aic_id   AIC Module

 @Description   AIC block API.

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         aic_runtime_id AIC runtime
 
 @Description   AIC runtime API.
 
 @{
*//***************************************************************************/

/*************************************************************//**
 @Collection    AIC error interrupt events to be used in error callback.
 @{
*//**************************************************************/
#define                     AIC_RX_DMA_OVERRUN      0X00008000
#define                     AIC_TX_DMA_UNDERRUN     0X80000000
#define                     AIC_JESD_TIMEOUT        0X00000002
/* @} */


/*************************************************************//**
 @Collection    AIC DMA Mode Select definitions
 @{
*//**************************************************************/
#define                     AIC_DMA_MODE_TX_DOWNSIZING_METHOD_TRUNCATION 0x00000000  /**< otherwise rounding mode */
#define                     AIC_DMA_MODE_TX_FLOW_CTRL_DISABLE            0x00100000
#define                     AIC_DMA_MODE_TX_NO_ACK                       0x00080000  /**< effects only DBM */
#define                     AIC_DMA_MODE_TX_NUM_OF_ANTENNAS_TWO          0x00010000  /**< otherwise 1 */
#define                     AIC_DMA_MODE_RX_DOWNSIZING_METHOD_TRUNCATION 0x00000400  /**< otherwise rounding mode */
#define                     AIC_DMA_MODE_DOUBLE_BUFFER_MODE              0x00000200  /**< otherwise Stream Mode (DBM and Stream Mode are described in AIC block RM)*/

#define                     AIC_DMA_MODE_RX_DATA_DESTINATION_MASK        0x00000180  /**< This is a 2 bits mask; 0x0 - to sys mem, 0x1 - to MAPLE and 0x2 - to both. */
#define                     AIC_DMA_MODE_RX_DATA_DESTINATION_SYS         0x00000000
#define                     AIC_DMA_MODE_RX_DATA_DESTINATION_MAPLE       0x00000080
#define                     AIC_DMA_MODE_RX_DATA_DESTINATION_MULTI       0x00000100

#define                     AIC_DMA_MODE_RX_OVERSAMPLING_MODE_2X         0x00000020  /**< otherwise normal (1X sampling) */
#define                     AIC_DMA_MODE_RX_SAMPLE_WIDTH_16_BITS         0x00000010  /**< otherwise 8 bits */
#define                     AIC_DMA_MODE_RX_NO_ACK                       0x00000008  /**< effects only DBM */
#define                     AIC_DMA_MODE_RX_NUM_OF_ANTENNAS_TWO          0x00000001  /**< otherwise 1 */
/* @} */


/*************************************************************//**
 @Collection    Channel open interrupting definitions
 @{
*//**************************************************************/
#define                     AIC_CHANNEL_OPEN_IRQ_CORE      0X0001   /**< AIC to interrupt SC */
#define                     AIC_CHANNEL_OPEN_IRQ_MAPLE     0X0002   /**< AIC to interrupt MAPLE */
/* @} */

/*************************************************************//**
 @Collection    AIC ADI lane control commands definitions
 @{
*//**************************************************************/
#define                     AIC_ADI_LANE_CTRL_RECONFIGURE           0X00000001   /**< reconfigure AIC ADI lane, a pointer to #aic_lane_open_params_t is expected as a command parameter. */
#define                     AIC_ADI_LANE_CTRL_IS_CAPTURE_COMPLETE   0X00000002   /**< returns 0X1 through the command parameter if capture complete event occurred, otherwise returns 0. */
#define                     AIC_UNSAFE_TX_DISABLE_TMR_CTRL          0X00000003   /**< Must be called at least 2 TTI's before AIC_UNSAFE_TX_DISABLE_DMA in order to disable TX path; the safer SIO version command SIO_DEVICE_TX_DISABLE busy-waits 2 TTI's internally; both functions must be called to disable TX path. */
#define                     AIC_UNSAFE_TX_DISABLE_DMA               0X00000004   /**< Must be called at least 2 TTI's after AIC_UNSAFE_TX_DISABLE_TMR_CTRL in order to disable TX path; the SIO safer SIO version command SIO_DEVICE_TX_DISABLE busy-waits 2 TTI's internally; both functions must be called to disable TX path.*/
#define                     AIC_UNSAFE_RX_DISABLE_TMR_CTRL          0X00000005   /**< Must be called at least 2 TTI's before AIC_UNSAFE_RX_DISABLE_DMA in order to disable RX path; the safer SIO version command SIO_DEVICE_RX_DISABLE busy-waits 2 TTI's internally; both functions must be called to disable RX path.*/
#define                     AIC_UNSAFE_RX_DISABLE_DMA               0X00000006   /**< Must be called at least 2 TTI's after AIC_UNSAFE_RX_DISABLE_TMR_CTRL in order to disable RX path; the SIO safer SIO version command SIO_DEVICE_RX_DISABLE busy-waits 2 TTI's internally; both functions must be called to disable RX path.*/
/* @} */


/**************************************************************************//**
 @Description   Interrupt types for AIC lanes interrupts table.
*//***************************************************************************/
typedef enum
{
    AIC_LANE_INT_RX_TX,
    AIC_LANE_INT_ERROR,
    AIC_LANE_TTI_UL,
    AIC_LANE_TTI_DL,
    AIC_LANE_INT_UL_OFF,
    AIC_LANE_INT_DL_OFF,
    AIC_LANE_INT_LAST                                    /**< Signifies last entry in the table*/
} aic_lane_interrupts_types_t;

/***************************************************************************************//**
 @Description   Interrupt structure to be used as an entry in AIC lanes interrupts table.
*//****************************************************************************************/
typedef struct
{
    aic_lane_interrupts_types_t interrupt_line_num; /**< interrupt EPIC index - device specific; last entry must be #AIC_LANE_INT_LAST!!! */
    os_hwi_priority             interrupt_priority; /**< the priority of the interrupt*/
} aic_lane_interrupt_t;


/**************************************************************************//**
 @Description   Interrupt types for AIC sniff interrupts table.
*//***************************************************************************/
typedef enum
{
    AIC_INT_SNIFF_CAPTURE_COMPLETE,
    AIC_INT_SNIFF_FRAME,
    AIC_SNIFF_INT_LAST                                    /**< Signifies last entry in the table*/
} aic_sniff_interrupts_types_t;

/***************************************************************************************//**
 @Description   Interrupt structure to be used as an entry in AIC Sniff interrupts table.
*//****************************************************************************************/
typedef struct
{
    aic_sniff_interrupts_types_t    interrupt_line_num; /**< interrupt EPIC index - device specific; last entry must be #AIC_SNIFF_INT_LAST!!! */
    os_hwi_priority                 interrupt_priority; /**< the priority of the interrupt*/
} aic_sniff_interrupt_t;

/**************************************************************************//**
 @Description   AIC SNIFF interrupt callback parameter structure.

                This structure is received by the user-defined error callback.
*//***************************************************************************/
typedef struct
{
    uint32_t                buffer_address;                             /**< Buffer address to store Sniff data */
    uint32_t                rollover;                                   /**< Rollover value */
    uint32_t                sniff_frame_int_interval;                   /**< Time interval for a periodic interrupt going out of the AIC SNIFF Block */
    uint32_t                capt_off;                                   /**< Capture offset value, counted on reference clock counter */
    uint32_t                capt_dur;                                   /**< Capture duration value, counted  on chip counter */
    aic_sniff_id_t          aic_sniff_id;                               /**< Sniff block identifier */
    aic_sniff_interrupt_t   *aic_sniff_interrupt_table;                 /**< Array of enabled interrupts; note last entrys' interrupt_line_num must be #AIC_SNIFF_INT_LAST */
    void                    (*sniff_capture_complete)(void *);          /**< User supplied callback for sniff capture complete event. */
    void                    (*sniff_frame_int)(void *);                 /**< User supplied callback for sniff frame interrupt. */
} aic_sniff_open_params_t;


/**************************************************************************//**
 @Description   AIC error interrupt callback parameter structure.

                This structure is received by the user-defined error callback.
*//***************************************************************************/
typedef struct
{
    union
    {
        aic_adi_lane_num_t      aic_adi_lane_num;                   /**< number of AIC ADI lane for which the CB is called */
#ifdef PSC9131
        aic_maxim_lane_num_t    aic_maxim_lane_num;                 /**< number of AIC MAXIM lane for which the CB is called */
#endif //PSC9131
    };
    uint32_t                    events;                             /**< Holds error events*/
} aic_error_cb_params_t;




/**************************************************************************//**
 @Description   AIC ADI lane open parameters.

                This structure should be used as a LLD parameter
                when calling osSioDeviceOpen() for opening AIC device lane.
*//***************************************************************************/
typedef struct
{
    iq_mbus_transaction_size_t  rx_mbus_transaction_size;   /**< size of iq RX mbus transaction */
    iq_mbus_transaction_size_t  tx_mbus_transaction_size;   /**< size of iq TX mbus transaction */
    uint16_t                    rx_mbus_priority_level[3];  /**< RX IQ MBus priority levels 1-3; if number of valid bytes per antenna in DMA RX buffer is greater than priority level k value, the MBus priority is updated to k.*/
    uint16_t                    tx_mbus_priority_level[3];  /**< TX IQ MBus priority levels 1-3; if number of valid bytes per antenna in DMA TX buffer is less than priority level k value, the MBus priority is updated to k. */
    uint16_t                    rx_maple_buffer_size;       /**< RX MAPLE buffer size */
 
    uint32_t                    maximum_symbol_size;        /**< Maximum Symbol Size; only in DBM. */
    uint32_t                    rx_maple_base_address;      /**< RX MAPLE base address */
    uint32_t                    rx_buffer_size;             /**< RX buffer size */
 

 
    uint32_t                    tx_buffer_size;             /**< TX buffer size */
    uint32_t                    mode_select_flag;           /**< Contains bits to represent the following:
                                                                   - TX Downsizing Method
                                                                   - TX Flow Control Disable (Stream Mode only)
                                                                   - TX No Acknowledge
                                                                   - TX Nummber of Antennas
                                                                   - RX Framer Enable Select (RF or Sniffer)
                                                                   - RX Downsizing Method
                                                                   - Double Buffer Mode
                                                                   - RX Data Destination (System only, MAPLE only or both)
                                                                   - RX Oversampling Mode
                                                                   - RX Sample Width
                                                                   - RX No Acknowledge
                                                                   - RX Number of Antennas */
    uint32_t                    rx_threshold;               /**< Only in Stream Mode - RX number of bytes to trigger an interrupt */
    uint32_t                    rx_first_threshold;         /**< The location of the first threshold in the RX data buffers*/
    uint32_t                    rx_second_threshold;        /**< The location of the second threshold in the RX data buffers*/
    uint32_t                    tx_threshold;               /**< Only in Stream Mode - TX number of bytes to trigger an interrupt */
    uint32_t                    tx_first_threshold;         /**< The location of the first threshold in the TX data buffers*/
    uint32_t                    tx_second_threshold;        /**< The location of the second threshold in the TX data buffers*/
 
    aic_lane_interrupt_t        *interrupts_table;          /**< Array of enabled interrupts; note last entrys' interrupt_line_num must be #AIC_LANE_INT_LAST */
 
    void                        (*error_callback)(aic_error_cb_params_t *); /**< User supplied error callback. */
    void                        (*tti_ul)(aic_adi_lane_num_t);  /**< User supplied TTI UL callback. */
    void                        (*tti_dl)(aic_adi_lane_num_t);  /**< User supplied TTI DL callback. */
    sio_dev_handle              aic_adi_handle;              /**< Returned from sioDeviceOpen(...) call for the ADI/MAXIM SIO device */
    aic_sniff_open_params_t     *aic_sniff_open_params;      /**< If this lane is to be harnessed for Sniffing, Sniffing parameters structure must be provided, otherwise NULL. */
 
    /* These  are 9132 rev1.1 additions:     */
    void                        (*tdd_ul_off)(aic_adi_lane_num_t); /**< User supplied UL_OFF event callback. */
    void                        (*tdd_dl_off)(aic_adi_lane_num_t); /**< User supplied DL_OFF event callback. */
} aic_lane_open_params_t;

typedef aic_lane_open_params_t aic_adi_lane_open_params_t;   /**< See #aic_lane_open_params_t */
typedef aic_lane_open_params_t aic_maxim_lane_open_params_t; /**< See #aic_lane_open_params_t */


/**************************************************************************//**
 @Description   AIC ADI open parameters.

                This structure should be used as a LLD parameter
                when calling osSioDeviceOpen() for opening AIC ADI device.
*//***************************************************************************/
typedef struct
{
    aic_adi_lane_num_t      *lane_nums;     /**< Array of size #num_of_lanes to represent number of used lanes */
    sio_dev_handle          aic_handle;     /**< Returned from sioDeviceOpen(...) call for the AIC SIO device */
    uint8_t                 num_of_lanes;   /**< Number of elements in #lane_nums array*/
} aic_adi_open_params_t;

/**************************************************************************//**
 @Description   AIC ADI opening parameters.

                This structure should be used as a LLD parameter
                when calling osSioDeviceOpen() for opening AIC device.
*//***************************************************************************/
typedef struct
{
    uint8_t         dummy;  /**< Prevents from struct being empty which is syntax error. Struct is reserved for future-compatibity*/
} aic_open_params_t;


/**************************************************************************//**
 @Description   AIC channel opening parameters.

                This structure should be used when opening AIC channel.
*//***************************************************************************/
typedef struct
{
    uint8_t interrupt_mode;              /**< defines whether AIC interrupts SC and/or MAPLE. Use #AIC_CHANNEL_OPEN_IRQ_CORE and #AIC_CHANNEL_OPEN_IRQ_MAPLE. */
} aic_channel_open_params_t;



/**************************************************************************//**
 @Description   AIC interrupt callback parameter structure.

                This structure is received by the user-defined callback.
*//***************************************************************************/
typedef struct
{
    union
    {
        aic_adi_lane_num_t      aic_adi_lane_num;                   /**< number of AIC ADI lane for which the CB is called */
#ifdef PSC9131
        aic_maxim_lane_num_t    aic_maxim_lane_num;                 /**< number of AIC MAXIM lane for which the CB is called */
#endif //PSC9131
    };
 
    union
    {
        aic_adi_channel_num_t   aic_adi_channel_num;                /**< number of AIC ADI lane for which the CB is called */
#if defined(PSC9131)
        aic_maxim_channel_num_t aic_maxim_channel_num;              /**< number of AIC MAXIM lane for which the CB is called */
#endif //PSC9131
    };

} aic_int_cb_params_t;


/**************************************************************************//**
 @Description   AIC SNIFF interrupt callback parameter structure.

                This structure is received by the user-defined callback.
*//***************************************************************************/
typedef struct
{
    union
    {
        aic_adi_lane_num_t      aic_adi_lane_num;                   /**< number of AIC ADI lane for which the CB is called */
#ifdef PSC9131
        aic_maxim_lane_num_t    aic_maxim_lane_num;                 /**< number of AIC MAXIM lane for which the CB is called */
#endif //PSC9131
    };
} aic_sniff_int_cb_params_t;



/** @} */ // end of aic_runtime_id
/** @} */ // end of aic_id

_OS_END_EXTERN_C

#endif // __aic_H

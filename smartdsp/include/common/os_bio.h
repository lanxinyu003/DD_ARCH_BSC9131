/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_bio.h,v 1.29 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_bio.h,v $
 $Revision: 1.29 $
******************************************************************************/

/**************************************************************************//**
 @File          os_bio.h

 @Description   OS BIO layer header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_BIO_H
#define __OS_BIO_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         bio_id   BIO – Buffered I/O Module API

 @Description   Buffer I/O routines, defines, enumerations and API
 
 @{
*//***************************************************************************/

/************************************************************//**
 @Collection    BIO Channel Modes
 
                When opening a channel, the application has to
                specify whether it is #BIO_READ or #BIO_WRITE.
                In addition, a channel may be opened as #BIO_ACTIVE
                or else be activated later.

 @{
*//*************************************************************/
#define BIO_READ                0x1 /**< Channel is for input */
#define BIO_WRITE               0x2 /**< Channel is for output */
#define BIO_ACTIVE              0x4 /**< Channel is active */

/* @} */ /* end of bio_ch_modes */


/************************************************************//**
 @Anchor        bio_ctrl_cmd
 
 @Collection    BIO Control Commands
 
                Used in osBioDeviceCtrl() and osBioChannelCtrl()
 
 @{
*//*************************************************************/
#define BIO_LLD_COMMAND         0x01000000
/**< Defines a command that will be handled by the Low Level Driver */
#define BIO_COMMAND_WRITE       0x00000000
/**< Defines a command for an output channel */
#define BIO_COMMAND_READ        0x10000000
/**< Defines a command for an input channel */


#define BIO_CHANNEL_TX_ENABLE   (0x00000001 | BIO_LLD_COMMAND | BIO_COMMAND_WRITE)
/**< Enable an output channel */
#define BIO_CHANNEL_RX_ENABLE   (0x00000002 | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Enable an input channel */
#define BIO_CHANNEL_TX_DISABLE  (0x00000003 | BIO_LLD_COMMAND | BIO_COMMAND_WRITE)
/**< Disable an output channel */
#define BIO_CHANNEL_RX_DISABLE  (0x00000004 | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Disable an input channel */
#define BIO_CHANNEL_STAT_GET    (0x0000000A | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Return the statistics on a channel */
#define BIO_CHANNEL_RX_ADD_ID   (0x0000000B | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Add a LLD ID to the channel */
#define BIO_CHANNEL_RX_REMOVE_ID (0x0000000C | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Remove a LLD ID to the channel */
#define BIO_CHANNEL_RX_PHYS_LLD_ADDR_ADD (0x0000000D | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Add a LLD physical address to the channel */
#define BIO_CHANNEL_RX_PHYS_LLD_ADDR_REMOVE (0x0000000E | BIO_LLD_COMMAND | BIO_COMMAND_READ)
/**< Remove a LLD physical address to the channel */

#define BIO_DEVICE_TX_ENABLE    (0x00000001 | BIO_LLD_COMMAND )
/**< Enable an output device */
#define BIO_DEVICE_RX_ENABLE    (0x00000002 | BIO_LLD_COMMAND )
/**< Enable an input device */
#define BIO_DEVICE_TX_DISABLE   (0x00000003 | BIO_LLD_COMMAND )
/**< Disable an output device */
#define BIO_DEVICE_RX_DISABLE   (0x00000004 | BIO_LLD_COMMAND )
/**< Disable an input device */
#define BIO_DEVICE_RESET        (0x00000008 | BIO_LLD_COMMAND )
/**< Reset the device */
#define BIO_DEVICE_SUPPORT_MULTIBUFFER        (0x00000009 | BIO_LLD_COMMAND )
/**< Check if device supports multi-buffered-frames */
#define BIO_DEVICE_STAT_GET     (0x00000009)
/**< Return the statistics on a device */

#define BIO_REMOVE_BUFFER  0xFFFF

/* @} */ /* end of bio_ctrl_cmd */


/**************************************************************************//**
 @Description   BIO device handle - passed as parameter to all device functions.
*//***************************************************************************/
typedef void* bio_dev_handle;


/**************************************************************************//**
 @Description   BIO device configuration parameters.
 
                This structure is passed to osBioDeviceOpen() and defines
                some of the device parameters that the BIO requires.
 
                The structure also contains a pointer to LLD-specific
                parameters. This pointer is passed to the LLD as it is.
*//***************************************************************************/
typedef struct
{
    os_mem_part_t   *common_pool;   /**< Pointer to initialized common buffers pool. */
    void            *lld_params;    /**< LLD-specific parameters. */
 
} bio_dev_open_params_t;


/**************************************************************************//**
 @Description   BIO channel configuration parameters.
 
                This structure is passed to osBioChannelOpen() and defines
                some of the channel parameters that the BIO requires.
 
                The structure also contains a pointer to LLD-specific
                parameters. This pointer is passed to the LLD as it is.
*//***************************************************************************/
typedef struct
{
    uint16_t            channel_num;    /**< Channel number. */
    void                (*callback)(void *param, uint32_t data, uint32_t error_status);
    /**< Application's callback for this channel;
         for Tx, the data is the confirmed frame; for Rx, data is always 0. */
    void                *cb_parameter;  /**< Application's parameter for the callback. */
    os_frames_pool_t    *frames_pool;   /**< Frames pool for the channel. */
    os_mem_part_t       *buffers_pool;  /**< Rx buffers pool. */
    void                *lld_params;    /**< LLD-specific channel parameters. */
 
} bio_ch_open_params_t;


/**************************************************************************//**
 @Description   BIO channel internal structure.
 
                This structure is exposed to the application layer to let the
                application allocate memory for the channels in a convenient
                way. The application should not change any field in this
                structure.

 @Cautions      This structure is declared publicly to enable allocating enough
                memory for it by the application. Users are prohibited from
                directly accessing the structure.
*//***************************************************************************/
typedef struct
{
    uint32_t            status;             /**< The channel's status. */
    void                (*callback)(void *param, uint32_t data, uint32_t error_status);
    /**< Application's callback for this channel;
         for Tx, the data is the confirmed frame; for Rx, data is always 0. */
    void                *cb_parameter;      /**< Application's parameter for the callback. */
    void                *lld_ch;            /**< The matching LLD channel handle. */
    void                *bio_interf;        /**< The channel's BIO device interface. */
    uint8_t             channel_type;       /**< The channel's type (read or write). */
    os_queue_handle     frames_queue;       /**< The channel's frames queue handle. */
    os_frames_pool_t    *frames_pool;       /**< The channel's frames pool. */
    os_mem_part_t       *buffers_pool;      /**< The channel's buffers pool (Rx channel only). */
    /* buffers_queue MUST be right after the buffers_pool */
    os_queue_handle     buffers_queue;      /**< The channel's buffers queue handle (Rx channel only). */
    void                (*f_RxFrameBuild)(void          *bio_ch_handle,
                                          os_frame_t    *frame,
                                          uint32_t      frame_length);
    /**< Frame build function (Rx channels only, for single-buffer or multi-buffer frames). */

} bio_channel_t;

/**************************************************************************//**
 @Description   BIO interface statistics
*//***************************************************************************/
typedef struct
{
    uint32_t        rx_frames;              /**< Num of RX frames.  */
    uint32_t        rx_bytes;               /**< Num of RX bytes.   */
    uint32_t        tx_frames;              /**< Num of TX frames.  */
    uint32_t        tx_bytes;               /**< Num of TX bytes.   */
    uint32_t        tx_errors;              /**< Num of TX errors.  */
    uint32_t        rx_errors;              /**< Num of RX errors.  */
    uint32_t        interface_speed;        /**< Speed.             */
    bool            is_promisc;             /**< IF is promiscuous. */
 
} bio_interface_stat_t;


/**************************************************************************//**
 @Group         bio_init_id     BIO Initialization API

 @Description   BIO upper layer (serializer) API for the
                application initialization stage
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osBioDeviceOpen

 @Description   Opens a BIO device for operation using the given parameters.
 
 @Param[in]     device_name     -  The name of the device.
 @Param[in]     dev_open_params -  Device parameters for use by both BIO
                                   and LLD, or NULL to use the default device
                                   parameters.

 @Return        A BIO device handle if the device was opened successfully
 @Return        NULL if the device failed to open
 
*//***************************************************************************/
bio_dev_handle  osBioDeviceOpen(char                    *device_name,
                                bio_dev_open_params_t   *dev_open_params);


/**************************************************************************//**
 @Function      osBioChannelOpen

 @Description   Opens a channel of a BIO device using the given parameters.
 
 @Param[in]     bio_handle     -  BIO device handle returned by osBioDeviceOpen()
 @Param[out]    bio_ch         -  Pointer to a BIO channel structure.
 @Param[in]     mode           -  Channel mode (#BIO_READ, #BIO_WRITE, #BIO_ACTIVE).
 @Param[in]     ch_open_params -  Channel parameters.

 @Retval        OS_SUCCESS if channel was opened successfully
 @Retval        OS_ERR_PARAM_INVALID if invalid parameters were provided to
                the function
 @Retval        OS_FAIL if the underlying LLD channel failed to open properly
 @Return        Error status, encoded in os_error.h, for other errors
 
*//***************************************************************************/
os_status   osBioChannelOpen(bio_dev_handle         bio_handle,
                             bio_channel_t          *bio_ch,
                             int                    mode,
                             bio_ch_open_params_t   *ch_open_params);


/**************************************************************************//**
 @Function      osBioChannelClose

 @Description   Closes an open channel of a BIO device.
 
 @Param[in]     bio_ch -  Pointer to a valid BIO channel returned by
                          osBioChannelOpen()

 @Retval        OS_SUCCESS if channel closed properly
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                channel failed to close properly
 
*//***************************************************************************/
os_status   osBioChannelClose(bio_channel_t *bio_ch);


/** @} */ // end of bio_init_id


/**************************************************************************//**
 @Group         bio_runtime_id     BIO Runtime API

 @Description   BIO upper layer (serializer) API for the
                application runtime stage
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osBioChannelTx

 @Description   Call this function to transmit a frame through a BIO channel
                that was previously opened for transmission.
 
 @Param[in]     bio_ch -  Pointer to a valid BIO channel returned by
                          osBioChannelOpen()
 @Param[in]     frame -  The frame to transmit (actually a pointer to an
                          os_frame_t structure)

 @Retval        OS_SUCCESS if frame was transmitted properly
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                channel failed to transmit the frame
 
*//***************************************************************************/
os_status   osBioChannelTx(bio_channel_t *bio_ch, void *frame);


/**************************************************************************//**
 @Function      osBioChannelRx

 @Description   This function returns a pointer to a received frame.
 
 @Param[in]     bio_ch -  Pointer to a valid BIO channel returned by
                          osBioChannelOpen()

 @Retval        Pointer to a received frame (actually a pointer to an
                os_frame_t structure).
 @Retval        NULL if no frames are available on the channel
 
*//***************************************************************************/
void *      osBioChannelRx(bio_channel_t *bio_ch);


/**************************************************************************//**
 @Function      osBioDeviceCtrl

 @Description   Performs control commands on a device.
 
 @Param[in]     bio_handle  -  BIO device handle returned by osBioDeviceOpen()
 @Param[in]     command     -  Command code - see @ref bio_ctrl_cmd "BIO Control Commands"
 @Param[in]     param       -  Command parameter, command specific

 @Retval        OS_SUCCESS if command was executed properly
 @Retval        OS_ERR_COMMAND_UNSUPPORTED if command is unsupported
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                device failed to execute the command
 
*//***************************************************************************/
os_status   osBioDeviceCtrl(bio_dev_handle bio_handle, uint32_t command, void *param);


/**************************************************************************//**
 @Function      osBioChannelCtrl

 @Description   Performs control commands on a channel.
 
 @Param[in]     bio_ch      -  Pointer to a valid BIO channel returned by
                               osBioChannelOpen()
 @Param[in]     command     -  Command code - see @ref bio_ctrl_cmd "BIO Control Commands"
 @Param[in]     param       -  Command parameter, command specific

 @Retval        OS_SUCCESS if command was executed properly
 @Retval        OS_ERR_COMMAND_INVALID if #BIO_READ command was called on a
                #BIO_WRITE channel, or vice versa
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                channel failed to execute the command
 
*//***************************************************************************/
os_status   osBioChannelCtrl(bio_channel_t *bio_ch, uint32_t command, void *param);


/** @} */ // end of bio_runtime_id

/** @} */ // end of bio_id

_OS_END_EXTERN_C

#endif // __OS_BIO_H

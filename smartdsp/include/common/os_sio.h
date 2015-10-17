/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/08/12 17:06:15 $
 $Id: os_sio.h,v 1.26 2014/08/12 17:06:15 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_sio.h,v $
 $Revision: 1.26 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sio.h

 @Description   OS SIO layer header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_SIO_H
#define __OS_SIO_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         sio_id   SIO – Synchronized I/O Module API

 @Description   Synchronized I/O routines, defines, enumerations and API
 
 @{
*//***************************************************************************/


/************************************************************//**
 @Collection    SIO Channel Modes
 
                When opening a channel, the application has to
                specify whether it is #SIO_READ or #SIO_WRITE.
                In addition, a channel may be opened as #SIO_ACTIVE
                or else be activated later.

 @{
*//*************************************************************/
#define SIO_READ                0x1 /**< Channel is for input */
#define SIO_WRITE               0x2 /**< Channel is for output */
#define SIO_ACTIVE              0x4 /**< Channel is active */

/* @} */ /* end of sio_ch_modes */

/************************************************************//**
 @Anchor        sio_ctrl_cmd
 
 @Collection    SIO Control Commands
 
                Used in osSioDeviceCtrl() and osSioChannelCtrl()
 
 @{
*//*************************************************************/

#define SIO_LLD_COMMAND         0
/**< Defines a command that will be handled by the Low Level Driver */
#define SIO_COMMAND_WRITE       0x10000000
/**< Defines a command for an output channel */
#define SIO_COMMAND_READ        0x20000000
/**< Defines a command for an input channel */

#define SIO_CHANNEL_TX_ENABLE   (0x00000001 | SIO_LLD_COMMAND | SIO_COMMAND_WRITE)
/**< Enable an output channel */
#define SIO_CHANNEL_RX_ENABLE   (0x00000002 | SIO_LLD_COMMAND | SIO_COMMAND_READ)
/**< Enable an input channel */
#define SIO_CHANNEL_TX_DISABLE  (0x00000003 | SIO_LLD_COMMAND | SIO_COMMAND_WRITE)
/**< Disable an output channel */
#define SIO_CHANNEL_RX_DISABLE  (0x00000004 | SIO_LLD_COMMAND | SIO_COMMAND_READ)
/**< Disable an input channel */

#define SIO_DEVICE_TX_ENABLE    (0x00000001 | SIO_LLD_COMMAND | SIO_COMMAND_WRITE)
/**< Enable an output device */
#define SIO_DEVICE_RX_ENABLE    (0x00000002 | SIO_LLD_COMMAND | SIO_COMMAND_READ)
/**< Enable an input device */
#define SIO_DEVICE_RX_TX_ENABLE (SIO_DEVICE_TX_ENABLE | SIO_COMMAND_READ)
/**< Enable an input/output device */
#define SIO_DEVICE_TX_DISABLE   (0x00000003 | SIO_LLD_COMMAND | SIO_COMMAND_WRITE)
/**< Disable an output device */
#define SIO_DEVICE_RX_DISABLE   (0x00000004 | SIO_LLD_COMMAND | SIO_COMMAND_READ)
/**< Disable an input device */
#define SIO_DEVICE_RX_INTERRUPT_STATUS   (0x00000005 | SIO_LLD_COMMAND | SIO_COMMAND_READ)

/* @} */ /* end of sio_ctrl_cmd */


/**************************************************************************//**
 @Description   SIO device handle - passed as parameter to all device functions.
 
*//***************************************************************************/
typedef void* sio_dev_handle;


/**************************************************************************//**
 @Description   SIO device configuration parameters.
 
                This structure is passed to osSioDeviceOpen() and defines
                some of the device parameters that the SIO requires.
 
                The structure also contains a pointer to LLD-specific
                parameters. This pointer is passed to the LLD as it is.
 
*//***************************************************************************/
typedef struct
{
    void    (*rx_callback)(void *param);    /**< Application's RX callback. */
    void    *rx_callback_parameter;         /**< Application's RX callback parameter. */
    void    (*tx_callback)(void *param);    /**< Application's TX callback. */
    void    *tx_callback_parameter;         /**< Application's TX callback parameter. */
    void    (*error_callback)(void *param); /**< Application's callback for underrun error. */
    void    *lld_params;                    /**< LLD parameters for device open. */
} sio_dev_open_params_t;


/**************************************************************************//**
 @Description   SIO channel configuration parameters.
 
                This structure is passed to osSioChannelOpen() and defines
                some of the channel parameters that the SIO requires.
 
                The structure also contains a pointer to LLD-specific
                parameters. This pointer is passed to the LLD as it is.
 
*//***************************************************************************/
typedef struct
{
    uint16_t    channel_num;             /**< Channel number. */
    void        *callback_parameter;     /**< Application's parameter for the data and error callbacks; If this field is NULL, the callback shall not be called for this channel. */
    uint16_t    num_of_buffers;          /**< How many buffers are going to be used by the channel. */
    uint32_t    buffer_size;             /**< The size of the complete channel buffer. */
    uint32_t    buffer_data_size;        /**< The size of the filled channel buffer. */
    uint16_t    num_of_channel_buffers;  /**< Number of buffers for the channel use at every given point in time. */
    uint8_t     *channel_buffers_base;   /**< Where to place the channel data; should be of size (buffer_size * num_of_buffers). */
    void        *lld_params;             /**< LLD-specific channel parameters. */
} sio_ch_open_params_t;


/**************************************************************************//**
 @Description   SIO channel internal structure.
 
                This structure is exposed to the application layer to let the
                application allocate memory for the channels in a convenient
                way. The application should not change any field in this
                structure.

 @Cautions      This structure is declared publicly to enable allocating enough
                memory for it by the application. Users are prohibited from
                directly accessing the structure.
 
*//***************************************************************************/
typedef struct  sio_channel_t
{
    void                    *sio_interf;           /**< Application's callback for data. */
    void                    *callback_parameter;   /**< Application's parameter for the data and error callbacks; If this field is NULL, the callback shall not be called for this channel. */
    uint8_t                 status;                /**< SIO channel status. */
    uint8_t                 mode;                  /**< Is this a TX or RX channel. */
    uint16_t                num_of_buffers;        /**< How many buffers are going to be used by the chanel. */
    uint32_t                buffer_size;           /**< The size of the complete channel buffer; This field is used only when common_db is NULL. */
    uint32_t                buffer_data_size;      /**< The size of the filled channel buffer; This field is used only when common_db is NULL. */
    uint8_t                 *channel_buffers_base; /**< Where to place the channel data; Should be of size (buffer_size * num_of_buffers). */
    volatile uint16_t       *first_driver_buffer;  /**< First buffer held by LLD; if common_driver_buffer_index != NULL it is used, and this field is redundant. */
    volatile uint16_t       *last_driver_buffer;   /**< Num of LLD buffers, used to calculate under run. */
    void *                  lld_channel;           /**< Used in close channel lld function. */
    struct sio_channel_t    *next;                 /**< The next channel in the list. */
    uint16_t                application_buffer;    /**< The next buffer for the user. */
} sio_channel_t;


/**************************************************************************//**
 @Group         sio_init_id     SIO Initialization API

 @Description   SIO upper layer API for the application initialization stage
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osSioDeviceOpen

 @Description   Opens a SIO device for operation using the given parameters.
 
 @Param[in]     device_name     -  The name of the device.
 @Param[in]     dev_open_params -  Device parameters for use by both SIO
                                   and LLD, or NULL to use the default device
                                   parameters.

 @Return        A SIO device handle if the device was opened successfully
 @Return        NULL if the device failed to open

*//***************************************************************************/
sio_dev_handle  osSioDeviceOpen(char                    *device_name,
                                sio_dev_open_params_t   *dev_open_params);


/**************************************************************************//**
 @Function      osSioChannelOpen

 @Description   Opens a channel of a SIO device using the given parameters.
 
 @Param[in]     sio_handle     -  SIO device handle returned by osSioDeviceOpen()
 @Param[out]    sio_ch         -  Pointer to a SIO channel structure.
 @Param[in]     mode           -  Channel mode (#SIO_READ, #SIO_WRITE, #SIO_ACTIVE).
 @Param[in]     ch_open_params -  Channel parameters.

 @Retval        OS_SUCCESS if channel was opened successfully
 @Retval        OS_FAIL if the underlying LLD channel failed to open properly

*//***************************************************************************/
os_status   osSioChannelOpen(sio_dev_handle         sio_handle,
                             sio_channel_t          *sio_ch,
                             int                    mode,
                             sio_ch_open_params_t   *ch_open_params);


/**************************************************************************//**
 @Function      osSioChannelClose

 @Description   Closes an open channel of a SIO device.
 
 @Param[in]     sio_ch -  Pointer to a valid SIO channel returned by
                          osSioChannelOpen()

 @Retval        OS_SUCCESS if channel closed properly
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                channel failed to close properly

*//***************************************************************************/
os_status   osSioChannelClose(sio_channel_t *sio_ch);

/** @} */ // end of sio_init_id


/**************************************************************************//**
 @Group         sio_runtime_id     SIO Runtime API

 @Description   SIO upper layer API for the application runtime stage
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osSioDeviceCtrl

 @Description   Performs control commands on a device.
 
 @Param[in]     sio_handle  -  SIO device handle returned by osSioDeviceOpen()
 @Param[in]     command     -  Command code - see @ref sio_ctrl_cmd "SIO Control Commands"
 @Param[in]     param       -  Command parameter, command specific

 @Retval        OS_SUCCESS if command was executed properly
 @Retval        OS_ERR_COMMAND_UNSUPPORTED if command is unsupported
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                device failed to execute the command
 
*//***************************************************************************/
os_status   osSioDeviceCtrl(sio_dev_handle sio_handle, uint32_t command, void *param);


/**************************************************************************//**
 @Function      osSioChannelCtrl

 @Description   Performs control commands on a channel.
 
 @Param[in]     sio_ch      -  Pointer to a valid SIO channel returned by
                               osSioChannelOpen()
 @Param[in]     command     -  Command code - see @ref sio_ctrl_cmd "SIO Control Commands"
 @Param[in]     param       -  Command parameter, command specific

 @Retval        OS_SUCCESS if command was executed properly
 @Retval        OS_ERR_COMMAND_INVALID if #SIO_READ command was called on a
                #SIO_WRITE channel, or vice versa
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                channel failed to execute the command
 
*//***************************************************************************/
os_status   osSioChannelCtrl(sio_channel_t *sio_ch, uint32_t command, void *param);

/**************************************************************************//**
 @Function      osSioBufferGet

 @Description   Gets a valid buffer from SIO if exists, or NULL
 
 @Param[in]     sio_ch -  Pointer to a valid SIO channel, osSioChannelOpen()
                          returns this pointer.
 @Param[out]    length -  buffer_data_size.

 @Retval        pointer to a valid buffer
 @Retval        NULL if no valid buffer exists for the channel
 
*//***************************************************************************/
uint8_t *   osSioBufferGet(sio_channel_t *sio_ch, uint16_t *length);

/**************************************************************************//**
 @Function      osSioBufferPut

 @Description   Puts a buffer previously to the SIO channel
 
                This function puts a buffer, obtained by calling osSioBufferGet(),
                to the SIO channel. The SIO module will put the buffers in the
                same order as they were given to the application
 
 @Param[in]     sio_ch -  Pointer to a valid SIO channel, osSioChannelOpen()
                          returns this pointer.

 @Retval        OS_SUCCESS if the application holds a valid buffer.
 @Retval        OS_FAIL if the application does not hold a valid buffer.
 
*//***************************************************************************/
os_status   osSioBufferPut(sio_channel_t *sio_ch);


/** @} */ // end of sio_runtime_id

/** @} */ // end of sio_id

_OS_END_EXTERN_C

#endif // __OS_SIO_H

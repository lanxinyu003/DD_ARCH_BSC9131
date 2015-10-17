/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_cio.h,v 1.21 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_cio.h,v $
 $Revision: 1.21 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_cio.h

 @Description   OS CIO layer header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_CIO_H
#define __OS_CIO_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         cio_id   CIO – Character I/O Module API

 @Description   Character I/O routines, defines, enumerations and API
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Collection    CIO Channel Modes
 
                When opening a channel, the application has to
                specify whether it is #CIO_READ or #CIO_WRITE.
                In addition, a channel may be opened as #CIO_ACTIVE
                or else be activated later.

 @{
*//***************************************************************************/
#define CIO_READ                0x1 /**< Channel is for input */
#define CIO_WRITE               0x2 /**< Channel is for output */
#define CIO_ACTIVE              0x4 /**< Channel is active */

/* @} */ /* end of cio_ch_modes */


/**************************************************************************//**
 @Anchor        cio_ctrl_cmd
 
 @Collection    CIO Control Commands
 
                Used in osCioDeviceCtrl() and osCioChannelCtrl()
 
 @{
*//***************************************************************************/
#define CIO_LLD_COMMAND         0x40000000
/**< Defines a command that will be handled by the Low Level Driver */
#define CIO_COMMAND_WRITE       0x20000000
/**< Defines a command for an output channel */
#define CIO_COMMAND_READ        0x10000000
/**< Defines a command for an input channel */


#define CIO_CHANNEL_TX_ENABLE   (0x00000001 | CIO_LLD_COMMAND | CIO_COMMAND_WRITE)
/**< Enable an output channel */
#define CIO_CHANNEL_RX_ENABLE   (0x00000002 | CIO_LLD_COMMAND | CIO_COMMAND_READ)
/**< Enable an input channel */
#define CIO_CHANNEL_TX_DISABLE  (0x00000003 | CIO_LLD_COMMAND | CIO_COMMAND_WRITE)
/**< Disable an output channel */
#define CIO_CHANNEL_RX_DISABLE  (0x00000004 | CIO_LLD_COMMAND | CIO_COMMAND_READ)
/**< Disable an input channel */

#define CIO_DEVICE_TX_ENABLE    (0x00000001 | CIO_LLD_COMMAND | CIO_COMMAND_WRITE)
/**< Enable an output device */
#define CIO_DEVICE_RX_ENABLE    (0x00000002 | CIO_LLD_COMMAND | CIO_COMMAND_READ)
/**< Enable an input device */
#define CIO_DEVICE_TX_DISABLE   (0x00000003 | CIO_LLD_COMMAND | CIO_COMMAND_WRITE)
/**< Disable an output device */
#define CIO_DEVICE_RX_DISABLE   (0x00000004 | CIO_LLD_COMMAND | CIO_COMMAND_READ)
/**< Disable an input device */

/* @} */ /* end of cio_ctrl_cmd */



/**************************************************************************//**
 @Description   CIO device handle - passed as parameter to all device functions
*//***************************************************************************/
typedef void* cio_dev_handle;


/**************************************************************************//**
 @Description   CIO channel configuration parameters.
 
                This structure is passed to osCioChannelOpen() and defines
                some of the channel parameters that the CIO requires.
 
                The structure also contains a pointer to LLD-specific
                parameters. This pointer is passed to the LLD as it is.
 
*//***************************************************************************/
typedef struct
{
    uint16_t    channel_num;    /**< Channel number. */
    uint8_t     *data_base;     /**< Character serializing buffer base address. */
    uint16_t    data_size;      /**< Character serializing buffer size (in bytes). */
    void        *lld_params;    /**< LLD-specific channel parameters. */
    void        (*cb_tx_rx)(void *ch, uint16_t size);
                                /**< Application callback for transmit or receive. */
    void        *cb_parameter;  /**< Application callback parameter. */
 
} cio_ch_open_params_t;


/**************************************************************************//**
 @Description   CIO channel internal structure.
 
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
    uint32_t    status;             /**< The channel's status. */
    uint8_t     *data_base;         /**< Data base pointer. */
    uint8_t     *data_end;          /**< Data end pointer. */
    uint8_t     *data_current;      /**< Pointer to data currently transmitted/received. */
    uint8_t     *data_current_free; /**< Pointer to unused (free) data. */
    uint8_t     *data_next;         /**< Pointer to next byte for allocation by application (Tx) or device (Rx). */
    void        *cb_parameter;      /**< Application's parameter for the callback. */
    uint16_t    borrow_right;       /**< Current reserved size at buffer end. */
    uint16_t    available_size;     /**< Current available data size. */
    uint16_t    max_size;           /**< Maximum data size. */
    void        (*cb_tx_rx)(void *ch, uint16_t size);
                                    /**< Application's callback for this channel. */
    void        *lld_ch;            /**< The matching LLD channel handle. */
    void        *cio_interf;        /**< The channel's CIO device interface. */
    uint8_t     channel_type;       /**< The channel's type (read or write). */
#ifdef CIO_STATISTICS
    uint32_t    processed_bytes;
    uint32_t    errors;
#endif // CIO_STATISTICS

} cio_channel_t;


/**************************************************************************//**
 @Group         cio_init_id     CIO Initialization API

 @Description   CIO upper layer API for the application initialization stage
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osCioDeviceOpen

 @Description   Opens a CIO device for operation using the given parameters.
 
 @Param[in]     device_name - The name of the device.
 @Param[in]     lld_params  - Device-specific parameters or NULL to use the
                              default device parameters.

 @Return        A CIO device handle if the device was opened successfully
 @Return        NULL if the device failed to open
 
*//***************************************************************************/
cio_dev_handle  osCioDeviceOpen(char *device_name, void *lld_params);


/**************************************************************************//**
 @Function      osCioChannelOpen

 @Description   Opens a channel of a CIO device using the given parameters.
 
 @Param[in]     cio_handle     -  CIO device handle returned by osCioDeviceOpen()
 @Param[out]    cio_ch         -  Pointer to a CIO channel structure.
 @Param[in]     mode           -  Channel mode (#CIO_READ, #CIO_WRITE, #CIO_ACTIVE).
 @Param[in]     ch_open_params -  Channel parameters.

 @Retval        OS_SUCCESS if channel was opened successfully
 @Retval        OS_FAIL if channel failed to open
 
*//***************************************************************************/
os_status   osCioChannelOpen(cio_dev_handle         cio_handle,
                             cio_channel_t          *cio_ch,
                             int                    mode,
                             cio_ch_open_params_t   *ch_open_params);


/**************************************************************************//**
 @Function      osCioChannelClose

 @Description   Closes an open channel of a CIO device.
 
 @Param[in]     cio_ch -  Pointer to a valid CIO channel returned by
                          osCioChannelOpen()

 @Retval        OS_SUCCESS if channel closed properly
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                channel failed to close properly
 
*//***************************************************************************/
os_status   osCioChannelClose(cio_channel_t *cio_ch);


/** @} */ // end of cio_init_id


/**************************************************************************//**
 @Group         cio_runtime_id     CIO Runtime API

 @Description   CIO upper layer API for the application runtime stage
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osCioChannelBufferGet

 @Description   Call this function to obtain free buffer space.
 
                You must call this function before transmitting or receiving
                (from the LLD).
 
 @Param[in]     cio_ch  - Pointer to a valid CIO channel.
                          osCioChannelOpen() returns this pointer.
 @Param[in]     size    - Requested size.
 
 @Return        Pointer to the allocated buffer.
 @Retval        Returns NULL if memory is not available.
 
*//***************************************************************************/
uint8_t *   osCioChannelBufferGet(cio_channel_t *cio_ch, uint16_t size);


/**************************************************************************//**
 @Function      osCioChannelTxBufferPut

 @Description   Call this function to initiate transmission of the contents of
                a previously allocated buffer.
 
 @Param[in]     cio_ch  - Pointer to a valid CIO channel.
                          osCioChannelOpen() returns this pointer.

 @Return        Number of characters which were accepted for transmit by LLD.
 
*//***************************************************************************/
uint16_t        osCioChannelTxBufferPut(cio_channel_t *cio_ch);


/**************************************************************************//**
 @Function      osCioChannelRxBufferGet

 @Description   This function returns a pointer to the received data
 
 @Param[in]     cio_ch  - Pointer to a valid CIO channel.
                          osCioChannelOpen() returns this pointer.
 @Param[out]    size    - Pointer to number of received bytes

 @Return        Pointer to the received data
 @Retval        NULL if memory is not available.
 
*//***************************************************************************/
uint8_t *   osCioChannelRxBufferGet(cio_channel_t *cio_ch, uint16_t *size);


/**************************************************************************//**
 @Function      osCioChannelRxBufferFree

 @Description   Call this function when the application is done processing
                the received data.
 
 @Param[in]     cio_ch  - Pointer to a valid CIO channel.
                          osCioChannelOpen() returns this pointer.
 @Param[in]     size    - Number of bytes to free.
 
*//***************************************************************************/
void        osCioChannelRxBufferFree(cio_channel_t *cio_ch, uint16_t size);


/**************************************************************************//**
 @Function      osCioDeviceCtrl

 @Description   Performs control commands on a device.
 
 @Param[in]     cio_handle  - CIO device handle.
 @Param[in]     command     - Command code - see @ref cio_ctrl_cmd "CIO Control Commands"
 @Param[in]     param       - Command parameter.

 @Retval        OS_SUCCESS if command was executed properly
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                device failed to execute the command
 
*//***************************************************************************/
os_status   osCioDeviceCtrl(cio_dev_handle cio_handle, uint32_t command, void *param);


/**************************************************************************//**
 @Function      osCioChannelCtrl

 @Description   Performs control commands on a channel.
 
 @Param[in]     cio_ch  - Pointer to a valid CIO channel.
                          osCioChannelOpen() returns this pointer.
 @Param[in]     command     - Command code - see @ref cio_ctrl_cmd "CIO Control Commands"
 @Param[in]     param   - Command parameter.

 @Retval        OS_SUCCESS if command was executed properly
 @Retval        OS_ERR_COMMAND_INVALID if #CIO_READ command was called on a
                #CIO_WRITE channel, or vice versa
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                channel failed to execute the command
 
*//***************************************************************************/
os_status   osCioChannelCtrl(cio_channel_t *cio_ch, uint32_t command, void *param);


/** @} */ // end of cio_runtime_id

/** @} */ /* end of cio_id */

_OS_END_EXTERN_C

#endif // __OS_CIO_H

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:26 $
 $Id: os_sio_lld_.h,v 1.15 2012/11/22 16:28:26 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_sio_lld_.h,v $
 $Revision: 1.15 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sio_lld_.h

 @Description   OS SIO-LLD Interface.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_SIO_LLD_H_
#define __OS_SIO_LLD_H_


/**************************************************************************//**
 @Group         sio_group   Synchronized I/O

 @Description   Synchronized I/O routines
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         sio_lld     SIO LLD API

 @Description   SIO lower layer (LLD) API
 
                This API is for use by the Low Level Driver developer. Users
                should use the SIO higher layer API within their applications.
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   SIO LLD channel parameters, passed by the SIO serializer to
                the LLD when a channel is opened.
 
*//***************************************************************************/

typedef struct
{
    uint16_t            channel_num;            /**< Channel number. */
    uint8_t             *data_base;             /**< Base address of data buffer. */
    uint32_t            buffer_data_size;       /**< Size of the data buffer. */
    uint32_t            buffer_size;            /**< Size of the data buffer. */
    uint16_t            *application_buffer;    /**< Index to buffer used by application. */
    volatile uint16_t   **first_driver_buffer;  /**< Index to first buffer held by driver. */
    volatile uint16_t   **last_driver_buffer;   /**< Index to last buffer held by driver. */
    uint16_t            num_of_channel_buffers; /**< Number of buffers for the channel use. */
    void                **lld_channel;          /**< A handle to channel, that is received from LLD and used in close channel. */
    void                *lld_params;            /**< LLD-specific channel parameters. */
    void                *sio_dev;
    void                *sio_channel;
 
} sio_lld_ch_open_t;


/**************************************************************************//**
 @Description   SIO LLD registration parameters.
 
*//***************************************************************************/
typedef struct
{
    os_status   (*device_open)(void *device_handle, void *lld_params);
    /**< Device-open function. */
    void *      (*channel_open)(void *device_handle, int mode, sio_lld_ch_open_t *lld_ch_params);
    /**< Channel-open function. */
    os_status   (*channel_close)(void *ch_handle, int mode);
    /**< Channel-close function. */
    os_status   (*channel_ctrl)(void *ch_handle, uint32_t command, void *param);
    /**< Channel-control function. */
    os_status   (*device_ctrl)(void *device_handle, uint32_t command, void *param);
    /**< Device-control function. */
    void        *lld_handle;
    /**< LLD Handle. */

} sio_register_params_t;


/**************************************************************************//**
 @Function      sioRegister

 @Description   This function registers a new device.
 
                The function takes the LLD's function pointers and basic LLD
                definitions as parameters. This function should be called by
                each LLD instance.
 
 @Param[in]    device_name -  The name of the of device.
 @Param[in]    register_params -  SIO registration parameters.

 @Return        None.

*//***************************************************************************/
sio_dev_handle sioRegister(char *device_name, sio_register_params_t *register_params);


/**************************************************************************//**
 @Function      sioDeviceCall

 @Description   This function notifies SIO that driver buffers has changed
                for some channels, so SIO passes through all the open channels in the
                device and calls the data callback and if necessary also the error
                callback. The function may apply for all read open channels, all write
                open channel or all open channels.
 
 @Param[in]    sio_dev -  A handle to an SIO device.
 @Param[in]    mode -  May be either SIO_READ or SIO_WRITE.

 @Return        None.

*//***************************************************************************/
void sioDeviceCall(void *sio_dev, uint32_t mode);


/**************************************************************************//**
 @Function      sioChannelCall

 @Description   This function notifies SIO that driver buffers has changed
                for a specific channel and calls the data callback for that channel.
 
 @Param[in]    sio_ch -  Pointer to a valid SIO channel.

 @Return        None.

*//***************************************************************************/
void sioChannelCall(sio_channel_t *sio_ch);


/** @} */ /* end of SIO LLD API group */

/** @} */ /* end of SIO group */


#endif // __OS_SIO_LLD_H_


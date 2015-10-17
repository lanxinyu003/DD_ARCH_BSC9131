/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/10/28 16:02:36 $
 $Id: os_bio_lld_.h,v 1.28 2014/10/28 16:02:36 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_bio_lld_.h,v $
 $Revision: 1.28 $
******************************************************************************/

/******************************************************************************
 
 @File          os_bio_lld_.h

 @Description   OS BIO-LLD Interface.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_BIO_LLD_H_
#define __OS_BIO_LLD_H_


/**************************************************************************//**
 @Group         bio_group   Buffer I/O

 @Description   Buffer I/O routines
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         bio_lld     BIO LLD API

 @Description   BIO lower layer (LLD) API
 
                This API is for use by the Low Level Driver developer. Users
                should use the BIO higher layer API within their applications.
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   BIO LLD channel parameters, passed by the BIO serializer to
                the LLD when a channel is opened.
 
*//***************************************************************************/
typedef struct
{
    void        *serializer;        /**< BIO serializer handle. */
    uint16_t    channel_num;        /**< Channel number. */
    uint32_t    max_rx_buffer_size;
    /**< For Rx channels: the maximum buffer size; the LLD should verify this size. */
    bool        using_common_pool;
    /**< For Rx channels: indicates if this channel uses the common buffers pool. */
    void        *lld_params;        /**< LLD-specific channel parameters. */
 
} bio_lld_ch_open_t;


/**************************************************************************//**
 @Description   BIO LLD registration parameters.
 
*//***************************************************************************/
typedef struct
{
    os_status   (*device_open)(void *device_handle, void *lld_params);
    /**< Device-open function. */
    void *      (*channel_open)(void *device_handle, int mode, bio_lld_ch_open_t *lld_ch_params);
    /**< Channel-open function. */
    os_status   (*channel_close)(void *ch_handle, int mode);
    /**< Channel-close function. */
    os_status   (*channel_tx)(void *ch_handle, void *frame);
    /**< Channel-transmit function. */
    os_status   (*channel_ctrl)(void *ch_handle, uint32_t command, void *param);
    /**< Channel-control function. */
    os_status   (*device_ctrl)(void *device_handle, uint32_t command, void *param);
    /**< Device-control function. */
    bool        use_common_pool;
    /**< Indicates whether the LLD uses the common buffers pool or not. */
    bool        internal_buffers_pool;
    /**< Indicates whether the LLD uses it's own buffers pool */
    uint16_t    tx_queue_size;
    /**< Size of Tx frames queue. */
    uint16_t    rx_queue_size;
    /**< Size of Rx frames queue. */
    uint16_t    rx_buffers_queue_size;
    /**< Size of Rx buffers queue; Set to zero if the LLD builds the received frames. */
    uint16_t    rx_common_buffers_queue_size;
    /**< Size of Rx common buffers queue. */
    void        *lld_handle;
    /**< LLD Handle. */
    bool        is_promisc;
    /**< Is interface promisc */
    uint32_t    interface_speed ;
    /**< Interface speed (bits/sec) */

} bio_register_params_t;


/**************************************************************************//**
 @Function      bioRegister

 @Description   This function registers a new device.
 
                The function takes the LLD's function pointers and basic LLD
                definitions as parameters. This function should be called by
                each LLD instance.
 
 @Param[in]    device_name -  The name of the of device.
 @Param[in]    register_params -  BIO registration parameters.

 @Return        The BIO device handle; This handle should be used by the LLD
                in any BIO call for this device.

*//***************************************************************************/
bio_dev_handle  bioRegister(char *device_name, bio_register_params_t *register_params);


/**************************************************************************//**
 @Function      bioGlobalChannelGet

 @Description   This function returns a pointer to the global (common) channel
                of the device.
 
 @Param[in]    bio_handle -  The BIO handle of the device. bioRegister()
                             returns this handle.

 @Return        The pointer to the global channel of the device; NULL if the
                device has no global channel.
 
 @Cautions
                - The LLD must not write directly to the returned pointer.
                - This pointer must be used only as an argument to
                  bioChannelRxBufferGet().
                - The LLD should not call this function before its own
                  device-open function is called, because the returned value
                  would be incorrect.

*//***************************************************************************/
bio_channel_t * bioGlobalChannelGet(bio_dev_handle bio_handle);


/**************************************************************************//**
 @Function      bioChannelRxBufferOffsetGet

 @Description   This function returns a size of buffer offset allocated for LLD device.
 
 @Param[in]    bio_ch -  Pointer to a valid BIO RX channel.

 @Return        The size of LLD offset.
 
*//***************************************************************************/
uint16_t bioChannelRxBufferOffsetGet(bio_channel_t *bio_ch);

/**************************************************************************//**
 @Function      bioChannelRxBufferGet

 @Description   Call this function to obtain free buffer space.

 @Param[in]    bio_ch -  Pointer to a valid BIO channel, or to the global
                          (common) channel of the device. bioGlobalChannelGet()
                          returns a pointer to the global channel of the device.
 @Param[out]    size -  Pointer to the size of the allocated buffer.

 @Return        Pointer to the allocated buffer. Returns NULL if memory is
                not available.
 
*//***************************************************************************/
uint8_t * bioChannelRxBufferGet(bio_channel_t *bio_ch, uint32_t *size);


/**************************************************************************//**
 @Function      bioChannelTxCb

 @Description   LLD calls this function to inform the BIO serializer that
                transmission of @a num_of_frames frames is complete.
 
 @Param[in]    bio_ch -  Pointer to a valid BIO channel.
 @Param[in]    num_of_frames -  Number of transmitted frames.
 @Param[in]    error_status -  LLD-specific error and status value.

*//***************************************************************************/
void    bioChannelTxCb(bio_channel_t *bio_ch, uint16_t num_of_frames, uint32_t error_status);


/**************************************************************************//**
 @Function      bioChannelRxCb

 @Description   LLD calls this function to inform the BIO serializer that new
                data has been received.
 
                BIO is responsible to build the frame according to the queued
                data buffers and the given frame length.
 
 @Param[in]    bio_ch -  Pointer to a valid BIO channel.
 @Param[in]    frame_length -  Length of received frame (in bytes).
 
*//***************************************************************************/
void    bioChannelRxCb(bio_channel_t *bio_ch, uint32_t frame_length);


/**************************************************************************//**
 @Function      bioChannelRxFrameNew

 @Description   The LLD should call this function to obtain a new empty frame.
 
                This function is used when the LLD takes the responsibility to
                build the frame, instead of BIO. After the frame is ready, the
                LLD should call bioChannelRxFrameCb().

 @Param[in]    bio_ch -  Pointer to a valid BIO channel.

 @Return        Pointer to the empty frame. Returns NULL if there is no
                available frame.
 
*//***************************************************************************/
os_frame_t * bioChannelRxFrameNew(bio_channel_t *bio_ch);


/**************************************************************************//**
 @Function      bioChannelRxFrameCb

 @Description   The LLD calls this function to inform the BIO serializer that a
                new frame has been received.
 
                This function is used when the LLD takes the responsibility to
                build the frame, instead of BIO. The LLD obtained an empty frame
                with bioChannelRxFrameNew(), built the frame and passes it to
                BIO through this function.
 
 @Param[in]    bio_ch -  Pointer to a valid BIO channel.
 @Param[in]    frame -  The received frame.
 
*//***************************************************************************/
void    bioChannelRxFrameCb(bio_channel_t *bio_ch, void *frame);


/**************************************************************************//**
 @Function      bioChannelRxError

 @Description   The LLD calls this function to inform the BIO serializer that
                an error has occured in a receive channel.
 
                If BIO is responsible to build the frame, queued data buffers
                are dequeued and released, according to the amount of erroneous
                buffers. If the application supplied an error handler, it is
                called once.
 
 @Param[in]    bio_ch -  Pointer to a valid BIO channel.
 @Param[in]    amount -  amount of erroneous buffers.
 @Param[in]    error_status -  LLD-specific error and status value.
 
*//***************************************************************************/
void    bioChannelRxError(bio_channel_t *bio_ch, uint16_t amount, uint32_t error_status);


/**************************************************************************//**
 @Function      bioChannelRxBufferFree

 @Description   The LLD calls this function to free an Rx buffer, usually after
                an error has occurred and the buffer must be discarded. After
                all erroneous buffers were freed, the LLD can proceed to call
                bioChannelRxError().
 
                This function must be used only when the LLD takes the
                responsibility to build the frame, instead of BIO. In the other
                scenario, where BIO builds the frame, the buffers are freed
                within bioChannelRxError().

 @Param[in]    bio_ch -  Pointer to a valid BIO channel.
 @Param[in]    buffer -  Pointer to the buffer to free.
 
*//***************************************************************************/
void    bioChannelRxBufferFree(bio_channel_t *bio_ch, uint8_t *buffer);


/** @} */ /* end of BIO LLD API group */

/** @} */ /* end of BIO group */


#endif // __OS_BIO_LLD_H_


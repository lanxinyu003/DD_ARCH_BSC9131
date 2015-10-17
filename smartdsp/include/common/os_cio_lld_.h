/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_cio_lld_.h,v 1.10 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_cio_lld_.h,v $
 $Revision: 1.10 $
******************************************************************************/

/******************************************************************************
 
 @File          os_cio_lld_.h

 @Description   OS CIO-LLD Interface.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_CIO_LLD_H_
#define __OS_CIO_LLD_H_


/**************************************************************************//**
 @Group         cio_group   Character I/O

 @Description   Character I/O routines
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         cio_lld     CIO LLD API

 @Description   CIO lower layer (LLD) API.
 
                This API is for use by the Low Level Driver developer. Users
                should use the CIO higher layer API within their applications.
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   CIO LLD channel parameters, passed by the CIO serializer to
                the LLD when a channel is opened.
 
*//***************************************************************************/
typedef struct
{
    void        *serializer;    /**< CIO serializer handle. */
    uint16_t    channel_num;    /**< Channel number. */
    uint8_t     *data_base;     /**< Base address of data buffer. */
    uint16_t    data_size;      /**< Size of the data buffer. */
    void        *lld_params;    /**< LLD-specific channel parameters. */
 
} cio_lld_ch_open_t;


/**************************************************************************//**
 @Description   CIO LLD registration parameters.
 
*//***************************************************************************/
typedef struct
{
    os_status   (*device_open)(void *device_handle, void *lld_params);
    /**< Device-open function. */
    void *      (*channel_open)(void *device_handle, int mode, cio_lld_ch_open_t *lld_ch_params);
    /**< Channel-open function. */
    os_status   (*channel_close)(void *ch_handle, int mode);
    /**< Channel-close function. */
    uint16_t    (*channel_tx)(void *ch_handle, uint8_t *data, uint16_t size);
    /**< Channel-transmit function. */
    os_status   (*channel_ctrl)(void *ch_handle, uint32_t command, void *param);
    /**< Channel-control function. */
    os_status   (*device_ctrl)(void *device_handle, uint32_t command, void *param);
    /**< Device-control function. */
    void        *lld_handle;
    /**< LLD Handle. */

} cio_register_params_t;


/**************************************************************************//**
 @Function      cioRegister

 @Description   This function registers a new device.
 
                The function takes the LLD's function pointers and basic LLD
                definitions as parameters. This function should be called by
                each LLD instance.
 
 @Param[in]    device_name -  The name of the of device.
 @Param[in]    register_params -  CIO registration parameters.

 @Return        Pointer to the CIO device handle; This handle should be used
                by the LLD in any CIO call.

*//***************************************************************************/
void *  cioRegister(char *device_name, cio_register_params_t *register_params);


/**************************************************************************//**
 @Function      cioChannelTxCb

 @Description   LLD calls this function to inform the CIO serializer that
                transmission of @a size bytes is complete.
 
                In some cases, the CIO would call the LLD's transmit
                function again, to continue the transmission.
 
 @Param[in]    cio_ch -  Pointer to a valid CIO channel.
 @Param[in]    size -  Number of transmitted bytes.

 @Cautions      Avoid calling this function from the LLD's transmit function,
                because doing so may produce unpredictable results.

*//***************************************************************************/
void    cioChannelTxCb(cio_channel_t *cio_ch, uint16_t size);


/**************************************************************************//**
 @Function      cioChannelRxCb

 @Description   LLD calls this function to inform the CIO serializer that new
                data has been received.
 
 @Param[in]    cio_ch -  Pointer to a valid CIO channel.
 @Param[in]    size -  Number of bytes received.
 
*//***************************************************************************/
void    cioChannelRxCb(cio_channel_t *cio_ch, uint16_t size);


/** @} */ // end of CIO LLD API group

/** @} */ // end of CIO group


#endif // __OS_CIO_LLD_H_

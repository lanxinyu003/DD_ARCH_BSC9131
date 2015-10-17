/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_sio_.h,v 1.13 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_sio_.h,v $
 $Revision: 1.13 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sio_.h

 @Description   OS SIO internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_SIO_H_
#define __OS_SIO_H_


#define SIO_NAME_SIZE   4



typedef struct sio_interface_t
{
    char      name[SIO_NAME_SIZE];
    /**< SIO device identification name. */
    uint32_t  status;
    /**< SIO Device status. */
    void                    *device_handle;
    /**< LLD handle. */
    struct sio_channel_t    *rx_channels;
    struct sio_channel_t    *tx_channels;
    /**< Link lists of channels. */
    void        (*rx_callback)(void *param);
    /**< Application's RX callback. */
    void        *rx_callback_parameter;
    /**< Application's RX callback parameter. */
    void        (*tx_callback)(void *param);
    /**< Application's TX callback. */
    void        *tx_callback_parameter;
    /**< Application's TX callback parameter. */
    os_status   (*device_open)(void *device_handle, void *lld_params);
    /**< LLD device open function. Returns a pointer to the first */
    void *      (*channel_open)(void    *device_handle,
                    int                 mode,
                    sio_lld_ch_open_t   *lld_ch_params);
    /**< LLD channel open function. */
    os_status   (*channel_close)(void *ch_handle, int mode);
    /**< LLD channel close function. */
    os_status   (*channel_ctrl)(void *ch_handle, uint32_t command, void *param);
    /**< LLD channel control function. */
    os_status   (*device_ctrl)(void *device_handle, uint32_t command, void *param);
    /**< LLD device control function. */
    void        (*error_callback)(void *param);
    /**< Application's callback for underrun error. */
} sio_interface_t;


void sioInsertChannel(sio_channel_t **sio_ch_list,
                             sio_channel_t *sio_ch);

os_status sioRemoveChannel(sio_channel_t *sio_ch);

#endif // __OS_SIO_H_

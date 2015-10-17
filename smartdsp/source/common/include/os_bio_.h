/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_bio_.h,v 1.14 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_bio_.h,v $
 $Revision: 1.14 $
******************************************************************************/

/******************************************************************************
 
 @File          os_bio_.h

 @Description   OS BIO internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_BIO_H_
#define __OS_BIO_H_


#define BIO_NAME_SIZE   4

typedef struct bio_interface_t
{
    char            name[BIO_NAME_SIZE];    /**< BIO device identification name. */
    uint32_t        status;                 /**< BIO Device status. */
    void            *device_handle;         /**< LLD handle. */
    bool            internal_buffers_pool;  /**< buffers pool is managed by the device and not by BIO */
    os_mem_part_t   *buffers_pool;          /**< Common buffers pool for receive channels (optional). */
    /* common_queue MUST be right after the buffers_pool */
    os_queue_handle common_queue;           /**< Common buffers queue handle. */
    uint16_t        tx_queue_size;          /**< Size of Tx frames queue. */
    uint16_t        rx_queue_size;          /**< Size of Rx frames queue. */
    uint16_t        rx_buffers_queue_size;  /**< Size of Rx buffers queue. */
    bio_interface_stat_t bio_stat;
    os_status   (*device_open)(void *device_handle, void *lld_params);
    /**< LLD device open function. */
    void *      (*channel_open)(void *device_handle, int mode, bio_lld_ch_open_t *lld_ch_params);
    /**< LLD channel open function. */
    os_status   (*channel_close)(void *ch_handle, int mode);
    /**< LLD channel close function. */
    os_status   (*channel_tx)(void *ch_handle, void *frame);
    /**< LLD channel transmit function. */
    os_status   (*channel_ctrl)(void *ch_handle, uint32_t command, void *param);
    /**< LLD channel control function. */
    os_status   (*device_ctrl)(void *device_handle, uint32_t command, void *param);
    /**< LLD device control function. */
 
} bio_interface_t;


void bioSbFrameBuild(void *bio_ch_handle, os_frame_t *frame, uint32_t frame_length);
void bioMbFrameBuild(void *bio_ch_handle, os_frame_t *frame, uint32_t frame_length);


#endif // __OS_BIO_H_

/******************************************************************************
 Copyright � 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_dma.h,v 1.23 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_dma.h,v $
 $Revision: 1.23 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_dma.h

 @Description   DMA API

                This file contains the functions and structures for the DMA.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_DMA_H
#define __OS_DMA_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         dma_id DMA Module API
 
 @Description   DMA Setup and control.
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Description   DMA controller handle.
 
*//***************************************************************************/
typedef void* dma_ctrl_handle;



/**************************************************************************//**
 @Description   Config parameters for the DMA channel.
 
                DMA channel is a control structure that can be bound to a DMA
                chain and move data. It also handles interrupts that are
                generated by the transfers.
 
*//***************************************************************************/
typedef struct
{
     uint32_t           priority;           /**< Channel priority. */
     os_hwi_function    dma_callback;       /**< Channel interrupt handler. */
     os_hwi_arg         dma_callback_param; /**< Channel interrupt handler parameter. */
     arch_specific      arch;               /**< Channel architecture-dependent parameters. */
 
} dma_channel_config_t;


/**************************************************************************//**
 @Description   Config parameters for the DMA chain.
 
                DMA chain is a structure that can store multiple DMA transfers
                and then be bound to a DMA channel and executed.
 
*//***************************************************************************/
typedef struct
{
    uint32_t        max_num_of_transfers;
    /**< Number of transfers that can be added to this chain by osDmaChainTransferAdd(). */
    bool            loop;
    /**< Last transfer wraps to the first. */
    arch_specific   arch;
    /**< Chain architecture-dependent parameters. */
    os_mem_type     heap;
    /**< Heap to use for a chain's BDs */
} dma_chain_config_t;


/**************************************************************************//**
 @Description   Config parameters for a DMA transfer.
 
                DMA transfer describes a specific data move that can be stored
                in a DMA chain and then executed with a DMA channel.
 
*//***************************************************************************/
typedef struct
{
    uint32_t        source;             /**< Source address for the transfer. */
    uint32_t        destination;        /**< Destination address for the transfer. */
    uint32_t        source_bus;         /**< The system bus towards the source. */
    uint32_t        destination_bus;    /**< The system bus towards the destination. */
    uint32_t        size;               /**< Size of the DMA transfer in bytes. */
    bool            interrupt;          /**< Whether to generate an interrupt after this transfer. */
    arch_specific   arch;               /**< Transfer architecture-dependent parameters. */
 
} dma_transfer_config_t;



/**************************************************************************//**
 @Function      osDmaControllerOpen

 @Description   Gets a handle to the DMA controller.

 @Param[in]     dma_name -  A string containing the name of the DMA controller.

 @Return       A handle to the DMA controller if successful.
 @Return       NULL if not successful.

*//***************************************************************************/
dma_ctrl_handle osDmaControllerOpen(char *dma_name);


/**************************************************************************//**
 @Function      osDmaChannelOpen

 @Description   Creates a DMA channel software structure and places it in
                the DMA controller structure.
 
                The function also initializes the channel structure according
                to the configuration parameters.

 @Param[in]     dma_controller      -  A handle to the DMA controller.
                                       osDmaControllerOpen() returns this handle.
 @Param[out]    dma_channel         -  Memory for use of the driver. This
                                       pointer also becomes the channel handle.
 @Param[in]     dma_channel_config  -  DMA channel configuration parameters.

 @Retval        OS_SUCCESS                      - succeeded in opening the channel
 @Retval        OS_ERR_DMA_RESOURCE_UNAVAILABLE - channel is unavailable
 @Return        Error status, encoded in os_error.h, for other errors
 

*//***************************************************************************/
os_status   osDmaChannelOpen(dma_ctrl_handle        dma_controller,
                             void                   *dma_channel,
                             dma_channel_config_t   *dma_channel_config);


/**************************************************************************//**
 @Function      osDmaChainCreate

 @Description   Creates a DMA chain and places it in the DMA controller.
 
                The function also initializes the chain structure according to
                the configuration parameters.

 @Param[in]     dma_controller   -  A handle to the DMA controller.
                                    osDmaControllerOpen() returns this handle.
 @Param[out]    dma_chain        -  Memory for use of the driver. This
                                    pointer also becomes the channel handle.
 @Param[in]     dma_chain_config -  DMA chain configuration parameters.

 @Retval        OS_SUCCESS       - succeeded in creating the chain
 @Return        Error status, encoded in os_error.h, for other errors
 
*//***************************************************************************/
os_status   osDmaChainCreate(dma_ctrl_handle    dma_controller,
                             void               *dma_chain,
                             dma_chain_config_t *dma_chain_config);


/**************************************************************************//**
 @Function      osDmaChainTransferAdd

 @Description   Adds a transfer to a DMA chain.

 @Param[in]     dma_chain           -  A handle to a DMA chain created with
                                       osDmaChainCreate().
 @Param[in]     dma_transfer_config -  DMA transfer configuration parameters.

 @Retval        OS_SUCCESS              - succeeded in adding the transfer
 @Retval        OS_ERR_DMA_CHAIN_BOUND  - the chain is currently bound -
                                          unavailable for adding more transfers
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status   osDmaChainTransferAdd(void                  *dma_chain,
                                  dma_transfer_config_t *dma_transfer_config);

/**************************************************************************//**
 @Function      osDmaChainTransferAddEx

 @Description   Adds a transfer to a DMA chain and get a handle to it.

 @Param[in]     dma_chain           -  A handle to a DMA chain created with
                                       osDmaChainCreate().
 @Param[in]     dma_transfer_config -  DMA transfer configuration parameters.

 @Param[out]    xfer                -  DMA transfer handle.
 
 @Retval        OS_SUCCESS              - succeeded in adding the transfer
 @Retval        OS_ERR_DMA_CHAIN_BOUND  - the chain is currently bound -
                                          unavailable for adding more transfers
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status   osDmaChainTransferAddEx(void                  *dma_chain,
                                    dma_transfer_config_t *dma_transfer_config,
                                    os_dma_xfer           *xfer);

/**************************************************************************//**
 @Function      osDmaChainReset

 @Description   Empty the chain from transfers.

 @Param[in]     dma_chain           -  A handle to a DMA chain created with
                                       osDmaChainCreate().
 @Return        None.

*//***************************************************************************/
void   osDmaChainReset(void                  *dma_chain);

/**************************************************************************//**
 @Function      osDmaTransferDestinationSet

 @Description   Modify a transfer destination address

 @Param[in]     xfer        -  DMA transfer handle received from
                               osDmaChainTransferAddEx()
 @Param[in]     destination -  New destination address

*//***************************************************************************/
void   osDmaTransferDestinationSet(uint32_t xfer, uint32_t destination);


/**************************************************************************//**
 @Function      osDmaTransferSourceSet

 @Description   Modify a transfer source address

 @Param[in]     xfer        -  DMA transfer handle received from
                               osDmaChainTransferAddEx()
 @Param[in]     source      -  New source address

*//***************************************************************************/
void   osDmaTransferSourceSet(uint32_t xfer, uint32_t source);

/**************************************************************************//**
 @Function      osDmaTransferSizeSet

 @Description   Modify a transfer size.

 @Param[in]     xfer        -  DMA transfer handle received from
                               osDmaChainTransferAddEx()
 @Param[in]     size        -  New size.

*//***************************************************************************/
void   osDmaTransferSizeSet(uint32_t xfer, uint32_t size);

/**************************************************************************//**
 @Function      osDmaChannelBind

 @Description   Prepares the channel for activation. This function programs
                the DMA channel registers with the DMA chain. The chain is
                executed with osDmaChannelStart().

 @Param[in]     dma_channel         -  A handle to a DMA chain created with
                                       osDmaChannelOpen().
 @Param[in]     dma_chain           -  A handle to a DMA chain created with
                                       osDmaChainCreate().

 @Retval        OS_SUCCESS          - succeeded in binding the chain with
                                      the channel
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status   osDmaChannelBind(void *dma_channel, void *dma_chain);


/**************************************************************************//**
 @Function      osDmaChannelUnbind

 @Description   Detaches the specified channel from the chain.
 

 @Param[in]     dma_channel         -  A handle to a DMA chain created with
                                       osDmaChannelOpen().

 @Retval        OS_SUCCESS          - succeeded in unbinding the chain from
                                      the channel
 @Return        Error status, encoded in os_error.h, for other errors
 
 @Cautions      Call this function only when the DMA channel is not active

*//***************************************************************************/
os_status   osDmaChannelUnbind(void *dma_channel);


/**************************************************************************//**
 @Function      osDmaChannelStart

 @Description   Starts a transfer on a channel that was previously bound to
                a DMA chain with osDmaChannelBind().

 @Param[in]     dma_channel         -  A handle to a DMA chain created with
                                       osDmaChannelOpen().
 
 @Retval        OS_SUCCESS                   - channel started
 @Retval        OS_ERR_DMA_CHANNEL_NOT_BOUND - no chain is bound to the channel

*//***************************************************************************/
os_status   osDmaChannelStart(void *dma_channel);


/**************************************************************************//**
 @Function      osDmaChannelIsActive

 @Description   Used to poll the channel to see when it is executing a transfer.

 @Param[in]     dma_channel         -  A handle to a DMA chain created with
                                       osDmaChannelOpen().

 @Retval        TRUE if the channel is active
 @Retval        FALSE if the channel is not active

*//***************************************************************************/
bool        osDmaChannelIsActive(void *dma_channel);


/**************************************************************************//**
 @Function      osDmaChannelClose

 @Description   Frees DMA channel resources (except memory) and removes these
                resources from the DMA controller.

 @Param[in]     dma_channel         -  A handle to a DMA chain created with
                                       osDmaChannelOpen().

 @Retval        OS_SUCCESS               - channel started
 @Retval        OS_ERR_DMA_CHANNEL_BOUND - the channel is bound to a chain

*//***************************************************************************/
os_status   osDmaChannelClose(void *dma_channel);


/**************************************************************************//**
 @Function      osDmaChainDelete

 @Description   Frees DMA chain resources (except memory) and removes the
                chain from the DMA controller.

 @Param[in]     dma_chain           -  A handle to a DMA chain created with
                                       osDmaChainCreate().

 @Retval        OS_SUCCESS              - chain was successfully deleted
 @Retval        OS_ERR_DMA_CHAIN_BOUND  - the chain is bound to a channel

*//***************************************************************************/
os_status   osDmaChainDelete(void *dma_chain);


/** @} */ // end of dma_id

_OS_END_EXTERN_C

#endif // __OS_DMA_H

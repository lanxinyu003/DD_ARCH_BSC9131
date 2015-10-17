/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:17 $
 $Id: dma.h,v 1.9 2012/11/22 16:28:17 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/dma/dma.h,v $
 $Revision: 1.9 $
******************************************************************************/

/**************************************************************************//**
 
 @File          dma.h

 @Description   This file contains the definitions, structures and prototypes that
                have an added value for an application that utilizes the driver.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __DMA_H
#define __DMA_H

#include "dma_init.h"

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         dma_id DMA Module API
 
 @Description   DMA Setup and control
 
 @{
*//***************************************************************************/
/***********************************************************//**
 @Group         dma_common_id Common DMA Module API
 @{
*//************************************************************/
/**************************************************************************//**
 @Group         dma_runtime_id DMA Runtime
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Buffer handle type.
                Used for buff parameter of dmaChainBufferAddEx.
*//***************************************************************************/
typedef uint32_t                        soc_dma_buff;

/**************************************************************************//**
 @Description   Aligns SPACE to the closest aligned value greater than it.
                ALIGNMENT must be a power of 2.
*//***************************************************************************/
#define DMA_ALIGN(SPACE, ALIGNMENT) ((((uint32_t)SPACE) + ALIGNMENT - 1) & (~(ALIGNMENT - 1)))
#define DMA_BURST_ALIGNMENT         16      /**< DMA burst alignment */
#define DMA_STRUCT_ALIGNMENT        4       /**< DMA struct alignment */
 
/**************************************************************************//**
 @Description   Size of memory needed for channel use.
*//***************************************************************************/
#define DMA_SIZE_OF_MEMORY_FOR_CHANNEL_USE  36

/**************************************************************************//**
 @Collection   Size and alignment of memory needed for chain use
 
               Important - Only DMA_SIZE_OF_MEMORY_FOR_CHAIN_USE should be
               used by the application.
 @{
*//***************************************************************************/
#define DMA_CHAIN_ALIGNMENT                   0x100 /* Do not use. */
#define DMA_SIZE_OF_MEMORY_FOR_CHAIN_USE(MAX_NUM_OF_TRANSFERS)  64

/* @} */

/**************************************************************************//**
 @Collection   Dma buffer mode
 @{
*//***************************************************************************/
#define DMA_READ  1
#define DMA_WRITE 0

/* @} */

/**************************************************************************//**
 @Description   DMA DREQ direction
*//***************************************************************************/
typedef enum
{
    DMA_SOURCE = 0x1,
    DMA_DESTINATION = 0x2
} dma_dreq_dir_t;

/**************************************************************************//**
 @Collection   DMA Buffer Descriptor
 @{
*//***************************************************************************/
#define DMA_BUFF_INTERRUPT           0x80000000
/**< Set status. */
#define DMA_BUFF_CYC                 0x40000000
/**< Cyclic address. */
#define DMA_BUFF_CONT                0x20000000
/**< Continuous buffer mode. */
/* Next port. */
#define DMA_BUFF_NO_INC              0x08000000
/**< Increments address. */
#define DMA_BUFF_FRZ                 0x00000040
/**< Freeze channel. */
#define DMA_BUFF_MASK_REQUESTS       0x00000020
/**< Mask Requests Until Data Reached Destination. */
#define DMA_BUFF_DONT_UPDATE         0x00000010
/**< Mask Requests Until Data Reached Destination. */

/* @} */

/**************************************************************************//**
 @Description   DMA MD BD
*//***************************************************************************/
#define DMA_MD_BD_LAST                 0x04000000
/**< Last Buffer In The Chain. */

/**************************************************************************//**
 @Collection   DMA channel priority
 @{
*//***************************************************************************/
#define DMA_CHANNEL_PRIORITY_HIGHEST       0x00000000
#define DMA_CHANNEL_PRIORITY_HIGH          0x00002000
#define DMA_CHANNEL_PRIORITY_LOW           0x00004000
#define DMA_CHANNEL_PRIORITY_LOWEST        0x00006000

/* @} */

/************************************************************//**
 @Collection    DMA Channel EDF Priority
 
    Table 3-13 Channels Sorting into 4 priority groups
    Time to dead line Priority Group
    0x00 - 0x01   0
    0x02 - 0x07   1
    0x08 - 0x3F   2
    0x40 - 0xFF   3
@{
*//*************************************************************/
#define DMA_CHANNEL_EDF_HIGHEST       0x01
#define DMA_CHANNEL_EDF_HIGH          0x07
#define DMA_CHANNEL_EDF_LOW           0x3F
#define DMA_CHANNEL_EDF_LOWEST        0xFE

/* @} */

/**************************************************************************//**
 @Description   Cast to unsigned to prevent sign extensio upon shift right.
*//***************************************************************************/
#define UVAL(U) ((uint32_t)(U))


/**************************************************************************//**
 @Description   SOC Single dimensional DMA BD
*//***************************************************************************/
typedef struct
{
    volatile uint32_t   bd_addr;
    /**< DMA BD address. */
    volatile uint32_t   bd_size;
    /**< Size of DMA BD. */
    volatile uint32_t   bd_attr;
    /**< DMA BD attributes. */
    volatile uint32_t   bd_bsize;
    /**< DMA BD buffer base size. */
} dma_bd_map_t;

/**************************************************************************//**
 @Description   SOC Multi dimensional DMA BD
*//***************************************************************************/
typedef struct
{
    volatile uint32_t   bd_addr;
    /**< DMA BD address. */
    volatile uint16_t   bd_bsize;
    /**< Size of DMA BD. */
    volatile uint16_t   bd_size;
    /**< DMA BD buffer base size. */
    volatile uint32_t   bd_attr;
    /**< DMA BD attributes. */
    volatile uint32_t   md[5];
    /**< DMA BD Multidimensional parameters. */
} dma_md_bd_map_t;



/**************************************************************************//**
 @Description   DMA channel freeze status
*//***************************************************************************/
typedef enum {
    NO_FREEZE,
    SRC_FROZEN,
    DST_FROZEN,
    CH_FROZEN
} dma_freeze_status_t;

/**************************************************************************//**
 @Description   DMA channel status
*//***************************************************************************/
typedef enum {
    DMA_CH_NOT_ACTIVE,
    DMA_CH_SRC_ACTIVE,
    DMA_CH_DST_ACTIVE,
    DMA_CH_IS_ACTIVE
} dma_ch_status_t;

/**************************************************************************//**
 @Description   DMA buffer dimension.
*//***************************************************************************/
typedef enum {
    DMA_BUFF_DIM_1 = 0x00000000,
    DMA_BUFF_DIM_2 = 0x01000000,
    DMA_BUFF_DIM_3 = 0x02000000,
    DMA_BUFF_DIM_4 = 0x03000000
} dma_buff_dim_t;

/**************************************************************************//**
 @Description   DMA buffer interrupt dimension.
*//***************************************************************************/
typedef enum {
    DMA_BUFF_INT_DIM_1 = 0x00000000,
    DMA_BUFF_INT_DIM_2 = 0x00400000,
    DMA_BUFF_INT_DIM_3 = 0x00800000,
    DMA_BUFF_INT_DIM_4 = 0x00C00000
} dma_buff_int_dim_t;

/**************************************************************************//**
 @Description   DMA buffer freeze dimension.
*//***************************************************************************/
typedef enum {
    DMA_BUFF_FRZ_DIM_1 = 0x00000000,
    DMA_BUFF_FRZ_DIM_2 = 0x00100000,
    DMA_BUFF_FRZ_DIM_3 = 0x00200000,
    DMA_BUFF_FRZ_DIM_4 = 0x00300000
} dma_buff_frz_dim_t;

/**************************************************************************//**
 @Description   DMA buffer cont dimension.
*//***************************************************************************/
typedef enum {
    DMA_BUFF_CONT_DIM_1 = 0x00000000,
    DMA_BUFF_CONT_DIM_2 = 0x00040000,
    DMA_BUFF_CONT_DIM_3 = 0x00080000,
    DMA_BUFF_CONT_DIM_4 = 0x000C0000
} dma_buff_cont_dim_t;

/**************************************************************************//**
 @Description   DMA buffer mask requests dimension.
*//***************************************************************************/
typedef enum {
    DMA_BUFF_MR_DIM_1 = 0x00000000,
    DMA_BUFF_MR_DIM_2 = 0x00010000,
    DMA_BUFF_MR_DIM_3 = 0x00020000,
    DMA_BUFF_MR_DIM_4 = 0x00030000
} dma_buff_mr_dim_t;

/**************************************************************************//**
 @Description   Possible sources of DMA parity error.
*//***************************************************************************/
typedef enum {
    DMA_PRAM,
    DMA_FIFO,
    DMA_BUS
} dma_parity_error_t;

/**************************************************************************//**
 @Description   DMA channel indexes.
*//***************************************************************************/
typedef enum {
    DMA_CH_00,
    DMA_CH_01,
    DMA_CH_02,
    DMA_CH_03,
    DMA_CH_04,
    DMA_CH_05,
    DMA_CH_06,
    DMA_CH_07,
    DMA_CH_08,
    DMA_CH_09,
    DMA_CH_10,
    DMA_CH_11,
    DMA_CH_12,
    DMA_CH_13,
    DMA_CH_14,
    DMA_CH_15,
    DMA_CHANNEL_ANY
} dma_channel_index_t;

/*****************************************************************************//**
 @Description   DMA chain maximal number of single dimensional buffers options.
*//******************************************************************************/
typedef enum {
    DMA_CHAIN_2_BUFF,
    DMA_CHAIN_4_BUFF,
    DMA_CHAIN_8_BUFF,
    DMA_CHAIN_16_BUFF,
    DMA_CHAIN_32_BUFF,
    DMA_CHAIN_64_BUFF,
    DMA_CHAIN_96_BUFF,
    DMA_CHAIN_128_BUFF,
    DMA_CHAIN_256_BUFF,
    DMA_CHAIN_512_BUFF,
    DMA_CHAIN_768_BUFF,
    DMA_CHAIN_1024_BUFF,
    DMA_CHAIN_MAX_SIZE
} dma_chain_max_num_of_transfers_t;

/**************************************************************************//**
 @Description   DMA channel optimization modes.
                In DMA_OPTIMIZE_LATENCY channel FIFO is written after 128
                bytes while in DMA_OPTIMIZE_UTILIZATION after 256 bytes.
*//***************************************************************************/
typedef enum {
    DMA_OPTIMIZE_LATENCY,
    DMA_OPTIMIZE_UTILIZATION
} dma_port_optimize_t;

/**************************************************************************//**
 @Description   Whether transfers are single or multidimensional.
*//***************************************************************************/
typedef enum {
    DMA_SINGLE_DIM,
    DMA_MULTI_DIM
} dma_dimension_level_t;


/**************************************************************************//**
 @Description   EDF modes.
                DMA_EDF_CONTINUOUS - countinue to next buffer
                DMA_EDF_RESET - Reload for next buffer.
                DMA_EDF_CYC - Mask DMA requests until count reaches 0 and then
                reload
*//***************************************************************************/
typedef enum {
    DMA_EDF_CONTINUOUS = 0x00000000,
    DMA_EDF_RESET      = 0x00008000,
    DMA_EDF_CYC        = 0x0000C000
} dma_edf_t;

/**************************************************************************//**
 @Description   DMA port priority.
                DMA port priority should be considered relative to other ports
                priorities in the system
*//***************************************************************************/
typedef enum {
    DMA_PP_LOWEST      = 0x00000000,
    DMA_PP_LOW         = 0x00001000,
    DMA_PP_HIGH        = 0x00002000,
    DMA_PP_HIGHEST     = 0x00003000
} dma_port_priority_t;

/**************************************************************************//**
 @Description   Size of DMA single access.
                DMA_XFER_SZ_BURST - is optimized for SOC internal bus
                performance.
                Its neccessary to fit access size to DDR for best performance.
*//***************************************************************************/
typedef enum {
    DMA_XFER_SZ_1_B      = 0x00000100,
    DMA_XFER_SZ_2_B      = 0x00000200,
    DMA_XFER_SZ_4_B      = 0x00000300,
    DMA_XFER_SZ_8_B      = 0x00000400,
    DMA_XFER_SZ_16_B     = 0x00000500,
    DMA_XFER_SZ_32_B     = 0x00000600,
    DMA_XFER_SZ_64_B     = 0x00000700,
    DMA_XFER_SZ_128_B    = 0x00000800,
    DMA_XFER_SZ_256_B    = 0x00000900,
    DMA_XFER_SZ_512_B    = 0x00000A00,
    DMA_XFER_SZ_BURST    = 0x00000A00,
    DMA_XFER_SZ_1024_B   = 0x00000B00
} dma_xfer_sz_t;


/**************************************************************************//**
 @Description   Size of DMA atomic accesses. BTSZ might be smaller then TSZ
                to allow more frequent arbitration
*//***************************************************************************/
typedef enum {
    DMA_XFER_BSZ_1_B      = 0x00000001,
    DMA_XFER_BSZ_2_B      = 0x00000002,
    DMA_XFER_BSZ_4_B      = 0x00000003,
    DMA_XFER_BSZ_8_B      = 0x00000004,
    DMA_XFER_BSZ_16_B     = 0x00000005,
    DMA_XFER_BSZ_32_B     = 0x00000006,
    DMA_XFER_BSZ_64_B     = 0x00000007,
    DMA_XFER_BSZ_BURST    = 0x00000007
} dma_xfer_bsz_t;


/**************************************************************************//**
 @Description   BTSZ must be less or equal to TSZ.
*//***************************************************************************/
#define DMA_TSZ_BTSZ_CHECK(TSZ, BTSZ) ((TSZ >> 8) >= BTSZ)


/**************************************************************************//**
 @Description   DMA dimension configuration.
*//***************************************************************************/
typedef struct dma_dim_config_t
{
    uint16_t count;
    /**< How many iterations of the lower dimension to perform.
     * Default -  No default.  */
    uint16_t base_count;
    /**< Reload to count.
     * Default -  If 0 - count.  */
    int      offset;
    /**< How many bytes to jump after each iteration.
     * Default -  No default. */
} dma_dim_config_t;

/**************************************************************************//**
 @Description   DMA buffer configuration
*//***************************************************************************/
typedef struct soc_dma_buffer_md_config_t
{
    dma_buff_dim_t        buffer_dim;
    /**< Possible values for buffer_dim, interrupt_dim, cont_dim, mask_requests_dim are:
     * DMA_BUFF_[ATTR]_DIM_[1..4]. There is no default, and the effect is only if [ATTR] is active. */
    dma_buff_int_dim_t    interrupt_dim;
    /**< Possible values for buffer_dim, interrupt_dim, cont_dim, mask_requests_dim are:
     * DMA_BUFF_[ATTR]_DIM_[1..4]. There is no default, and the effect is only if [ATTR] is active. */
    dma_buff_frz_dim_t    freeze_dim;
    /**< Possible values for buffer_dim, interrupt_dim, cont_dim, mask_requests_dim are:
     * DMA_BUFF_[ATTR]_DIM_[1..4]. There is no default, and the effect is only if [ATTR] is active. */
    dma_buff_cont_dim_t   cont_dim;
    /**< Possible values for buffer_dim, interrupt_dim, cont_dim, mask_requests_dim are:
     * DMA_BUFF_[ATTR]_DIM_[1..4]. There is no default, and the effect is only if [ATTR] is active. */
    dma_buff_mr_dim_t     mask_requests_dim;
    /**< Possible values for buffer_dim, interrupt_dim, cont_dim, mask_requests_dim are:
     * DMA_BUFF_[ATTR]_DIM_[1..4]. There is no default, and the effect is only if [ATTR] is active. */
    dma_dim_config_t  high_dim[3];
    /**< Count and offset of higher (2nd, 3rd and 4th) dimensions.
     * Default -  No default. */
    uint32_t            last;
    /**< This is useful when using CONTD so it is not necessarily at the end.
     * Possible values: DMA_MD_BD_LAST or 0. Default -  0. */
} soc_dma_buffer_md_config_t;

/**************************************************************************//**
 @Description   SOC specific Attributes of DMA transfers and buffers.
*//***************************************************************************/
typedef struct soc_dma_attributes_config_t
{
    uint32_t             base_size;
    /**< Size to be reloaded if size reaches zero.
     * Default -  Equal to size field from wrapper structure. */
    uint32_t             cyclic;
    /**< If TRUE reinitialize address to the original one.
     * Possible values: DMA_BUFF_CYC or 0. Default -  DMA_BUFF_CYC. */
    uint32_t             not_increment;
    /**< Keep accessing the same address.
     * Possible values: DMA_BUFF_NO_INC or 0. Default -  0. */
    dma_edf_t            edf_mode;
    /**< How to use EDF.
     *  Default -  DMA_EDF_CONTINUOUS. */
    dma_port_priority_t  port_priority;
    /**< DMA port priority in the system.
     * Default -  DMA_PP_LOWEST. */
    dma_xfer_sz_t        dma_xfer_sz;
    /**< Size of a single DMA access.
     *  Default -  DMA_XFER_SZ_BURST. */
    dma_xfer_bsz_t        dma_xfer_bsz;
    /**< Size of a atomic DMA access.
     * Default -  DMA_XFER_BSZ_64. */
    uint32_t              freeze;
    /**< Freeze the channel when size reaches zero.
     * Possible values: DMA_BUFF_FRZ or 0. Default -  0. */
    uint32_t               mask_requests;
    /**< Mask DMA requests when size reaches zero.
     * Possible values: DMA_BUFF_MASK_REQUESTS or 0. Default -  FALSE. */
    uint32_t               dont_update;
    /**< Dont write back DMA BD.
     * Makes the DMA BD more reusable. */
} soc_dma_attributes_config_t;


/**************************************************************************//**
 @Description   SOC specific Configuration parameters of a DMA channel.
*//***************************************************************************/
typedef struct soc_dma_channel_config_t
{
    dma_port_optimize_t destination_optimize;
    /**< Optimization mode for DMA channel destination.
     * Default -  DMA_OPTIMIZE_LATENCY. */
    dma_channel_index_t dma_channel_index;
    /**< Required hardware index of DMA channel.
     * Default - DMA_CHANNEL_ANY. */
    os_hwi_priority       irq_priority;
    /**< Interrupt input Line of PIC for threshold interrupts */
} soc_dma_channel_config_t;


/**************************************************************************//**
 @Description   SOC specific Configuration parameters of a DMA chain.
                If the chain is not pure single dimensional, it can be populated
                only with buffers and not with transfers.
*//***************************************************************************/
typedef struct soc_dma_chain_config_t
{
    dma_dimension_level_t source_dimension_level;
    /**< Is source single or multi dimensional.
     * Default -  DMA_SINGLE_DIM. */
    dma_dimension_level_t destination_dimension_level;
    /**< Is destination single or multi dimensional.
     * Default -  DMA_SINGLE_DIM. */
    bool                  edf_error_interrupt_enable;
    /**< Mask EDF violation interrupts.
     * Default -  TRUE. */
    uint8_t               edf_base_counter;
    /**< EDF base counter.
     * Default -  0xFE. */
    uint8_t               edf_threshold;
    /**< EDF threshold.
     * Default -  0x0. */
} soc_dma_chain_config_t;

/**************************************************************************//**
 @Description   SOC specific Configuration parameters of a DMA transfer.
*//***************************************************************************/
typedef struct soc_dma_transfer_config_t
{
    soc_dma_attributes_config_t xfer_source;
    /**< Transfer source buffer.
     * Default -  See above. */
    soc_dma_attributes_config_t xfer_destination;
    /**< Transfer destination buffer.
     * Default -  See above. */
} soc_dma_transfer_config_t;

/**************************************************************************//**
 @Description   SOC specific Configuration parameters of a DMA buffer.
*//***************************************************************************/
typedef struct soc_dma_buffer_config_t
{
    uint32_t                         address;
    /**< Buffer address.
     * Default -  No default. */
    dma_port_t                      dma_port;
    /**< From which port to access the address.
     * Default -  No default. */
    bool                            interrupt;
    /**< Generate end of buffer interrupt
     * Default -  No default. */
    uint32_t                        size;
    /**< Size of buffer.
     * Default -  No default. */
    soc_dma_attributes_config_t  *arch;
    /**< SOC specific parameter.
     * Default -  See above. */
    soc_dma_buffer_md_config_t   md;
    /**< Multidimensional parameter.
     * Default -  See above. Only considered with multidimensional chains */
} soc_dma_buffer_config_t;


/**************************************************************************//**
 @Description   SOC transfer handle.
                Allocated by the user and used by reference in
                osDmaChainTransferAddEx
*//***************************************************************************/
typedef struct soc_dma_transfer_t
{
    void   *dma_source_buffer;
    void   *dma_destination_buffer;
} soc_dma_transfer_t;

/**************************************************************************//**
 @Description   SOC buffer handle.
                Allocated by the user and used by reference in
                dmaChainBufferAddEx
*//***************************************************************************/
typedef struct soc_dma_buffer_t
{
    void   *dma_buffer;
} soc_dma_buffer_t;

/**************************************************************************//**
 @Function      dmaChannelPrioritySet

 @Description   Sets DMA channel RR group. This function is useful only
                after channel is bound otherwise it is overwritten in binding.

 @Param         dma_channel - DMA channel handle.
 @Param         priority    - DMA channel RR group.
*//***************************************************************************/
void     dmaChannelPrioritySet(void *dma_channel, uint32_t priority);

/**************************************************************************//**
 @Function      dmaChannelFreeze

 @Description   Freezes a running DMA channel, so it stays active and can be
                defrosted.

 @Param         dma_channel - DMA channel handle.
*//***************************************************************************/
void     dmaChannelFreeze(void *dma_channel);

/**************************************************************************//**
 @Function      dmaChannelFrozenStatus

 @Description   Used to poll the channel to see when it is frozen.

 @Param         dma_channel - DMA channel handle.

 @Retval        NO_FREEZE  if the channel is not frozen.
 @Retval        SRC_FROZEN if source is frozen.
 @Retval        DST_FROZEN if destination is frozen.
 @Retval        CH_FROZEN  if the channel is fully frozen.
*//***************************************************************************/
dma_freeze_status_t  dmaChannelFrozenStatus(void *dma_channel);

/**************************************************************************//**
 @Function      dmaChannelIDefrost

 @Description   Defrosts a frozen DMA channel.

 @Param         dma_channel - DMA channel handle.
*//***************************************************************************/
void     dmaChannelDefrost(void *dma_channel);

/**************************************************************************//**
 @Function      dmaChannelIDisable

 @Description   Deactivates a DMA channel.

 @Param         dma_channel - DMA channel handle.
*//***************************************************************************/
void     dmaChannelDisable(void *dma_channel);

/**************************************************************************//**
 @Function      dmaChannelIEdfBaseTimeSet

 @Description   Sets DMA channel EDF base time. This function is useful only
                after channel is bound otherwise it is overwritten in binding.

 @Param         dma_channel - DMA channel handle.
 @Param         base_time   - New EDF base time.
*//***************************************************************************/
void     dmaChannelEdfBaseTimeSet(void *dma_channel, uint8_t base_time);

/**************************************************************************//**
 @Function      dmaChannelIEdfThresholdSet

 @Description   Sets DMA channel EDF threshold. This function is useful only
                after channel is bound otherwise it is overwritten in binding.

 @Param         dma_channel - DMA channel handle.
 @Param         threshold   - New EDF threshold.
*//***************************************************************************/
void     dmaChannelEdfThresholdSet(void *dma_channel, uint8_t threshold);

/**************************************************************************//**
 @Function      dmaChainLoop

 @Description   Determine loop mode of a chain direction.

 @Param         dma_chain  - A handle to a DMA chain.
 @Param         loop       - whether to loop.
 @Param         mode       - DMA_READ or DMA_WRITE.
*//***************************************************************************/
void   dmaChainLoop(void *dma_chain, bool loop, int mode);

/**************************************************************************//**
 @Function      dmaChainBufferAdd

 @Description   Adds a transfer to a DMA chain.

 @Param         dma_chain                - A handle to a DMA chain.
 @Param         soc_dma_buffer_config - DMA buffer configuration parameters.
 @Param         mode                     - DMA_READ or DMA_WRITE.

 @Retval        OS_SUCCESS     - On success.
 @Retval        OS_FAIL        - On failure.
*//***************************************************************************/
os_status   dmaChainBufferAdd(void *dma_chain,
                                soc_dma_buffer_config_t *soc_dma_buffer_config,
                                int mode);

/**************************************************************************//**
 @Function      dmaChainBufferAddEx

 @Description   Adds a buffer to a DMA chain and get a handle to it.

 @Param         dma_chain           - A handle to a DMA chain.
 @Param         dma_buffer_config   - DMA buffer configuration parameters.
 @Param         mode                - DMA_READ or DMA_WRITE.
 @Param         buff                - Buffer handle.
 
 @Retval        OS_SUCCESS     - On success.
 @Retval        OS_FAIL        - On failure.
*//***************************************************************************/
os_status   dmaChainBufferAddEx(void *dma_chain,
                                    soc_dma_buffer_config_t *dma_buffer_config,
                                    int mode,
                                    soc_dma_buff *buff);

/**************************************************************************//**
 @Function      dmaBufferModify

 @Description   Modifies an existing buffer.
                Maintins BD_ATTR[CONT], BD_ATTR[NPRT] and BD_ATTR[NBD]

 @Param         md                  - Single/Multi dimensional buffer.
 @Param         dma_buffer_config   - DMA buffer configuration parameters.
 @Param         buff                - Buffer handle returned by dmaChainBufferAddEx.
 
 @Cautions      Setting interrupts on source buffers is not detected yet
                not supported by the hardware.
 
 @Return        OS status.

*//***************************************************************************/
os_status   dmaBufferModify(dma_dimension_level_t md,
                            soc_dma_buffer_config_t *dma_buffer_config,
                            soc_dma_buff buff);

/**************************************************************************//**
 @Function      dmaBufferAddressSet

 @Description   Modify a buffer address.

 @Param[in]     buff           - A handle to a DMA buffer that was recieved
                                 from dmaChainBufferAddEx().
 @Param[in]     address -  New address.
*//***************************************************************************/
void   dmaBufferAddressSet(uint32_t buff, uint32_t address);

/**************************************************************************//**
 @Function      dmaBufferSizeSet

 @Description   Modify a buffer size.

 @Param[in]     buff           - A handle to a DMA buffer that was recieved
                                 from dmaChainBufferAddEx().
 @Param[in]     size -  New size.
*//***************************************************************************/
void   dmaBufferSizeSet(uint32_t buff, uint32_t size);


/**************************************************************************//**
 @Function      dmaDreqSet

 @Description   Set one of the DREQ pins to control a DMA channel.

 @Param[in]    dma_channel -  DMA channel handle.
 @Param[in]    dir -  Direction to control via DREQ.
 @Param[in]    dreq_num -  Number of DREQ pin (0/1).
 
 @Cautions      Function doesn't clear GPIO configurations as they will (presumably) be
                reconfigured as necessary by other functions
*//***************************************************************************/
void   dmaDreqSet(void *dma_channel, dma_dreq_dir_t dir, unsigned int dreq_num);

/**************************************************************************//**
 @Function      dmaDreqClear

 @Description   Clear the current DREQ association.

 @Param[in]     dreq_num -  Number of DREQ pin.
*//***************************************************************************/
void   dmaDreqClear(unsigned int dreq_num);


/**************************************************************************//**
 @Function      dmaMdBufferSizeSet

 @Description   Modify a multi-dimentional buffer size.

 @Param[in]     buff           - A handle to a DMA Multi-dimentional buffer that was recieved
                                 from dmaChainBufferAddEx().
 @Param[in]     size -  New size.
*//***************************************************************************/
void   dmaMdBufferSizeSet(uint32_t buff, uint16_t size);


/**************************************************************************//**
 @Function      dmaChannelInterruptMask

 @Description   Mask interrupts from a given channel

 @Param[in]     dma_channel -  DMA channel handle.
*//***************************************************************************/
void   dmaChannelInterruptMask(void *dma_channel);

/**************************************************************************//**
 @Function      dmaChannelInterruptUnmask

 @Description   Unmask interrupts from a given channel

 @Param[in]     dma_channel -  DMA channel handle.
*//***************************************************************************/
void   dmaChannelInterruptUnmask(void *dma_channel);


/**************************************************************************//**
 @Function     dmaChannelStatus

 @Description  Return the status of a given channel

 @Param[in]    dma_channel -  DMA channel handle.

 @Return       DMA channel status.
*//***************************************************************************/
dma_ch_status_t dmaChannelStatus(void *dma_channel);

/** @} */ // end of dma_runtime_id
/** @} */ // end of dma_common_id
/** @} */ // end of dma_id

_OS_END_EXTERN_C

#endif // __DMA_H

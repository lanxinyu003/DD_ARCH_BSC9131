/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/05/15 10:23:36 $
 $Id: aic_init.c,v 1.54 2014/05/15 10:23:36 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/aic/aic_init.c,v $
 $Revision: 1.54 $
******************************************************************************/

/******************************************************************************

 @File          aic_init.c

 @Description   This file contains the implementation of the AIC
                driver, functions that are used only in initialization.

 @Cautions      None.

*//***************************************************************************/


#include "smartdsp_os_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_init.h"

#include "os_sio_lld_.h"
#include "os_sio_.h"
#include "starcore_utils_.h"

#ifdef PSC9131
#include "psc9131_aic.h"
#include "psc9131_aic_.h"
#elif defined(PSC9132)
#include "psc9132_aic.h"
#include "psc9132_aic_.h"
#endif


#include "aic_memmap.h"
#include "aic_init.h"
#include "aic.h"

#include "aic_.h"

/* Placement in BSS spares us from the necessity to zero locks */
static aic_t                * volatile aic_blocks[AIC_NUM_OF_BLOCKS] __attribute__((section(".os_shared_data_bss")));
static aic_adi_lane_t       * volatile aic_adi_lanes_array[AIC_NUM_OF_BLOCKS][AIC_ADI_MAX_NUM_OF_LANES]   __attribute__((section(".os_shared_data_bss")));
aic_sniff_t                 * volatile aic_sniffers[AIC_NUM_OF_SNIFFERS] __attribute__((section(".os_shared_data_bss")));

/*****************************************************************************/

static os_status aicOpen (void *device_handle, void *lld_params)
{
    aic_t               *aic                = (aic_t *) device_handle;
    aic_open_params_t   *aic_open_params     = (aic_open_params_t *) lld_params;

    if (lld_params == NULL)
    {
        return OS_SUCCESS;
    }

    else
    {
        RETURN_ERROR(OS_ERROR(OS_ERR_PARAM_INVALID, OS_ERRMODULE_AIC));
    }
}

/*****************************************************************************/

static os_status aicAdiLaneInterruptsAssign(aic_adi_lane_t *aic_adi_lane, aic_adi_lane_open_params_t *aic_adi_lane_open_params)
{
    int                             i;
    os_status                       status = OS_SUCCESS;
    dma_configuration_registers_t   *dma_configuration_registers;
    framer_timer_control_registers_t *framer_timer_control_registers;
    sniff_block_registers_t         *sniff_block_registers;
    aic_sniff_t                     *aic_sniff;
#if (OS_MULTICORE == ON)
    uint32_t                        ier_reg;
#endif

#ifndef DISABLE_ALL_ASSERTS // assert that for each multiplexed interrupt (ie physical interrupt line) that user chooses same priority.
    static  int                     error_int_priority = -1, aic_int_priority = -1;
#endif //DISABLE_ALL_ASSERTS

 
    OS_ASSERT_COND(aic_adi_lane != NULL);
    OS_ASSERT_COND(aic_adi_lane_open_params != NULL);
 
    dma_configuration_registers =
                    &(aic_adi_lane->aic->aic_memory_map_registers_base->adi_lanes[aic_adi_lane->aic_adi_lane_num].dma_configuration_registers);
    framer_timer_control_registers  =
            &(aic_adi_lane->aic->aic_memory_map_registers_base->adi_lanes[aic_adi_lane->aic_adi_lane_num].framer_timer_control_registers);
 
    for (i = 0; aic_adi_lane_open_params->interrupts_table[i].interrupt_line_num != AIC_LANE_INT_LAST; ++i)
    {
        switch (aic_adi_lane_open_params->interrupts_table[i].interrupt_line_num)
        {
            case AIC_LANE_INT_ERROR:
#if (OS_MULTICORE == ON)
                aic_adi_lane->error_parallel_lock.lock[AIC_LOCK_INIT_VALUE].core[osGetCoreID()] = 1;
#endif // OS_MULTICORE
#ifndef DISABLE_ALL_ASSERTS
                if (error_int_priority == -1)
                {
                    error_int_priority = aic_adi_lane_open_params->interrupts_table[i].interrupt_priority;
                }
                else
                {
                    OS_ASSERT_COND(error_int_priority == aic_adi_lane_open_params->interrupts_table[i].interrupt_priority);
                }
#endif //DISABLE_ALL_ASSERTS
                status = osHwiMultiplexedCreate(OS_INT_AIC_ERROR, aic_adi_lane_open_params->interrupts_table[i].interrupt_priority,
                        LEVEL_MODE, aicAdiErrorInterruptIsr, (os_hwi_arg)aic_adi_lane);
                aic_adi_lane->registered_interrupts[osGetCoreID()][i].hwi_num   = OS_INT_AIC_ERROR;
                aic_adi_lane->registered_interrupts[osGetCoreID()][i].isr       = aicAdiErrorInterruptIsr;  // It's a multiplexed interrupt, that's why the ISR is required to identify it when we'll remove it.
                OS_ASSERT_COND(status == OS_SUCCESS);
                OS_RETURN_NO_SUCCESS(status);
                osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
                aic_adi_lane->error_callback = aic_adi_lane_open_params->error_callback;
                SET_UINT32(dma_configuration_registers->ier, AIC_RX_DMA_OVERRUN | AIC_TX_DMA_UNDERRUN);
                SET_UINT32(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.jesd_cntl_status[aic_adi_lane->aic_adi_lane_num].cntl, AIC_JESD_TIMEOUT_ENABLE);
                osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
                break;
 
            case AIC_LANE_INT_RX_TX:
#if (OS_MULTICORE == ON)
                READ_UINT32(ier_reg, dma_configuration_registers->ier);
                if (ier_reg & (AIC_RX_FIRST_THRESHOLD | AIC_RX_SECOND_THRESHOLD))
                {
                    aic_adi_lane->rx_threshold_parallel_lock.lock[AIC_LOCK_INIT_VALUE].core[osGetCoreID()] = 1;
                }
                if (ier_reg & (AIC_TX_FIRST_THRESHOLD | AIC_TX_SECOND_THRESHOLD))
                {
                    aic_adi_lane->tx_threshold_parallel_lock.lock[AIC_LOCK_INIT_VALUE].core[osGetCoreID()] = 1;
                }
#endif // OS_MULTICORE
#ifndef DISABLE_ALL_ASSERTS
                if (aic_int_priority == -1)
                {
                    aic_int_priority = aic_adi_lane_open_params->interrupts_table[i].interrupt_priority;
                }
                else
                {
                    OS_ASSERT_COND(aic_int_priority == aic_adi_lane_open_params->interrupts_table[i].interrupt_priority);
                }
#endif //DISABLE_ALL_ASSERTS
                status = osHwiMultiplexedCreate(OS_INT_AIC, aic_adi_lane_open_params->interrupts_table[i].interrupt_priority,
                        LEVEL_MODE, aicAdiInterruptHandleRoute, (os_hwi_arg)aic_adi_lane);
                aic_adi_lane->registered_interrupts[osGetCoreID()][i].hwi_num   = OS_INT_AIC;
                aic_adi_lane->registered_interrupts[osGetCoreID()][i].isr       = aicAdiInterruptHandleRoute;   // It's a multiplexed interrupt, that's why the ISR is required to identify it when we'll remove it.
                OS_ASSERT_COND(status == OS_SUCCESS);
                OS_RETURN_NO_SUCCESS(status);
                if (aic_adi_lane_open_params->aic_sniff_open_params != NULL)
                {
                    osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
                    SET_UINT32(dma_configuration_registers->ier, 0X00000001);
                    osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
                }
                break;
 
            case AIC_LANE_TTI_UL:
#if (OS_MULTICORE == ON)
                aic_adi_lane->tti_ul_parallel_lock.lock[AIC_LOCK_INIT_VALUE].core[osGetCoreID()] = 1;
#endif // OS_MULTICORE
                aic_adi_lane->tti_ul = aic_adi_lane_open_params->tti_ul;
                status = osHwiCreate(OS_INT_AIC_TTI_UL0 + (uint16_t)aic_adi_lane->aic_adi_lane_num, aic_adi_lane_open_params->interrupts_table[i].interrupt_priority,
                        LEVEL_MODE, aicTtiUlIsr, (os_hwi_arg)aic_adi_lane);
                OS_ASSERT_COND(status == OS_SUCCESS);
                OS_RETURN_NO_SUCCESS(status);
                aic_adi_lane->registered_interrupts[osGetCoreID()][i].hwi_num = OS_INT_AIC_TTI_UL0 + (uint16_t)aic_adi_lane->aic_adi_lane_num;
                SET_UINT32(framer_timer_control_registers->rftimer_intr_ctrl_reg, AIC_TTI_UL_DELAY);
                break;
 
            case AIC_LANE_TTI_DL:
#if (OS_MULTICORE == ON)
                aic_adi_lane->tti_dl_parallel_lock.lock[AIC_LOCK_INIT_VALUE].core[osGetCoreID()] = 1;
#endif // OS_MULTICORE
                OS_ASSERT_COND(aic_adi_lane->aic_adi_lane_num == AIC_ADI_LANE1);    //There is a TTI DL interrupt only for the 1st lane
                aic_adi_lane->tti_dl = aic_adi_lane_open_params->tti_dl;
                status = osHwiCreate(OS_INT_AIC_TTI_DL0 + (uint16_t)aic_adi_lane->aic_adi_lane_num, aic_adi_lane_open_params->interrupts_table[i].interrupt_priority,
                        LEVEL_MODE, aicTtiDlIsr, (os_hwi_arg)aic_adi_lane);
                OS_ASSERT_COND(status == OS_SUCCESS);
                OS_RETURN_NO_SUCCESS(status);
                aic_adi_lane->registered_interrupts[osGetCoreID()][i].hwi_num = OS_INT_AIC_TTI_DL0 + (uint16_t)aic_adi_lane->aic_adi_lane_num;
                SET_UINT32(framer_timer_control_registers->rftimer_intr_ctrl_reg, AIC_TTI_DL_DELAY);
                break;
 
            case AIC_LANE_INT_UL_OFF:
#ifndef DISABLE_ALL_ASSERTS
#if (OS_MULTICORE == ON)
                osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
                OS_ASSERT_COND((aic_adi_lane->flags & AIC_LANE_TTI_UL_OWNED) == 0); // Another core is registered to this event, we don't see any use of multiple cores responding to this event.
                aic_adi_lane->flags |= AIC_LANE_TTI_UL_OWNED;
                osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
#endif // OS_MULTICORE
#endif //DISABLE_ALL_ASSERTS
                aic_adi_lane->tdd_ul_off = aic_adi_lane_open_params->tdd_ul_off;
                status = aicArchLteTddUlConfigure(aic_adi_lane, aic_adi_lane_open_params->interrupts_table, (uint8_t)i);
                OS_ASSERT_COND(status == OS_SUCCESS);
                OS_RETURN_NO_SUCCESS(status);
                break;
 
            case AIC_LANE_INT_DL_OFF:
#ifndef DISABLE_ALL_ASSERTS
#if (OS_MULTICORE == ON)
                osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
                OS_ASSERT_COND((aic_adi_lane->flags & AIC_LANE_TTI_DL_OWNED) == 0); // Another core is registered to this event, we don't see any use of multiple cores responding to this event.
                aic_adi_lane->flags |= AIC_LANE_TTI_DL_OWNED;
                osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
#endif // OS_MULTICORE
#endif //DISABLE_ALL_ASSERTS
                aic_adi_lane->tdd_dl_off = aic_adi_lane_open_params->tdd_dl_off;
                status = aicArchLteTddDlConfigure(aic_adi_lane, aic_adi_lane_open_params->interrupts_table, (uint8_t)i);
                OS_ASSERT_COND(status == OS_SUCCESS);
                OS_RETURN_NO_SUCCESS(status);
                break;
 
            default:
                OS_ASSERT;
                RETURN_ERROR(OS_ERROR(OS_ERR_PARAM_INVALID, OS_ERRMODULE_AIC));
        }
    }
#if (OS_MULTICORE == ON) //We strongly assume the application opens the lane on all using cores prior to enabling the lane
    osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
    aic_adi_lane->rx_threshold_parallel_lock.lock[AIC_LOCK].all_cores |= aic_adi_lane->rx_threshold_parallel_lock.lock[AIC_LOCK_INIT_VALUE].all_cores;
    aic_adi_lane->tx_threshold_parallel_lock.lock[AIC_LOCK].all_cores |= aic_adi_lane->tx_threshold_parallel_lock.lock[AIC_LOCK_INIT_VALUE].all_cores;
    aic_adi_lane->error_parallel_lock.lock[AIC_LOCK].all_cores |= aic_adi_lane->error_parallel_lock.lock[AIC_LOCK_INIT_VALUE].all_cores;
    aic_adi_lane->tti_ul_parallel_lock.lock[AIC_LOCK].all_cores |= aic_adi_lane->tti_ul_parallel_lock.lock[AIC_LOCK_INIT_VALUE].all_cores;
    aic_adi_lane->tti_dl_parallel_lock.lock[AIC_LOCK].all_cores |= aic_adi_lane->tti_dl_parallel_lock.lock[AIC_LOCK_INIT_VALUE].all_cores;
    osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
#endif // OS_MULTICORE
 
    if (aic_adi_lane_open_params->aic_sniff_open_params == NULL)
    {
        return status;
    }
 
    if (aic_adi_lane->aic_sniff_id == AIC_SNIFF0)
    {
        sniff_block_registers = &(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.sniff_registers.sniff0_registers);
    }
    else
    {
        sniff_block_registers = &(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.sniff_registers.sniff1_registers);
    }
    aic_sniff = aic_sniffers[(uint8_t)aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_id];
 
    osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
    if (!(aic_adi_lane->flags & AIC_LANE_SNIFF_INITIALIZED))
    {
        for (i = 0; aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_interrupt_table[i].interrupt_line_num != AIC_SNIFF_INT_LAST; ++i)
        {
            switch (aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_interrupt_table[i].interrupt_line_num)
            {
                case AIC_INT_SNIFF_FRAME:
                    OS_ASSERT_COND((sniff_block_registers->frmint % (aic_adi_lane_open_params->aic_sniff_open_params->rollover + 1)) == 0);
                    aic_sniff->sniff_frame_int = aic_adi_lane_open_params->aic_sniff_open_params->sniff_frame_int;
                    WRITE_UINT32(sniff_block_registers->frmint, aic_adi_lane_open_params->aic_sniff_open_params->sniff_frame_int_interval);
                    CLEAR_UINT32(sniff_block_registers->int_stat, AIC_SNIFF_FRAME_INT_EN);
                    status = osHwiCreate(OS_INT_AIC_SNIFF_FRAME0 + (uint16_t)aic_adi_lane->aic_sniff_id, aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_interrupt_table[i].interrupt_priority,
                            LEVEL_MODE, (aic_adi_lane->aic_sniff_id == AIC_SNIFF0) ? aicSniff0FrameIsr : aicSniff1FrameIsr, (os_hwi_arg)aic_sniff);
                    OS_ASSERT_COND(status == OS_SUCCESS);
                    OS_RETURN_NO_SUCCESS(status);
                    aic_sniff->registered_interrupts_num[osGetCoreID()][i] = OS_INT_AIC_SNIFF_FRAME0 + (uint16_t)aic_adi_lane->aic_sniff_id;
                    SET_UINT32(sniff_block_registers->inten, AIC_SNIFF_FRAME_INT_EN);
                    break;
                case AIC_INT_SNIFF_CAPTURE_COMPLETE:
                    aic_sniff->sniff_capture_complete = aic_adi_lane_open_params->aic_sniff_open_params->sniff_capture_complete;
                    status = osHwiCreate(OS_INT_AIC_SNIFF_CAPT_COMP0 + (uint16_t)aic_adi_lane->aic_sniff_id, aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_interrupt_table[i].interrupt_priority,
                            LEVEL_MODE, (aic_adi_lane->aic_sniff_id == AIC_SNIFF0) ? aicSniff0CaptureCompleteIsr : aicSniff1CaptureCompleteIsr, (os_hwi_arg)aic_sniff);
                    OS_ASSERT_COND(status == OS_SUCCESS);
                    OS_RETURN_NO_SUCCESS(status);
                    aic_sniff->registered_interrupts_num[osGetCoreID()][i] = OS_INT_AIC_SNIFF_CAPT_COMP0 + (uint16_t)aic_adi_lane->aic_sniff_id;
                    SET_UINT32(sniff_block_registers->inten, AIC_SNIFF_CAPT_COMPLETE_INT_EN);
                    break;
                default:
                    OS_ASSERT;
                    RETURN_ERROR(OS_ERROR(OS_ERR_PARAM_INVALID, OS_ERRMODULE_AIC));
            }
        }
        aic_adi_lane->flags |= AIC_LANE_SNIFF_INITIALIZED;
    }
    osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
 
    return status;
}

/*****************************************************************************/

os_status aicAdiLaneOpen(void *aic_adi_handle, void *open_params)
{
    os_status                   status;
    aic_adi_lane_t              *aic_adi_lane               = (aic_adi_lane_t *)(aic_adi_handle);
    aic_adi_lane_open_params_t  *aic_adi_lane_open_params   = (aic_adi_lane_open_params_t*)open_params;
    aic_registers_type1_t       *adi_lane_registers;
    sniff_block_registers_t     *sniff_block_registers;
    int                         i;
    aic_t                       *aic                        = (aic_t *)((sio_interface_t *)(aic_adi_lane_open_params->aic_adi_handle))->device_handle;
    aic_sniff_t                 *aic_sniff;
    uint32_t                    ier_reg = 0;

    OS_ASSERT_COND(aic_adi_handle != NULL);
    OS_ASSERT_COND(open_params != NULL);

    osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
    if (!(aic_adi_lane->flags & AIC_LANE_INITIALIZED))
    {
        /* Assert input parameters */
        OS_ASSERT_COND(aic_adi_lane_open_params->rx_buffer_size < 0X01000000);
        OS_ASSERT_COND(aic_adi_lane_open_params->tx_buffer_size < 0X00010000);
 
 
        OS_ASSERT_COND(((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) && (aic_adi_lane_open_params->aic_sniff_open_params == NULL)) ||
                ((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) == 0));
 
        OS_ASSERT_COND(((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) == 0) ||
                (aic_adi_lane_open_params->rx_buffer_size % aic_adi_lane_open_params->maximum_symbol_size) == 0); // Must be multiple of MSS if DBM
        OS_ASSERT_COND(((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) == 0) ||
                (aic_adi_lane_open_params->tx_buffer_size % aic_adi_lane_open_params->maximum_symbol_size) == 0); // Must be multiple of MSS if DBM
        OS_ASSERT_COND(((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) == 0) ||
                (aic_adi_lane_open_params->rx_maple_buffer_size % aic_adi_lane_open_params->maximum_symbol_size) == 0); // Must be multiple of MSS if DBM
        OS_ASSERT_COND(((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) == 0) || (aic_adi_lane_open_params->maximum_symbol_size & 0x0000000F) == 0);
        OS_ASSERT_COND((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) ||
                (aic_adi_lane_open_params->rx_buffer_size % (64 << (uint8_t)aic_adi_lane_open_params->rx_mbus_transaction_size) == 0)); // Must be multiple of transaction size if Stream Mode
        OS_ASSERT_COND((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) ||
                (aic_adi_lane_open_params->tx_buffer_size % (64 << (uint8_t)aic_adi_lane_open_params->tx_mbus_transaction_size) == 0)); // Must be multiple of transaction size if Stream Mode
        OS_ASSERT_COND((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) ||
                (aic_adi_lane_open_params->rx_maple_buffer_size % (64 << (uint8_t)aic_adi_lane_open_params->rx_mbus_transaction_size) == 0)); // Must be multiple of transaction size if Stream Mode
        OS_ASSERT_COND(((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) == 0) ||
                (aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_RX_SAMPLE_WIDTH_16_BITS));         // sample width must be 16 bits in DBM.

        OS_ASSERT_COND((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) ||
                (aic_adi_lane_open_params->rx_threshold % (64 << (uint8_t)aic_adi_lane_open_params->rx_mbus_transaction_size) == 0));
        OS_ASSERT_COND((aic_adi_lane_open_params->mode_select_flag & AIC_DMA_MODE_DOUBLE_BUFFER_MODE) ||
                (aic_adi_lane_open_params->tx_threshold % (64 << (uint8_t)aic_adi_lane_open_params->tx_mbus_transaction_size) == 0));
 
        for (i = 0; i < 3; ++i)
        {
            OS_ASSERT_COND( (aic_adi_lane_open_params->rx_mbus_priority_level[i] & 0XF ) == 0); // Must be 16 bytes aligned
            OS_ASSERT_COND( (aic_adi_lane_open_params->tx_mbus_priority_level[i] & 0XF ) == 0); // Must be 16 bytes aligned
        }

        aic_adi_lane->aic					= aic;

        aic_adi_lane->rx_buffer_size        = aic_adi_lane_open_params->rx_buffer_size;
        aic_adi_lane->tx_buffer_size        = aic_adi_lane_open_params->tx_buffer_size;
        aic_adi_lane->maximum_symbol_size   = aic_adi_lane_open_params->maximum_symbol_size;

        aic_adi_lane->last_channel_index    = (uint16_t)(aic_adi_lane->rx_buffer_size/aic_adi_lane->maximum_symbol_size);

        for (i = 0; i < SOC_MAX_NUM_OF_CORES; ++i)
        {
            aic_adi_lane->open_channels[i] = 0;
        }

        adi_lane_registers = &(aic->aic_memory_map_registers_base->adi_lanes[aic_adi_lane->aic_adi_lane_num]);

        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.mss, aic_adi_lane_open_params->maximum_symbol_size);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.dmsr, aic_adi_lane_open_params->mode_select_flag);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.riqbs, aic_adi_lane_open_params->rx_buffer_size);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.tiqbs, aic_adi_lane_open_params->tx_buffer_size);

        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.riqmts, aic_adi_lane_open_params->rx_mbus_transaction_size);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.tiqmts, aic_adi_lane_open_params->tx_mbus_transaction_size);

        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.riqmpl1, aic_adi_lane_open_params->rx_mbus_priority_level[0]);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.riqmpl23, (aic_adi_lane_open_params->rx_mbus_priority_level[2] << 16) | aic_adi_lane_open_params->rx_mbus_priority_level[1]);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.tiqmpl1, aic_adi_lane_open_params->tx_mbus_priority_level[0]);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.tiqmpl23, (aic_adi_lane_open_params->tx_mbus_priority_level[2] << 16) | aic_adi_lane_open_params->tx_mbus_priority_level[1]);

        if (aic_adi_lane_open_params->rx_first_threshold > 0)
        {
            WRITE_UINT32(adi_lane_registers->dma_configuration_registers.riqft, aic_adi_lane_open_params->rx_first_threshold);
            ier_reg |= AIC_RX_FIRST_THRESHOLD;
        }
        if (aic_adi_lane_open_params->rx_second_threshold > 0)
        {
            WRITE_UINT32(adi_lane_registers->dma_configuration_registers.riqst, aic_adi_lane_open_params->rx_second_threshold);
            ier_reg |= AIC_RX_SECOND_THRESHOLD;
        }
        if (aic_adi_lane_open_params->tx_first_threshold > 0)
        {
            WRITE_UINT32(adi_lane_registers->dma_configuration_registers.tiqft, aic_adi_lane_open_params->tx_first_threshold);
            ier_reg |= AIC_TX_FIRST_THRESHOLD;
        }
        if (aic_adi_lane_open_params->tx_second_threshold > 0)
        {
            WRITE_UINT32(adi_lane_registers->dma_configuration_registers.tiqst, aic_adi_lane_open_params->tx_second_threshold);
            ier_reg |= AIC_TX_SECOND_THRESHOLD;
        }
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.ier, ier_reg);

        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.rmba, aic_adi_lane_open_params->rx_maple_base_address);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.rmbs, aic_adi_lane_open_params->rx_maple_buffer_size);

        /* relevant only for Stream Mode or Sniffing */
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.riqt, aic_adi_lane_open_params->rx_threshold);
        WRITE_UINT32(adi_lane_registers->dma_configuration_registers.tiqt, aic_adi_lane_open_params->tx_threshold);

        if (aic_adi_lane_open_params->aic_sniff_open_params != NULL)
        {
            OS_ASSERT_COND(aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_id < AIC_NUM_OF_SNIFFERS);
            OS_ASSERT_COND((aic_adi_lane_open_params->aic_sniff_open_params->capt_dur % aic_adi_lane_open_params->rx_threshold) == 0);

            aic_sniff = aic_sniffers[(uint8_t)aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_id];
            if (aic_sniff == NULL)
            {
                aic_sniff = aic_sniffers[(uint8_t)aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_id] =
                        (aic_sniff_t *) osMalloc(sizeof(aic_sniff_t), AIC_DRIVER_INTERNAL_STRUCTURES_MEM_TYPE);
                if (aic_sniff == NULL)
                {
                    OS_ASSERT;
                    RETURN_ERROR(OS_ERROR(OS_ERR_NO_MEMORY, OS_ERRMODULE_AIC));
                }
                aic_sniff->aic_lane = aic_adi_lane;
            }
            OS_ASSERT_COND(aic_sniff->aic_lane == aic_adi_lane);
            aic_sniff->aic_lane = aic_adi_lane;
            aic_adi_lane->flags |= AIC_LANE_OPERATION_MODE_SNIFF;
            aic_adi_lane->aic_sniff_id = aic_adi_lane_open_params->aic_sniff_open_params->aic_sniff_id ;

            if (aic_adi_lane->aic_sniff_id == AIC_SNIFF0)
            {
                sniff_block_registers = &(aic->aic_memory_map_registers_base->aic_type2.sniff_registers.sniff0_registers);
            }
            else
            {
                sniff_block_registers = &(aic->aic_memory_map_registers_base->aic_type2.sniff_registers.sniff1_registers);
            }

            WRITE_UINT32(adi_lane_registers->dma_configuration_registers.riqba[0], aic_adi_lane_open_params->aic_sniff_open_params->buffer_address);
            WRITE_UINT32(sniff_block_registers->refcnt, aic_adi_lane_open_params->aic_sniff_open_params->rollover);
            SET_UINT32(sniff_block_registers->cntrl, AIC_SNIFF_REF_COUNTER_EN | AIC_SNIFF_PRE_CAPT_EN);
            OS_WAIT(MICROSECOND * (150/1000)); // must wait 150us before starting sniffing
            SET_UINT32(adi_lane_registers->dma_configuration_registers.dmsr, AIC_DMA_MODE_RX_FRAMER_ENABLE_SELECT_SNIFFER);
            WRITE_UINT32(sniff_block_registers->capt_off, aic_adi_lane_open_params->aic_sniff_open_params->capt_off);
            WRITE_UINT32(sniff_block_registers->capt_dur, aic_adi_lane_open_params->aic_sniff_open_params->capt_dur);
        }
        else
        {
            aic_adi_lane->flags &= ~AIC_LANE_OPERATION_MODE_SNIFF;
        }
        // Clear status bits. It's important for DSP recovery flow, as errors may remain from previous OS instance.
        WRITE_UINT32(adi_lane_registers->framer_timer_control_registers.dmaisr, 0XFFFFFFFF);
        // Disable all interrupts we support, in a case the AIC block is not in its reset state.
        CLEAR_UINT32(adi_lane_registers->dma_configuration_registers.ier, 0XFFFFFFFF);
        CLEAR_UINT32(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.jesd_cntl_status[aic_adi_lane->aic_adi_lane_num].cntl, AIC_JESD_TIMEOUT_ENABLE);
        aic_adi_lane->flags |= AIC_LANE_INITIALIZED;
    }
    osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
 
    status = aicAdiLaneInterruptsAssign(aic_adi_lane, aic_adi_lane_open_params);
    OS_RETURN_NO_SUCCESS(status);

    return OS_SUCCESS;
}

/*****************************************************************************/

static void* aicAdiLaneChannelOpen (void *aic_adi_lane_handle, int mode, sio_lld_ch_open_t *open_params)
{
    aic_adi_lane_t                  *aic_adi_lane = (aic_adi_lane_t *)aic_adi_lane_handle;
    aic_channel_t                   *aic_channel;
    os_status                       status;
    dma_configuration_registers_t   *dma_configuration_registers;
    aic_channel_open_params_t       *aic_channel_open_params;
    adi_registers_t                 *adi_registers;

    OS_ASSERT_COND(aic_adi_lane_handle != NULL);
    OS_ASSERT_COND(open_params != NULL);
    OS_ASSERT_COND(open_params->lld_params != NULL);
    OS_ASSERT_COND((mode & (SIO_WRITE | SIO_READ)) != 0);
    OS_ASSERT_COND((mode & (SIO_WRITE | SIO_READ)) != (SIO_WRITE | SIO_READ));
 
    dma_configuration_registers = &(aic_adi_lane->aic->aic_memory_map_registers_base->adi_lanes[aic_adi_lane->aic_adi_lane_num].dma_configuration_registers);
    adi_registers               = &(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers);
 
    if (!IS_ALIGNED((uint32_t)open_params->data_base, ALIGNED_16_BYTES))
    {
        OS_ASSERT;
        return NULL; /* buffer_base_address must be 16 bytes aligned */
    }

    status = osMmuDataVirtToPhys((void*) open_params->data_base, (void**) &open_params->data_base);
    if (status != OS_SUCCESS)
    {
        OS_ASSERT;
        return NULL;
    }
 
    aic_channel_open_params = (aic_channel_open_params_t *)open_params->lld_params;
 
    if (mode & SIO_WRITE) //write (transmit) channel
    {
        osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));    // to ptotect aic_adi_lane->open_channels
        if ( (open_params->buffer_size * open_params->num_of_channel_buffers) != aic_adi_lane->tx_buffer_size ||   //buffer size * buffers number should be the AIC ADI initialized buffer size
                (aic_adi_lane->open_channels[osGetCoreID()] & (1 << (AIC_ADI_MAX_NUM_OF_CHANNELS + open_params->channel_num))))   //already open channel
        {
            OS_ASSERT;
            return NULL;
        }
        osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock)); //from now on we can assume only one core executes for this channel num
 
        aic_channel = &(aic_adi_lane->aic_tx_channels[open_params->channel_num]);
        *open_params->first_driver_buffer = &aic_channel->index;
        *open_params->last_driver_buffer  = &aic_channel->index;
 
        WRITE_UINT32(dma_configuration_registers->tiqba[open_params->channel_num], (uint32_t)open_params->data_base);
 
        osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
        if (aic_channel_open_params->interrupt_mode & AIC_CHANNEL_OPEN_IRQ_CORE)
        {
            SET_UINT32(dma_configuration_registers->ier, (AIC_TX_THRESHOLD_ANT0 << open_params->channel_num)); //Enable the desired antennas' TX interrupt.
        }
 
        if (aic_channel_open_params->interrupt_mode & AIC_CHANNEL_OPEN_IRQ_MAPLE)
        {
            SET_UINT32(adi_registers->interrupt_mux_crtl_reg, AIC_DMA_MAPLE_TX_ENABLE);
            aicArchAdiLaneIrqToMapleOpen(mode, (aic_adi_channel_num_t)open_params->channel_num);
        }
        aic_adi_lane->open_channels[osGetCoreID()] |= (1 << (AIC_ADI_MAX_NUM_OF_CHANNELS + open_params->channel_num));
        osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
    }
    else // if (mode & SIO_READ) //read (receive) channel
    {
        osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));    //// to ptotect aic_adi_lane->open_channels
        if ( (open_params->buffer_size * open_params->num_of_channel_buffers) != aic_adi_lane->rx_buffer_size ||   //buffer size * buffers number should be the AIC ADI initialized buffer size
                (aic_adi_lane->open_channels[osGetCoreID()] & (1 << (open_params->channel_num))))                                  //already open channel
        {
            OS_ASSERT;
            return NULL;
        }
        osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock)); //from now on we can assume only one core executes for this channel num
 
        aic_channel = &(aic_adi_lane->aic_rx_channels[open_params->channel_num]);
        *open_params->first_driver_buffer = &aic_channel->index;
        *open_params->last_driver_buffer  = &aic_channel->index;
 
        WRITE_UINT32(dma_configuration_registers->riqba[open_params->channel_num], (uint32_t)open_params->data_base);
        osSpinLockSwiftGet(&(aic_adi_lane->configuration_lock));
        if (aic_channel_open_params->interrupt_mode & AIC_CHANNEL_OPEN_IRQ_CORE)
        {
            SET_UINT32(dma_configuration_registers->ier, (AIC_RX_THRESHOLD_ANT0 << open_params->channel_num)); //Enable the desired antennas' RX interrupt.
        }
 
        if (aic_channel_open_params->interrupt_mode & AIC_CHANNEL_OPEN_IRQ_MAPLE)
        {
            SET_UINT32(adi_registers->interrupt_mux_crtl_reg, AIC_DMA_MAPLE_RX_ENABLE);
            aicArchAdiLaneIrqToMapleOpen(mode, (aic_adi_channel_num_t)open_params->channel_num);
        }
        aic_adi_lane->open_channels[osGetCoreID()] |= (1 << (open_params->channel_num));
        osSpinLockSwiftRelease(&(aic_adi_lane->configuration_lock));
    }

    aic_channel->aic_adi_channel_num = (aic_adi_channel_num_t) open_params->channel_num;
    aic_channel->aic_lane = aic_adi_lane;
    aic_channel->index = 0;
    aic_channel->interrupt_mode = aic_channel_open_params->interrupt_mode;

    return (void*)aic_channel;
}

/*****************************************************************************/

static os_status aicAdiLaneChannelClose (void *aic_adi_lane_channel_handle, int mode)
{
    aic_channel_t *aic_channel = (aic_channel_t *)aic_adi_lane_channel_handle;
    dma_configuration_registers_t *dma_configuration_registers;
    adi_registers_t               *adi_registers;

    OS_ASSERT_COND(aic_adi_lane_channel_handle != NULL);
    OS_ASSERT_COND((mode & (SIO_WRITE | SIO_READ)) != 0);
    OS_ASSERT_COND((mode & (SIO_WRITE | SIO_READ)) != (SIO_WRITE | SIO_READ));
 
    dma_configuration_registers =
            &(aic_channel->aic_lane->aic->aic_memory_map_registers_base->adi_lanes[aic_channel->aic_lane->aic_adi_lane_num].dma_configuration_registers);
    adi_registers               = &(aic_channel->aic_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers);

    osSpinLockSwiftGet(&(aic_channel->aic_lane->configuration_lock));
    if (mode & SIO_WRITE) //write (transmit) channel
    {
        if (aic_channel->interrupt_mode & AIC_CHANNEL_OPEN_IRQ_CORE)
        {
            CLEAR_UINT32(dma_configuration_registers->ier, (AIC_TX_THRESHOLD_ANT0 << aic_channel->aic_adi_channel_num));
        }
        if (aic_channel->interrupt_mode & AIC_CHANNEL_OPEN_IRQ_MAPLE)
        {
            CLEAR_UINT32(adi_registers->interrupt_mux_crtl_reg, AIC_DMA_TX_ENABLE);
            aicArchAdiLaneIrqToMapleClose(mode, (aic_adi_channel_num_t)aic_channel->aic_adi_channel_num);
        }
 
        aic_channel->aic_lane->open_channels[osGetCoreID()] &= ~(1 << (AIC_ADI_MAX_NUM_OF_CHANNELS + aic_channel->aic_adi_channel_num));
    }
    else // if (mode & SIO_READ) //read (receive) channel
    {
        if (aic_channel->interrupt_mode & AIC_CHANNEL_OPEN_IRQ_CORE)
        {
            CLEAR_UINT32(dma_configuration_registers->ier, (AIC_RX_THRESHOLD_ANT0 << aic_channel->aic_adi_channel_num));
        }
        if (aic_channel->interrupt_mode & AIC_CHANNEL_OPEN_IRQ_MAPLE)
        {
            CLEAR_UINT32(adi_registers->interrupt_mux_crtl_reg, AIC_DMA_RX_ENABLE);
            aicArchAdiLaneIrqToMapleClose(mode, (aic_adi_channel_num_t)aic_channel->aic_adi_channel_num);
        }
 
        aic_channel->aic_lane->open_channels[osGetCoreID()] &= ~(1 << (aic_channel->aic_adi_channel_num));
    }
    osSpinLockSwiftRelease(&(aic_channel->aic_lane->configuration_lock));
 
    return OS_SUCCESS;
}

/*****************************************************************************/

static os_status aicAdiLaneCtrl(void *aic_adi_lane_handle, uint32_t command, void *params)
{
    switch (command)
    {
        case SIO_DEVICE_TX_ENABLE:
            aicTxEnable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;

        case SIO_DEVICE_RX_ENABLE:
            aicRxEnable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;

        case SIO_DEVICE_TX_DISABLE:
            aicTxDisable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;

        case SIO_DEVICE_RX_DISABLE:
            aicRxDisable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;

        case SIO_DEVICE_RX_TX_ENABLE:
            aicRxTxEnable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;
 
        case AIC_UNSAFE_TX_DISABLE_TMR_CTRL:
            aicUnsafeTxTmrCtrlDisable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;
 
        case AIC_UNSAFE_TX_DISABLE_DMA:
            aicUnsafeTxDmaDisable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;
 
        case AIC_UNSAFE_RX_DISABLE_TMR_CTRL:
            aicUnsafeRxTmrCtrlDisable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;
 
        case AIC_UNSAFE_RX_DISABLE_DMA:
            aicUnsafeRxDmaDisable((aic_lane_t *) aic_adi_lane_handle);
            return OS_SUCCESS;
 
 
        case AIC_ADI_LANE_CTRL_RECONFIGURE:
            OS_ASSERT_COND(aic_adi_lane_handle != NULL);
            return aicLaneReconfigure(aic_adi_lane_handle, params);

        case AIC_ADI_LANE_CTRL_IS_CAPTURE_COMPLETE:
            OS_ASSERT_COND(aic_adi_lane_handle != NULL);
            return aicLaneSniffCaptureCompleteFlagGet(aic_adi_lane_handle, params);

        default:
            OS_ASSERT;
            RETURN_ERROR(OS_ERROR(OS_ERR_COMMAND_UNSUPPORTED, OS_ERRMODULE_AIC));
    }
}

/*****************************************************************************/

static os_status aicAdiLaneChannelCtrl (void *aic_adi_lane_channel_handle, uint32_t command, void *params)
{
    VAR_UNUSED(aic_adi_lane_channel_handle);
    VAR_UNUSED(command);
    VAR_UNUSED(params);
    RETURN_ERROR(OS_ERROR(OS_ERR_COMMAND_UNSUPPORTED, OS_ERRMODULE_AIC));
}

/*****************************************************************************/

static os_status aicAdiOpen (void *device_handle, void *lld_params)
{
    aic_t                   *aic                    = (aic_t *) device_handle;
    aic_adi_open_params_t *aic_adi_open_params = (aic_adi_open_params_t *) lld_params;
    sio_register_params_t sio_register;
    aic_adi_lane_t           *aic_adi_lane;
 
    static char           aic_adi_name[]       = AIC_ADI_DEV_NAME1;
    int                   i;

    OS_ASSERT_COND(device_handle != NULL);
    OS_ASSERT_COND(lld_params != NULL);
 
    sio_register.device_open     = aicAdiLaneOpen;
    sio_register.channel_open     = aicAdiLaneChannelOpen;
    sio_register.channel_close    = aicAdiLaneChannelClose;
    sio_register.device_ctrl     = aicAdiLaneCtrl;
    sio_register.channel_ctrl     = aicAdiLaneChannelCtrl;

    for (i = 0; i < aic_adi_open_params->num_of_lanes; i++)
    {
        aic_adi_name[3] = (char) ('1' + (char) aic_adi_open_params->lane_nums[i]);

        osSpinLockSwiftGet(&(aic->configuration_lock));
        aic_adi_lane = aic_adi_lanes_array[aic->aic_block_id][(uint8_t) aic_adi_open_params->lane_nums[i]];
        if (aic_adi_lane == NULL)
        {
            aic_adi_lane = aic_adi_lanes_array[aic->aic_block_id][(uint8_t) aic_adi_open_params->lane_nums[i]] =
                    (aic_adi_lane_t *) osMalloc(sizeof(aic_adi_lane_t), AIC_DRIVER_INTERNAL_STRUCTURES_MEM_TYPE);
            if (aic_adi_lane == NULL)
            {
                OS_ASSERT;
                RETURN_ERROR(OS_ERROR(OS_ERR_NO_MEMORY, OS_ERRMODULE_AIC));
            }
            aic_adi_lane->aic_adi_lane_num = aic_adi_open_params->lane_nums[i];
        }
        osSpinLockSwiftRelease(&(aic->configuration_lock));
 
        sio_register.lld_handle = (void *) aic_adi_lane;
        aic_adi_lane->sio_dev[osGetCoreID()] = sioRegister(aic_adi_name, &sio_register);
        if (aic_adi_lane->sio_dev[osGetCoreID()] == NULL)
        {
            OS_ASSERT;
            RETURN_ERROR(OS_ERROR(OS_ERR_UNKNOWN, OS_ERRMODULE_AIC));
        }
    }
    return OS_SUCCESS;
}

/*****************************************************************************/

os_status aicAdiInitialize(aic_t *aic)
{
    int                     i;
    sio_register_params_t   sio_register;
    static char             aic_block_adi_name[] = AIC_ADI_DEV_NAME0;

    OS_ASSERT_COND(aic != NULL);
 
    for (i = 0; i < AIC_ADI_MAX_NUM_OF_LANES; ++i)
    {
        aic_adi_lanes_array[aic->aic_block_id][i] = NULL;
    }

    sio_register.device_open = aicAdiOpen;
    sio_register.channel_open = NULL;
    sio_register.channel_close = NULL;
    sio_register.device_ctrl = NULL;
    sio_register.channel_ctrl = NULL;

    sio_register.lld_handle = (void *) aic;

    if (sioRegister(aic_block_adi_name, &sio_register) == NULL)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERROR(OS_ERR_UNKNOWN, OS_ERRMODULE_AIC));
    }

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status aicMaximInitialize(aic_t *aic)
{
    VAR_UNUSED(aic);
    //TODO update when MAXIM is supported
    //RETURN_ERROR_MESSAGE(OS_FAIL, "module not supported");
 
    return OS_SUCCESS;
}

/*****************************************************************************/

os_status aicInitialize(aic_init_params_t *aic_init_params)
{
    sio_register_params_t   sio_register;
    aic_t                   *aic;
    static char             aic_block_name[] = AIC_DEV_NAME0;
    os_status               status;
    int                     i;

    OS_ASSERT_COND(aic_init_params != NULL);
    OS_ASSERT_COND(aic_init_params->initializing_core != 0);
    OS_ASSERT_COND(aic_init_params->aic_block_id < AIC_NUM_OF_BLOCKS);
 
    if (aic_init_params->initializing_core == (1 << osGetCoreID()))
    {
        aic_blocks[aic_init_params->aic_block_id] =
            (aic_t *) osMalloc(sizeof(aic_t), AIC_DRIVER_INTERNAL_STRUCTURES_MEM_TYPE);
    }
    else
    {
        while (aic_blocks[aic_init_params->aic_block_id] == NULL) {}
    }
 
    aic = aic_blocks[aic_init_params->aic_block_id];
 
    if (aic == NULL)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERROR(OS_ERR_NO_MEMORY, OS_ERRMODULE_AIC));
    }
 
    for (i=0; i < AIC_NUM_OF_SNIFFERS; ++i)
    {
        aic_sniffers[i] = NULL;
    }
 
    /* TODO - cores overwrite each other, perhaps a cleaner approach is available? */
    aic->aic_memory_map_registers_base  = aic_init_params->aic_memory_map_registers_base;
    aic->initializing_core              = aic_init_params->initializing_core;
    aic->aic_block_id                   = aic_init_params->aic_block_id;

    sio_register.device_open    = aicOpen;
    sio_register.channel_open   = NULL;
    sio_register.channel_close  = NULL;
    sio_register.device_ctrl    = NULL;
    sio_register.channel_ctrl   = NULL;

    sio_register.lld_handle = (void *) aic;

    aic_block_name[3] = (char) (aic_block_name[3] + (char)aic->aic_block_id);

    if (sioRegister(aic_block_name, &sio_register) == NULL)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERROR(OS_ERR_UNKNOWN, OS_ERRMODULE_AIC));
    }
 
    status = aicAdiInitialize(aic);
    OS_RETURN_NO_SUCCESS(status);
#ifdef PSC9131
    status = aicMaximInitialize(aic);
    OS_RETURN_NO_SUCCESS(status);
#endif // PSC9131
    return OS_SUCCESS;

}

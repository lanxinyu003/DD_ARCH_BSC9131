/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:01 $
 $Id: class_profiling.c,v 1.8 2012/11/22 16:28:01 sw Exp $
 $Source: /cvsdata/SmartDSP/drivers/class/rev1/class_profiling.c,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************

 @File          class_profiling.c

 @Description   CLASS debug and profiling Monitor API.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "class_profiling.h"

typedef struct
{
    class_map_t             *class_memmap;      /* CLASS structure */
    uint32_t                wp_info;            /* Info for synchronizing WPU and PU */
    uint32_t                interrupt_enable;   /* Current interrupt enable mask */
    uint32_t                timeout;            /* Current timeout value */
} class_tracking_t;

/* class_tracking_t.wp_info contains:
 * wp_info[28]   - 1 for CLASS_BEING_PROFILED_WITH_WP
 * wp_info[16]   - 1 for target, 0 for initiator watchpoint
 * wp_info[15:0] - port number
 * */

static class_tracking_t classes [] =
{
    { (class_map_t *)0xFFF18000, 0, 0, 0 }
};

#define CLASS_NUM                           0 // Only one CLASS supported

#define CLASS_INIT_TRGT_MASK                CLASS_TRGT
#define CLASS_TRGT_PROFILE_MASK             0x00000200
#define CLASS_TRGT_PROFILE_NORMALIZER       CLASS_TRGT_PROFILE_MASK
#define CPISR_OVE                           0x00000001  /**< Overflow enable in CLASS profiling IRQ/ISR registers */
#define CPISR_WPE                           0x00000002  /**< Watchpoint enable in CLASS profiling IRQ/ISR registers */
#define CLASS_BEING_PROFILED_WITH_WP        0x10000000
#define CLASS_WPU_PU_AFFECT                 0x00000300
#define CLASS_ENUMERATION_OFFSET            CLASS0_BLK_NUM
#define NUM_CLASS_COUNTERS                  4
#define TIMEOUT_ENABLE                      0x00000010
#define PROFILING_ENABLE                    0x00000001
#define WATCHPOINT_ADDR_IRRELEVANT_BITS     12
#define EOT_ATTR_OFFSET                     24
#define SRC_ID_OFFSET                       11
#define CPCR_WPEC_MASK                      0x00000300



/* Handles for future multicore support */
static inline os_status obtain_class(class_tracking_t * tracker)
{
    VAR_UNUSED(tracker);
    return OS_SUCCESS;
}

static inline void release_class(class_tracking_t * tracker)
{
    VAR_UNUSED(tracker);
    return;
}


os_status classConfigureWatchpoint(soc_class_watchpoint_param_t *config)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    uint32_t    reg;
    os_status status;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;
    uint32_t interrupt_mask = 0;

    status = obtain_class(class_tracker);
    OS_ASSERT_COND(status == OS_SUCCESS);

    class_memmap = class_tracker->class_memmap;

    WRITE_UINT32(class_memmap->class_general_registers.cwpcr, config->enable_mask);

    /* Build CWPACR */
    reg =  (uint32_t)(config->watchpoint_param);
    reg |= config->base_addr >> WATCHPOINT_ADDR_IRRELEVANT_BITS;
    WRITE_UINT32(class_memmap->class_general_registers.cwpacr, reg);

    /* Build CWPEACR */
    reg =  (uint32_t)(config->watchpoint_ext_param);
    reg |= (config->eot_attr & 0xF) << EOT_ATTR_OFFSET;
    reg |= (config->src_id & 0x1F) << SRC_ID_OFFSET;
    reg |= (config->priority);
    reg |= (config->byte_cnt & 0x1FF);
    WRITE_UINT32(class_memmap->class_general_registers.cwpeacr, reg);

    /* Build CWPAMR */
    WRITE_UINT32(class_memmap->class_general_registers.cwpamr, (uint32_t)(config->addr_mask & 0xFF));

    /* "Find" proper port */
    reg = (uint32_t)PORT_NUM(config->watchpoint_port);
    OS_ASSERT_COND(reg < 16);

    /* Prepare the CLASS tracking strcuture for enablement */
    class_tracker->wp_info = reg | (config->watchpoint_port & CLASS_INIT_TRGT_MASK);

    /* Enable interrupts */
    if (config->generate_wp_event)
        interrupt_mask |= CPISR_WPE;
    if (config->overflow_int)
        interrupt_mask |= CPISR_OVE;
    class_tracker->interrupt_enable = interrupt_mask;
    WRITE_UINT32(class_memmap->class_general_registers.cpier, interrupt_mask);

    release_class(class_tracker);
    return OS_SUCCESS;
}


os_status classEnableWatchpoint(soc_class_ports_t port)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    os_status status;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;
    bool wp_on_trgt;
    uint32_t wp_port_num;

    VAR_UNUSED(port);

    status = obtain_class(class_tracker);
    OS_ASSERT_COND(status == OS_SUCCESS);

    class_memmap = class_tracker->class_memmap;

    wp_on_trgt = (bool)((class_tracker->wp_info & CLASS_INIT_TRGT_MASK) != 0);
    wp_port_num = class_tracker->wp_info & CLASS_PORT_MASK;

    /* Enable the watchpoint */
    if (wp_on_trgt)
    {
        WRITE_SYNCIO_UINT32(class_memmap->class_general_registers.ctwpcr, (uint32_t)(1 << wp_port_num));
    }
    else
    {
        WRITE_SYNCIO_UINT32(class_memmap->ciwpcr[wp_port_num], 1);
    }

    release_class(class_tracker);
    return OS_SUCCESS;

}


os_status classDisableWatchpoint(soc_class_ports_t port)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    os_status status;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;

    VAR_UNUSED(port);

    status = obtain_class(class_tracker);
    OS_ASSERT_COND(status == OS_SUCCESS);

    class_memmap = class_tracker->class_memmap;

    if ((class_tracker->wp_info & CLASS_INIT_TRGT_MASK) == CLASS_INIT)
    {
        WRITE_UINT32(class_memmap->ciwpcr[class_tracker->wp_info & CLASS_PORT_MASK],  0);
    }
    else
    {
        WRITE_UINT32(class_memmap->class_general_registers.ctwpcr, 0);
    }

    // Disable watchpoint interrupts
    class_tracker->interrupt_enable  &= ~CPISR_WPE;
    WRITE_UINT32(class_memmap->class_general_registers.cpier, class_tracker->interrupt_enable);

    release_class(class_tracker);
    return OS_SUCCESS;
}



os_status classConfigureProfiling(soc_class_profiling_param_t *config)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    uint8_t port_num;
    os_status status;
    soc_class_ports_t port = config->profile_port;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;


    OS_ASSERT_COND(((port & CLASS_INIT_TRGT_MASK) == (config->profiling_type & CLASS_INIT_TRGT_MASK)));

    class_memmap = class_tracker->class_memmap;

    /* If there is a conenciton between the wathcpoint unit and profiling unit, configure the watcpoit unit */
    if (config->watchpoint_enable != NO_AFFECT)
    {
        os_status status;
        OS_ASSERT_COND((config->watchpoint_config->watchpoint_port & BLOCK_MASK) == (port & BLOCK_MASK));

        status = classConfigureWatchpoint(config->watchpoint_config);
        if (status != OS_SUCCESS)
        {
            release_class(class_tracker);
            return status;
        }
        class_tracker->wp_info |= CLASS_BEING_PROFILED_WITH_WP;

        /* Set CPCR[WPEC] */
        WRITE_UINT32(class_memmap->class_general_registers.cpcr, config->watchpoint_enable);
    }
    else
    {
        status = obtain_class(class_tracker);
        OS_ASSERT_COND(status == OS_SUCCESS);

        if (config->overflow_int)
            class_tracker->interrupt_enable |= CPISR_OVE;
        else
            class_tracker->interrupt_enable &= ~CPISR_OVE;
        WRITE_UINT32(class_memmap->class_general_registers.cpier,class_tracker->interrupt_enable);

    }

    /* configure the timeout period */
    class_tracker->timeout = config->time_out_period;
    WRITE_UINT32(class_memmap->class_general_registers.cptor, config->time_out_period);

    port_num = (uint8_t)PORT_NUM(config->profile_port);

    /* Configure the CLASS watchpoint unit */
    if ((config->profiling_type & CLASS_INIT_TRGT_MASK) == CLASS_INIT)
    {
        if (config->profiling_type == TARGET_BW)
        {
            WRITE_UINT32(class_memmap->cipcr[port_num], (uint32_t)(TARGET_BW | config->trgt_profiled_by_init));
        }
        else
        {
            WRITE_UINT32(class_memmap->cipcr[port_num], config->profiling_type);
        }
    }
    else
    {
        // Remove the bit
        WRITE_UINT32(class_memmap->class_general_registers.ctpcr,
             (uint32_t)((config->profiling_type & ~CLASS_TRGT) | (port_num << 8)));
    }


    release_class(class_tracker);
    return OS_SUCCESS;

}

os_status classGetProfilingResults(soc_class_ports_t port, class_profile_results_t * restrict results)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    char i;
    os_status status;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;

    VAR_UNUSED(port);

    status = obtain_class(class_tracker);
    OS_ASSERT_COND(status == OS_SUCCESS);

    class_memmap = class_tracker->class_memmap;

    results->class_ref_counter = GET_UINT32(class_memmap->class_general_registers.cprcr);

    for (i=0 ; i<NUM_CLASS_COUNTERS ; i++)
        READ_UINT32(results->class_counters[i], class_memmap->class_general_registers.cpgcr[i]);

    release_class(class_tracker);
    return OS_SUCCESS;
}


os_status classStartProfiling(soc_class_ports_t port)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    uint32_t reg = PROFILING_ENABLE;
    os_status status;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;

    VAR_UNUSED(port);

    /* If there is intervention from the watchpoint unit, this function should not be called */
    if (class_tracker->wp_info & CLASS_BEING_PROFILED_WITH_WP)
        OS_ASSERT;

    status = obtain_class(class_tracker);
    OS_ASSERT_COND(status == OS_SUCCESS);

    class_memmap = class_tracker->class_memmap;

    if (class_tracker->timeout)
        reg |= TIMEOUT_ENABLE;

    WRITE_SYNCIO_UINT32(class_memmap->class_general_registers.cpcr, reg);

    release_class(class_tracker);
    return OS_SUCCESS;

}


os_status classStopProfiling(soc_class_ports_t port)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    os_status status;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;


    VAR_UNUSED(port);

    status = obtain_class(class_tracker);
    OS_ASSERT_COND(status == OS_SUCCESS);

    /* If there is intervention from the watchpoint unit, this function should not be called */
    if (class_tracker->wp_info & CLASS_BEING_PROFILED_WITH_WP)
        OS_ASSERT;

    class_memmap = class_tracker->class_memmap;

    /* Disable the profiling unit */
    CLEAR_SYNCIO_UINT32(class_memmap->class_general_registers.cpcr, PROFILING_ENABLE);

    release_class(class_tracker);
    return OS_SUCCESS;
}


os_status classDisableProfiling(soc_class_ports_t port)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    os_status status;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;
    uint8_t port_num;

    /* MUST call classStopProfiling() before disabling CLASS profiling */
    status = classStopProfiling(port);
    OS_ASSERT_COND(status == OS_SUCCESS);

    status = obtain_class(class_tracker);
    OS_ASSERT_COND(status == OS_SUCCESS);

    class_memmap = class_tracker->class_memmap;


    /* "Erase" previous configurations in preperation for next confguration */
    port_num = (uint8_t)PORT_NUM(port);
    WRITE_UINT32(class_memmap->cipcr[port_num], 0x0);
    WRITE_UINT32(class_memmap->class_general_registers.ctpcr, 0x0);

    class_tracker->wp_info &= ~CLASS_BEING_PROFILED_WITH_WP;

    release_class(class_tracker);
    return OS_SUCCESS;
}


os_status classRestartProfiling(soc_class_ports_t port)
{
    class_tracking_t *class_tracker = &classes[CLASS_NUM];
    os_status status;
    class_map_t * restrict class_memmap = (class_map_t * )&g_dsp_ccsr_map->class0;

    VAR_UNUSED(port);

    status = obtain_class(class_tracker);
    OS_ASSERT_COND(status == OS_SUCCESS);

    class_memmap = class_tracker->class_memmap;

    // Clear interrupt from status register
    WRITE_UINT32(class_memmap->class_general_registers.cpisr, CPISR_OVE);
    // Restart counter
    WRITE_UINT32(class_memmap->class_general_registers.cpcr, ~PROFILING_ENABLE);
    WRITE_UINT32(class_memmap->class_general_registers.cpcr, PROFILING_ENABLE);

    release_class(class_tracker);
    return OS_SUCCESS;
}



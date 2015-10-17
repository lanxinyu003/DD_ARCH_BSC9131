/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/03/03 14:55:24 $
 $Id: sc3x00_mmu.c,v 1.20 2014/03/03 14:55:24 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_mmu.c,v $
 $Revision: 1.20 $
******************************************************************************/

/******************************************************************************

 @File          sc3x00_mmu.c

 @Description   OS MMU module runtime functions.

 @Cautions      None.

*//***************************************************************************/
#include "smartdsp_os_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_runtime.h"
#include "os_mmu.h"
#include "os_mmu_.h"
#include "sc3x00_mmu.h"
#include "sc3x00_mmu_.h"
#include "os_.h"
#include "os_list_.h"
#include "os_task_.h"

static dsp_plat_mmu_map_t* mmu = (dsp_plat_mmu_map_t*)(SOC_DSP_PLAT_DEFAULT + 0x6000);

#define PC_MMU     0xFFEFFE2C
#define STATUS_INVALID 0xFFFFFFFF
#define STATUS_BUSY    0xFFFFFFFE


#if (OS_MMU_SYSTEM_CONTEXT != 1)
#error OS_MMU_SYSTEM_CONTEXT should be 1
#endif

/* maximal value for program id/data id*/
/* Max number of available tasks - excluding the shared task (0) ID */
#define MAX_ID  255

/* These vars are initialized by system initialization code */
int os_global_num_of_data_seg;
int os_global_num_of_prog_seg;

/* These vars are initialized by system initialization code */
uint8_t os_global_prog_segments [NUMBER_OF_MMU_PROG_MATT];
uint8_t os_global_data_segments [NUMBER_OF_MMU_DATA_MATT];

static int data_context_num;
static int prog_context_num;

os_mmu_context_t *data_context_list;
os_mmu_context_t *prog_context_list;

extern os_tcb_t             *g_all_tasks_array;
extern uint32_t              g_total_tasks_number;
extern os_tcb_t             *g_running_task;


static os_status osMmuCheckValidity();
static os_status osMmuFsmCheckRestrictions(void const* virt_addr,
                                void const * phys_addr, uint32_t
                                reg_size);


/*****************************************************************************/

os_status osMmuDataContextFind(os_mmu_context_t** context)
{
    int i;
    osHwiSwiftDisable();
    for(i = 0; i < data_context_num; i++)
    {
        if(data_context_list[i].status == STATUS_INVALID)
        {
            data_context_list[i].status = STATUS_BUSY;
            *context = &data_context_list[i];
            osHwiSwiftEnable();
            return OS_SUCCESS;
        }

    }
    osHwiSwiftEnable();
    return OS_FAIL;
}

/*****************************************************************************/

os_status osMmuProgContextFind(os_mmu_context_t** context)
{
    int i;
    osHwiSwiftDisable();
    for(i = 0; i < prog_context_num; i++)
    {
        if(prog_context_list[i].status == STATUS_INVALID)
        {
            *context = &prog_context_list[i];
            prog_context_list[i].status = STATUS_BUSY;
            osHwiSwiftEnable();
            return OS_SUCCESS;
        }
    }
    osHwiSwiftEnable();
    return OS_FAIL;
}

/*****************************************************************************/

os_status osMmuDataContextCreate(os_mmu_context_handle context, uint32_t id, void* arch)
{
    VAR_UNUSED(arch);

    osHwiSwiftDisable();
    context->status = STATUS_BUSY;
    context->id = id;
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuProgContextCreate(os_mmu_context_handle context_handle, uint32_t id, void* arch)
{
    VAR_UNUSED(arch);

    osHwiSwiftDisable();
    context_handle->status = STATUS_BUSY;
    context_handle->id = id;
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuDataContextFindExisting(os_mmu_context_handle* context_handle, uint32_t id)
{
    int i;
    osHwiSwiftDisable();
    for(i = 1; i < data_context_num; i++)
    {
        if(data_context_list[i].id == id)
        {
            *context_handle = &data_context_list[i];
            osHwiSwiftEnable();
            return OS_SUCCESS;
        }
    }
    osHwiSwiftEnable();
    return OS_FAIL;
}


/*****************************************************************************/

os_status osMmuProgContextFindExisting(os_mmu_context_handle* context_handle, uint32_t id)
{
    int i;
    osHwiSwiftDisable();
    for(i = 1; i < prog_context_num; i++)
    {
        if(prog_context_list[i].id == id)
        {
            *context_handle = &prog_context_list[i];
            osHwiSwiftEnable();
            return OS_SUCCESS;
        }
    }
    osHwiSwiftEnable();
    return OS_FAIL;
}

/*****************************************************************************/

os_status osMmuDataContextDelete(os_mmu_context_handle context)
{
    OS_ASSERT_COND(context);
    osHwiSwiftDisable();
    context->status = STATUS_INVALID;
    context->segments = data_context_list[OS_MMU_SYSTEM_CONTEXT].segments;
    osHwiSwiftEnable();
    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuProgContextDelete(os_mmu_context_handle context)
{
    OS_ASSERT_COND(context);
    osHwiSwiftDisable();
    context->status = STATUS_INVALID;
    context->segments = prog_context_list[OS_MMU_SYSTEM_CONTEXT].segments;
    osHwiSwiftEnable();
    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuDataSegmentFind(os_mmu_segment_handle* descriptor)
{
    OS_ASSERT_COND(descriptor);
    osHwiSwiftDisable();
    if(os_global_num_of_data_seg >= NUMBER_OF_MMU_DATA_MATT)
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_FAIL);
    }

    *descriptor = (os_mmu_segment_handle)os_global_num_of_data_seg++;
    osHwiSwiftEnable();
    return OS_SUCCESS;
}


/*****************************************************************************/

os_status osMmuProgSegmentFind(os_mmu_segment_handle* descriptor)
{
    OS_ASSERT_COND(descriptor);
    osHwiSwiftDisable();
    if(os_global_num_of_prog_seg >= NUMBER_OF_MMU_PROG_MATT)
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_FAIL);
    }

    *descriptor = (os_mmu_segment_handle)os_global_num_of_prog_seg++;
    osHwiSwiftEnable();
    return OS_SUCCESS;

}

/*****************************************************************************/
os_status osMmuDataSegmentCreate(os_mmu_segment_handle descriptor, os_const_virt_ptr virt_addr,
                                os_const_phys_ptr phys_addr, uint32_t
                                reg_size, os_mmu_attr attr, void * arch
                                )
{
    uint32_t matt;

#ifdef MMU_ERROR_CHECKING
    os_status status;

    /* Validate that descriptor is valid */
    if(descriptor >= NUMBER_OF_MMU_DATA_MATT)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERR_MMU_WRONG_DESC);
    }

    /* Validate the minimal address alignment */
    if ((uint32_t)virt_addr & 0x000000FF)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERR_MMU_WRONG_ALIGNMENT);
    }

    /* Validate the minimal address alignment */
    if ((uint32_t)phys_addr & 0x000000FF)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERR_MMU_WRONG_ALIGNMENT);
    }

    /* IMPORTANT: adds restiction that the hardware doesn't have -
        M_DSDCx[DSPM] can't equal 1 if the FSM can't be used */

    if (attr & MMU_FLEX_SEGMENT_MODEL)
    {
        /* Validate the size */
        if (reg_size < 0x100 || reg_size > 0x7F0000)
        {
            OS_ASSERT;
            RETURN_ERROR(OS_ERR_MMU_WRONG_SIZE);
        }

        /* Validate that size is a multiplication of 256 */
        if(reg_size & 0xFF)
        {
            OS_ASSERT;
            RETURN_ERROR(OS_ERR_MMU_WRONG_SIZE);
        }

        status = osMmuFsmCheckRestrictions(virt_addr, phys_addr, reg_size);
        if (status != OS_SUCCESS)
        {
            OS_ASSERT;
            return status;
        }


        /* M_DSR[DSPE] will indicate other errors */
    }
    else
    {
        /* Validate that size is a power of two larger than 256 */
        if((reg_size & (reg_size-1)) || (reg_size < 0x100))
        {
            OS_ASSERT;
            RETURN_ERROR(OS_ERR_MMU_WRONG_SIZE);
        }

        /* Validate that size of segment fits virtual address */
        if((reg_size-1) & (uint32_t)virt_addr)
        {
            OS_ASSERT;
            RETURN_ERROR(OS_ERR_MMU_WRONG_SIZE);
        }
    }
#endif

    VAR_UNUSED(arch);

    osHwiSwiftDisable();
    if (attr & MMU_FLEX_SEGMENT_MODEL)
    {
        /* program all mmu registers */
        matt = (reg_size >> 8) | (attr & MMU_DSDC_ATTR_MASK);
        mmu->m_dsd[descriptor].m_dsdc = matt;

        matt = (uint32_t)phys_addr | ( (attr & MMU_DSDB_ATTR_MASK)>>16) ;
        mmu->m_dsd[descriptor].m_dsdb = matt;

        matt = (uint32_t)virt_addr | (attr & MMU_DSDA_ATTR_MASK) ;
        WRITE_SYNCIO_UINT32(mmu->m_dsd[descriptor].m_dsda, matt);

#ifdef MMU_ERROR_CHECKING
        if (mmu->m_dsr & M_DSR_DSPE)
        {
            OS_ASSERT;
            osHwiSwiftEnable();
            RETURN_ERROR(OS_ERR_MMU_DSPE_ERR);
        }
#endif
    }
    else
    {
        /* program both registers */
        matt = (uint32_t)phys_addr | ( (attr & MMU_DSDB_ATTR_MASK)>>16) ;
        mmu->m_dsd[descriptor].m_dsdb = matt;

        matt = (uint32_t)virt_addr | (reg_size>>1) | (attr & MMU_DSDA_ATTR_MASK) ;
        WRITE_SYNCIO_UINT32(mmu->m_dsd[descriptor].m_dsda, matt);
    }
    osHwiSwiftEnable();


    return OS_SUCCESS;

}

/*****************************************************************************/

os_status osMmuProgSegmentCreate(os_mmu_segment_handle descriptor,os_const_virt_ptr virt_addr,os_const_phys_ptr phys_addr,
                                 uint32_t reg_size,
                                 os_mmu_attr attr, void * arch)
{
    uint32_t matt;

#ifdef MMU_ERROR_CHECKING
    os_status status;
    /* Validate that descriptor is valid */
    if(descriptor >= NUMBER_OF_MMU_PROG_MATT)
    {
            OS_ASSERT;
            RETURN_ERROR(OS_ERR_MMU_WRONG_DESC);
    }

    /* Validate address alignment */
    if(  (uint32_t)virt_addr & 0x000000FF )
    {
        RETURN_ERROR(OS_ERR_MMU_WRONG_ALIGNMENT);
        OS_ASSERT;
    }

    /* Validate the minimal address alignment */
    if ((uint32_t)phys_addr & 0x000000FF)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERR_MMU_WRONG_ALIGNMENT);
    }

    /* Validate address alignment */
    /* IMPORTANT: adds restiction that the hardware doesn't have -
        M_DSDCx[DSPM] can't equal 1 if the FSM can't be used */
    if (attr & MMU_FLEX_SEGMENT_MODEL)
    {
        /* Validate the size */
        if (reg_size < 0x100 || reg_size > 0x7F0000)
        {
            OS_ASSERT;
            RETURN_ERROR(OS_ERR_MMU_WRONG_SIZE);
        }

        /* Validate that size is a multiplication of 256 */
        if(reg_size & 0xFF)
        {
            OS_ASSERT;
            RETURN_ERROR(OS_ERR_MMU_WRONG_SIZE);
        }

        status = osMmuFsmCheckRestrictions(virt_addr, phys_addr, reg_size);
        if (status != OS_SUCCESS)
        {
            OS_ASSERT;
            return status;
        }

    }
    else
    {
        /* Validate that size is a power of two larger than 256 */
        if((reg_size & (reg_size-1)) || (reg_size < 0x100))
        {
            RETURN_ERROR(OS_ERR_MMU_WRONG_SIZE);
            OS_ASSERT;
        }

        /* Validate that size of segment fits virtual address */
        if((reg_size-1) & (uint32_t)virt_addr)
        {
            RETURN_ERROR(OS_ERR_MMU_WRONG_SIZE);
            OS_ASSERT;
        }
    }

#endif

    VAR_UNUSED(arch);

    osHwiSwiftDisable();
    if (attr & MMU_FLEX_SEGMENT_MODEL)
    {
        /* program all mmu registers */
        matt = (reg_size >> 8) | (attr & MMU_PSDC_ATTR_MASK);
        mmu->m_psd[descriptor].m_psdc = matt;

        matt = (uint32_t)phys_addr | ( (attr & MMU_PSDB_ATTR_MASK)>>16) ;
        mmu->m_psd[descriptor].m_psdb = matt;

        matt = (uint32_t)virt_addr | (attr & MMU_PSDA_ATTR_MASK) ;
        WRITE_SYNCIO_UINT32(mmu->m_psd[descriptor].m_psda, matt);

#ifdef MMU_ERROR_CHECKING
        if (mmu->m_psr & M_PSR_PSPE)
        {
            OS_ASSERT;
            osHwiSwiftEnable();
            RETURN_ERROR(OS_ERR_MMU_PSPE_ERR);
        }
#endif
    }
    else
    {
        matt = (uint32_t)phys_addr | ( (attr>>16) & 0x1F) ;
        mmu->m_psd[descriptor].m_psdb = matt;

        matt = (uint32_t)virt_addr | (reg_size>>1) | (attr & 0x1F) ;
        WRITE_SYNCIO_UINT32(mmu->m_psd[descriptor].m_psda, matt);

    }
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuDataSegmentEnable(os_mmu_segment_handle descriptor, bool enable)
{
    uint32_t m_dsda;

    osHwiSwiftDisable();
    if(enable)
        m_dsda = mmu->m_dsd[descriptor].m_dsda | MMU_MATT_ENABLE;
    else
        m_dsda = mmu->m_dsd[descriptor].m_dsda & (~MMU_MATT_ENABLE);
    WRITE_SYNCIO_UINT32(mmu->m_dsd[descriptor].m_dsda,m_dsda);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

os_status osMmuProgSegmentEnable(os_mmu_segment_handle descriptor, bool enable)
{
    uint32_t m_psda;

    osHwiSwiftDisable();
    if(enable)
        m_psda = mmu->m_psd[descriptor].m_psda | MMU_MATT_ENABLE;
    else
        m_psda = mmu->m_psd[descriptor].m_psda & ~MMU_MATT_ENABLE;
    WRITE_SYNCIO_UINT32(mmu->m_psd[descriptor].m_psda,m_psda);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuProgContextSegmentsAdd(os_mmu_context_handle mmu_context, os_mmu_segment_handle segment)
{

    OS_ASSERT_COND(mmu_context != NULL);

    osHwiSwiftDisable();
    mmu_context->segments |= 1<<segment;
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuDataContextSegmentsAdd(os_mmu_context_handle mmu_context, os_mmu_segment_handle segment)
{
    OS_ASSERT_COND(mmu_context != NULL);

    osHwiSwiftDisable();
    mmu_context->segments |= 1<<segment;
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuProgSystemContextSegmentAdd(os_mmu_segment_handle segment)
{
    int i;

    osHwiSwiftDisable();
    for(i = 0; i < prog_context_num; i++)
        prog_context_list[i].segments |= 1<<segment;

    // update in all tasks except current task
    if (OS_HWI_INACTIVE)
    {
        for(i=0 ; i<g_total_tasks_number ; i++)
            if ((g_all_tasks_array[i].stack_pointer != NULL) && (&(g_all_tasks_array[i]) != g_running_task))
                g_all_tasks_array[i].stack_pointer[REG_OFF_MMU_PROG_SEG-CONTEXT_SIZE] |= 1<<segment;
    }
    else
        OS_ASSERT;  // Can't update system context in interrupt context

    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuDataSystemContextSegmentAdd(os_mmu_segment_handle segment)
{
    int i;

    osHwiSwiftDisable();
    for(i = 0; i < data_context_num; i++)
        data_context_list[i].segments |= 1<<segment;
 
    // update in all tasks except current task
    if (OS_HWI_INACTIVE)
    {
         for(i=0 ; i<g_total_tasks_number ; i++)
            if ((g_all_tasks_array[i].stack_pointer != NULL) && (&(g_all_tasks_array[i]) != g_running_task))
                g_all_tasks_array[i].stack_pointer[REG_OFF_MMU_DATA_SEG-CONTEXT_SIZE] |= 1<<segment;
    }
    else
        OS_ASSERT;  // Can't update system context in interrupt context

     osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuProgContextSegmentRemove(os_mmu_context_handle mmu_context, os_mmu_segment_handle segment)
{

    OS_ASSERT_COND(mmu_context != NULL);

    osHwiSwiftDisable();
    mmu_context->segments &= ~(1<<segment);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuDataContextSegmentRemove(os_mmu_context_handle mmu_context, os_mmu_segment_handle segment)
{
    OS_ASSERT_COND(mmu_context != NULL);

    osHwiSwiftDisable();
    mmu_context->segments &= ~(1<<segment);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuProgSystemContextSegmentRemove(os_mmu_segment_handle segment)
{
    int i;

    osHwiSwiftDisable();
    for(i = 0; i < prog_context_num; i++)
        prog_context_list[i].segments &= ~(1<<segment);

    // update in all tasks except current task
    if (OS_HWI_INACTIVE)
    {
        for(i=0 ; i<g_total_tasks_number ; i++)
            if ((g_all_tasks_array[i].stack_pointer != NULL) && (&(g_all_tasks_array[i]) != g_running_task))
                g_all_tasks_array[i].stack_pointer[REG_OFF_MMU_PROG_SEG-CONTEXT_SIZE] &= ~(1<<segment);
    }
    else
        OS_ASSERT;  // Can't update system context in interrupt context

    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuDataSystemContextSegmentRemove(os_mmu_segment_handle segment)
{
    int i;

    osHwiSwiftDisable();
    for(i = 0; i < data_context_num; i++)
        data_context_list[i].segments &= ~(1<<segment);

    // update in all tasks except current task
    if (OS_HWI_INACTIVE)
    {
        for(i=0 ; i<g_total_tasks_number ; i++)
            if ((g_all_tasks_array[i].stack_pointer != NULL) && (&(g_all_tasks_array[i]) != g_running_task))
                g_all_tasks_array[i].stack_pointer[REG_OFF_MMU_DATA_SEG-CONTEXT_SIZE] &= ~(1<<segment);
    }
    else
        OS_ASSERT;  // Can't update system context in interrupt context

    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMmuDataContextActivate(os_mmu_context_t  *new_desc, os_mmu_context_t  *prev)
{
    osHwiSwiftDisable();
    if(prev)
    {
        prev->segments = mmu->m_dsdcr;
        prev->id = mmu->m_cdid;
    }

    WRITE_SYNCIO_UINT32(mmu->m_dsdcr,new_desc->segments);
    WRITE_SYNCIO_UINT32(mmu->m_cdid,new_desc->id);
    osHwiSwiftEnable();

    return OS_SUCCESS;

}

/*****************************************************************************/

os_status osMmuProgContextActivate(os_mmu_context_t  *new_desc, os_mmu_context_t  *prev)
{
    osHwiSwiftDisable();
    if(prev)
    {
        prev->segments = mmu->m_psdcr;
        prev->id = mmu->m_cpid;
    }
    WRITE_SYNCIO_UINT32(mmu->m_psdcr,new_desc->segments);
    WRITE_SYNCIO_UINT32(mmu->m_cpid,new_desc->id);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}


/*****************************************************************************/

uint32_t osMmuDataCurrentIDSet(uint32_t id)
{
    uint32_t ret;

    osHwiSwiftDisable();
    ret = mmu->m_cdid;
    WRITE_SYNCIO_UINT32(mmu->m_cdid,id);
    osHwiSwiftEnable();

    return ret;
}


/*****************************************************************************/
uint32_t osMmuProgCurrentIDSet(uint32_t id)
{
    uint32_t ret;

    osHwiSwiftDisable();
    ret = mmu->m_cpid;
    WRITE_SYNCIO_UINT32(mmu->m_cpid,id);
    osHwiSwiftEnable();

    return ret;
}

/*****************************************************************************/
uint32_t osMmuProgCurrentIDGet()
{
    return mmu->m_cpid;
}


/*****************************************************************************/
uint32_t osMmuDataCurrentIDGet()
{
    return mmu->m_cdid;
}

/*****************************************************************************/
uint32_t osMmuContextIDGet(os_mmu_context_t* cont)
{
    OS_ASSERT_COND(cont != NULL);
    return cont->id;
}


/*****************************************************************************/
int osMmuDataNumOfUsedSegmentsGet()
{
    return os_global_num_of_data_seg;
}

/*****************************************************************************/
int osMmuProgNumOfUsedSegmentsGet()
{
    return os_global_num_of_prog_seg;
}


/*****************************************************************************/
int osMmuDataNumOfSegmentsGet()
{
    return NUMBER_OF_MMU_DATA_MATT;
}

/*****************************************************************************/
int osMmuProgNumOfSegmentsGet()
{
    return NUMBER_OF_MMU_PROG_MATT;
}


/*****************************************************************************/
os_status osMmuDataSegmentProbe(os_const_virt_ptr virt_addr, os_mmu_segment_handle *segment_num)
{
    os_status status;
 
    osHwiSwiftDisable();
    status = osMmuDataVirtProbe(virt_addr);
    *segment_num = (mmu->m_dqsr & 0x0000001F);
    osHwiSwiftEnable();
 
    return status;
}

/*****************************************************************************/
os_status osMmuProgSegmentProbe(os_const_virt_ptr virt_addr, os_mmu_segment_handle *segment_num)
{
    os_status status;
 
    osHwiSwiftDisable();
    status = osMmuProgVirtProbe(virt_addr);
    *segment_num = (mmu->m_pqsr & 0x0000000F);
    osHwiSwiftEnable();
 
    return status;
}

/*****************************************************************************/
os_status osMmuDataGetAttr(os_const_virt_ptr virt_addr, os_mmu_attr *attr)
{
    os_status status;
    os_mmu_segment_handle seg_num;
    uint32_t matt;
 
    osHwiSwiftDisable();
    status = osMmuDataVirtProbe(virt_addr);
    if(status == OS_SUCCESS)
    {
        seg_num = (mmu->m_dqsr & 0x0000001F);
        OS_ASSERT_COND(seg_num < NUMBER_OF_MMU_DATA_MATT);
        matt = (mmu->m_dsd[seg_num].m_dsdb & (MMU_DSDB_ATTR_MASK >> 16)) << 16;
        matt |= mmu->m_dsd[seg_num].m_dsda & 0x3F;
        matt |= mmu->m_dsd[seg_num].m_dsdc & MMU_FLEX_SEGMENT_MODEL;
        *attr = matt;
    }
    osHwiSwiftEnable();
 
    return status;
}

/*****************************************************************************/
os_status osMmuProgGetAttr(os_const_virt_ptr virt_addr, os_mmu_attr *attr)
{
    os_status status;
    os_mmu_segment_handle seg_num;
    uint32_t matt;
 
    osHwiSwiftDisable();
    status = osMmuProgVirtProbe(virt_addr);
    if(status == OS_SUCCESS)
    {
        seg_num = (mmu->m_pqsr & 0x0000000F);
        OS_ASSERT_COND(seg_num < NUMBER_OF_MMU_PROG_MATT);
        matt  = mmu->m_psd[seg_num].m_psda & 0x1F;
        matt |= (mmu->m_psd[seg_num].m_psdb & 0x1F) << 16;
        matt |= mmu->m_psd[seg_num].m_psdc & (MMU_PROG_NEXT_LINE_PREFETCH | MMU_FLEX_SEGMENT_MODEL);
        *attr = matt;
    }
    osHwiSwiftEnable();
 
    return status;
}

/*****************************************************************************/
asm os_status osMmuProgVirtProbe(os_const_virt_ptr virt_addr)
{
asm_header
    .arg
    _virt_addr in $r0;
    return in $d0;
    .reg $r0,$d0,$d1,$d2;
asm_body

    // read di counter & disable interrupts
    [
        move.l   _g_int_disable_counter,d2
        di
    ]

    [
        pquery r0
        syncm
    ]
 
    nobtb jmp %C_osMmuProgVirtProbe_next

%C_osMmuProgVirtProbe_next: // (sync program and data pipeline)
    move.l   #$FFF07008,r0 //Status register
    move.l   (r0),d0       //status  -> d3
    and      #$70000,d0,d1 //check if fetch succeded
    tsteq.l  d1            //check status register
    [
        iff sub d0,d0,d0
        ift move.w #$1,d0
    ]

    //check if we need to enable interrupts
    tsteq.l  d2
    ift ei

asm_end

}




/*****************************************************************************/
asm os_status osMmuDataVirtProbe(os_const_virt_ptr virt_addr)
{

asm_header
    .arg
    _virt_addr in $r0;
    return in $d0;
    .reg $r0,$d0,$d1,$d2;
asm_body


    // read di counter & disable interrupts
    [
        move.l   _g_int_disable_counter,d2
        di
    ]

    nop
    [
        dquery (r0)
        syncm
    ]

    move.l   #$FFF06108,r0 //Status register
    move.l   (r0),d0       //status  -> d3
    and      #$70000,d0,d1 //check if fetch succeded
    tsteq.l  d1            //check status register
    [
        iff sub d0,d0,d0
        ift move.w #$1,d0
    ]

    //check if we need to enable interrupts
    tsteq.l  d2
    ift ei

asm_end

}


/*****************************************************************************/
asm os_status osMmuDataVirtToPhys(os_const_virt_ptr virt_addr,os_phys_ptr* phys_addr)
{
asm_header
    .arg
    _virt_addr in $r0;
    _phys_addr in $r1;
    return in $d0;
    .reg $r0,$r1,$r2,$d0,$d1;
asm_body

    // read di counter & disable interrupts
    [
        move.l _g_int_disable_counter,d1
        di
    ]

//  nop
    [
        dquery (r0)             // virtual address -> r0
        syncm
    ]

    move.l   ($FFF0610C),r2     // physical address  -> r2
    move.l   ($FFF06108),d0     // query status -> d0

    bmtstc   #$7,d0.h           // check if fetch succeded
    [
        iff sub d0,d0,d0        // if failed - OS_FAIL
        ifa bf %C_osMmuDataVirtToPhys_exit
        and #$0,r0.h          // remove the top four nibbles from the physical address
                                // allows the address target to be derived from the virtual one
    ]

    [
        and    #$ff,r0.l
        move.w #$1,d0           // OS_SUCCESS
    ]
    ora      r0,r2              // create the address
    move.l   r2,(r1)

%C_osMmuDataVirtToPhys_exit
    //check if we need to enable interrupts
    tsteq.l  d1
    ift ei

asm_end
}


/*****************************************************************************/
asm os_status osMmuProgVirtToPhys(os_const_virt_ptr virt_addr,os_phys_ptr* phys_addr)
{
asm_header
    .arg
    _virt_addr in $r0;
    _phys_addr in $r1;
    return in $d0;
    .reg $r0,$r1,$r2,$d0,$d1;
asm_body


    // read di counter & disable interrupts
    [
        move.l _g_int_disable_counter,d1
        di
    ]

    [
        pquery r0
        syncm
    ]
    nobtb jmp %C_osMmuProgVirtToPhys_next

%C_osMmuProgVirtToPhys_next: // (sync program and data pipeline)
    move.l   ($FFF0700C),r2     // physical address  -> r2
    move.l   ($FFF07008),d0     // query status -> d0

    bmtstc   #$7,d0.h           // check if fetch succeded
    [
        iff sub d0,d0,d0        // if failed - OS_FAIL
        ifa bf %C_osMmuProgVirtToPhys_exit
        and #$0,r0.h          // remove the top nibble from the physical address
    ]

    [
        and    #$ff,r0.l
        move.w #$1,d0           // OS_SUCCESS
    ]
    ora      r0,r2              // create the address
    move.l   r2,(r1)

%C_osMmuProgVirtToPhys_exit
    //check if we need to enable interrupts
    tsteq.l  d1
    ift ei

asm_end
}



/*****************************************************************************/
os_status osMmuInit(int data_context, int prog_context)
{
    int i, j;
    os_status status;
    int current_context;
    data_context_num = data_context;
    prog_context_num = prog_context;

    mmu = (dsp_plat_mmu_map_t*)&g_dsp_plat_map->mmu;

    data_context_list = osMalloc(sizeof(os_mmu_context_t)*data_context_num, OS_MEM_LOCAL);
    prog_context_list = osMalloc(sizeof(os_mmu_context_t)*prog_context_num, OS_MEM_LOCAL);
 
    OS_ASSERT_COND(data_context_list != NULL);
    OS_ASSERT_COND(prog_context_list != NULL);

    /* Set system context.
       Those segments were defined previosly on system init */
    data_context_list[OS_MMU_SYSTEM_CONTEXT].segments = mmu->m_dsdcr & 0x000FFFFF;
    prog_context_list[OS_MMU_SYSTEM_CONTEXT].segments = mmu->m_psdcr & 0x00000FFF;

    data_context_list[OS_MMU_SYSTEM_CONTEXT].id = OS_MMU_SYSTEM_CONTEXT;
    data_context_list[OS_MMU_SYSTEM_CONTEXT].status = STATUS_BUSY;
    prog_context_list[OS_MMU_SYSTEM_CONTEXT].id = OS_MMU_SYSTEM_CONTEXT;
    prog_context_list[OS_MMU_SYSTEM_CONTEXT].status = STATUS_BUSY;

    /* Initialize all other data contexts */
    for(i = 2; i < data_context_num ; i++)
    {
        data_context_list[i].status = STATUS_INVALID;
        data_context_list[i].id = (uint32_t)(MAX_ID - i);
    }

    /* Initialize all other program contexts */
    for(i = 2; i < prog_context_num ; i++)
    {
        prog_context_list[i].status = STATUS_INVALID;
        prog_context_list[i].id = (uint32_t)(MAX_ID - i);
    }


    /* trace thru initialization table and */
    /* find for each ID related segments   */
    /* begin with 2 because 1 is reserved for system */

    /* current_context is 2 because 1 is system */
    current_context = 2;

    for(i = 2; i < MAX_ID; i++)
    {
        /* build bitmask for each segment */
        for(j = 0; j < os_global_num_of_prog_seg; j++)
            if(os_global_prog_segments[j] == i)
            {
                prog_context_list[current_context].segments |= (1<<j);
            }
        /* if some segments exists, then continue to next context */
        if (prog_context_list[current_context].segments)
        {
            prog_context_list[current_context].id = (uint32_t)i;
            prog_context_list[current_context].status = STATUS_BUSY;
            current_context++;
        }
    }

    current_context = 2;

    /* trace thru initialization table and */
    /* find for each ID related segments   */
    /* begin with 2 because 1 is reserved for system */

    /* Valid ID starting from 2 */

    for(i = 2; i < MAX_ID; i++)
    {
        for(j = 0; j < os_global_num_of_data_seg; j++)
            if(os_global_data_segments[j] == i)
            {
                data_context_list[current_context].segments |= (1<<j);
            }
        if (data_context_list[current_context].segments)
        {
            data_context_list[current_context].id = (uint32_t)i;
            data_context_list[current_context].status = STATUS_BUSY;
            current_context++;
        }

    }

    status = osMmuCheckValidity();
    if(status != OS_SUCCESS)
        return status;

    /*add system segments to all contexts*/
    for(i = 0; i < data_context_num ; i++)
        data_context_list[i].segments |= data_context_list[OS_MMU_SYSTEM_CONTEXT].segments;

    for(i = 0; i < prog_context_num; i++)
        prog_context_list[i].segments |= prog_context_list[OS_MMU_SYSTEM_CONTEXT].segments;

    return OS_SUCCESS;
}




/*****************************************************************************/
static os_status osMmuCheckValidity()
{
    int i,j;
    os_status status;

    /*  Check the validity of segments programming
        1. Check that there is no  virtual addresses
           overlapping for the same context (PID/DID)
        2. Check that there is no overlapping virtual addresses
           for any system/shared registers and
           any other virtual addresses
    */
    // 1.

    for(i = os_global_num_of_data_seg-1; i >= 0; i--)
    for(j = 0; j < i; j++)
    {
        //Check only the same PID segments and system/shared segments
        if ((os_global_data_segments[i] == os_global_data_segments[j]) |
            (os_global_data_segments[i] == MMU_SYSTEM_PID) |
            (os_global_data_segments[j] == MMU_SYSTEM_PID)
            )
        {
            uint32_t  virt_addr;
            if (mmu->m_dsd[i].m_dsda & MMU_MATT_ENABLE)
            {
                virt_addr = mmu->m_dsd[i].m_dsda & 0xFFFFFF80;
                if ((status = osMmuDataVirtProbe((os_const_virt_ptr)virt_addr)) != OS_SUCCESS)
                    return status;
            }
            if (mmu->m_dsd[j].m_dsda & MMU_MATT_ENABLE)
            {
                virt_addr = mmu->m_dsd[j].m_dsda & 0xFFFFFF80;
                if ((status = osMmuDataVirtProbe((os_const_virt_ptr)virt_addr)) != OS_SUCCESS)
                    return status;
            }

        }
    }

    for(i = os_global_num_of_prog_seg-1; i >= 0; i--)
    for(j = 0; j < i; j++)
    {

        //Check only the same PID segments and system/shared segments
        if( (os_global_prog_segments[i] == os_global_prog_segments[j]) |
            (os_global_prog_segments[i] == MMU_SYSTEM_PID) |
            (os_global_prog_segments[j] == MMU_SYSTEM_PID)
            )

        {
            uint32_t  virt_addr;
            if (mmu->m_psd[i].m_psda & MMU_MATT_ENABLE)
            {
                virt_addr = mmu->m_psd[i].m_psda & 0xFFFFFF80;
                if ((status = osMmuProgVirtProbe((os_const_virt_ptr)virt_addr)) != OS_SUCCESS)
                    return status;
            }
            if (mmu->m_psd[j].m_psda & MMU_MATT_ENABLE)
            {
                virt_addr = mmu->m_psd[j].m_psda & 0xFFFFFF80;
                if ((status = osMmuProgVirtProbe((os_const_virt_ptr)virt_addr)) != OS_SUCCESS)
                    return status;
            }

        }
    }


    return OS_SUCCESS;
}


/*****************************************************************************/
/*
-------------+----------------------+---------------------
Segment Size | Boundary Restriction | Alignment Restriction
-------------+----------------------+---------------------
256B-32KB    |          64KB        |       256B
-------------+----------------------+---------------------
1KB-128KB    |          256KB       |       1KB
-------------+----------------------+---------------------
16KB-2MB     |          4MB         |       16KB
-------------+----------------------+---------------------
64KB-8MB     |          16MB        |       64KB
-------------+----------------------+---------------------
*/
static inline os_status osMmuCheckAlignmentRestrictions(void const* virt_addr,
                                void const * phys_addr, uint32_t reg_size, uint32_t required_align)
{
    if (!IS_ALIGNED(virt_addr, required_align) ||
        !IS_ALIGNED(phys_addr, required_align) ||
        !IS_ALIGNED(reg_size, required_align))
    {
        return OS_ERR_MMU_WRONG_ALIGNMENT;
    }

    return OS_SUCCESS;
}

static inline os_status osMmuCheckBoundaryRestrictions(void const* virt_addr,
                                void const * phys_addr, uint32_t reg_size, uint32_t boundary)
{
    uint32_t aligned_size;

    aligned_size = ALIGN_DOWN_ADDRESS(virt_addr, boundary);
    if ((uint32_t)virt_addr+reg_size > aligned_size+boundary)
    {
        return OS_ERR_MMU_WRONG_BOUNDARY;
    }
    aligned_size = ALIGN_DOWN_ADDRESS(phys_addr, boundary);
    if ((uint32_t)phys_addr+reg_size > aligned_size+boundary)
    {
        OS_ASSERT;
        return OS_ERR_MMU_WRONG_BOUNDARY;
    }

    return OS_SUCCESS;
}

static os_status osMmuFsmCheckRestrictions(void const* virt_addr, void const * phys_addr, uint32_t reg_size)
{
    os_status status = OS_SUCCESS;
#ifdef MMU_ERROR_CHECKING

    if ((0x100 <= reg_size) && (reg_size <= 0x8000))
    {
        status = osMmuCheckBoundaryRestrictions(virt_addr, phys_addr, reg_size, 0x10000);
        if (status == OS_SUCCESS)
            status = osMmuCheckAlignmentRestrictions(virt_addr, phys_addr, reg_size, 0x100);
        if(status == OS_SUCCESS)
            return OS_SUCCESS;
    }

    if ((0x400 <= reg_size) && (reg_size <= 0x20000))
    {
        status = osMmuCheckBoundaryRestrictions(virt_addr, phys_addr, reg_size, 0x40000);
        if (status == OS_SUCCESS)
            status = osMmuCheckAlignmentRestrictions(virt_addr, phys_addr, reg_size, 0x400);
        if(status == OS_SUCCESS)
            return OS_SUCCESS;
    }

    if ((0x4000 <= reg_size) && (reg_size <= 0x200000))
    {
        status = osMmuCheckBoundaryRestrictions(virt_addr, phys_addr, reg_size, 0x400000);
        if (status == OS_SUCCESS)
            status = osMmuCheckAlignmentRestrictions(virt_addr, phys_addr, reg_size, 0x4000);
        if(status == OS_SUCCESS)
            return OS_SUCCESS;
    }

    if ((0x10000 <= reg_size) && (reg_size < 0x800000))
    {
        status = osMmuCheckBoundaryRestrictions(virt_addr, phys_addr, reg_size, 0x1000000);
        if (status == OS_SUCCESS)
            status = osMmuCheckAlignmentRestrictions(virt_addr, phys_addr, reg_size, 0x10000);
        if(status == OS_SUCCESS)
            return OS_SUCCESS;
    }
#else
    VAR_UNUSED(virt_addr);
    VAR_UNUSED(phys_addr);
    VAR_UNUSED(reg_size);
#endif
    return status;
}


/*****************************************************************************/
os_status osMmuDataErrorDetect(struct os_mmu_error* err)
{
    uint32_t dsr = GET_UINT32(mmu->m_dsr);
    os_status status = OS_ERR_UNKNOWN;

    if(M_DSR_DMSD & dsr)
    {
        status = OS_ERR_MMU_MULTIPLE_HIT;
    }
    else if(M_DSR_DNME & dsr)
    {
        status = OS_ERR_MMU_NON_MAPPED;
    }
    else if(M_DSR_DPV & dsr)
    {
        status = OS_ERR_MMU_PRIVILEGE;
    }
    else if(M_DSR_DNAE & dsr)
    {
        status = OS_ERR_MMU_NOT_ALIGNED;
    }
    else if(M_DSR_WSBE & dsr)
    {
        status = OS_ERR_MMU_THESAME_ADDR;
    }
    else if(M_DSR_DSM & dsr)
    {
        status = OS_ERR_MMU_SEG_MISS;
    }
    else if(M_DSR_DEDC & dsr)
    {
        status = OS_ERR_MMU_EDC;
    }
    else if(M_DSR_DSPE & dsr)
    {
        status = OS_ERR_MMU_DSPE_ERR;
    }

    if(status == OS_ERR_UNKNOWN)
        RETURN_ERROR (status);

    err->rw_access = !!(M_DSR_DAVD & dsr);
    err->access_width = (uint32_t)(1<<((dsr & M_DSR_DAVW)>>18))*8;

    /* copy exception PC from OCE register */
    err->error_pc = *((volatile uint32_t*)PC_MMU);

    if(mmu->m_cr & M_CR_CVAE)
    {
        err->priviledge_level = !!(M_DSR_DPL & dsr);
        err->error_address = mmu->m_dva;
    }

    RETURN_ERROR (status);
}


/*****************************************************************************/
os_status osMmuProgErrorDetect(struct os_mmu_error* err)
{
    uint32_t psr = GET_UINT32(mmu->m_psr);
    os_status status = OS_ERR_UNKNOWN;

    if(M_PSR_PMSD & psr)
    {
        status = OS_ERR_MMU_MULTIPLE_HIT;
    }
    else if(M_PSR_PNME & psr)
    {
        status = OS_ERR_MMU_NON_MAPPED;
    }
    else if(M_PSR_PPV & psr)
    {
        status = OS_ERR_MMU_PRIVILEGE;
    }
    else if(M_PSR_PNAE & psr)
    {
        status = OS_ERR_MMU_NOT_ALIGNED;
    }
    else if(M_PSR_PSM & psr)
    {
        status = OS_ERR_MMU_SEG_MISS;
    }
    else if(M_PSR_PEDC & psr)
    {
        status = OS_ERR_MMU_EDC;
    }
    else if(M_PSR_PSPE & psr)
    {
        status = OS_ERR_MMU_PSPE_ERR;
    }

    if(status == OS_ERR_UNKNOWN)
        RETURN_ERROR (status);

    /* copy exception PC from OCE register */
    err->error_pc = *((volatile uint32_t*)PC_MMU);

    if(mmu->m_cr & M_CR_CVAE)
    {
        err->priviledge_level = !!(M_PSR_PPL & psr);
    }
 
    RETURN_ERROR (status);
}

/*****************************************************************************/
void osMmuDataErrorClear()
{
    SET_UINT32(mmu->m_cr, M_CR_CMIR);
}

/*****************************************************************************/
void osMmuProgErrorClear()
{
    SET_UINT32(mmu->m_cr, M_CR_CMIR);
}




/******************************************************************//*
    Manual translation functions - use them only for debugging
*//******************************************************************/
os_status osMmuProgVirtToPhysManual(void const* virt_addr,void ** phys_addr)
{
    return osMmuProgMattInfo(virt_addr, phys_addr, NULL, NULL);
}


/*****************************************************************************/
os_status osMmuDataVirtToPhysManual(void const* virt_addr,void ** phys_addr)
{
    return osMmuDataMattInfo(virt_addr, phys_addr, NULL, NULL);
}

/*****************************************************************************/
os_status osMmuDataPhysToVirtManual(void const* phys_addr,void ** virt_addr)
{
    int i, j;
    uint32_t seg_size = 0;
    uint32_t  seg_vaddr;
    uint32_t  seg_paddr;

    osHwiSwiftDisable();
    for(i = 0; i < NUMBER_OF_MMU_DATA_MATT; i++)
    {
        /* not enabled */
        if((mmu->m_dsd[i].m_dsda & MMU_MATT_ENABLE) == 0) continue;

        /* virtual address */
        seg_vaddr = mmu->m_dsd[i].m_dsda & 0xFFFFFF80;
        seg_paddr = mmu->m_dsd[i].m_dsdb & MMU_MATT_ADDRESS_MASK;

        /* segment size */
        if (mmu->m_dsd[i].m_dsdc & MMU_FLEX_SEGMENT_MODEL)
        {
            seg_vaddr = mmu->m_dsd[i].m_dsda & 0xFFFFFF00;
            seg_size = (mmu->m_dsd[i].m_dsdc & 0x00007FFF) << 0x08;
        }
        else
        {
            for (j = 7; j < 32; j++)
            {
                if (seg_vaddr & (1 << j))
                {
                    seg_vaddr &= ~(1 << j);
                    seg_size = 0x100 << (j - 7);
                    break;
                }
            }
        }

        if( ((uint32_t)phys_addr >= seg_paddr) && ((uint32_t)phys_addr < seg_paddr+(seg_size)) )
        {
            *virt_addr = (void*) (seg_vaddr + ((uint32_t)phys_addr - seg_paddr));
            osHwiSwiftEnable();
            return OS_SUCCESS;
        }
    }
    osHwiSwiftEnable();

    return OS_FAIL;
}

/******************************************************************/
int osMmuNumOfDataContextsGet()
{
    return data_context_num;
}

/******************************************************************/
int osMmuNumOfProgContextsGet()
{
    return prog_context_num;
}

/******************************************************************/
int osMmuProgMaxIdGet()
{
    return MAX_ID;
}

/******************************************************************/
int osMmuDataMaxIdGet()
{
    return MAX_ID;
}

/******************************************************************/

/* Internal functions */
os_status osMmuDataTasksContextSet(uint32_t* task_stack, os_mmu_context_handle mmu_context)
{
    osHwiSwiftDisable();
    task_stack[REG_OFF_MMU_DATA_ID]  = mmu_context->id;
    task_stack[REG_OFF_MMU_DATA_SEG] = mmu_context->segments;
    osHwiSwiftEnable();

    return OS_SUCCESS;

}

/******************************************************************/
os_status osMmuProgTasksContextSet(uint32_t* task_stack, os_mmu_context_handle mmu_context)
{
    osHwiSwiftDisable();
    task_stack[REG_OFF_MMU_PROG_ID] = mmu_context->id;
    task_stack[REG_OFF_MMU_PROG_SEG] = mmu_context->segments;
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/******************************************************************/
os_status osMmuProgMattInfo(void const* virt_addr,void ** phys_addr, uint32_t *size, void **phys_base)
{
    int i, j;
    uint32_t vaddr_val, seg_size = 0;

    osHwiSwiftDisable();
    for(i = 0; i < NUMBER_OF_MMU_PROG_MATT; i++)
    {
        if(!(mmu->m_psd[i].m_psda & MMU_MATT_ENABLE)) continue;

        /* get virtual base of this segment */
        if (mmu->m_psd[i].m_psdc & MMU_FLEX_SEGMENT_MODEL)
        {
            vaddr_val = mmu->m_psd[i].m_psda & 0xFFFFFF00;
            seg_size = 0x100 * (mmu->m_psd[i].m_psdc & 0x0000EFFF);
        }
        else
        {
            vaddr_val = mmu->m_psd[i].m_psda & MMU_MATT_VIRT_ADDRESS_MASK;
            for (j = 7; j < 32; j++)
            {
                if (vaddr_val & 1 << j)
                {
                    vaddr_val &= ~(1 << j);
                    seg_size = (uint32_t)(0x100 << (j - 7));
                    break;
                }
            }
        }

        if(((uint32_t)virt_addr >= vaddr_val) && ((uint32_t)virt_addr < vaddr_val+(seg_size)))
        {
            if(phys_addr)
               *phys_addr = (void*)((mmu->m_psd[i].m_psdb & MMU_MATT_ADDRESS_MASK) +
                            (uint32_t)virt_addr - vaddr_val);
            if (size)
                *size = seg_size;

            if (phys_base)
                *phys_base = (void*)(mmu->m_psd[i].m_psdb & MMU_MATT_ADDRESS_MASK);

            osHwiSwiftEnable();

            return OS_SUCCESS;
        }
    }
    osHwiSwiftEnable();

    return OS_FAIL;
}


/******************************************************************/
os_status osMmuDataMattInfo(void const* virt_addr,void ** phys_addr, uint32_t *size, void **phys_base)
{
    int i, j;
    uint32_t vaddr_val, seg_size = 0;

    osHwiSwiftDisable();
    for(i = 0; i < NUMBER_OF_MMU_DATA_MATT; i++)
    {
        if(!(mmu->m_dsd[i].m_dsda & MMU_MATT_ENABLE)) continue;

        /* get virtual base of this segment */
        if (mmu->m_dsd[i].m_dsdc & MMU_FLEX_SEGMENT_MODEL)
        {
            vaddr_val = mmu->m_dsd[i].m_dsda & 0xFFFFFF00;
            seg_size = 0x100 * (mmu->m_dsd[i].m_dsdc & 0x0000EFFF);
        }
        else
        {
            vaddr_val = mmu->m_dsd[i].m_dsda & 0xFFFFFF80;
            for (j = 7; j < 32; j++)
            {
                if (vaddr_val & 1 << j)
                {
                    vaddr_val &= ~(1 << j);
                    seg_size = (uint32_t)(0x100 << (j - 7));
                    break;
                }
            }
        }

        if(((uint32_t)virt_addr >= vaddr_val) && ((uint32_t)virt_addr < vaddr_val+(seg_size)))
        {
            if(phys_addr)
               *phys_addr = (void*)((mmu->m_dsd[i].m_dsdb & MMU_MATT_ADDRESS_MASK) +
                            (uint32_t)virt_addr - vaddr_val);
            if (size)
                *size = seg_size;

            if (phys_base)
                *phys_base = (void*)(mmu->m_dsd[i].m_dsdb & MMU_MATT_ADDRESS_MASK);

            osHwiSwiftEnable();

            return OS_SUCCESS;
        }
    }
    osHwiSwiftEnable();

    return OS_FAIL;
}



/**************************************************************************/
/*                        DEFAULT MMU INTERRUPT HANDLERS                  */
/**************************************************************************/
void osHwiDMMUError(os_hwi_arg arg)
{
    struct os_mmu_error err = { 0xffffffff,0xffffffff,0xffffffff,
                                0xffffffff,0xffffffff };
    os_sys_info_t        sys_info;
    os_status status;

    VAR_UNUSED(arg);

    status = osSysInfoGet(&sys_info);
    status = osMmuDataErrorDetect(&err);

    if (debug_hook_functions[OS_DEBUG_DATA_MMU])
        debug_hook_functions[OS_DEBUG_DATA_MMU]((debug_hook_arg)&sys_info, (debug_hook_arg)&err);
    else
    {
        /* stop here if there is no debug hook */
        asm("   debug");
    }
 
    osMmuDataErrorClear();
}

/**************************************************************************/
void osHwiIMMUError(os_hwi_arg arg)
{
    struct os_mmu_error err = { 0xffffffff,0xffffffff,0xffffffff,
                                0xffffffff,0xffffffff };
    os_sys_info_t        sys_info;
    os_status status;

    VAR_UNUSED(arg);

    status = osSysInfoGet(&sys_info);
    status = osMmuProgErrorDetect(&err);

    if (debug_hook_functions[OS_DEBUG_PROG_MMU])
        debug_hook_functions[OS_DEBUG_PROG_MMU]((debug_hook_arg)&sys_info, (debug_hook_arg)&err);
    else
    {
        /* stop here if there is no debug hook */
        asm("   debug");
    }
 
    osMmuProgErrorClear();

}


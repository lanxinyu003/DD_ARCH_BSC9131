/******************************************************************************
 Copyright �1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/04/06 18:25:16 $
 $Id: psc9x3x_init.c,v 1.51 2014/04/06 18:25:16 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/psc9x3x/psc9x3x_init.c,v $
 $Revision: 1.51 $
******************************************************************************/

/******************************************************************************

 @File          psc9x3x_init.c

 @Description   PSC9X3X Initialization file.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os.h"
#include "smartdsp_init.h"
#include "smartdsp_os_device.h"
#include "starcore_utils_.h"
#include "os_init.h"
#include "hw_sem_init.h"
#include "psc913x_heterogeneous.h"
#include "os_config.h"
#include "psc9x3x_init.h"

/* platform configuration definition */
extern platform_init_params_t platform_params;
#ifndef PSC9X3X_SC_ONLY_BSP

#ifndef OS_MIMIC_PA_ARCH
#define OS_MIMIC_PA_ARCH OFF
#endif // OS_MIMIC_PA_ARCH

#ifndef OS_MULTIMODE
#define OS_MULTIMODE OFF
#endif // OS_MULTIMODE

os_het_control_t     *g_os_het_control = (os_het_control_t*)(SOC_HET_CTRL_BASE);
#endif  //PSC9X3X_SC_ONLY_BSP


/***********************************************************************//*

    PSC9X3X local memory allocation

*//************************************************************************/
#if ((OS_MULTIMODE == 0) || (OS_HEAP_SIZE > 0))
/* local memory for OS use - the variable name must remain as defined ! */
uint8_t g_heap[ALIGN_SIZE(OS_HEAP_SIZE,8)];
#pragma align g_heap             8
#endif

#ifndef OS_LOCAL_HEAP_MNGMNT_SIZE
#define OS_LOCAL_HEAP_MNGMNT_SIZE OS_LOCAL_HEAP_MNGMNT_DEFAULT_SIZE
#endif

#if OS_LOCAL_HEAP_MNGMNT_SIZE > 0
#ifdef OS_LOCAL_HEAP_MNGMNT_SECTION
uint8_t g_local_mngmnt_space[OS_LOCAL_HEAP_MNGMNT_SIZE] __attribute__ ((aligned (8))) __attribute__ ((section (OS_LOCAL_HEAP_MNGMNT_SECTION)));
#else //default section
uint8_t g_local_mngmnt_space[OS_LOCAL_HEAP_MNGMNT_SIZE] __attribute__ ((aligned (8)));
#endif
#endif

/* local non-cacheable memory for OS use - the variable name must remain as defined ! */
/* g_heap_nocache is forced to be placed inside data segment to keep bss aligment 8   */
#if defined(OS_HEAP_NONCACHEABLE_SIZE) && (OS_HEAP_NONCACHEABLE_SIZE > 0)
uint8_t   g_heap_nocache[OS_HEAP_NONCACHEABLE_SIZE] = { 0 };
#pragma align g_heap_nocache OS_HEAP_NONCACHEABLE_SIZE
uint32_t g_heap_nocache_size=OS_HEAP_NONCACHEABLE_SIZE;
#elif !defined(OS_HEAP_NONCACHEABLE_SIZE)
#define OS_HEAP_NONCACHEABLE_SIZE 0
uint8_t *g_heap_nocache=0;
uint32_t g_heap_nocache_size=0;
#endif

#if defined(OS_HEAP_M3_SIZE) && (OS_HEAP_M3_SIZE > 0)
extern uint8_t g_heap_m3[ALIGN_SIZE(OS_HEAP_M3_SIZE, ARCH_CACHE_LINE_SIZE)];
#if defined(PSC9131)
#error "M3 Not available on PSC9131"
#endif // PSC9131
#elif !defined(OS_HEAP_M3_SIZE)
#define OS_HEAP_M3_SIZE 0
#endif

#if defined(OS_HEAP_DDR0_SIZE) && (OS_HEAP_DDR0_SIZE > 0)
extern uint8_t g_heap_ddr0[ALIGN_SIZE(OS_HEAP_DDR0_SIZE, ARCH_CACHE_LINE_SIZE)];
#ifdef PSC9131
#error "DDR0 Not available on PSC9131"
#endif // PSC9131
#elif !defined(OS_HEAP_DDR0_SIZE)
#define OS_HEAP_DDR0_SIZE 0
#endif

#if defined(OS_HEAP_DDR1_SIZE) && (OS_HEAP_DDR1_SIZE > 0)
extern uint8_t g_heap_ddr1[ALIGN_SIZE(OS_HEAP_DDR1_SIZE, ARCH_CACHE_LINE_SIZE)];
#elif !defined(OS_HEAP_DDR1_SIZE)
#define OS_HEAP_DDR1_SIZE 0
#endif

#if defined(OS_HEAP_M3_NOCACHEABLE_SIZE) && (OS_HEAP_M3_NOCACHEABLE_SIZE > 0)
extern uint8_t  g_heap_m3_nocacheable[ALIGN_SIZE(OS_HEAP_M3_NOCACHEABLE_SIZE,8)];
#if defined(PSC9131)
#error "M3 Not available on PSC9131"
#endif // PSC9131
#elif !defined(OS_HEAP_M3_NOCACHEABLE_SIZE)
#define OS_HEAP_M3_NOCACHEABLE_SIZE 0
#endif

#if defined(OS_HEAP_DDR0_NOCACHEABLE_SIZE) && (OS_HEAP_DDR0_NOCACHEABLE_SIZE > 0)
extern uint8_t g_heap_ddr0_nocacheable[ALIGN_SIZE(OS_HEAP_DDR0_NOCACHEABLE_SIZE,8)];
#ifdef PSC9131
#error "DDR0 Not available on PSC9131"
#endif // PSC9131
#elif !defined(OS_HEAP_DDR0_NOCACHEABLE_SIZE)
#define OS_HEAP_DDR0_NOCACHEABLE_SIZE 0
#endif

#if defined(OS_HEAP_DDR1_NOCACHEABLE_SIZE) && (OS_HEAP_DDR1_NOCACHEABLE_SIZE > 0)
extern uint8_t g_heap_ddr1_nocacheable[ALIGN_SIZE(OS_HEAP_DDR1_NOCACHEABLE_SIZE,8)];
#elif !defined(OS_HEAP_DDR1_NOCACHEABLE_SIZE)
#define OS_HEAP_DDR1_NOCACHEABLE_SIZE 0
#endif

/* check - non-cacheable size must be power of 2    */
#if (OS_HEAP_NONCACHEABLE_SIZE & (OS_HEAP_NONCACHEABLE_SIZE - 1))
#error "Local non-cacheable heap size must be power of 2"
#endif

/* check - non-cacheable size must be more than 256 */
#if (OS_HEAP_NONCACHEABLE_SIZE > 0 && OS_HEAP_NONCACHEABLE_SIZE < 256)
#error "Local non-cacheable heap size must be not less than 256"
#endif

#ifndef OS_L2_CACHE_SIZE
#define OS_L2_CACHE_SIZE 0
#endif

/* Size of L2 cache, from lcf */
extern uint32_t _L2_cache_size;
/* Size of 1st segment, from lcf */
extern uint32_t LocalData_size;
/* Using local variable in order to support different M2/L2 splits for different cores */
const static volatile uint32_t l2_cache_size = OS_L2_CACHE_SIZE;
/* Needed for CPP support */
#ifdef __cplusplus
extern "C"
#endif
const uint32_t __local_data_size = (uint32_t)&LocalData_size;



/* local memory heap list for OS use - the variable name must remain as defined ! */
os_mem_heap_t g_mem_heap_local[] =
{
 
#if ((OS_MULTIMODE == 0) || (OS_HEAP_SIZE > 0))
    (volatile uint32_t*)(g_heap + sizeof(g_heap)),
    sizeof(g_heap),
    OS_MEM_LOCAL,
    (volatile void*)NULL,
    (volatile void*)NULL,
#else // ((OS_MULTIMODE == 1) && (OS_HEAP_SIZE == 0))
    (volatile uint32_t*)NULL,
    0,
    OS_MEM_LOCAL,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif
 
#if OS_HEAP_NONCACHEABLE_SIZE > 0
    (volatile uint32_t*)(g_heap_nocache + OS_HEAP_NONCACHEABLE_SIZE),
    OS_HEAP_NONCACHEABLE_SIZE,
    OS_MEM_LOCAL_NONCACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#elif (OS_MULTIMODE == 1)
    (volatile uint32_t*)NULL,
    0,
    OS_MEM_LOCAL_NONCACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif
 
#if OS_HEAP_M3_SIZE > 0
    (volatile uint32_t*)(g_heap_m3 + sizeof(g_heap_m3)),
    sizeof(g_heap_m3),
    OS_MEM_M3_LOCAL,
    (volatile void*)NULL,
    (volatile void*)NULL,
#elif (OS_MULTIMODE == 1)
    (volatile uint32_t*)NULL,
    0,
    OS_MEM_M3_LOCAL,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif
 
#if OS_HEAP_DDR0_SIZE > 0
    (volatile uint32_t*)(g_heap_ddr0 + sizeof(g_heap_ddr0)),
    sizeof(g_heap_ddr0),
    OS_MEM_DDR0_LOCAL,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if OS_HEAP_DDR1_SIZE > 0
    (volatile uint32_t*)(g_heap_ddr1 + sizeof(g_heap_ddr1)),
    sizeof(g_heap_ddr1),
    OS_MEM_DDR1_LOCAL,
    (volatile void*)NULL,
    (volatile void*)NULL,
#elif (OS_MULTIMODE == 1)
    (volatile uint32_t*)NULL,
    0,
    OS_MEM_DDR1_LOCAL,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if (OS_MULTICORE == OFF)
    0x0, // dummy value, it will be initialized after psc9x3xHetInitialize()
    0x0,
    OS_MEM_HET_DDR1_CACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if (OS_HEAP_M3_NOCACHEABLE_SIZE > 0)
    (volatile uint32_t*)(g_heap_m3_nocacheable + sizeof(g_heap_m3_nocacheable)),
    sizeof(g_heap_m3_nocacheable),
    OS_MEM_M3_LOCAL_NONCACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#elif (OS_MULTIMODE == 1)
    /* Keep entry for re-initialization */
    (volatile uint32_t*)NULL,
    0,
    OS_MEM_M3_LOCAL_NONCACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if OS_HEAP_DDR0_NOCACHEABLE_SIZE > 0
    (volatile uint32_t*)(g_heap_ddr0_nocacheable + sizeof(g_heap_ddr0_nocacheable)),
    sizeof(g_heap_ddr0_nocacheable),
    OS_MEM_DDR0_LOCAL_NONCACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if OS_HEAP_DDR1_NOCACHEABLE_SIZE > 0
    (volatile uint32_t*)(g_heap_ddr1_nocacheable + sizeof(g_heap_ddr1_nocacheable)),
    sizeof(g_heap_ddr1_nocacheable),
    OS_MEM_DDR1_LOCAL_NONCACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#elif (OS_MULTIMODE == 1)
    (volatile uint32_t*)NULL,
    0,
    OS_MEM_DDR1_LOCAL_NONCACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif
 
};


int g_num_of_local_heaps = sizeof(g_mem_heap_local)/sizeof(g_mem_heap_local[0]);

#if (OS_MULTIMODE == 0)

/* background stack               */
#ifndef OS_BACKGROUND_STACK_SIZE
#define OS_BACKGROUND_STACK_SIZE 0x1000
#endif // OS_BACKGROUND_STACK_SIZE

/* background stack for OS use - the variable name must remain as defined ! */
uint8_t           g_background_stack[ALIGN_SIZE(OS_BACKGROUND_STACK_SIZE,8)];
uint32_t          g_background_stack_size = sizeof(g_background_stack);
#pragma align g_background_stack 8
#endif // OS_MULTIMODE

/* main stack (become ISR stack) for OS use - the variable name must remain as defined ! */
uint8_t  StackStart[ALIGN_SIZE(OS_STACK_SIZE,8)];
#pragma align StackStart            8


/***********************************************************************//*

    PSC9X3X device references

*//************************************************************************/
#if (PSC9X3X_DMA == ON)
#include "dma_init.h"
#include "dma.h"
extern soc_dma_config_t         psc9x3x_dma_config;
#endif // PSC9X3X_DMA

#if ((TDM0 == ON) || (TDM1 == ON))
#include "tdm.h"
#include "tdm_init.h"
#endif //(TDM0 == ON) || (TDM0 == ON)

#if (TDM0 == ON)
extern tdm_init_params_t    *tdm0_init_params;
#endif // TDM0

#if (TDM1 == ON)
extern tdm_init_params_t    *tdm1_init_params;
#endif // TDM1

#ifndef SOC_IPC
#define SOC_IPC OFF
#endif //SOC_IPC
#ifndef SOC_IPC_DSP
#define SOC_IPC_DSP OFF
#endif //SOC_IPC_DSP

#if (SOC_IPC == ON) || (SOC_IPC_DSP == ON)
#include "psc913x_ipc_init.h"
#endif //(SOC_IPC == ON) || (SOC_IPC_DSP == ON)
#if (SOC_IPC_DSP == ON)
extern os_ipc_dsp_t* dsp_ipc_init_params;
#endif //SOC_IPC_DSP == ON

#if (AIC == ON)
#include "aic_init.h"
extern aic_init_params_t    *aic_init_params;
#endif // AIC

#if ((MAPLE == ON) || (MAPLE_TVPE == ON) || (MAPLE_CRCPE == ON) || (MAPLE_FTPE_0 == ON) || (MAPLE_TVPE == ON) || (MAPLE_PUFFT == ON) || (MAPLE_EQPE == ON))
#include "maple_init.h"
extern maple_init_params_t maple_init_params;
#endif

#if (MAPLE_DEPE == ON)
_OS_BEGIN_EXTERN_C
os_status mapleDepeChannelDispatch(void *channel, void *jobs, int *num_jobs);
void      mapleDepeChannelReap(void *channel, void *maple);
_OS_END_EXTERN_C

#include "maple_depe_init.h"
extern maple_depe_init_params_t maple_depe_init_params;
#endif

#if (MAPLE_TVPE == ON)
_OS_BEGIN_EXTERN_C
os_status mapleTvpeChannelDispatch(void *channel, void *jobs, int *num_jobs);
void      mapleTvpeChannelReap(void *channel, void *maple);
_OS_END_EXTERN_C

#include "maple_tvpe_init.h"
extern maple_tvpe_init_params_t maple_tvpe_init_params;
#endif

#if (MAPLE_CRCPE == ON)
_OS_BEGIN_EXTERN_C
os_status mapleCrcpeChannelDispatch(void *channel, void *jobs, int *num_jobs);
void      mapleCrcpeChannelReap(void *channel, void *maple);
_OS_END_EXTERN_C

#include "maple_crcpe_init.h"
extern maple_crcpe_init_params_t maple_crcpe_init_params;
#endif

#if (MAPLE_FTPE_0 == ON)
_OS_BEGIN_EXTERN_C
os_status mapleFtpeChannelDispatch(void *channel, void *jobs, int *num_jobs);
void      mapleFtpeChannelReap(void *channel, void *maple);
_OS_END_EXTERN_C

#include "maple_ftpe_init.h"
extern maple_ftpe_init_params_t maple_ftpe_init_params;
#endif  /* (MAPLE_FTPE_0 == ON) */

#if (MAPLE_PUFFT == ON)
_OS_BEGIN_EXTERN_C
os_status maplePufftChannelDispatch(void *channel, void *jobs, int *num_jobs);
void      maplePufftChannelReap(void *channel, void *maple);
_OS_END_EXTERN_C

#include "maple_pufft_init.h"
extern maple_pufft_init_params_t maple_pufft_init_params;
#endif

#if (MAPLE_PUSCH == ON)
_OS_BEGIN_EXTERN_C
os_status maplePuschChannelDispatch(void *channel, void *jobs, int *num_jobs);
void      maplePuschChannelReap(void *channel, void *maple);
_OS_END_EXTERN_C

#include "maple_pusch_init.h"
extern maple_pusch_init_params_t maple_pusch_init_params;
#endif

#if (MAPLE_PDSCH == ON)
_OS_BEGIN_EXTERN_C
os_status maplePdschChannelDispatch(void *channel, void *jobs, int *num_jobs);
void      maplePdschChannelReap(void *channel, void *maple);
_OS_END_EXTERN_C

#include "maple_pdsch_init.h"
extern maple_pdsch_init_params_t maple_pdsch_init_params;
#endif

#if (MAPLE_EQPE == ON)
_OS_BEGIN_EXTERN_C
os_status mapleEqpeChannelDispatch(void *channel, void *jobs, int *num_jobs);
void      mapleEqpeChannelReap(void *channel, void *maple);
_OS_END_EXTERN_C

#include "maple_eqpe_init.h"
extern maple_eqpe_init_params_t maple_eqpe_init_params;
#endif

#if (DEBUG_PRINT_SUPPORT == ON)
#include "psc9x3x_debug_print_init.h"
extern psc9x3x_debug_print_init_params_t psc9x3x_debug_print_init_params;
#endif


#if defined(PSC9132_FAMILY)
#if ((CPRI0 == ON) || (CPRI1 == ON))
#include "cpri_init.h"
extern cpri_init_params_t (*cpri_init_params)[];
extern cpri_global_init_params_t *cpri_global_params;
#endif
#endif // (PSC9132_FAMILY)

#if ((OS_ESPI0 == ON) || (OS_ESPI1 == ON))
#include "spi_init.h"
#endif

/*****************************************************************************/
os_status osArchInitialize()
{
    os_status status;


    status = psc9x3xInitialize( (float)(OS_CLKIN),
                                (void *)SOC_DSP_PLAT_DEFAULT,
                                (void *)SOC_PA_CCSR_DEFAULT,
                                (void *)SOC_DSP_CCSR_DEFAULT,
#ifndef PSC9X3X_SC_ONLY_BSP
                                (void *)SOC_HET_CTRL_BASE,
#else
                                NULL,
#endif
                                DCACHE_ENABLE,
                                ICACHE_ENABLE,
                                L2CACHE_ENABLE,
                                l2_cache_size,
                                &platform_params);

    if(OS_SUCCESS != status)
    {
        OS_ASSERT;
        return status;
    }
 

#ifdef MMU_SUPPORT

#ifndef MMU_DATA_CONTEXT_NUM
#define MMU_DATA_CONTEXT_NUM 12
#endif
#ifndef MMU_PROG_CONTEXT_NUM
#define MMU_PROG_CONTEXT_NUM 12
#endif

    status = osMmuInit(MMU_DATA_CONTEXT_NUM, MMU_PROG_CONTEXT_NUM);
    if(OS_SUCCESS != status)
    {
        OS_ASSERT;
        return status;
    }

#endif
    return status;
}


/*****************************************************************************/
os_status osArchDevicesInitialize()
{
    os_status status = OS_SUCCESS;

#if (PSC9X3X_DMA == ON)
    status = osDmaInitialize((arch_specific *)&psc9x3x_dma_config);
    if (status != OS_SUCCESS)
        return status;
#endif // PSC9X3X_DMA

#if (TDM0 == ON)
    status = tdmInitialize(0, tdm0_init_params);
    if (status != OS_SUCCESS)
        return status;
#endif // TDM0

#if (TDM1 == ON)
    status = tdmInitialize(1, tdm1_init_params);
    if (status != OS_SUCCESS)
        return status;
#endif //TDM1
 
#if (AIC == ON)
    status = aicInitialize((aic_init_params_t *)&aic_init_params);
    OS_RETURN_NO_SUCCESS(status);
#endif // AIC
    
#if defined(PSC9132_FAMILY)
#if ((CPRI0 == ON) || (CPRI1 == ON))
    status = hssiInitialize(NULL);
    if (status != OS_SUCCESS)
        return status;
#endif
#endif // (PSC9132_FAMILY)
 

#if (MAPLE == ON)
    status = mapleInitialize((struct maple_init_params_s *)&maple_init_params.device_init[0], maple_init_params.num_maples);
    if (status != OS_SUCCESS)
        return status;
#endif // MAPLE

#if (MAPLE_DEPE == ON)
#if (MAPLE == OFF)
#error "MAPLE_must be set to ON in order to use PE"
#endif
    /* Default - driver prepares BD and manages ring */
    status = mapleDepeInitialize((maple_depe_init_params_s *)&maple_depe_init_params.pe_init[0],
                                  maple_depe_init_params.num_devices,
                                  mapleDepeChannelDispatch,
                                  mapleDepeChannelReap);
    if (status != OS_SUCCESS)
        return status;
#endif // MAPLE_DEPE

#if (MAPLE_TVPE == ON)
#if (MAPLE == OFF)
#error "MAPLE_must be set to ON in order to use PE"
#endif

    /* Default - driver prepares BD and manages ring */
    status = mapleTvpeInitialize((maple_tvpe_init_params_s *)&maple_tvpe_init_params.pe_init[0],
                                  maple_tvpe_init_params.num_devices,
                                  mapleTvpeChannelDispatch,
                                  mapleTvpeChannelReap);
    if (status != OS_SUCCESS)
        return status;
#endif // MAPLE_TVPE

#if (MAPLE_CRCPE == ON)
#if (MAPLE == OFF)
#error "MAPLE_must be set to ON in order to use PE"
#endif

    status = mapleCrcpeInitialize((maple_crcpe_init_params_s *)&maple_crcpe_init_params.pe_init[0],
                                  maple_crcpe_init_params.num_devices,
                                  mapleCrcpeChannelDispatch,
                                  mapleCrcpeChannelReap);
    if (status != OS_SUCCESS)
        return status;
#endif // MAPLE_CRCPE

#if (MAPLE_FTPE_0 == ON)
#if (MAPLE == OFF)
#error "MAPLE_must be set to ON in order to use PE"
#endif
 
    /* Default - driver prepares BD and manages ring */
    status = mapleFtpeInitialize((maple_ftpe_init_params_s *)&maple_ftpe_init_params.pe_init[0],
                                  maple_ftpe_init_params.num_devices,
                                  mapleFtpeChannelDispatch,
                                  mapleFtpeChannelReap);
    if (status != OS_SUCCESS)
        return status;
#endif  /* (MAPLE_FTPE_0 == ON) */

#if (MAPLE_PUFFT == ON)
    /* Default - driver prepares BD and manages ring */
    status = maplePufftInitialize((maple_pufft_init_params_s *)&maple_pufft_init_params.pe_init[0],
                                  maple_pufft_init_params.num_devices,
                                  maplePufftChannelDispatch,
                                  maplePufftChannelReap);
    if (status != OS_SUCCESS)
        return status;
#endif
 
#if (MAPLE_PDSCH == ON)
#if (MAPLE == OFF)
#error "MAPLE_must be set to ON in order to use PE"
#endif

    /* Default - driver prepares BD and manages ring */
    status = maplePdschInitialize((maple_pdsch_init_params_s *)&maple_pdsch_init_params.pe_init[0],
                                  maple_pdsch_init_params.num_devices,
                                  maplePdschChannelDispatch,
                                  maplePdschChannelReap);
    if (status != OS_SUCCESS)
        return status;
#endif // MAPLE_PDSCH
 
#if (MAPLE_PUSCH == ON)
#if (MAPLE == OFF)
#error "MAPLE_must be set to ON in order to use PE"
#endif
    /* Default - driver prepares BD and manages ring */
    status = maplePuschInitialize((maple_pusch_init_params_s *)&maple_pusch_init_params.pe_init[0],
                                  maple_pusch_init_params.num_devices,
                                  maplePuschChannelDispatch,
                                  maplePuschChannelReap);
    if (status != OS_SUCCESS)
        return status;
#endif

#if (MAPLE_EQPE == ON)
#if (MAPLE == OFF)
#error "MAPLE_must be set to ON in order to use PE"
#endif
    /* Default - driver prepares BD and manages ring */
    status = mapleEqpeInitialize((maple_eqpe_init_params_s *)&maple_eqpe_init_params.pe_init[0],
                                  maple_eqpe_init_params.num_devices,
                                  mapleEqpeChannelDispatch,
                                  mapleEqpeChannelReap);
    if (status != OS_SUCCESS)
        return status;
#endif // MAPLE_EQPE

#if (DEBUG_PRINT_SUPPORT == ON)
    status = psc9x3xDebugPrintInitialize(&psc9x3x_debug_print_init_params);
    if (status != OS_SUCCESS)
           return status;
#endif // DEBUG_PRINT_SUPPORT

#if defined(PSC9132_FAMILY)
#if (CPRI0 == ON || CPRI1 == ON)
    status = cpriInitialize(cpri_global_params, cpri_init_params);
    if (status != OS_SUCCESS)
        return status;
#endif
#endif //PSC9132_FAMILY
 
#if (SOC_IPC_DSP == ON)
    status = osIpcInit((void*) &dsp_ipc_init_params);
    if (status != OS_SUCCESS)
        return status;
#elif (SOC_IPC == ON)
    status = osIpcInit(NULL);
    if (status != OS_SUCCESS)
        return status;
#endif //SOC_IPC_DSP
 
#ifndef PSC9X3X_SC_ONLY_BSP
    // HW semaphores must be initialized when running with PA
    status = hwSemaphoreInitialize();
    if (status != OS_SUCCESS)
        return status;
#endif
 
#if (OS_MIMIC_PA_ARCH == OFF)
    /* Take hw semaphore, PA should release it */
    psc9x3xHetBootInitialize();
#endif

#if (OS_ESPI0 == ON)
    status = spiInitialize(ESPI_ID0, &spi_init_params[0]);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_ESPI0

#if (OS_ESPI1 == ON)
    status = spiInitialize(ESPI_ID1, &spi_init_params[1]);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_ESPI1

#ifdef PSC9131
#if (OS_ESPI2 == ON)
    status = spiInitialize(ESPI_ID2, &spi_init_params[2]);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_ESPI2

#if (OS_ESPI3 == ON)
    status = spiInitialize(ESPI_ID3, &spi_init_params[3]);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_ESPI3

#endif //PSC9131
 
    return status;
}

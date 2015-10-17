/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/08/24 16:27:07 $
 $Id: os_mem_init.c,v 1.75 2014/08/24 16:27:07 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_mem_init.c,v $
 $Revision: 1.75 $
******************************************************************************/

/******************************************************************************

 @File          os_mem_init.c

 @Description   OS Memory Manager initialization functions.

 @Cautions      None.

*//***************************************************************************/
#include "smartdsp_os_.h"
#include "os_init.h"
#include "os_mem_shared_.h"
#include "os_l1_defense.h"

#include <string.h>

#define FREE_LIST_SLOT          (0x00000001)
#define ALIGNED_MINIMAL         ALIGNED_8_BYTES

#define IS_POWER_OF_2(_val) \
    (((((uint32_t)_val) != 0) && ((((uint32_t)_val) & (~((uint32_t)_val) + 1)) == ((uint32_t)_val))))

struct mem_slot_t
{
    volatile struct mem_slot_t * volatile next;
 
    volatile uint32_t base;
    volatile uint32_t end;
};

#if (OS_MULTICORE == ON)
os_mem_heap_t *g_os_mem_heap_shared;
int  g_os_num_of_shared_heaps;
#endif // OS_MULTICORE

os_mem_heap_t *g_os_mem_heap_local;
int  g_os_num_of_local_heaps;

uint16_t        g_total_mem_parts;
os_mem_part_t   **g_mem_part_list = NULL;

uint8_t * gs_local_mngmnt_space;
#if OS_MULTICORE == ON
uint8_t * gs_shared_mngmnt_space[OS_SOC_MAX_NUM_OF_CORES] __attribute__((section(".os_shared_data_bss")));
#endif

uint32_t g_local_heap_mngmnt_size;
#if OS_MULTICORE == ON
uint32_t g_shared_heap_mngmnt_size[OS_SOC_MAX_NUM_OF_CORES] __attribute__((section(".os_shared_data_bss")));
#endif

/*****************************************************************************/
os_status osMemLocalHeapSet(os_virt_ptr mem_ptr, uint32_t mem_size, os_mem_type type)
{
    int i = 0;
 
    OS_ASSERT_COND(g_os_mem_heap_local != NULL);
    OS_ASSERT_COND(IS_ALIGNED(mem_size, ALIGNED_MINIMAL));
    OS_ASSERT_COND(IS_ALIGNED(mem_ptr,  ALIGNED_MINIMAL));
    if (type & OS_MEM_CACHEABLE_TYPE)
    {
        OS_ASSERT_COND(IS_ALIGNED(mem_size, ARCH_CACHE_LINE_SIZE));
        OS_ASSERT_COND(IS_ALIGNED(mem_ptr,  ARCH_CACHE_LINE_SIZE));
    }
 
    for (i = 0; i < g_os_num_of_local_heaps; i++)
    {
        if (g_os_mem_heap_local[i].mem_type == type)
        {
            g_os_mem_heap_local[i].mem_start = (volatile uint32_t *)((uint8_t *)mem_ptr + mem_size);
            g_os_mem_heap_local[i].mem_size  = mem_size;
            return OS_SUCCESS;
        }
    }
    return OS_FAIL;
}

/*****************************************************************************/
os_status osMemInitialize(os_mem_heap_t os_mem_heap_local[], int num_of_local_heaps
#if (OS_MULTICORE == ON)
		, os_mem_heap_t os_mem_heap_shared[], int  num_of_shared_heaps
#endif
		)
{
	if(os_mem_heap_local == NULL)
	{
		OS_ASSERT;
		return OS_FAIL;
	}
    g_os_mem_heap_local  = os_mem_heap_local;
    g_os_num_of_local_heaps = num_of_local_heaps;
 
#if (OS_MULTICORE == ON)
	if(os_mem_heap_shared == NULL)
	{
		OS_ASSERT;
		return OS_FAIL;
	}
    g_os_mem_heap_shared  = os_mem_heap_shared;
    g_os_num_of_shared_heaps = num_of_shared_heaps;
#endif
 
    return OS_SUCCESS;
}

os_status osMemLocalInitialize(uint16_t  total_mem_parts, uint8_t * local_mngmnt_space, uint32_t local_mngmnt_size)
{
    int i;
    struct mem_slot_t * slot;
    gs_local_mngmnt_space = local_mngmnt_space;
    g_local_heap_mngmnt_size = local_mngmnt_size;
 
    slot = (struct mem_slot_t *)gs_local_mngmnt_space;
    while((((uint32_t)slot) + sizeof(struct mem_slot_t)) < ( ((uint32_t)gs_local_mngmnt_space) + g_local_heap_mngmnt_size) )
    {
        slot->base = FREE_LIST_SLOT;
        slot++;
    }
 
    for(i = 0; i < g_os_num_of_local_heaps; i++)
    {
        if(!IS_ALIGNED(g_os_mem_heap_local[i].mem_start, 8))
        {
            OS_ASSERT;
            return OS_FAIL;
        }
 
        if(g_os_mem_heap_local[i].mem_start != NULL && g_os_mem_heap_local[i].mem_size != 0)
        {
            osHeapInit(g_os_mem_heap_local[i].mem_start, g_os_mem_heap_local[i].mem_size, (os_mem_type)g_os_mem_heap_local[i].mem_type);
        }
    }

    /* prepare memory partition lookup */
    g_total_mem_parts = total_mem_parts;

    /* allocate memory partitions list */
    if (g_total_mem_parts > 0)
    {
        g_mem_part_list = (os_mem_part_t **)
            osMalloc((sizeof(os_mem_part_t *) * g_total_mem_parts), OS_MEM_LOCAL);

        if (g_mem_part_list == NULL)
        {
            RETURN_ERROR(OS_ERR_NO_MEMORY);
        }
    }

    return OS_SUCCESS;
}

#if (OS_MULTICORE == ON)
os_status osMemSharedInitialize(uint8_t * shared_mngmnt_space, uint32_t shared_mngmnt_size)
{
    int i;
    uint32_t l1d_mode = osL1dGetResetMode();
    gs_shared_mngmnt_space[osGetCoreID()] = shared_mngmnt_space;
    g_shared_heap_mngmnt_size[osGetCoreID()] = shared_mngmnt_size;

    if(((l1d_mode == OS_L1D_NO_WARM_RESET) || (l1d_mode == OS_L1D_MODE_3)))
    {
        if(osGetCoreID() == osGetMasterCore())
        {
            volatile struct mem_slot_t * slot = (volatile struct mem_slot_t *)gs_shared_mngmnt_space[osGetCoreID()];
 
            osSpinLockInitialize(&g_os_mem_guard);
 
            while((((uint32_t)slot) + sizeof(struct mem_slot_t)) < ( ((uint32_t)gs_shared_mngmnt_space[osGetCoreID()]) + g_shared_heap_mngmnt_size[osGetCoreID()]) )
            {
                slot->base = FREE_LIST_SLOT;
                slot++;
            }
        }
 
        for(i = 0; i < g_os_num_of_shared_heaps; i++)
        {
            if(!IS_ALIGNED(g_os_mem_heap_shared[i].mem_start, 8))
            {
                OS_ASSERT;
                return OS_FAIL;
            }
 
            if((g_os_mem_heap_shared[i].mem_type & OS_MEM_CACHEABLE_TYPE) &&
                !IS_ALIGNED(g_os_mem_heap_shared[i].mem_start, ARCH_CACHE_LINE_SIZE))
            {
                OS_ASSERT;
                return OS_FAIL;
            }
 
 
            if(osGetCoreID() == osGetMasterCore())
            {
                if(g_os_mem_heap_shared[i].mem_start != NULL && g_os_mem_heap_shared[i].mem_size != 0)
                {
                    osHeapInit(g_os_mem_heap_shared[i].mem_start, g_os_mem_heap_shared[i].mem_size, (os_mem_type)g_os_mem_heap_shared[i].mem_type);
                }
            }
        }
 
        if(osGetCoreID() == osGetMasterCore())
        {
#ifdef SC39XX
            DBAR_HWSYNC();
#endif
            g_heap_shared_initiated = TRUE;
        }
 
        while(g_heap_shared_initiated == FALSE)
        {
#ifdef SC39XX
            DBAR_HWSYNC();
#endif
        }
    } //((l1d_mode == OS_L1D_NO_WARM_RESET) || (l1d_mode == OS_L1D_MODE_3))
 
    return OS_SUCCESS;
}
#endif // OS_MULTICORE

void * _osAlignedMalloc(uint32_t size, os_mem_type type, uint32_t alignment)
{
    void * addr = _mmGet(size,type,alignment);
    if(addr != NULL)
    {
        memset(addr, 0, size);
    }
 
    return addr;
}

/* Stuff for when (OS_PRINT_MALLOC_STATISTICS == 1) */

static inline char *__utoa(os_mem_type type)
{
    static char _str [] = "0x00000000 (Unrecognized)";
    uint32_t i;
    uint32_t _type = (uint32_t)type;

    /* reset to original all 0 value */
    for (i=2 ; i<10 ; i++)
        _str[i] = '0';

    i=9; // offsett to last nibble in _str[];
    while(_type)
    {
        uint32_t nibble = _type & 0xF;
        _type >>= 4;
        _str[i--] = (char)((nibble>9) ? ('A'+(nibble-10)) : ('0'+nibble));
    }

    return _str;
}

static inline char * __mem_to_str(os_mem_type type)
{
    switch (type)
    {
        case OS_MEM_LOCAL                   : return "OS_MEM_LOCAL";
        case OS_MEM_LOCAL_NONCACHEABLE      : return "OS_MEM_LOCAL_NONCACHEABLE";
        case OS_MEM_SHARED                  : return "OS_MEM_SHARED";
        case OS_MEM_QE_PRAM                 : return "OS_MEM_QE_PRAM";
        case OS_MEM_MAPLE_PRAM              : return "OS_MEM_MAPLE_PRAM";
        case OS_MEM_M2_SHARED_CACHEABLE     : return "OS_MEM_M2_SHARED_CACHEABLE";
        case OS_MEM_M3_LOCAL                : return "OS_MEM_M3_LOCAL";
        case OS_MEM_M3_LOCAL_NONCACHEABLE   : return "OS_MEM_M3_LOCAL_NONCACHEABLE";
        case OS_MEM_M3_SHARED               : return "OS_MEM_M3_SHARED";
        case OS_MEM_M3_SHARED_CACHEABLE     : return "OS_MEM_M3_SHARED_CACHEABLE";
        case OS_MEM_DDR0_LOCAL              : return "OS_MEM_DDR0_LOCAL";
        case OS_MEM_DDR0_LOCAL_NONCACHEABLE : return "OS_MEM_DDR0_LOCAL_NONCACHEABLE";
        case OS_MEM_DDR0_SHARED             : return "OS_MEM_DDR0_SHARED";
        case OS_MEM_DDR0_SHARED_CACHEABLE   : return "OS_MEM_DDR0_SHARED_CACHEABLE";
        case OS_MEM_DDR1_LOCAL              : return "OS_MEM_DDR1_LOCAL";
        case OS_MEM_DDR1_LOCAL_NONCACHEABLE : return "OS_MEM_DDR1_LOCAL_NONCACHEABLE";
        case OS_MEM_DDR1_SHARED             : return "OS_MEM_DDR1_SHARED";
        case OS_MEM_DDR1_SHARED_CACHEABLE   : return "OS_MEM_DDR1_SHARED_CACHEABLE";
        case OS_MEM_HET_DDR1_CACHEABLE      : return "OS_MEM_HET_DDR1_CACHEABLE";
        case OS_MEM_HET_DDR0_CACHEABLE      : return "OS_MEM_HET_DDR0_CACHEABLE";
        case OS_MEM_MAPLE0_BD_RING          : return "OS_MEM_MAPLE0_BD_RING";
        case OS_MEM_MAPLE1_BD_RING          : return "OS_MEM_MAPLE1_BD_RING";
        case OS_MEM_MAPLE2_BD_RING          : return "OS_MEM_MAPLE2_BD_RING";
        default                             : return __utoa(type);
    }
}

#if (OS_PRINT_MALLOC_STATISTICS == 1)
FILE * __os_malloc_file = NULL;

void * __os_malloc_msg(uint32_t size, os_mem_type type, uint32_t alignment, const char * file, int line);
void * __os_malloc_msg(uint32_t size, os_mem_type type, uint32_t alignment, const char * file, int line)
{
	void *ret;
    if (__os_malloc_file == NULL)
        __os_malloc_file = stdout;
 
    ret = _osAlignedMalloc(size, type, alignment);
    fprintf(__os_malloc_file, "Core %d. File: %s. Line: %d. osMalloc(%d,%s) returned 0x%08X\n", osGetCoreID(), file, line, size, __mem_to_str(type), ret);

    return ret;
}

void * __os_aligned_malloc_msg(uint32_t size, os_mem_type type, uint32_t alignment, const char * file, int line);
void * __os_aligned_malloc_msg(uint32_t size, os_mem_type type, uint32_t alignment, const char * file, int line)
{
	void *ret;
    if (__os_malloc_file == NULL)
        __os_malloc_file = stdout;
    ret = _osAlignedMalloc(size, type, alignment);
    fprintf(__os_malloc_file, "Core %d. File: %s. Line: %d. osAlignedMalloc(%d,%s,%d) returned 0x%08X\n", osGetCoreID(), file, line, size, __mem_to_str(type), alignment, ret);

    return ret;
}

void * __os_fast_malloc_msg(uint32_t size, os_mem_type type, const char * file, int line);
void * __os_fast_malloc_msg(uint32_t size, os_mem_type type, const char * file, int line)
{
	void *ret;
    if (__os_malloc_file == NULL)
        __os_malloc_file = stdout;
    ret = _mmGet(size, type, ALIGNED_8_BYTES);
    fprintf(__os_malloc_file, "Core %d. File: %s. Line: %d. osFastMalloc(%d,%s) returned 0x%08X\n", osGetCoreID(), file, line, size, __mem_to_str(type), ret);

    return ret;
}

void * __os_fast_aligned_malloc_msg(uint32_t size, os_mem_type type, uint32_t alignment, const char * file, int line);
void * __os_fast_aligned_malloc_msg(uint32_t size, os_mem_type type, uint32_t alignment, const char * file, int line)
{
	void *ret;
    if (__os_malloc_file == NULL)
        __os_malloc_file = stdout;
    ret = _mmGet(size,type,alignment);
    fprintf(__os_malloc_file, "Core %d. File: %s. Line: %d. osFastAlignedMalloc(%d,%s,%d) returned 0x%08X\n", osGetCoreID(), file, line, size, __mem_to_str(type), alignment, ret);

    return ret;
}

os_status __os_free_msg(void * addr, const char * file, int line, const char * ptr);
os_status __os_free_msg(void * addr, const char * file, int line, const char * ptr)
{
    if (__os_malloc_file == NULL)
        __os_malloc_file = stdout;

    fprintf(__os_malloc_file, "Core %d. File: %s. Line: %d. osFree(%s) address 0x%08X\n", osGetCoreID(), file, line, ptr, addr);
 
    return _mmPut(addr);
}
#endif //OS_PRINT_MALLOC_STATISTICS

INLINE void _getMemGuard(os_mem_type type)
{
#if (OS_MULTICORE == ON)
    if (type & OS_MEM_SHARED_TYPE)
    {
        osSpinLockIrqGet(&g_os_mem_guard);
    }
    else
    {
#endif
    	VAR_UNUSED(type);
        osHwiSwiftDisable();
#if (OS_MULTICORE == ON)
    }
#endif
}

INLINE void _releaseMemGuard(os_mem_type type)
{
#if (OS_MULTICORE == ON)
    if (type & OS_MEM_SHARED_TYPE)
    {
        osSpinLockIrqRelease(&g_os_mem_guard);
    }
    else
    {
#endif
        VAR_UNUSED(type);
        osHwiSwiftEnable();
#if (OS_MULTICORE == ON)
    }
#endif
}

/*
 * calculate the correct slot size, taking barriers into account.
 */
INLINE uint32_t _calcSlotSize(volatile struct mem_slot_t * slot_ptr)
{
    OS_ASSERT_COND(slot_ptr != NULL);
    OS_ASSERT_COND(slot_ptr->end > slot_ptr->base);
 
    return (slot_ptr->end - slot_ptr->base);
}

/*
 * this function is used to release the list node (mem_slot_t) from usage
 * later a new list node (mem_slot_t) will take its place
 */
INLINE os_status _releseMemSlotNode(volatile struct mem_slot_t * slot)
{
    if(slot == NULL)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return OS_FAIL;
    }
 
    slot->base = FREE_LIST_SLOT;
 
    return OS_SUCCESS;
}

INLINE volatile struct mem_slot_t * _getMemSlotNode(os_mem_type type)
{
    volatile struct mem_slot_t * node = NULL;
#if (OS_MULTICORE == ON)
    uint32_t start_addr = (uint32_t) ((type & OS_MEM_SHARED_TYPE) ? (uint32_t)gs_shared_mngmnt_space[osGetCoreID()] : (uint32_t)gs_local_mngmnt_space);
    uint32_t mngmnt_size = (uint32_t) ((type & OS_MEM_SHARED_TYPE) ? g_shared_heap_mngmnt_size[osGetCoreID()] : g_local_heap_mngmnt_size);
#else
    uint32_t start_addr = (uint32_t) gs_local_mngmnt_space;
    uint32_t mngmnt_size = (uint32_t) g_local_heap_mngmnt_size;
    VAR_UNUSED(type);
#endif
 
#ifdef MEM_ERROR_ASSERT
    OS_ASSERT_COND(osMemTypeIsValid(type));
    OS_ASSERT_COND(start_addr != NULL);
    OS_ASSERT_COND(mngmnt_size > 0);
#endif
 
    node = (volatile struct mem_slot_t *)start_addr;
 
    while( !(node->base & FREE_LIST_SLOT) && (((uint32_t)node) < (start_addr + mngmnt_size)))
    {
        node++;
    }

    if((uint32_t)node >= (start_addr + mngmnt_size))
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return NULL;
    }
    node->base &= ~FREE_LIST_SLOT;
 
    return node;
}

INLINE os_mem_heap_t * _getHeapFromMemType(os_mem_type type)
{
    uint8_t i;
    os_mem_heap_t* heap_list = g_os_mem_heap_local;
    uint32_t heap_num = (uint32_t) g_os_num_of_local_heaps;
 
    OS_ASSERT_COND(osMemTypeIsValid(type));
 
#if (OS_MULTICORE == ON)
    if (type & OS_MEM_SHARED_TYPE)
    {
        heap_list   = g_os_mem_heap_shared;
        heap_num    = (uint32_t)g_os_num_of_shared_heaps;
    }
#endif
 
    for (i = 0; i < heap_num; i++)
    {
        if (heap_list[i].mem_type == type)
        {
            return &heap_list[i];
        }
    }

#ifdef MEM_ERROR_ASSERT
    OS_ASSERT;
#endif

    return NULL;
}

uint32_t _mmGetMngmntSize(os_mem_type type);
uint32_t _mmGetTotalMngmntSize(os_mem_type type);

void osDumpMemory(os_mem_type type)
{
    double maxBlockSize;
    double totalFreeSpace;
    double fragmentation = 0;
    volatile struct mem_slot_t * busy_slot;
    volatile struct mem_slot_t * free_slot;
    os_mem_heap_t * heap;
 
    /* temp saved data */
    #define MAX_NUM_OF_MEM_SLOTS 30
    uint32_t i = 0;
    uint32_t start_addr[MAX_NUM_OF_MEM_SLOTS] = {0};
    uint32_t end_addr[MAX_NUM_OF_MEM_SLOTS]   = {0};
    bool isSlotBusy[MAX_NUM_OF_MEM_SLOTS]     = {0};
 
    if(!osMemTypeIsValid(type))
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return;
    }
 
    heap = _getHeapFromMemType(type);
    if(heap == NULL)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return;
    }

    maxBlockSize = (double) osGetMaxBlockSize(type, ALIGNED_MINIMAL);
    totalFreeSpace = (double) osGetFreeMemSize(type);

    _getMemGuard(type);
 
    busy_slot = (volatile struct mem_slot_t *) heap->busy_list_head;
    free_slot = (volatile struct mem_slot_t *) heap->free_list_head;

    while(busy_slot || free_slot)
    {
        if(busy_slot)
        {
            if(free_slot)
            {
                if(free_slot->base < busy_slot->base)
                {
                    start_addr[i] = free_slot->base;
                    end_addr[i] = free_slot->end;
                    isSlotBusy[i] = FALSE;
                    free_slot = free_slot->next;
                }
                else
                {
                    start_addr[i] = busy_slot->base;
                    end_addr[i] = busy_slot->end;
                    isSlotBusy[i] = TRUE;
                    busy_slot = busy_slot->next;
                }
            }
            else
            {
                start_addr[i] = busy_slot->base;
                end_addr[i] = busy_slot->end;
                isSlotBusy[i] = TRUE;
                busy_slot = busy_slot->next;
            }
        }
        else
        {
            start_addr[i] = free_slot->base;
            end_addr[i] = free_slot->end;
            isSlotBusy[i] = FALSE;
            free_slot = free_slot->next;
        }
 
        i++;
 
        if(i >= MAX_NUM_OF_MEM_SLOTS) break;
    }
 
    _releaseMemGuard(type);
 
    printf("\n Memory dump\n");
    printf(  "-------------\n");
    printf("Heap type: %s\n", __mem_to_str(type));
    printf("Free space: %ul Bytes\n", (uint32_t)totalFreeSpace);
    printf("Management size: %ul Bytes\n", _mmGetMngmntSize(type));
    printf("Total Management size (local/shared): %ul Bytes\n", _mmGetTotalMngmntSize(type));
 
    if(totalFreeSpace != 0)
    {
        fragmentation = 1.0 - (maxBlockSize / totalFreeSpace);
    }
 
    if(fragmentation < 0.001) fragmentation = 0;
    printf("Fragmentation percentage: %3.1f%% \n", fragmentation * 100);
 
    printf("max block size: %ul Bytes\n", (uint32_t)maxBlockSize);
    printf("Memory map:\n");
    for(i=0; i<MAX_NUM_OF_MEM_SLOTS; i++)
    {
        if(end_addr[i] == 0) break;
        if(isSlotBusy[i] == TRUE)
            printf("\t0x%x - 0x%x: %ul Bytes \tbusy\n", start_addr[i], end_addr[i], end_addr[i] - start_addr[i]);
        else
            printf("\t0x%x - 0x%x: %ul Bytes \tfree\n", start_addr[i], end_addr[i], end_addr[i] - start_addr[i]);
    }
 
    if(i == MAX_NUM_OF_MEM_SLOTS)
        printf("\tMore slots may exist ...\n");
 
    printf("\n");
 
    #undef MAX_NUM_OF_MEM_SLOTS
}

INLINE volatile struct mem_slot_t * _createSlot(os_mem_type type, uint32_t base, uint32_t size)
{
    volatile struct mem_slot_t * free_block;
 
    if(!osMemTypeIsValid(type))
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return NULL;
    }
 
    if(size == 0 || base == 0)
    {
        return NULL;
    }
 
    free_block = _getMemSlotNode(type);
 
    if ( !free_block )
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return NULL;
    }

    free_block->base = base;
    free_block->end = base + size;
    free_block->next = NULL;

    return free_block;
}

os_status osHeapInit(volatile uint32_t * mem_ptr, volatile uint32_t mem_size, os_mem_type type)
{
    os_mem_heap_t * heap;
    volatile uint32_t new_base, new_end;
    volatile struct mem_slot_t * new_slot;
 
    if(mem_ptr == NULL || mem_size == 0)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return OS_FAIL;
    }

    if(!osMemTypeIsValid(type))
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return OS_FAIL;
    }
 
    heap = _getHeapFromMemType(type);
    if(heap == NULL)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return OS_FAIL;
    }
 
    heap->busy_list_head = NULL;

    new_base = ALIGN_ADDRESS(((uint32_t)mem_ptr) - mem_size, ALIGNED_MINIMAL );
    if(new_base >= (uint32_t)mem_ptr)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return OS_FAIL;
    }
 
    new_end = (volatile uint32_t) mem_ptr;
 
    _getMemGuard(type);
 
    if(new_base < new_end)
    {
        new_slot = _getMemSlotNode(type);
        if(new_slot == NULL)
        {
#ifdef MEM_ERROR_ASSERT
            OS_ASSERT;
#endif
            return OS_FAIL;
        }
 
        new_slot->base = new_base;
        new_slot->end = new_end;
        new_slot->next = NULL;
 
        heap->free_list_head = (volatile void *)new_slot;
    }
    else
    {
        heap->free_list_head = NULL;
    }

    _releaseMemGuard(type);
 
    return OS_SUCCESS;
}

static uint32_t _mmGetTotalMngmntSize(os_mem_type type)
{
    struct mem_slot_t * slot;
    uint32_t mngmnt_size = 0;
 
 
 
#if OS_MULTICORE == 1
    if(((uint32_t)type) & OS_MEM_SHARED_TYPE)
    {
        _getMemGuard(type);
 
        slot = (struct mem_slot_t *)gs_shared_mngmnt_space[osGetCoreID()];
        while((((uint32_t)slot) + sizeof(struct mem_slot_t)) < ( ((uint32_t)gs_shared_mngmnt_space[osGetCoreID()]) + g_shared_heap_mngmnt_size[osGetCoreID()]) )
        {
            if(!(slot->base & FREE_LIST_SLOT))
            {
                mngmnt_size += sizeof(struct mem_slot_t);
            }
            slot++;
        }
 
        _releaseMemGuard(type);
    }
    else
#else
        VAR_UNUSED(type);
#endif
    {
        _getMemGuard(type);
 
        slot = (struct mem_slot_t *)gs_local_mngmnt_space;
        while((((uint32_t)slot) + sizeof(struct mem_slot_t)) < ( ((uint32_t)gs_local_mngmnt_space) + g_local_heap_mngmnt_size) )
        {
            if(!(slot->base & FREE_LIST_SLOT))
            {
                mngmnt_size += sizeof(struct mem_slot_t);
            }
            slot++;
        }
 
        _releaseMemGuard(type);
    }
 
    return mngmnt_size;
}

static uint32_t _mmGetMngmntSize(os_mem_type type)
{
    volatile struct mem_slot_t * list_node;
    uint32_t mngmnt_size = 0;
    os_mem_heap_t* heap;

    OS_ASSERT_COND(osMemTypeIsValid(type));
 
    heap = _getHeapFromMemType(type);
    if(heap == NULL)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return 0;
    }

    _getMemGuard(type);
 
    list_node = (volatile struct mem_slot_t *)heap->free_list_head;
 
    while(list_node != NULL)
    {
        mngmnt_size += sizeof(struct mem_slot_t);
        list_node = (volatile struct mem_slot_t *) list_node->next;
    }
 
    list_node = (volatile struct mem_slot_t *)heap->busy_list_head;
 
    while(list_node != NULL)
    {
        mngmnt_size += sizeof(struct mem_slot_t);
        list_node = (volatile struct mem_slot_t *) list_node->next;
    }
 
    _releaseMemGuard(type);
 
    return mngmnt_size;
}

uint32_t osGetFreeMemSize(os_mem_type type)
{
    volatile struct mem_slot_t * free_list_node;
    uint32_t free_size = 0;
    os_mem_heap_t* heap;
 
    OS_ASSERT_COND(osMemTypeIsValid(type));
 
    heap = _getHeapFromMemType(type);
    if(heap == NULL)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return 0;
    }
 
    _getMemGuard(type);
 
    free_list_node = (volatile struct mem_slot_t *)heap->free_list_head;
 
    while(free_list_node != NULL)
    {
        free_size += _calcSlotSize(free_list_node);
        free_list_node = (volatile struct mem_slot_t *) free_list_node->next;
    }
 
    _releaseMemGuard(type);
 
    return free_size;
}

// returns the maximum consecutive free memory block size aligned to requested alignment
uint32_t osGetMaxBlockSize(os_mem_type type, uint32_t alignment)
{
    uint32_t max_free_size = 0;
    uint32_t free_size = 0;
    volatile struct mem_slot_t * slot_i = NULL;
    os_mem_heap_t* heap;
 
    if(!osMemTypeIsValid(type))
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return 0;
    }
 
    if(!IS_POWER_OF_2(alignment))
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return 0;
    }
 
    heap = _getHeapFromMemType(type);
    if(heap == NULL)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return 0;
    }

    _getMemGuard(type);
 
    slot_i = (volatile struct mem_slot_t *)heap->free_list_head;

    while(slot_i != NULL)
    {
        uint32_t alignedBase = ALIGN_ADDRESS(slot_i->base, alignment);
        if((slot_i->end > alignedBase) && (slot_i->base <= alignedBase))
        {
            uint32_t size_align = (type & OS_MEM_CACHEABLE_TYPE) ? ARCH_CACHE_LINE_SIZE : ALIGNED_MINIMAL;
            free_size = ALIGN_DOWN_SIZE(slot_i->end - alignedBase, size_align);
 
            if(free_size > max_free_size)
            {
                max_free_size = free_size;
            }
        }
        slot_i = slot_i->next;
    }
 
    _releaseMemGuard(type);
 
    return max_free_size;
}

static os_status _addBusy(os_mem_heap_t * heap, volatile struct mem_slot_t * new_busy_block )
{
    volatile struct mem_slot_t *curr_busy_block, *prev_busy_block;

    OS_ASSERT_COND(heap != NULL);
    OS_ASSERT_COND(new_busy_block != NULL);
    OS_ASSERT_COND(new_busy_block->end > new_busy_block->base);
 
    /* finds a place of a new busy block in the list of busy blocks */
    prev_busy_block = NULL;
    curr_busy_block = heap->busy_list_head;

    while ( curr_busy_block && new_busy_block->base > curr_busy_block->base )
    {
        prev_busy_block = curr_busy_block;
        curr_busy_block = curr_busy_block->next;
    }

    /* insert the new busy block into the list of busy blocks */
    if ( curr_busy_block )
    {
        new_busy_block->next = curr_busy_block;
    }
 
    if ( prev_busy_block )
    {
        prev_busy_block->next = new_busy_block;
    }
    else
    {
        heap->busy_list_head = new_busy_block;
    }
 
    return OS_SUCCESS;
}

static os_status _addFreeBlock( os_mem_heap_t * heap, uint32_t base, uint32_t end )
{
    volatile struct mem_slot_t *curr_free_block, *prev_free_block, *new_free_block;
 
    OS_ASSERT_COND(heap != NULL);
    OS_ASSERT_COND(base != 0);
    OS_ASSERT_COND(end > base);
 
    prev_free_block = new_free_block = NULL;
    curr_free_block = heap->free_list_head;

    while ( curr_free_block && base > curr_free_block->base )
    {
        prev_free_block = curr_free_block;
        curr_free_block = curr_free_block->next;
    }

    if ( curr_free_block != NULL)
    {
        if(curr_free_block->base - end < ALIGNED_MINIMAL)
        {
            curr_free_block->base = base;
        }
        else
        {
            new_free_block = _createSlot((os_mem_type)heap->mem_type,base, end - base);
            if(new_free_block == NULL)
            {
#ifdef MEM_ERROR_ASSERT
                OS_ASSERT;
#endif
                return OS_FAIL;
            }
            new_free_block->next = curr_free_block;
        }
    }
    else
    {
        new_free_block = _createSlot((os_mem_type)heap->mem_type,base, end - base);
        if(new_free_block == NULL)
        {
#ifdef MEM_ERROR_ASSERT
            OS_ASSERT;
#endif
            return OS_FAIL;
        }
    }
 
    if ( prev_free_block )
    {
        if(base - prev_free_block->end < ALIGNED_MINIMAL)
        {
            volatile struct mem_slot_t *block = (new_free_block != NULL) ? new_free_block : curr_free_block;
 
            prev_free_block->end = block->end;
            prev_free_block->next = block->next;
 
            if( _releseMemSlotNode(block) != OS_SUCCESS)
            {
#ifdef MEM_ERROR_ASSERT
                OS_ASSERT;
#endif
                return OS_FAIL;
            }
        }
        else
        {
            prev_free_block->next = (new_free_block != NULL) ? new_free_block : curr_free_block;
        }
    }
    else
    {
        heap->free_list_head = (new_free_block != NULL) ? new_free_block : curr_free_block;
    }

    return OS_SUCCESS;
}

static os_status _cutFreeBlock(os_mem_heap_t * heap, uint32_t holdBase, uint32_t holdEnd)
{
    volatile struct mem_slot_t *prev_block, *curr_block;

    OS_ASSERT_COND(heap != NULL);
    OS_ASSERT_COND(heap->mem_start != NULL);
    OS_ASSERT_COND(osMemTypeIsValid((os_mem_type)heap->mem_type));
    OS_ASSERT_COND(holdBase != 0);
    OS_ASSERT_COND(holdEnd - holdBase >= ALIGNED_MINIMAL);
    OS_ASSERT_COND(IS_ALIGNED(holdBase, ALIGNED_MINIMAL));
    OS_ASSERT_COND(IS_ALIGNED(holdEnd, ALIGNED_MINIMAL));
    if(heap->mem_type & OS_MEM_CACHEABLE_TYPE)
    {
        OS_ASSERT_COND(IS_ALIGNED(holdBase, ARCH_CACHE_LINE_SIZE));
        OS_ASSERT_COND(IS_ALIGNED(holdEnd, ARCH_CACHE_LINE_SIZE));
    }

    prev_block = NULL;
    curr_block = (volatile struct mem_slot_t *) heap->free_list_head;

    while ( curr_block )
    {
        uint32_t base = curr_block->base;
        uint32_t end = curr_block->end;
 
        if(holdBase >= base && holdBase < end)
        {
            if(holdBase == base)
            {
                if(holdEnd == end)
                {
                    if(prev_block != NULL)
                    {
                        prev_block->next = curr_block->next;
                    }
                    else
                    {
                        heap->free_list_head = curr_block->next;
                    }
                    _releseMemSlotNode(curr_block);
                }
                else
                {
                    curr_block->base = holdEnd;
                }
            }
            else
            {
                curr_block->end = holdBase;
                if(holdEnd != end)
                {
                    volatile struct mem_slot_t *new_block;
                    new_block = _createSlot((os_mem_type)heap->mem_type ,holdEnd, end - holdEnd);
                    if(new_block == NULL)
                    {
#ifdef MEM_ERROR_ASSERT
                    	OS_ASSERT;
#endif
                    	return OS_FAIL;
                    }
                    new_block->next = curr_block->next;
                    curr_block->next = new_block;
                }
            }
        }
        else
        {
            prev_block = curr_block;
            curr_block = curr_block->next;
        }
    }
 
    return OS_SUCCESS;
}

void * _mmGet(uint32_t size, os_mem_type type, uint32_t alignment)
{
    os_mem_heap_t *heap;
    volatile struct mem_slot_t *free_block;
    volatile struct mem_slot_t *new_busy_block;
    uint32_t holdBase, holdEnd, alignBase = 0;
 
    if(!osMemTypeIsValid(type))
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return NULL;
    }
 
    if(!IS_POWER_OF_2(alignment))
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return NULL;
    }
 
    if(size == 0)
    {
        return NULL;
    }
 
    heap = _getHeapFromMemType(type);
    if(heap == NULL)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        return NULL;
    }
 
    if(type & OS_MEM_CACHEABLE_TYPE)
    {
        if (alignment < ARCH_CACHE_LINE_SIZE)
        {
            alignment = ARCH_CACHE_LINE_SIZE;
        }
 
        //assumption: ARCH_CACHE_LINE_SIZE is ALIGNED_MINIMAL (cache-line is 8 bytes aligned)
        OS_ASSERT_COND(IS_ALIGNED(ARCH_CACHE_LINE_SIZE, ALIGNED_MINIMAL));
        size = ALIGN_SIZE(size, ARCH_CACHE_LINE_SIZE);
    }
    else
    {
        if (alignment < ALIGNED_MINIMAL)
        {
            alignment = ALIGNED_MINIMAL;
        }
 
        size = ALIGN_SIZE(size, ALIGNED_MINIMAL);
    }
 
    _getMemGuard(type);
 
    free_block = (volatile struct mem_slot_t *) heap->free_list_head;
    while ( free_block )
    {
        alignBase = ALIGN_ADDRESS(free_block->base, alignment);
        if ( (alignBase >= free_block->base) && (alignBase < free_block->end) && (size <= (free_block->end - alignBase)) )
        {
            break;
        }

        free_block = free_block->next;
    }

    if ( !free_block )
    {
        _releaseMemGuard(type);
        return NULL;
    }

    holdBase = alignBase;
    holdEnd = alignBase + size;

    new_busy_block = _createSlot((os_mem_type)heap->mem_type, holdBase, size);
    if(new_busy_block == NULL)
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        _releaseMemGuard(type);
        return NULL;
    }

    if (_cutFreeBlock(heap, holdBase, holdEnd) != OS_SUCCESS )
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        _releaseMemGuard(type);
        return NULL;
    }

    _addBusy (heap, new_busy_block);
    _releaseMemGuard(type);
 
    return (void *) holdBase;
}

os_status _mmPut(void * addr)
{
    uint8_t i;
    os_mem_heap_t * heap = NULL;
    os_mem_heap_t* heap_list = g_os_mem_heap_local;
    uint32_t heap_num = (uint32_t) g_os_num_of_local_heaps;
    volatile struct mem_slot_t *busy_block, *prev_busy_block;
 
    if(addr == NULL)
    {
        return OS_ERR_PARAM_INVALID;
    }
 
    for (i = 0; i < heap_num; i++)
    {
        if ( ((uint32_t)heap_list[i].mem_start) > ((uint32_t)addr))
        {
            if( (((uint32_t)heap_list[i].mem_start) - heap_list[i].mem_size) <= ((uint32_t)addr)  )
            {
                heap = &heap_list[i];
                break;
            }
        }
    }
 
#if (OS_MULTICORE == ON)
    if(i == heap_num)
    {
        heap_list   = g_os_mem_heap_shared;
        heap_num    = (uint32_t)g_os_num_of_shared_heaps;

        for (i = 0; i < heap_num; i++)
        {
            if ( ((uint32_t)heap_list[i].mem_start) > ((uint32_t)addr))
            {
                if( (((uint32_t)heap_list[i].mem_start) - heap_list[i].mem_size) <= ((uint32_t)addr) )
                {
                    heap = &heap_list[i];
                    break;
                }
            }
        }
    }
#endif

    if((i == heap_num) || (heap == NULL))
    {
        return OS_FAIL;
    }
 
    prev_busy_block = NULL;
 
    _getMemGuard((os_mem_type)heap->mem_type);
 
    busy_block = (volatile struct mem_slot_t *) heap->busy_list_head;
    while ( busy_block && (((uint32_t)addr) != busy_block->base) )
    {
        prev_busy_block = busy_block;
        busy_block = busy_block->next;
    }

    if ( !busy_block )
    {
        _releaseMemGuard((os_mem_type)heap->mem_type);
        return OS_ERR_ALREADY_FREE;
    }

    if ( _addFreeBlock(heap, busy_block->base, busy_block->end) != OS_SUCCESS )
    {
#ifdef MEM_ERROR_ASSERT
        OS_ASSERT;
#endif
        _releaseMemGuard((os_mem_type)heap->mem_type);
        return OS_ERR_NO_ROOM;
    }

    if ( prev_busy_block )
    {
        prev_busy_block->next = busy_block->next;
    }
    else
    {
        heap->busy_list_head = (volatile void *)busy_block->next;
    }
 
    _releseMemSlotNode(busy_block);
 
    _releaseMemGuard((os_mem_type)heap->mem_type);
 
    return OS_SUCCESS;
}

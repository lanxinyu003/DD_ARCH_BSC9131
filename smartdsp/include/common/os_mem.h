/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/02/20 13:05:37 $
 $Id: os_mem.h,v 1.93 2014/02/20 13:05:37 b42006 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_mem.h,v $
 $Revision: 1.93 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_mem.h

 @Description   OS Memory Management header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MEM_H
#define __OS_MEM_H

#include "os_multicore_sync.h"  /* Necessary since os_mem.h embeds calls to spinlock API */
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#ifndef OS_PRINT_MALLOC_STATISTICS
#define OS_PRINT_MALLOC_STATISTICS 0 /**< Set to 0 for no print. Set to 1 for print. Default is 0 */
#endif

/**************************************************************************//**
 @Group         mem_id   System Memory Management

 @Description   Memory allocation, alignment and buffers management.
 
 @{
*//***************************************************************************/

/************************************************************//**
 @Anchor        mem_offset
 
 @Collection    Memory offsets
 
 @{
*//*************************************************************/
#define OFFSET_0_BYTES     0x0     /**< 0-bytes offset */
#define OFFSET_2_BYTES     0x2     /**< 2-bytes offset */
#define OFFSET_4_BYTES     0x4     /**< 4-bytes offset */
#define OFFSET_8_BYTES     0x8     /**< 8-bytes offset */
#define OFFSET_16_BYTES    0x10    /**< 16-bytes offset */
#define OFFSET_32_BYTES    0x20    /**< 32-bytes offset */
#define OFFSET_64_BYTES    0x40    /**< 64-bytes offset */
#define OFFSET_128_BYTES   0x80    /**< 128-bytes offset */
#define OFFSET_256_BYTES   0x100   /**< 256-bytes offset */
#define OFFSET_512_BYTES   0x200   /**< 512-bytes offset */
#define OFFSET_1024_BYTES  0x400   /**< 1024-bytes offset */
#define OFFSET_2048_BYTES  0x800   /**< 2048-bytes offset */
#define OFFSET_4096_BYTES  0x1000  /**< 4096-bytes offset */

/* @} */ /* end of mem_offset */

/************************************************************//**
 @Anchor        mem_align
 
 @Collection    Memory offsets
 
 @{
*//*************************************************************/
#define ALIGNED_1_BYTES     0x1     /**< 1-bytes alignment */
#define ALIGNED_2_BYTES     0x2     /**< 2-bytes alignment */
#define ALIGNED_4_BYTES     0x4     /**< 4-bytes alignment */
#define ALIGNED_8_BYTES     0x8     /**< 8-bytes alignment */
#define ALIGNED_16_BYTES    0x10    /**< 16-bytes alignment */
#define ALIGNED_32_BYTES    0x20    /**< 32-bytes alignment */
#define ALIGNED_64_BYTES    0x40    /**< 64-bytes alignment */
#define ALIGNED_128_BYTES   0x80    /**< 128-bytes alignment */
#define ALIGNED_256_BYTES   0x100   /**< 256-bytes alignment */
#define ALIGNED_512_BYTES   0x200   /**< 512-bytes alignment */
#define ALIGNED_1024_BYTES  0x400   /**< 1024-bytes alignment */
#define ALIGNED_2048_BYTES  0x800   /**< 2048-bytes alignment */
#define ALIGNED_4096_BYTES  0x1000  /**< 4096-bytes alignment */

/* @} */ /* end of mem_align */

#define ALIGN_SIZE(SIZE, ALIGNMENT)                 \
        ((((uint32_t)(SIZE)) + ((uint32_t)(ALIGNMENT)) - 1) & (~(((uint32_t)(ALIGNMENT)) - 1)))
        /**< Align a given size - equivalent to ceil(SIZE,ALIGNMENT) */

#define ALIGN_DOWN_SIZE(SIZE, ALIGNMENT)      \
        ((uint32_t)(SIZE) & (~(((uint32_t)(ALIGNMENT)) - 1)))
        /**< Align a given size to a lower aligned size - equivalent to floor(SIZE,ALIGNMENT) */

#define ALIGNABLE_SIZE(SIZE, ALIGNMENT)             \
        (((uint32_t)(SIZE)) + ((uint32_t)(ALIGNMENT)) - 1)
        /**< Extend a given size to make it alignable */

#define ALIGN_ADDRESS(ADDRESS, ALIGNMENT)           \
        ((((uint32_t)(ADDRESS)) + ((uint32_t)(ALIGNMENT)) - 1) & (~(((uint32_t)(ALIGNMENT)) - 1)))
        /**< Align a given address - equivalent to ceil(ADDRESS,ALIGNMENT) */

#define ALIGN_DOWN_ADDRESS(ADDRESS, ALIGNMENT)      \
        ((uint32_t)(ADDRESS) & (~(((uint32_t)(ALIGNMENT)) - 1)))
        /**< Align a given address to a lower aligned address - equivalent to floor(ADDRESS,ALIGNMENT) */

#define ALIGN_DOWN_PHYS_ADDRESS(ADDRESS, ALIGNMENT)      \
        ((os_phys_ptr)(ADDRESS) & (~(((os_phys_ptr)(ALIGNMENT)) - 1)))
        /**< Align a given address to a lower aligned address - equivalent to floor(ADDRESS,ALIGNMENT) */

#define IS_ALIGNED(ADDRESS, ALIGNMENT)              \
        (!(((uint32_t)(ADDRESS)) & (((uint32_t)(ALIGNMENT)) - 1)))
        /**< Check if a given address is aligned */

#define IS_COMPILER_ALIGNED(ADDRESS, ALIGNMENT)              \
        (((uint32_t)(ADDRESS)) % ((uint32_t)(ALIGNMENT)) == 0)
        /**< Check if a given address is aligned; Use it for cw_assert() */

/**************************************************************************//**
 @Description   Determines the needed memory size for a memory manager.
 
                Use this macro to set the size of the memory space dedicated
                for the memory manager. This memory space should be passed as the
                @a mem_part parameter to osMemPartCreate(). If you use a smaller
                size the stack will be corrupted.
*//***************************************************************************/
#define MEM_PART_SIZE(NUM_OF_BUFFERS)               \
        ALIGNABLE_SIZE((sizeof(os_mem_part_t) + ((NUM_OF_BUFFERS) * sizeof(os_virt_ptr))), ALIGNED_4_BYTES)

/**************************************************************************//**
 @Description   Determines the needed memory size for all memory blocks.
 
                Use this macro to set the size of the memory space dedicated
                for the allocated memory blocks. This memory space should be
                passed as the @a data_address parameter to osMemPartCreate().
                If you use a smaller size the stack will be corrupted.
*//***************************************************************************/
#define MEM_PART_DATA_SIZE(NUM, BUF_SIZE, ALIGNMENT) \
        ALIGNABLE_SIZE(((NUM)*(ALIGN_SIZE(BUF_SIZE, ALIGNMENT))), ALIGNMENT)



/**************************************************************************//**
 @Description   Memory Management Allocation Structure.
 
*//***************************************************************************/
typedef struct
{
    /* MUST be first */
    os_spinlock_handle  guard;          /**< Multicore synchronization element. */
    volatile uint8_t    **array_addr;   /**< Memory blocks array. */
    volatile int32_t    curr_index;     /**< Memory block index to get or free. */
    volatile uint32_t   block_size;     /**< Size of each block (in bytes). */
    volatile uint16_t   shared;         /**< Shared between cores           */
    volatile uint16_t   buffer_offset;  /**< buffer space reserved for LLD  */
} os_mem_part_t;


/************************************************************//**
 @Collection    Memory Type Enumeration definitions
 
 @{
*//*************************************************************/
#define OS_VALID_NUM_MASK          0xFFF00000   /**< SmartDSP validity check number is encoded here */
#define OS_MEM_FLAGS_MASK          0x000FF000   /**< Memory flags (see below) should be encoded in these bits */
#define OS_MEM_TYPE_MASK           0x0000001F   /**< Memory type should be encoded in these bits */

#define OS_VALID_NUM               0xEC900000   /**< Used by SmartDSP code to ensure that this is a valid heap */

#define OS_SMARTDSP_HEAP           0x00080000   /**< Bit signifying that the heap is defined by SmartDSP OS */
#define OS_MEM_CACHEABLE_TYPE      0x00040000   /**< Bit signifying that the heap is cacheable */
#define OS_MEM_SHARED_TYPE         0x00020000   /**< Bit signifying that the heap is shared */
#define OS_MEM_HET_TYPE            0x00010000   /**< Heap in PA DDR where SC owns memory allocator - for heterogeneous SoC only. */

#define OS_MEM_M1_TYPE             0x00000001   /**< Heap in M1 memory */
#define OS_MEM_M2_TYPE             0x00000002   /**< Heap in M2 memory */
#define OS_MEM_M3_TYPE             0x00000003   /**< Heap in M3 memory */
#define OS_MEM_DDR0_TYPE           0x00000004   /**< Heap in DDR0 memory */
#define OS_MEM_DDR1_TYPE           0x00000005   /**< Heap in DDR1 memory */
#define OS_MEM_QE_PRAM_TYPE        0x00000006   /**< Heap in QE PRAM memory */
#define OS_MEM_MAPLE_PRAM_TYPE     0x00000007   /**< Heap in MAPLE PRAM memory */
#define OS_MEM_MAPLE0_BD_RING_TYPE (OS_MEM_MAPLE_PRAM_TYPE | 0x00000010)   /**< Heap in MAPLE0 BD ring memory */
#define OS_MEM_MAPLE1_BD_RING_TYPE (OS_MEM_MAPLE_PRAM_TYPE | 0x00000020)   /**< Heap in MAPLE1 BD ring memory */
#define OS_MEM_MAPLE2_BD_RING_TYPE (OS_MEM_MAPLE_PRAM_TYPE | 0x00000030)   /**< Heap in MAPLE2 BD ring memory */

/* @} */ /* end of mem_type_defs */

/**************************************************************************//**
 @Description   Memory Type Enumeration
 
                Each heap managed by SmartDSP MUST have a unique identifier which
                will allows SmartDSP to differentiate it from other heaps. The
                @a os_mem_type enumeration provides identifiers for all SmartDSP
                defined heaps.
 
                The SmartDSP heap enumeration is split into 3 parts - "magic number",
                flags and memory type. Each such part has a bit mask as shown below:
                #OS_VALID_NUM_MASK, #OS_MEM_FLAGS_MASK and #OS_MEM_TYPE_MASK.
                The "magic number" is used in order to verify the validity of a heap.
                The flags specify the characteristics of the heap.
                The memory type specifies the type of memory the heap resides in.
 
 @Cautions      All heaps provided by SmartDSP will have OS_SMARTDSP_HEAP
                defined in their enumeration. In order to prevent a user defined heap's
                identifier from clashing with a SmartDSP identifier; the user MUST NOT
                define OS_SMARTDSP_HEAP as part of the heap identifier. All other flags
                are assumed to be valid in any user defined heap identifier. As such
                SmartDSP MAY perform checks on the heap type (cacheable, shared etc.)
                before performing any runtime code using the heap.
 
                Not all heaps are supported on all architectures
 
*//***************************************************************************/
typedef enum
{
    OS_MEM_LOCAL                    = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE),
    /**< Local cacheable memory - This heap is required in all SmartDSP OS applications; the physical
         memory location is chosen by the OS based on the the architecture */
    OS_MEM_LOCAL_NONCACHEABLE       = (OS_VALID_NUM | OS_SMARTDSP_HEAP),
    /**< Local non-cacheable memory - This heap is required in most SmartDSP OS applications; the physical
         memory location is chosen by the OS based on the the architecture */
    OS_MEM_SHARED                   = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE),
    /**< Shared non-cacheable memory - This heap is required in all multicore SmartDSP OS applications;
         the physical memory location is chosen by the OS based on the the architecture */
    OS_MEM_QE_PRAM                  = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE    | OS_MEM_QE_PRAM_TYPE),
    /**< QUICC Engine PRAM heap */
    OS_MEM_MAPLE_PRAM               = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE    | OS_MEM_MAPLE_PRAM_TYPE),
    /**< MAPLE heap */
    OS_MEM_M2_SHARED_CACHEABLE      = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_SHARED_TYPE | OS_MEM_M2_TYPE),
    /**< Shared cacheable heap in M2 memory */
    OS_MEM_M3_LOCAL                 = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_M3_TYPE),
    /**< Local cacheable heap in M3 memory */
    OS_MEM_M3_LOCAL_NONCACHEABLE    = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_M3_TYPE),
    /**< Local non-cacheable heap in M3 memory */
    OS_MEM_M3_SHARED                = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE    | OS_MEM_M3_TYPE),
    /**< Shared non-cacheable heap in M3 memory */
    OS_MEM_M3_SHARED_CACHEABLE      = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_SHARED_TYPE | OS_MEM_M3_TYPE),
    /**< Shared cacheable heap in M3 memory */
    OS_MEM_DDR0_LOCAL               = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_DDR0_TYPE),
    /**< Local cacheable heap in DDR0 memory */
    OS_MEM_DDR0_LOCAL_NONCACHEABLE  = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_DDR0_TYPE),
    /**< Local non-cacheable heap in DDR0 memory */
    OS_MEM_DDR0_SHARED              = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE    | OS_MEM_DDR0_TYPE),
    /**< Shared non-cacheable heap in DDR0 memory */
    OS_MEM_DDR0_SHARED_CACHEABLE    = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_SHARED_TYPE | OS_MEM_DDR0_TYPE),
    /**< Shared cacheable heap in DDR0 memory */
    OS_MEM_DDR1_LOCAL               = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_DDR1_TYPE),
    /**< Local cacheable heap in DDR1 memory */
    OS_MEM_DDR1_LOCAL_NONCACHEABLE  = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_DDR1_TYPE),
    /**< Local non-cacheable heap in DDR1 memory */
    OS_MEM_DDR1_SHARED              = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE    | OS_MEM_DDR1_TYPE),
    /**< Shared non-cacheable heap in DDR1 memory */
    OS_MEM_DDR1_SHARED_CACHEABLE    = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_SHARED_TYPE | OS_MEM_DDR1_TYPE),
    /**< Shared cacheable heap in DDR1 memory */
    OS_MEM_HET_DDR1_CACHEABLE       = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_SHARED_TYPE | OS_MEM_DDR1_TYPE | OS_MEM_HET_TYPE),
    /**< Shared cacheable heap in PA DDR where SC owns memory allocator.
     * This heap is shared for multicore devices and local for single core devices */
    OS_MEM_HET_DDR0_CACHEABLE       = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_CACHEABLE_TYPE | OS_MEM_SHARED_TYPE | OS_MEM_DDR0_TYPE | OS_MEM_HET_TYPE),
    /**< Shared cacheable heap in PA DDR where SC owns memory allocator.
     * This heap is shared for multicore devices and local for single core devices */
    OS_MEM_MAPLE0_BD_RING           = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE    | OS_MEM_MAPLE0_BD_RING_TYPE),
      /**< MAPLE0 BD ring heap */
    OS_MEM_MAPLE1_BD_RING           = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE    | OS_MEM_MAPLE1_BD_RING_TYPE),
      /**< MAPLE1 BD ring heap */
    OS_MEM_MAPLE2_BD_RING           = (OS_VALID_NUM | OS_SMARTDSP_HEAP | OS_MEM_SHARED_TYPE    | OS_MEM_MAPLE2_BD_RING_TYPE)
      /**< MAPLE2 BD ring heap*/
 

} os_mem_type;


/* Backward compatible defines */
#define OS_MEM_PRAM                      OS_MEM_QE_PRAM                  /**< QE parameter RAM memory       */
#define OS_MEM_DDR_LOCAL                 OS_MEM_DDR0_LOCAL               /**< DDR0 local memory             */
#define OS_MEM_DDR_SHARED                OS_MEM_DDR0_SHARED              /**< DDR0 shared memory            */
#define OS_MEM_DDR_SHARED_CACHEABLE      OS_MEM_DDR0_SHARED_CACHEABLE    /**< DDR0 shared cacheable memory  */
#define OS_MEM_DDR_LOCAL_NONCACHEABLE    OS_MEM_DDR0_LOCAL_NONCACHEABLE  /**< DDR0 local non-cacheable      */
#define OS_MEM_SHARED_CACHEABLE          OS_MEM_DDR0_SHARED_CACHEABLE    /**< DDR0 shared cacheable memory  */

/**************************************************************************//**
 @Group         mem_alloc_id Memory Allocation API

 @Description   Memory allocation API
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osMalloc

 @Description   Simple memory allocation, from the system heap.
 
 @Param[in]     size -  The memory size to allocate (in bytes).
 @Param[in]     type -  The memory type to allocate.

 @Return        Pointer to the allocated memory, NULL if not available

*//***************************************************************************/
void *          osMalloc(uint32_t size, os_mem_type type);

/**************************************************************************//**
 @Function      osFastMalloc

 @Description   Simple memory allocation, from the system heap, without initialization.
 
 @Param[in]     size -  The memory size to allocate (in bytes).
 @Param[in]     type -  The memory type to allocate.

 @Return        Pointer to the allocated memory, NULL if not available

*//***************************************************************************/
void *          osFastMalloc(uint32_t size, os_mem_type type);

/**************************************************************************//**
 @Function      osAlignedMalloc

 @Description   Aligned memory allocation, from the system heap.
 
 @Param[in]     size      -  The memory size to allocate (in bytes).
 @Param[in]     type      -  The memory type to allocate.
 @Param[in]     alignment -  address alignment required.

 @Return        Pointer to the allocated memory, NULL if not available

*//***************************************************************************/
void *          osAlignedMalloc(uint32_t size, os_mem_type type, uint32_t alignment);

/**************************************************************************//**
 @Function      osFastAlignedMalloc

 @Description   Aligned memory allocation, from the system heap, without initialization.
 
 @Param[in]     size      -  The memory size to allocate (in bytes).
 @Param[in]     type      -  The memory type to allocate.
 @Param[in]     alignment -  address alignment required.

 @Return        Pointer to the allocated memory, NULL if not available

*//***************************************************************************/
void *          osFastAlignedMalloc(uint32_t size, os_mem_type type, uint32_t alignment);

/**************************************************************************//**
 @Function      osFree

 @Description   Free memory allocation.
 
 @Param[in]     addr      -  Pointer to the allocated memory (as received by osMalloc).

 @Return        OS_SUCCESS if operation succeeded or an error code otherwise

*//***************************************************************************/
os_status       osFree(void * addr);

/**************************************************************************//**
 @Function      osMemTypeIsValid

 @Description   Verifies that a heap indicator is valid.
 
 @Param[in]     type      -  The heap indicator.

 @Retval        TRUE  - If the heap is valid
 @Retval        FALSE - If the heap is invalid

*//***************************************************************************/
INLINE bool    osMemTypeIsValid(os_mem_type type)
{
    return (bool)((type & OS_VALID_NUM_MASK) == OS_VALID_NUM);
}


/**************************************************************************//**
 @Function      osDumpMemory

 @Description   Print statistics and memory map of given memory type.
 
 @Param[in]     type      -  The heap indicator.

 @Retval        None

*//***************************************************************************/
void            osDumpMemory(os_mem_type type);

/**************************************************************************//**
 @Function      osGetFreeMemSize

 @Description   free heap size.
 
 @Param[in]     type      -  The heap indicator.

 @Retval        Free memory amount (in Bytes)

*//***************************************************************************/
uint32_t        osGetFreeMemSize(os_mem_type type);

/**************************************************************************//**
 @Function      osGetMaxBlockSize

 @Description   maximum free consecutive memory.
 
 @Param[in]     type      -  The heap indicator.
 @Param[in]     alignment -  address alignment required.

 @Retval        Maximum free consecutive memory size aligned to given parameter (in Bytes)

*//***************************************************************************/
uint32_t        osGetMaxBlockSize(os_mem_type type, uint32_t alignment);
/** @} */ // end of mem_alloc_id


/**************************************************************************//**
 @Group         mem_pool_id  Buffers Pool Management API

 @Description   Memory allocation API
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osMemPartCreate

 @Description   Initializes a memory structure for aligned, fixed-size blocks.
 
 @Param[in]     block_size    -  Size of allocatable block.
 @Param[in]     num_of_blocks -  Maximum number of allocatable blocks.
 @Param[in]     data_address  -  Address at which to begin allocation.
                                 Use the MEM_PART_DATA_SIZE() macro to
                                 determine the required memory size.
 @Param[in]     alignment     -  Alignment of the blocks (in bytes).
 @Param[in]     buffer_offset -  Buffer header allocated to LLD only
 @Param[in]     mem_part      -  Address of the memory manager space to
                                 initialize. Use the MEM_PART_SIZE() macro to
                                 determine the required memory size.
 @Param[in]     shared       -   Flag indicating whether this partition
                                 is shared between cores.
 
 @Return        Pointer to the initialized memory structure.

 @Cautions      The allocation size must be larger than (@a alignment - 1)
 
*//***************************************************************************/
os_mem_part_t * osMemPartCreate(uint32_t        block_size,
                                uint32_t        num_of_blocks,
                                uint8_t         *data_address,
                                uint32_t        alignment,
                                uint16_t        buffer_offset,
                                os_mem_part_t   *mem_part
#if (OS_MULTICORE == 1)
                                ,bool shared
#endif
                                );


/**************************************************************************//**
 @Function      osMemBlockSize

 @Description   Gets the size of the blocks in the memory structure.
 
 @Param[in]     mem_part -  The memory structure to get the block size from.

 @Return        The block size.

*//***************************************************************************/
#define osMemBlockSize(mem_part)    ((mem_part)->block_size)

/**************************************************************************//**
 @Function      osMemBlockOffset

 @Description   Gets the lld offset of the blocks in the memory structure.
 
 @Param[in]     mem_part -  The memory structure to get the block offset from.

 @Return        The block offset.

*//***************************************************************************/
#define osMemBlockOffset(mem_part)    ((mem_part)->buffer_offset)


/**************************************************************************//**
 @Function      osMemBlockGet

 @Description   Gets a block from the memory structure.
 
 @Param[in]     mem_part -  The memory structure to get the block from.

 @Return        The requested memory block.

*//***************************************************************************/
INLINE void * osMemBlockGet(os_mem_part_t *mem_part)
{
    void *addr = NULL;
 
    osHwiSwiftDisable();

    if (mem_part->curr_index >= 0)
        addr = ((void *)mem_part->array_addr[mem_part->curr_index--]);
 
    osHwiSwiftEnable();
 
    return addr;
}


/**************************************************************************//**
 @Function      osMemBlockFree

 @Description   Releases a block back to the memory structure.
 
 @Param[in]     mem_part -  The memory structure that owns the block.
 @Param[in]     addr -  The block address.

*//***************************************************************************/
INLINE void osMemBlockFree(os_mem_part_t *mem_part, void *addr)
{
    osHwiSwiftDisable();
 
    mem_part->array_addr[++mem_part->curr_index] = (volatile uint8_t*)addr;
 
    osHwiSwiftEnable();
}


/**************************************************************************//**
 @Function      osMemBlockUnsafeGet

 @Description   Gets a block from the memory structure (unsafe version).
 
                Use this function when you can be sure that the memory
                structure cannot be accessed from any other source.
 
 @Param[in]     mem_part -  The memory structure to get the block from.

 @Return        The requested memory block.

*//***************************************************************************/
#define osMemBlockUnsafeGet(mem_part) \
    (((mem_part)->curr_index >= 0) ? ((void *)(mem_part)->array_addr[((mem_part)->curr_index)--]) : NULL)


/**************************************************************************//**
 @Function      osMemBlockUnsafeFree

 @Description   Releases a block back to the memory structure (unsafe version).
 
                Use this function when you can be sure that the memory
                structure cannot be accessed from any other source.
 
 @Param[in]     mem_part -  The memory structure that owns the block.
 @Param[in]     addr -  The block address.

*//***************************************************************************/
#define osMemBlockUnsafeFree(mem_part, addr) \
    { (mem_part)->array_addr[++((mem_part)->curr_index)] = addr; }

/**************************************************************************//**
 @Function      osMemBlockCountGet

 @Description   Get number of free buffers in memory pool
 
 @Param[in]     mem_part -  The memory structure of buffer pool

 @Return        number of free buffers

*//***************************************************************************/
#define osMemBlockCountGet(mem_part) ((mem_part)->curr_index + 1)

/**************************************************************************//**
 @Function      osMemLocalHeapSet

 @Description   Updates the entry of the local heap with new values
 
                Function to be used inside appMemHeapsSet which is defined by
                application and activated inside osInitialize().
 
 @Param[in]     mem_ptr  -  Virtual start pointer of the memory allocated for this heap
 @Param[in]     mem_size -  The size of the memory allocated for this heap
 @Param[in]     type     -  The name of the heap
 
 @Return        OS_FAIL or OS_SUCCESS

*//***************************************************************************/
os_status osMemLocalHeapSet(os_virt_ptr mem_ptr, uint32_t mem_size, os_mem_type type);

#if (OS_MULTICORE == 1)

/**************************************************************************//**
 @Function      osMemBlockSyncGet

 @Description   Gets a block from the memory structure (multicore safe).
 
 @Param[in]     mem_part -  The memory structure to get the block from.

 @Return        The requested memory block.

*//***************************************************************************/
INLINE void * osMemBlockSyncGet(os_mem_part_t *mem_part)
{
    void *addr = NULL;
 
    osSpinLockIrqGet(&mem_part->guard);
 
    if (mem_part->curr_index >= 0)
        addr = (void *)mem_part->array_addr[mem_part->curr_index--];
 
    osSpinLockIrqRelease(&mem_part->guard);
 
    return addr;
}


/**************************************************************************//**
 @Function      osMemBlockSyncFree

 @Description   Releases a block back to the memory structure (multicore safe).
 
 @Param[in]     mem_part -  The memory structure that owns the block.
 @Param[in]     addr -  The block address.

*//***************************************************************************/
INLINE void osMemBlockSyncFree(os_mem_part_t *mem_part, void *addr)
{
    osSpinLockIrqGet(&mem_part->guard);
 
    mem_part->array_addr[++mem_part->curr_index] = (uint8_t *)addr;
 
    osSpinLockIrqRelease(&mem_part->guard);
}

#else // OS_MULTICORE

#define osMemBlockSyncGet(mem_part)         osMemBlockGet((mem_part))
#define osMemBlockSyncFree(mem_part,addr)   osMemBlockFree((mem_part),(addr))

#endif // OS_MULTICORE

#ifndef OS_DOCUMENTATION_ONLY
os_status osHeapInit(volatile uint32_t * mem_ptr, volatile uint32_t mem_size, os_mem_type type);

/* Used for reporting osAlignedMalloc footprint during runtime */
void * _osAlignedMalloc(uint32_t size, os_mem_type type, uint32_t alignment);
void * _mmGet(uint32_t size, os_mem_type type, uint32_t alignment);
os_status _mmPut(void * addr);

#if (OS_PRINT_MALLOC_STATISTICS == 0)
    #define osMalloc(size, type) _osAlignedMalloc(size, type, ALIGNED_8_BYTES)
    #define osAlignedMalloc(size, type, alignment) _osAlignedMalloc(size, type, alignment)
    #define osFastMalloc(size, type) _mmGet(size,type, ALIGNED_8_BYTES)
    #define osFastAlignedMalloc(size, type, alignment)  _mmGet(size,type,alignment)
    #define osFree(ptr) _mmPut(ptr)
#else
	#include <stdio.h>
	#include <stdlib.h>

	#define osMalloc(size, type) __os_malloc_msg(size, type, ALIGNED_8_BYTES, __FILE__, __LINE__)
	#define osAlignedMalloc(size, type, alignment) __os_aligned_malloc_msg(size, type, alignment, __FILE__, __LINE__)
	#define osFastMalloc(size, type) __os_fast_malloc_msg(size, type, __FILE__, __LINE__)
	#define osFastAlignedMalloc(size, type, alignment) __os_fast_aligned_malloc_msg(size, type, alignment, __FILE__, __LINE__)
	#define osFree(ptr) __os_free_msg(ptr, __FILE__, __LINE__, #ptr)

extern FILE * __os_malloc_file;
#define os_malloc_file_handle (__os_malloc_file)
void * __os_malloc_msg(uint32_t, os_mem_type, uint32_t, const char *, int);
void * __os_aligned_malloc_msg(uint32_t, os_mem_type, uint32_t, const char *, int);
void * __os_fast_malloc_msg(uint32_t, os_mem_type, const char *, int);
void * __os_fast_aligned_malloc_msg(uint32_t, os_mem_type, uint32_t, const char *, int);
os_status __os_free_msg(void * addr, const char *, int, const char *);
#endif // OS_PRINT_MALLOC_STATISTICS

#endif // OS_DOCUMENTATION_ONLY

/** @} */ // mem_pool_id
 
/** @} */ // end of mem_id

_OS_END_EXTERN_C

#endif // __OS_MEM_H

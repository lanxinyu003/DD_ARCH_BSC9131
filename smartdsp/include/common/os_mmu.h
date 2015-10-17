/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/09/03 07:10:17 $
 $Id: os_mmu.h,v 1.35 2013/09/03 07:10:17 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_mmu.h,v $
 $Revision: 1.35 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_mmu.h

 @Description   OS MMU common file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MMU_H
#define __OS_MMU_H

#include "os_datatypes.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C


/**************************************************************************//**
 @Group         mem_mmu_id   MMU – Memory Management Unit API

 @Description   Memory Managment Unit, defines, enumerations and API
 
                The SmartDSP OS MMU module make a destinct separation between
                program and data aspects of the MMU
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   MMU memory type - describes the type of memory managed
*//***************************************************************************/
typedef enum mmu_memory_type {
    OS_MMU_DATA = 0,   /**< Data MMU */
    OS_MMU_PROG        /**< Program MMU */
} mmu_memory_t;

#if defined(SC3X00) || defined(SC3X50)
/**************************************************************************//**
 @Description   MMU context - software context pertaining to MMU
*//***************************************************************************/
typedef struct os_mmu_context_t
{
        uint32_t  segments;           /**< bit map of the segments. */
        uint32_t  status;             /**< status of this context   */
        uint32_t  id;                 /**< ID of this context       */
        mmu_memory_t mmu_memory;      /**< type of MMU context */
        uint32_t  arch_specific;      /**< arch specific */
        void*     arch_specific_p;    /**< arch specific */
}os_mmu_context_t;

/**************************************************************************//**
 @Description   MMU context handle - passed to MMU context API
*//***************************************************************************/
typedef os_mmu_context_t* os_mmu_context_handle;

#endif

/**************************************************************************//**
 @Description   MMU error - populated when reporting MMU exceptions
*//***************************************************************************/
struct os_mmu_error
{
    uint32_t error_address;     /**< violation address */
    uint32_t error_pc;          /**< program counter that caused the exception */
    uint32_t rw_access;         /**< read is 0, write is 1 */
    uint32_t priviledge_level;  /**< user is 0, supervisor is 1 */
    uint32_t access_width;      /**< Access Violation Width */
};


/**************************************************************************//**
 @Description   MMU segment handle - passed to MMU segment API
*//***************************************************************************/
typedef uint32_t os_mmu_segment_handle;


#if defined(SC39XX)
typedef  uint64_t                os_mmu_attr;
#else
typedef  uint32_t                os_mmu_attr;
#endif

#define CME_QCR_POLING_BIT_MASK		0x00000001
#define OS_MMU_SYSTEM_CONTEXT       1
/**< This is reserved and default ID for system context - MAY NOT BE CHANGED */



#if defined(SC3X00) || defined(SC3X50)
/**************************************************************************//**
 @Function      osMmuInit

 @Description   Basic MMU initialization

                Should be called prior to any other os mmu calls
                (except for sysMmu... functions)
 
 @Param[in]     data_context -  Number of data contexts in the system
 @Param[in]     prog_context -  Number of program contexts in the system
 
 @Return        OS_SUCCESS if succeeded in initializating the MMU module
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuInit(int data_context, int prog_context);


/**************************************************************************//**
 @Function      osMmuContextIDGet

 @Description   Get context ID, for program or data contexts

 @Param[in]     cont    -  MMU context
 
 @Return        Context ID

*//***************************************************************************/
uint32_t osMmuContextIDGet(os_mmu_context_t* cont);



/**************************************************************************//**
 @Group         mem_mmu_data_id Data MMU API

 @Description   Data Memory Managment Unit, defines, enumerations and API
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osMmuDataContextFind

 @Description   Find a new data context.
 
 @Param[out]    context -  Return context's handle
 
 @Retval        OS_SUCCESS if successful
 @Retval        OS_FAIL if there is no context available

*//***************************************************************************/
os_status osMmuDataContextFind(os_mmu_context_handle* context);


/**************************************************************************//**
 @Function      osMmuDataContextCreate

 @Description   Create new data context according to ID and arch parameters
 
 @Param[in]     context -  context's handle returned by osMmuDataContextFind()
 @Param[in]     id      -  context ID
 @Param[in]     arch    -  arch parameters (currently not used)
 
 @Retval        OS_SUCCESS if successful
 @Retval        OS_FAIL if not successful

*//***************************************************************************/
os_status osMmuDataContextCreate(os_mmu_context_handle context, uint32_t id, void* arch);


/**************************************************************************//**
 @Function      osMmuDataContextFindExisting

 @Description   Find existing (created) data context according to ID.
 
 @Param[out]    context_handle -  Return pointer to context
 @Param[in]     id             -  Context ID
 
 
 @Retval        OS_SUCCESS if successful
 @Retval        OS_FAIL if not successful

*//***************************************************************************/
os_status osMmuDataContextFindExisting(os_mmu_context_handle* context_handle, uint32_t id);


/**************************************************************************//**
 @Function      osMmuDataContextDelete

 @Description   Delete data context.
 
 @Param[in]     context -  context's handle
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuDataContextDelete(os_mmu_context_handle context);

#endif

/**************************************************************************//**
 @Function      osMmuDataSegmentFind

 @Description   Find a free data segment in MMU

 @Param[out]    descriptor -  descriptor's pointer
 
 @Retval        OS_SUCCESS if successful
 @Retval        OS_FAIL if not available
 
 @Cautions      This function returns a free segment in MMU.
                There is no function that deletes the segment.

*//***************************************************************************/
os_status osMmuDataSegmentFind(os_mmu_segment_handle* descriptor);


/**************************************************************************//**
 @Function      osMmuDataSegmentCreate

 @Description   Create data segment in MMU

                This function creates new MMU data segment based on
                the user's configuration.
 
 @Param[in]     descriptor -  Descriptor handle returned by
                              osMmuDataSegmentFind()
 @Param[in]     virt_addr  -  Virtual base address of this segment.
 @Param[in]     phys_addr  -  Physical base address of this segment.
 @Param[in]     size       -  Size of this segment in bytes.
 @Param[in]     attr       -  Attributes of the segment (arch dependant).
 @Param[in]     arch       -  Arch specific parameters
 
 @Retval        OS_SUCCESS                  if successful
 @Retval        OS_ERR_MMU_WRONG_DESC       if descriptor is not legal
 @Retval        OS_ERR_MMU_WRONG_ALIGNMENT  if virt_addr or phys_addr
                                            alignment  is illegal
 @Retval        OS_ERR_MMU_WRONG_SIZE       if size is illegal
 @Retval        OS_ERR_MMU_WRONG_BOUNDARY   if there are boundary crossing
                                            issues with the combination of
                                            size and virt_addr or phys_addr
 @Retval        OS_ERR_MMU_DSPE_ERR         if the MMU hardware rejected the
                                            configuration
 
*//***************************************************************************/
os_status osMmuDataSegmentCreate(os_mmu_segment_handle descriptor,
                                 os_const_virt_ptr virt_addr,
                                 os_const_phys_ptr phys_addr,
                                 uint32_t size,
                                 os_mmu_attr attr,
                                 void * arch);

/**************************************************************************//**
 @Function      osMmuDataSegmentEnable

 @Description   Enable a data segment in MMU

 
 @Param[in]     descriptor -  decriptor's handle returned by
                              osMmuDataSegmentFind()
 @Param[in]     enable     -  TRUE - enable, FALSE - disable
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuDataSegmentEnable(os_mmu_segment_handle descriptor, bool enable);

/**************************************************************************//**
 @Function      osMmuDataSegmentSizeGet

 @Description   Retrieve the segment size of data segment
 
 @Param[in]     descriptor -  Descriptor handle returned by
                              osMmuDataSegmentFind() or osMmuDataSegmentProbe()
 @Param[in]     size       -  Size of this segment in bytes; used as return value.
 
 @Retval        OS_SUCCESS      if successful
 @Retval        OS_FAIL         if size calculation failed
 
*//***************************************************************************/
os_status osMmuDataSegmentSizeGet(os_mmu_segment_handle descriptor, uint32_t *size);

/**************************************************************************//**
 @Function      osMmuDataSegmentVirtBaseGet

 @Description   Retrieve the segment virtual base of data segment
 
 @Param[in]     descriptor -  Descriptor handle returned by
                              osMmuDataSegmentFind() or osMmuDataSegmentProbe()
 @Param[in]     virt_base  -  Virtual base of segment; used as return value.
 
 @Retval        OS_SUCCESS      if successful
 @Retval        OS_FAIL         if size calculation failed
 
*//***************************************************************************/
os_status osMmuDataSegmentVirtBaseGet(os_mmu_segment_handle descriptor, os_virt_ptr *virt_base);

/**************************************************************************//**
 @Function      osMmuProgSegmentSizeGet

 @Description   Retrieve the segment size of Program segment
 
 @Param[in]     descriptor -  Descriptor handle returned by
                              osMmuProgSegmentFind() or osMmuProgSegmentProbe()
 @Param[in]     size       -  Size of this segment in bytes; used as return value.
 
 @Retval        OS_SUCCESS      if successful
 @Retval        OS_FAIL         if size calculation failed
 
*//***************************************************************************/
os_status osMmuProgSegmentSizeGet(os_mmu_segment_handle descriptor, uint32_t *size);

/**************************************************************************//**
 @Function      osMmuProgSegmentVirtBaseGet

 @Description   Retrieve the segment virtual base of Program segment
 
 @Param[in]     descriptor -  Descriptor handle returned by
                              osMmuProgSegmentFind() or osMmuProgSegmentProbe()
 @Param[in]     virt_base  -  Virtual base of segment; used as return value.
 
 @Retval        OS_SUCCESS      if successful
 @Retval        OS_FAIL         if size calculation failed
 
*//***************************************************************************/
os_status osMmuProgSegmentVirtBaseGet(os_mmu_segment_handle descriptor, os_virt_ptr *virt_base);

#if defined(SC3X00) || defined(SC3X50)
/**************************************************************************//**
 @Function      osMmuDataContextSegmentsAdd

 @Description   Add new data segment to dynamic context.

                This function adds a data segment (segment) to dynamic context.
 
 @Param[in]     mmu_context -  Pointer to context, returned by osMmuDataContextFind()
 @Param[in]     segment     -  segment to add, returned by osMmuDataSegmentFind()
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuDataContextSegmentsAdd(os_mmu_context_handle mmu_context, os_mmu_segment_handle segment);


/**************************************************************************//**
 @Function      osMmuDataContextSegmentRemove

 @Description   Remove a data segment from mmu_context
 
 @Param[in]     mmu_context -  Pointer to context, returned by osMmuDataContextFind()
 @Param[in]     segment     -  segment to add, returned by osMmuDataSegmentFind()
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuDataContextSegmentRemove(os_mmu_context_handle mmu_context, os_mmu_segment_handle segment);


/**************************************************************************//**
 @Function      osMmuDataSystemContextSegmentAdd

 @Description   Adds a segment to all contexts

                This function adds a segment to all contexts in the system
 
 @Param[in]     segment     -  segment to add, returned by osMmuDataSegmentFind()

 @Retval        OS_SUCCESS if successful
 
 @Cautions      May not be called from interrupt (hardware or software) context.

*//***************************************************************************/
os_status osMmuDataSystemContextSegmentAdd(os_mmu_segment_handle segment);


/**************************************************************************//**
 @Function      osMmuDataSystemContextSegmentRemove

 @Description   Removes a segment from all contexts

                This function removes a segment from all contexts in the system
 
 @Param[in]     segment     -  segment to remove, returned by osMmuDataSegmentFind()

 @Retval        OS_SUCCESS if successful
 
 @Cautions      May not be called from interrupt (hardware or software) context.

*//***************************************************************************/
os_status osMmuDataSystemContextSegmentRemove(os_mmu_segment_handle segment);


/**************************************************************************//**
 @Function      osMmuDataContextActivate

 @Description   Activate given data context.

                This function activates a data segment and data ID
                according  to previosly created context
 
 @Param[in]     new_desc -  Pointer to context to activate
 @Param[out]    prev     -  Pointer to previous context (may be NULL)
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuDataContextActivate(os_mmu_context_handle new_desc, os_mmu_context_t  *prev);



/**************************************************************************//**
 @Function      osMmuDataCurrentIDSet

 @Description   Set current data ID

                Set current data ID. Optinal use-case is when two virtual
                segments reside on the same addresses

 @Param[in]     id -  ID to set
 
 @Return        previous data ID

*//***************************************************************************/
uint32_t osMmuDataCurrentIDSet(uint32_t id);


/**************************************************************************//**
 @Function      osMmuDataCurrentIDGet

 @Description   Get current Data ID
 
 @Return        data ID

*//***************************************************************************/
uint32_t osMmuDataCurrentIDGet();
#endif

/**************************************************************************//**
 @Function      osMmuDataVirtToPhys

 @Description   Translate a data virtual address to the corresponding physical one

 @Param[in]     virt_addr -  virtual address pointer
 @Param[out]    phys_addr -  pointer to a physical address pointer
 
 @Return        OS_SUCCESS if translation succedded
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuDataVirtToPhys(os_const_virt_ptr virt_addr, os_phys_ptr* phys_addr);


/**************************************************************************//**
 @Function      osMmuDataVirtProbe

 @Description   Check if  a data virtual address is valid

 @Param[in]     virt_addr -  virtual address pointer

 @Return        OS_SUCCESS if address is legal within existing context
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuDataVirtProbe(os_const_virt_ptr virt_addr);


/**************************************************************************//**
 @Function      osMmuDataSegmentProbe

 @Description   Check which MMU segment maps a given address

 @Param[in]     virt_addr   -  virtual address pointer
 @Param[out]    segment_num -  segment number that maps the address

 @Return        OS_SUCCESS if address is legal within existing context
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuDataSegmentProbe(os_const_virt_ptr virt_addr, os_mmu_segment_handle *segment_num);


/**************************************************************************//**
 @Function      osMmuDataNumOfSegmentsGet

 @Description   Get number total number data segments
 
 @Return        number of used data segments

*//***************************************************************************/
int osMmuDataNumOfSegmentsGet();


/**************************************************************************//**
 @Function      osMmuDataNumOfUsedSegmentsGet

 @Description   Get number of used data segments

 @Return        number of used data segments

*//***************************************************************************/
int osMmuDataNumOfUsedSegmentsGet();


/**************************************************************************//**
 @Function      osMmuDataMaxIdGet

 @Description   Get the number of maximum data id
 
 @Return        The highest data id supported by MMU

*//***************************************************************************/
int osMmuDataMaxIdGet();

#if defined(SC3X00) || defined(SC3X50)
/**************************************************************************//**
 @Function      osMmuNumOfDataContextsGet

 @Description   Get the number of maximum possible data contexts

 @Return        number of maximum data contexts

*//***************************************************************************/
int osMmuNumOfDataContextsGet();

#endif

/**************************************************************************//**
 @Function      osMmuDataErrorDetect

 @Description   Read the MMU and populate the os_mmu_error structre
 
 @Param[out]    err     - error structre

 @Return        Error status, encoded in os_error.h,

*//***************************************************************************/
os_status osMmuDataErrorDetect(struct os_mmu_error* err);


/**************************************************************************//**
 @Function      osMmuDataErrorClear

 @Description   Clear the data MMU status
 
*//***************************************************************************/
void osMmuDataErrorClear();


/** @} */ // end of mem_mmu_data_id




/**************************************************************************//**
 @Group         mem_mmu_prog_id Program MMU API

 @Description   Program Memory Managment Unit, defines, enumerations and API
 
 @{
*//***************************************************************************/


#if defined(SC3X00) || defined(SC3X50)
/**************************************************************************//**
 @Function      osMmuProgContextFind

 @Description   Find a new prog context.
 
 @Param[out]    context -  Return context's handle
 
 @Retval        OS_SUCCESS if successful
 @Retval        OS_FAIL if there is no context available

*//***************************************************************************/
os_status osMmuProgContextFind(os_mmu_context_handle* context);


/**************************************************************************//**
 @Function      osMmuProgContextCreate

 @Description   Create new prog context according to ID and arch parameters
 
 @Param[in]     context -  context's handle returned by osMmuProgContextFind()
 @Param[in]     id      -  context ID
 @Param[in]     arch    -  arch parameters (currently not used)
 
 @Retval        OS_SUCCESS if successful
 @Retval        OS_FAIL if not successful

*//***************************************************************************/
os_status osMmuProgContextCreate(os_mmu_context_handle context, uint32_t id, void* arch);


/**************************************************************************//**
 @Function      osMmuProgContextFindExisting

 @Description   Find existing (created) prog context according to ID.
 
 @Param[out]    context_handle -  Return pointer to context
 @Param[in]     id             -  Context ID
 
 
 @Retval        OS_SUCCESS if successful
 @Retval        OS_FAIL if not successful

*//***************************************************************************/
os_status osMmuProgContextFindExisting(os_mmu_context_handle* context_handle, uint32_t id);


/**************************************************************************//**
 @Function      osMmuProgContextDelete

 @Description   Delete prog context.
 
 @Param[in]     context -  context's handle
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuProgContextDelete(os_mmu_context_handle context);

#endif

/**************************************************************************//**
 @Function      osMmuProgSegmentFind

 @Description   Find a free prog segment in MMU

 @Param[out]    descriptor -  descriptor's pointer
 
 @Retval        OS_SUCCESS if successful
 @Retval        OS_FAIL if not available
 
 @Cautions      This function returns a free segment in MMU.
                There is no function that deletes the segment.

*//***************************************************************************/
os_status osMmuProgSegmentFind(os_mmu_segment_handle* descriptor);


/**************************************************************************//**
 @Function      osMmuProgSegmentCreate

 @Description   Create prog segment in MMU

                This function creates new MMU prog segment based on
                the user's configuration.
 
 @Param[in]     descriptor -  Descriptor handle returned by
                              osMmuProgSegmentFind()
 @Param[in]     virt_addr  -  Virtual base address of this segment.
 @Param[in]     phys_addr  -  Physical base address of this segment.
 @Param[in]     size       -  Size of this segment.
 @Param[in]     attr       -  Attributes of the segment (arch dependant).
 @Param[in]     arch       -  Arch specific parameters
 
 @Retval        OS_SUCCESS                  if successful
 @Retval        OS_ERR_MMU_WRONG_DESC       if descriptor is not legal
 @Retval        OS_ERR_MMU_WRONG_ALIGNMENT  if virt_addr or phys_addr
                                            alignment  is illegal
 @Retval        OS_ERR_MMU_WRONG_SIZE       if size is illegal
 @Retval        OS_ERR_MMU_WRONG_BOUNDARY   if there are boundary crossing
                                            issues with the combination of
                                            size and virt_addr or phys_addr
 @Retval        OS_ERR_MMU_DSPE_ERR         if the MMU hardware rejected the
                                            configuration
 
*//***************************************************************************/
os_status osMmuProgSegmentCreate(os_mmu_segment_handle descriptor,
                                 os_const_virt_ptr virt_addr,
                                 os_const_phys_ptr phys_addr,
                                 uint32_t size,
                                 os_mmu_attr attr,
                                 void * arch);


/**************************************************************************//**
 @Function      osMmuProgSegmentEnable

 @Description   Enable a prog segment in MMU

 
 @Param[in]     descriptor -  decriptor's handle returned by
                              osMmuProgSegmentFind()
 @Param[in]     enable     -  TRUE - enable, FALSE - disable
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuProgSegmentEnable(os_mmu_segment_handle descriptor, bool enable);

#if defined(SC3X00) || defined(SC3X50)
/**************************************************************************//**
 @Function      osMmuProgContextSegmentsAdd

 @Description   Add new prog segment to dynamic context.

                This function adds a prog segment (segment) to dynamic context.
 
 @Param[in]     mmu_context -  Pointer to context, returned by osMmuProgContextFind()
 @Param[in]     segment     -  segment to add, returned by osMmuProgSegmentFind()
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuProgContextSegmentsAdd(os_mmu_context_handle mmu_context, os_mmu_segment_handle segment);


/**************************************************************************//**
 @Function      osMmuProgContextSegmentRemove

 @Description   Remove a prog segment from mmu_context
 
 @Param[in]     mmu_context -  Pointer to context, returned by osMmuProgContextFind()
 @Param[in]     segment     -  segment to add, returned by osMmuProgSegmentFind()
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuProgContextSegmentRemove(os_mmu_context_handle mmu_context, os_mmu_segment_handle segment);


/**************************************************************************//**
 @Function      osMmuProgSystemContextSegmentAdd

 @Description   Adds a segment to all contexts

                This function adds a segment to all contexts in the system
 
 @Param[in]     segment     -  segment to add, returned by osMmuProgSegmentFind()

 @Retval        OS_SUCCESS if successful
 
 @Cautions      May not be called from interrupt (hardware or software) context.

*//***************************************************************************/
os_status osMmuProgSystemContextSegmentAdd(os_mmu_segment_handle segment);


/**************************************************************************//**
 @Function      osMmuProgSystemContextSegmentRemove

 @Description   Removes a segment from all contexts

                This function removes a segment from all contexts in the system
 
 @Param[in]     segment     -  segment to remove, returned by osMmuProgSegmentFind()

 @Retval        OS_SUCCESS if successful
 
 @Cautions      May not be called from interrupt (hardware or software) context.

*//***************************************************************************/
os_status osMmuProgSystemContextSegmentRemove(os_mmu_segment_handle segment);


/**************************************************************************//**
 @Function      osMmuProgContextActivate

 @Description   Activate given prog context.

                This function activates a prog segment and prog ID
                according  to previosly created context
 
 @Param[in]     new_desc -  Pointer to context to activate
 @Param[out]    prev     -  Pointer to previous context (may be NULL)
 
 @Retval        OS_SUCCESS if successful

*//***************************************************************************/
os_status osMmuProgContextActivate(os_mmu_context_handle new_desc, os_mmu_context_t  *prev);

#endif

/**************************************************************************//**
 @Function      osMmuProgCurrentIDSet

 @Description   Set current prog ID

                Set current prog ID. Optinal use-case is when two virtual
                segments reside on the same addresses

 @Param[in]     id -  ID to set
 
 @Return        previous prog ID

*//***************************************************************************/
uint32_t osMmuProgCurrentIDSet(uint32_t id);


/**************************************************************************//**
 @Function      osMmuProgCurrentIDGet

 @Description   Get current Prog ID
 
 @Return        prog ID

*//***************************************************************************/
uint32_t osMmuProgCurrentIDGet();


/**************************************************************************//**
 @Function      osMmuProgVirtToPhys

 @Description   Translate a prog virtual address to the corresponding physical one

 @Param[in]     virt_addr -  virtual address pointer
 @Param[out]    phys_addr -  pointer to a physical address pointer
 
 @Return        OS_SUCCESS if translation succedded
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuProgVirtToPhys(os_const_virt_ptr virt_addr,os_phys_ptr * phys_addr);


/**************************************************************************//**
 @Function      osMmuProgVirtProbe

 @Description   Check if  a prog virtual address is valid

 @Param[in]     virt_addr -  virtual address pointer

 @Return        OS_SUCCESS if address is legal within existing context
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuProgVirtProbe(os_const_virt_ptr virt_addr);



/**************************************************************************//**
 @Function      osMmuProgSegmentProbe

 @Description   Check which MMU segment maps a given address

 @Param[in]     virt_addr   -  virtual address pointer
 @Param[out]    segment_num -  segment number that maps the address

 @Return        OS_SUCCESS if address is legal within existing context
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuProgSegmentProbe(os_const_virt_ptr virt_addr, os_mmu_segment_handle *segment_num);


/**************************************************************************//**
 @Function      osMmuDataGetAttr

 @Description   Returns the attribute of MMU data segement according to virtual address

                Example: if the function returns OS_SUCCESS, use the attr as follows:
                         (attr & MMU_DATA_CACHEABLE_WRITETHROUGH) to check if the segment is cacheable (sc3x00)

 @Param[in]     virt_addr   -  virtual address pointer
 @Param[out]    attr        -  attributes of the segement in a form of masks. masks defined in
                               specific arch h file should be used

 @Return        OS_SUCCESS if address is legal within existing context
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuDataGetAttr(os_const_virt_ptr virt_addr, os_mmu_attr *attr);


/**************************************************************************//**
 @Function      osMmuProgGetAttr

 @Description   Returns the attribute of MMU program segement according to virtual address

                Example: if the function returns OS_SUCCESS, use the attr as follows:
                         (attr & MMU_PROG_L2_CACHEABLE) to check if the segment is cacheable (sc3x00)
 
 @Param[in]     virt_addr   -  virtual address pointer
 @Param[out]    attr        -  attributes of the segement in a form of masks. masks defined in
                               specific arch h file should be used

 @Return        OS_SUCCESS if address is legal within existing context
 @Return        Error status, encoded in os_error.h, for other errors

*//***************************************************************************/
os_status osMmuProgGetAttr(os_const_virt_ptr virt_addr, os_mmu_attr *attr);


/**************************************************************************//**
 @Function      osMmuProgNumOfSegmentsGet

 @Description   Get number total number prog segments
 
 @Return        number of used prog segments

*//***************************************************************************/
int osMmuProgNumOfSegmentsGet();


/**************************************************************************//**
 @Function      osMmuProgNumOfUsedSegmentsGet

 @Description   Get number of used prog segments

 @Return        number of used prog segments

*//***************************************************************************/
int osMmuProgNumOfUsedSegmentsGet();


/**************************************************************************//**
 @Function      osMmuProgMaxIdGet

 @Description   Get the number of maximum prog id
 
 @Return        The highest prog id supported by MMU

*//***************************************************************************/
int osMmuProgMaxIdGet();

#if defined(SC3X00) || defined(SC3X50)
/**************************************************************************//**
 @Function      osMmuNumOfProgContextsGet

 @Description   Get the number of maximum possible prog contexts

 @Return        number of maximum prog contexts

*//***************************************************************************/
int osMmuNumOfProgContextsGet();

#endif

/**************************************************************************//**
 @Function      osMmuProgErrorDetect

 @Description   Read the MMU and populate the os_mmu_error structre
 
 @Param[out]    err     - error structre

 @Return        Error status, encoded in os_error.h,

*//***************************************************************************/
os_status osMmuProgErrorDetect(struct os_mmu_error* err);

/**************************************************************************//**
 @Function      osMmuProgErrorClear

 @Description   Clear the data MMU status
 
*//***************************************************************************/
void osMmuProgErrorClear();

/**************************************************************************//**
 @Function      osMmuDataPhysToVirtManual

 @Description   Translate data physical address to virtual by manual inspection

 @Param[in]     phys_addr -  physical address
 @Param[out]    virt_addr -  virtual address
 
 @Cautions      This is an extremely slow implementation and shouldn't be used in runtime
 
 @Return        OS_SUCCESS if segment exists
*//***************************************************************************/
os_status osMmuDataPhysToVirtManual(os_const_phys_ptr phys_addr,os_virt_ptr* virt_addr);
/** @} */ // end of mem_mmu_prog_id

/** @} */ // end of mem_mmu_id

_OS_END_EXTERN_C

#endif // __OS_MMU_H

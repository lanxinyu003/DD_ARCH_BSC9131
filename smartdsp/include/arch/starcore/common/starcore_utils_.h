/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/03/14 12:21:28 $
 $Id: starcore_utils_.h,v 1.18 2013/03/14 12:21:28 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/common/starcore_utils_.h,v $
 $Revision: 1.18 $
******************************************************************************/

/******************************************************************************
 
 @File          starcore_utils_.h

 @Description   StarCore-specific utility functions internal header.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __STARCORE_UTILS_H_
#define __STARCORE_UTILS_H_


/**************************************************************************//**
 @Function      get_num_of_first_one_bit

 @Description   Finds the index of the first 1 bit in the argument.

 @Param[in]    value -  The 32-bit argument.

 @Return        Index of the first 1 bit in the argument.

*//***************************************************************************/
int get_num_of_first_one_bit(uint32_t value);

/**************************************************************************//**
 @Function      reverse_bits

 @Description   Performs a bit reverse on a 32 bit value.

 @Param[in]    value -  The 32-bit argument.

 @Return        The bitwise reverse value of the 32 bit value.

*//***************************************************************************/
uint32_t reverse_bits(uint32_t value);

/**************************************************************************//**
 @Function      readEMR

 @Description   Reads the EMR register.

 @Return        The value in EMR register.

*//***************************************************************************/
int readEMR();


/**************************************************************************//**
 @Function      ceil_power_of_two

 @Description   Alignes a value to the closest power of two higher or
                equal to the value.

 @Param[in]    value -  The 32-bit argument.

 @Return        The value in EMR register.

*//***************************************************************************/
#define ceil_power_of_two(value) \
    (((1 << get_num_of_first_one_bit((uint32_t)value)) == value) ? \
        value : (1 << (get_num_of_first_one_bit((uint32_t)value)+1)))



/**************************************************************************//**
 @Function      indexedResourceIsFree

 @Description   Safely obtains a resource that is managed by a bitmask.

 @Param[in]    resource -   Pointer to resource managing bitmask.
 @Param[in]    index -   Index of resource to check

 @Retval        TRUE  - resource is free.
 @Retval        FALSE - resource is not free.

*//***************************************************************************/
INLINE bool indexedResourceIsFree(volatile uint32_t *resource, uint32_t index)
{
    return ((*resource & (1 << index)) != 0);
}

/**************************************************************************//**
 @Function      indexedResourceGet

 @Description   Safely obtains a resource that is managed by a bitmask.

 @Param[in]    resource -   Pointer to resource managing bitmask.
 @Param[out]    index -  Index of resource allocated

 @Return        OS Status.

*//***************************************************************************/
INLINE os_status indexedResourceGet(volatile uint32_t *resource, uint32_t* index)
{
    os_status status = OS_FAIL;
 
    if (*resource != 0)
    {
        *index = (uint32_t)get_num_of_first_one_bit(*resource);
        *resource &= (~(0x1 << *index));
        status = OS_SUCCESS;
    }
 
    return status;
}

/**************************************************************************//**
 @Function      indexedResourceForceGet

 @Description   Force resource that is managed by a bitmask.

 @Param[in]    resource -   Pointer to resource managing bitmask.
 @Param[in]    index -  I ndex of resource allocated

 @Return        OS Status.

*//***************************************************************************/
INLINE os_status indexedResourceForceGet(volatile uint32_t *resource, uint32_t index)
{
    os_status status = OS_FAIL;
 
    if (indexedResourceIsFree(resource, index))
    {
        *resource &= (~(0x1 << index));
        status = OS_SUCCESS;
    }
 
    return status;
}


/**************************************************************************//**
 @Function      reversedIndexedResourceGet

 @Description   Finds the first available bit starting from 0.
 
                Used for the resources that are managed by a bitmask.

 @Param[in]    resource -   Pointer to resource managing bitmask.
 @Param[out]    index -  Index of resource allocated

 @Return        OS Status.

*//***************************************************************************/
INLINE os_status reversedIndexedResourceGet(volatile uint32_t *resource, uint32_t* index)
{
    os_status   status = OS_FAIL;
    uint32_t    shift = 0x00000001;
    int         counter = 0;
    uint32_t    value = *resource;
 
    value = reverse_bits(value);
 
    status = indexedResourceGet(&value, index);
    if (status == OS_SUCCESS)
    {
        value = reverse_bits(value);
        *resource = value;
        *index = 31 - *index;
    }
 
    return status;
}
/**************************************************************************//**
 @Function      indexedResourceFree

 @Description   Safely returns a resource that is managed by a bitmask.

 @Param[in]    resource -   Pointer to resource managing bitmask.
 @Param[in]    index -   Index of resource to return

 @Return        None.

*//***************************************************************************/
INLINE void indexedResourceFree(volatile uint32_t *resource, uint32_t index)
{
    *resource |= (0x1 << index);
}


#endif // __STARCORE_UTILS_H_

/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/04/09 15:02:07 $
 $Id: os_readylist_.h,v 1.9 2014/04/09 15:02:07 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_readylist_.h,v $
 $Revision: 1.9 $
******************************************************************************/

/******************************************************************************
 
 @File          os_readylist.h

 @Description   OS Ready-List header file.
 
                This module is private, and not included in the API.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_READYLIST_H
#define __OS_READYLIST_H


#define ANY_TASK    255
#define NO_TASK     0xFFFF

 

typedef struct os_ready_list_
{
    uint16_t        number;
    uint16_t        priority;
} os_ready_list_;

typedef volatile struct os_ready_list
{
    uint16_t        ready_cnt;
    uint16_t        size;
    os_ready_list_  *list;
} os_ready_list;



/**************************************************************************//**
 @Function      osReadyListInitialize

 @Description   Initializes the ready-list.
 
 @Param[in]    ready -  Pointer to the ready-list to initialize.
 @Param[in]    size -  Requested size of the ready-list.
 
 @Return        OS status

*//***************************************************************************/
os_status   osReadyListInitialize(os_ready_list *ready, uint16_t size);


/**************************************************************************//**
 @Function      osReadyListRemove

 @Description   Removes the passed in element from the ready-list.
 
 @Param[in]    ready -  Pointer to the ready-list.
 @Param[in]    number -  The ready element number to remove.
 
 @Retval        OS_SUCCESS  - if the element was found and removed.
 @Retval        OS_FAIL     - if the element was not found.

*//***************************************************************************/
os_status   osReadyListRemove(os_ready_list *ready, uint16_t number);


/**************************************************************************//**
 @Function      osReadyListPush

 @Description   Places an element on associated ready-list in order of priority.
 
 @Param[in]    ready -  Pointer to the ready-list.
 @Param[in]    number -  Number associated with the pushed element.
 @Param[in]    priority -  Priority associated with the pushed element.

 @Retval        OS_SUCCESS  - if the task was found and removed.
 @Retval        OS_FAIL     - if the task was not found.

*//***************************************************************************/
os_status   osReadyListPush(os_ready_list *ready, uint16_t number, uint16_t priority);


/**************************************************************************//**
 @Function      osReadyListPop

 @Description   Checks if there is a task with a higher priority (than the
                passed in priority) in the list, and removes it from the list.
                The new task must be switched into.
 
 @Param[in]    ready -  Pointer to the ready-list.
 @Param[in]    priority -  The priority to compare with.

 @Return        The higher priority task that is ready to run; if there is no
                such task, the function returns NO_TASK.

*//***************************************************************************/
uint16_t    osReadyListPop(os_ready_list *ready, uint16_t priority);


#endif // __OS_READYLIST_H

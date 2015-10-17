/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/11/12 08:54:22 $
 $Id: os_readylist.c,v 1.18 2013/11/12 08:54:22 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_readylist.c,v $
 $Revision: 1.18 $
******************************************************************************/

/******************************************************************************
 
 @File          os_readylist.c

 @Description   OS Ready-List functions.
 
                This module is private, and not included in the API.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_readylist_.h"


/*****************************************************************************/
os_status osReadyListInitialize(os_ready_list *ready, uint16_t size)
{
    uint16_t i;
 
    /* allocate ready list data */
    ready->list = (os_ready_list_ *)
        osMalloc((sizeof(os_ready_list_) * (size + 1)), OS_MEM_LOCAL);
 
    if (ready->list == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }

    /* set size */
    ready->size = size;

    /* clear ready count */
    ready->ready_cnt = 0;

    /* fill list with 0x1fff to ensure osReadyListPush() will break
       out during search. */
    for ( i=0; i <= size; i++ )
    {
        ready->list[i].number = 0x1fff;
        ready->list[i].priority = 0x1fff;
    }
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osReadyListRemove(os_ready_list *ready, uint16_t number)
{
    uint16_t    i, j;

    /* search the requested item */
    for ( i=0; i <= ready->ready_cnt; i++ )
    {
        if (ready->list[i].number == number)
            break;  /* item found */
    }

    if (i > ready->ready_cnt)
    {
        /* item was not found */
        return OS_FAIL;
    }

    /* update ready-list */
    for ( j=i; j <= ready->ready_cnt; j++ )
    {
        ready->list[j].priority = ready->list[(j+1)].priority;
        ready->list[j].number = ready->list[(j+1)].number;
    }
    ready->ready_cnt--;

    return OS_SUCCESS;
}



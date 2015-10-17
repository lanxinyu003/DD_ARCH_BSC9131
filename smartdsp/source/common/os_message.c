/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_message.c,v 1.3 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_message.c,v $
 $Revision: 1.3 $
******************************************************************************/

/******************************************************************************

 @File          os_messages.c

 @Description   os intercore messages runtime functions.

                In this file messages functions are implemented.The basic idea
                is to connect a shared 4 bytes of data with a virtual interrupt
                to enable one core pass a message to another. While the message
                is posted, no other core can post a message on it so the data
                is not changed until the destination core fetches the message.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_message_.h"
#include "os_rm_.h"
#include "os_message_shared_.h"

#if (OS_MULTICORE == ON)

extern volatile uint16_t  g_max_os_messages;

inline static void interruptCorePointToPoint(uint32_t dest_core, os_msg_handle msg_num)
{
    osHwiSwiftDisable();
    WRITE_UINT32(g_dsp_plat_map->mmu.m_dbe,
        1 << (OS_SOC_MAX_NUM_OF_CORES * dest_core + msg_num));
    osHwiSwiftEnable();
}

static inline uint16_t dest_msg_type(os_msg_handle msg_num, uint32_t dest_core)
{
    return shared_messages[msg_num].messages[osGetCoreID()][dest_core].type;
}

/*****************************************************************************/
os_status osMessagePost(os_msg_handle   msg_num,
                        uint32_t        dest_core,
                        uint32_t        msg_data)
{
    uint32_t posted = 0;

#ifdef MESSAGE_ERROR_CHECKING
    if ((dest_core >= OS_SOC_MAX_NUM_OF_CORES) || (dest_msg_type(msg_num, dest_core) != OS_MESSAGE_SYNC_TYPE))
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_MSG_DEST_INVALID);
    }
#endif /* MESSAGE_ERROR_CHECKING */

    msg_num = (os_msg_handle)MSG_NUM_MSG_NUM(msg_num);
    if(shared_messages[msg_num].messages[osGetCoreID()][dest_core].guard.count == 0) // to avoid spinlock nesting for the same core
    {
        posted = osSpinLockTryGet(&shared_messages[msg_num].messages[osGetCoreID()][dest_core].guard);
    }

    if (!posted)
    {
        return OS_ERR_MSG_DEST_LOCKED;
    }

    WRITE_UINT32_DBAR_IBSL(shared_messages[msg_num].messages[osGetCoreID()][dest_core].data, msg_data);
    interruptCorePointToPoint(dest_core, msg_num);

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osMessagePostIrq(os_msg_handle   msg_num,
                           uint32_t        dest_core,
                           uint32_t        msg_data)
{
    uint32_t posted;

#ifdef MESSAGE_ERROR_CHECKING
    if ((dest_core >= OS_SOC_MAX_NUM_OF_CORES) || (dest_msg_type(msg_num, dest_core) != OS_MESSAGE_SYNC_TYPE))
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_DEST_INVALID;
    }
#endif /* MESSAGE_ERROR_CHECKING */

    osHwiSwiftDisable();
    msg_num = (os_msg_handle)MSG_NUM_MSG_NUM(msg_num);
    if(shared_messages[msg_num].messages[osGetCoreID()][dest_core].guard.count == 0) // to avoid spinlock nesting for the same core
    {
        posted = osSpinLockTryGet(&shared_messages[msg_num].messages[osGetCoreID()][dest_core].guard);
    }

    if (!posted)
    {
        osHwiSwiftEnable();
        return OS_ERR_MSG_DEST_LOCKED;
    }

    WRITE_UINT32_DBAR_IBSL(shared_messages[msg_num].messages[osGetCoreID()][dest_core].data, msg_data);
    interruptCorePointToPoint(dest_core, msg_num);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/
uint32_t osMessageGet(os_msg_handle msg_num)
{
    uint32_t msg_data;
    uint16_t src_core = (uint16_t)MSG_NUM_SRC_CORE(msg_num);

    msg_num = (os_msg_handle)MSG_NUM_MSG_NUM(msg_num);

#ifdef MESSAGE_ERROR_CHECKING
    if (msg_num >= g_max_os_messages)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return 0;
    }
    if (src_core > OS_SOC_MAX_NUM_OF_CORES)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_SRC_INVALID;
    }
#endif /* MESSAGE_ERROR_CHECKING */


    osHwiSwiftDisable();


#ifdef MESSAGE_ERROR_CHECKING
    if (shared_messages[msg_num].messages[src_core][osGetCoreID()].status < OS_RESOURCE_USED)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        osHwiSwiftEnable();
        return 0;
    }
    if (shared_messages[msg_num].messages[src_core][osGetCoreID()].guard.count == 0)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /*MESSAGE_ERROR_ASSERT */
        osHwiSwiftEnable();
        return OS_ERR_MSG_NO_MSG_TO_GET;
    }
#endif /* MESSAGE_ERROR_CHECKING */

    msg_data = shared_messages[msg_num].messages[src_core][osGetCoreID()].data;
    osSpinLockRelease(&shared_messages[msg_num].messages[src_core][osGetCoreID()].guard);

    osHwiSwiftEnable();

    return msg_data;
}



/*****************************************************************************/
os_status osMessagePostAsync(os_msg_handle   msg_num,
                        uint32_t        dest_core)
{

#ifdef MESSAGE_ERROR_CHECKING
    if ((dest_core >= OS_SOC_MAX_NUM_OF_CORES) || (dest_msg_type(msg_num, dest_core) != OS_MESSAGE_ASYNC_TYPE))
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_DEST_INVALID;
    }
#endif /* MESSAGE_ERROR_CHECKING */

    interruptCorePointToPoint(dest_core, (uint16_t)MSG_NUM_MSG_NUM(msg_num));

    return OS_SUCCESS;
}


#endif // (OS_MULTICORE == ON)



/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/04/04 07:22:04 $
 $Id: os_message.h,v 1.17 2011/04/04 07:22:04 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_message.h,v $
 $Revision: 1.17 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_message.h

 @Description   OS Intercore Messages header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MESSAGE_H
#define __OS_MESSAGE_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#if (OS_MULTICORE == 1)

/**************************************************************************//**
 @Group         inter_msg_id Intercore Messages

 @Description   Intercore Messages API.
 
                This module is available only on multicore systems.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         inter_msg_msg_id Messages API

 @Description   Intercore Messages setup and control.
 

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osMessageCreate

 @Description   Installs an intercore message handler for the calling core.
 
                Enables the calling core to be notified when the specified
                message arrives. The calling core can then handle the message.
 
 @Param[in]     msg_num     -  The intercore message number.
 @Param[in]     msg_handler -  The function to call when the message arrives.

 @Retval        OS_SUCCESS                  - The message handler was successfully installed.
 @Retval        OS_ERR_MSG_INVALID          - Invalid message number.
 @Retval        OS_ERR_MSG_FUNCTION_INVALID - NULL message handler.
 @Retval        OS_ERR_MSG_ALREADY_CREATED  - The message number is already created.
*//***************************************************************************/
os_status   osMessageCreate(os_msg_handle   msg_num, os_hwi_function msg_handler);


/**************************************************************************//**
 @Function      osMessageCreateAsync

 @Description   Installs an intercore message handler for the calling core.
 
                Enables the calling core to be notified when the specified
                message arrives. The calling core can then handle the message.
 
 @Param[in]     msg_num     -  The intercore message number.
 @Param[in]     msg_handler -  The function to call when the message arrives.
 @Param[in]     msg_data    -  The data to include in the message.
 @Param[in]     v_p         -  virtual interrupt priority

 @Retval        OS_SUCCESS                  - The message handler was successfully installed.
 @Retval        OS_ERR_MSG_INVALID          - Invalid message number.
 @Retval        OS_ERR_MSG_FUNCTION_INVALID - NULL message handler.
 @Retval        OS_ERR_MSG_ALREADY_CREATED  - The message number is already created.
*//***************************************************************************/
os_status   osMessageCreateAsync(os_msg_handle   msg_num,
                                 os_hwi_function msg_handler,
                                 uint32_t        msg_data,
                                 os_hwi_priority v_p);

/**************************************************************************//**
 @Function      osMessageDelete

 @Description   Deletes an existing intercore message handler.
 
                The deleted message number cannot be used after it is deleted.
 
 @Param[in]     msg_num -  The intercore message number.

 @Retval        OS_SUCCESS              - The message handler was successfully deleted.
 @Retval        OS_ERR_MSG_INVALID      - Invalid message number.
 @Retval        OS_ERR_MSG_NOT_CREATED  - The message number was not created, or it was already deleted.
*//***************************************************************************/
os_status   osMessageDelete(os_msg_handle msg_num);


/**************************************************************************//**
 @Function      osMessagePost

 @Description   Posts the given message number with the given data to the
                destination core.
 
                While the message is posted, no other core can post the same
                message number to the same core; as a result, the data is safe
                until the destination core fetches the message.
 
 @Param[in]     msg_num      -  The intercore message number.
 @Param[in]     dest_core    -  The core that should receive the message.
 @Param[in]     msg_data     -  The data to include in the message.

 @Retval        OS_SUCCESS              - The message was successfully posted.
 @Retval        OS_ERR_MSG_INVALID      - Invalid message number.
 @Retval        OS_ERR_MSG_DEST_INVALID - Invalid destination.
 @Retval        OS_ERR_MSG_BUSY         - Destination is locked for posting.
*//***************************************************************************/
os_status   osMessagePost(os_msg_handle msg_num,
                          uint32_t      dest_core,
                          uint32_t      msg_data);


/**************************************************************************//**
 @Function      osMessagePostIrq

 @Description   Posts the given message number with the given data to the
                destination core while interrupts are disabled.
 
                While the message is posted, no other core can post the same
                message number to the same core; as a result, the data is safe
                until the destination core fetches the message.
 
 @Param[in]    msg_num      -  The intercore message number.
 @Param[in]    dest_core    -  The core that should receive the message.
 @Param[in]    msg_data     -  The data to include in the message.

 @Retval        OS_SUCCESS              - The message was successfully posted.
 @Retval        OS_ERR_MSG_INVALID      - Invalid message number.
 @Retval        OS_ERR_MSG_DEST_INVALID - Invalid destination.
 @Retval        OS_ERR_MSG_BUSY         - Destination is locked for posting.
*//***************************************************************************/
os_status osMessagePostIrq(os_msg_handle   msg_num,
                           uint32_t        dest_core,
                           uint32_t        msg_data);


/**************************************************************************//**
 @Function      osMessageGet

 @Description   Retrieves the data from a message that was posted to the
                calling core.
 
                This function must be called by the message handler.
 
 @Param[in]     msg_num -  The intercore message number.

 @Return        The data contained in the message; passing an invalid or a
                non-created message number will always return zero.

 @Cautions      You must call this function in your message handler, even if
                the message data has no meaning for the calling core. If you
                fail to follow this rule, messages of the same message
                number cannot be posted to the calling core again.
*//***************************************************************************/
uint32_t    osMessageGet(os_msg_handle msg_num);


/**************************************************************************//**
 @Function      osMessagePost

 @Description   Posts the given message number with the given data to the
                destination core.
 
 @Param[in]     msg_num      -  The intercore message number.
 @Param[in]     dest_core    -  The core that should receive the message.

 @Retval        OS_SUCCESS              - The message was successfully posted.
 @Retval        OS_ERR_MSG_INVALID      - Invalid message number.
 @Retval        OS_ERR_MSG_DEST_INVALID - Invalid destination.
*//***************************************************************************/
os_status   osMessagePostAsync(os_msg_handle msg_num, uint32_t      dest_core);


/**************************************************************************//**
 @Function      osMessageFind

 @Description   Finds the first available intercore message number.
 
 @Param[out]    msg_num -  Receives the intercore message number.

 @Retval        OS_SUCCESS              - Found an available message number.
 @Retval        OS_ERR_MSG_UNAVAILABLE  - No message number is available.
*//***************************************************************************/
os_status   osMessageFind(os_msg_handle *msg_num);

/** @} */ // inter_msg_msg_id
/** @} */ // inter_msg_id

#endif // OS_MULTICORE

_OS_END_EXTERN_C

#endif // __OS_MESSAGE_H


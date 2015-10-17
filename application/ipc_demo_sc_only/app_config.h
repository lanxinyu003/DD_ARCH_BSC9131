/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:27:45 $
 $Id: app_config.h,v 1.4 2012/11/22 16:27:45 sw Exp $
 $Source: /cvsdata/SmartDSP/demos/starcore/psc9x3x/ipc_demo_sc_only/app_config.h,v $
 $Revision: 1.4 $
******************************************************************************/

/******************************************************************************
 
 @File          app_config.h

 @Description   Application Configuration definitions.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

//#define CONTINUOUS_TEST

#define MESSAGE_CHANNELS /*POINTER_CHANNELS*/
#define /*VIRQ_INTERRUPT*/   MESH_INTERRUPT

#define SHOW_CHANNEL_PEEK_USAGE

/* Application Configuration *************************************************/
#define MESSAGE_LENGTH           256 //bytes
#define NUM_OF_CHANNELS          SOC_MAX_NUM_OF_CORES
#define NUM_OF_MESSAGES          200
#define BD_RING_SIZE             4
#define BUFFERS_TO_KEEP          1 //number of buffers that might be kept  - relevant only for message channels
#define NUM_OF_BUFFERS           (NUM_OF_CHANNELS * BD_RING_SIZE + BUFFERS_TO_KEEP)

#define CHANNEL_ID_1             0x00000001
#define CHANNEL_ID_2             0x00000002
#define MAX_BD_SIZE              4
#define IPC_CHANNELS_NUMBER      SOC_MAX_NUM_OF_CORES

#endif __APP_CONFIG_H

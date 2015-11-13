/******************************************************************************
 Copyright �1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/01/21 12:17:43 $
 $Id: ipc_demo.c,v 1.11 2013/01/21 12:17:43 b08551 Exp $
 $Source: /cvsdata/SmartDSP/demos/starcore/psc9x3x/ipc_demo_sc_only/ipc_demo.c,v $
 $Revision: 1.11 $
******************************************************************************/

/******************************************************************************

 @File          malloc_demo.c

 @Description   PSC9X3X demo for shared/ non-shared memory allocation.

*//***************************************************************************/

//#define OS_PRINT_MALLOC_STATISTICS 1
#include "smartdsp_os.h"
#include "os_config.h"
#include "app_config.h"
#include "os_cache.h"
#include "sc3x00_cache.h"
#include "psc913x_heterogeneous.h"
#include "psc913x_heterogeneous_ipc.h"
#include "psc913x_ipc.h"
#include "psc913x_ipc_init.h"
#ifndef PSC9X3X_SC_ONLY_BSP
#include "psc9x3x_het_bsp_stub.h"
#endif

extern os_het_control_t het_control;
uint32_t  channel_handles[NUM_OF_CHANNELS];
uint32_t  message[MESSAGE_LENGTH/4]__attribute__ ((section(".os_shared_data_bss")));

os_mem_part_t       *buffers_pool;
uint8_t             buffers_space[MEM_PART_DATA_SIZE(NUM_OF_BUFFERS, MESSAGE_LENGTH, ALIGNED_64_BYTES)] __attribute__ ((section(".os_shared_data_bss")));
uint8_t             mem_manager[MEM_PART_SIZE(NUM_OF_BUFFERS)] __attribute__ ((section(".os_shared_data_bss")));
uint32_t            rx_counter[2] = {0}, tx_counter[2] = {0};
void *temp;

static void DemoErrorCallback(void* param)
{
    VAR_UNUSED(param);
}

static os_het_status_t demoMessageChannelMessageSend(void* channel, void* message_data)
{
    uint32_t        **message_begin_ptr, *message_begin, *data, *message_data_ptr;
    os_het_status_t status;
    uint32_t i;
 
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(message_data != NULL);
 
    tx_counter[0]++;
    if (tx_counter[0] == 0)
       tx_counter[1]++;
 
    message_data_ptr = (uint32_t*)message_data;
    message_begin_ptr = &message_begin;
    status = osIpcMessagePtrGet (channel, (void**)message_begin_ptr);
    OS_ASSERT_COND(status == OS_HETERO_SUCCESS);
    data = *message_begin_ptr;
    for ( i=0 ; i < MESSAGE_LENGTH/4 ; i++ )
        data[i] = message_data_ptr[i];

    status = osIpcMessageSendPtr(channel, (void*)*message_begin_ptr, MESSAGE_LENGTH, IPC_DO_NOT_SEND_INDICATION);
    OS_ASSERT_COND(status == OS_HETERO_SUCCESS);
}

static os_het_status_t demoPointerChannelMessageSend(void* channel, void* message_data)
{
    os_het_status_t status;

    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(message_data != NULL);
 
    tx_counter[0]++;
    if (tx_counter[0] == 0)
       tx_counter[1]++;

    status = osIpcMessageSendPtr(channel, message_data, MESSAGE_LENGTH, IPC_DO_NOT_SEND_INDICATION);
    OS_ASSERT_COND(status == OS_HETERO_SUCCESS);
}

static int intCount = 0;

static void receiveMessageCallback(void* ch, void* data, uint32_t length)
{
   uint32_t i;
   uint32_t *data_begin, *data_ptr;
   os_het_status_t status;
   void     *peek_data;
   uint32_t  peek_length;
   
   intCount++;
 
   OS_ASSERT_COND(ch != NULL);
   data_begin = data_ptr = (uint32_t*)data;
   for ( i = 1 ; i < length/4 ; i++ )
       OS_ASSERT_COND(data_ptr[i] == message[i]);
 
   rx_counter[0]++;
   if (rx_counter[0] == 0)
       rx_counter[1]++;
#ifndef CONTINUOUS_TEST
   if (*data_begin < NUM_OF_MESSAGES)
#else
   if ((rx_counter[0] & 0xfffff) == 0)
       printf ("\n0x%x%x messages were sent and received \n", rx_counter[1], rx_counter[0]);
   if(1)
#endif
   {
       message[0] = *data_begin + 1;
#ifdef POINTER_CHANNELS
       status =  demoPointerChannelMessageSend((void*)ch, (void*)&message[0]);
#else
       if (message[0] == 53)
       {
           temp = data;
           osIpcMessageChannelBufferReplace(ch);
       } else if (message[0] == 63)
       {
           osIpcMessageChannelBufferRelease(ch, temp);
       }
 
#ifdef SHOW_CHANNEL_PEEK_USAGE
       status = osIpcChannelPeek(ch, &peek_data, &peek_length);
       OS_ASSERT_COND(peek_data = data);
       OS_ASSERT_COND(peek_length = length);
#endif //SHOW_PEEK_USAGE
 
       status =  demoMessageChannelMessageSend(ch, (void*)&message[0]);
#endif
       OS_ASSERT_COND(status == OS_HETERO_SUCCESS);
   }
   else
   {
       printf ("\nIPC demo PASSED for %d cores and %d channels!!!\n%d messages with 0x%x bytes of data were sent and received correctly between cores\n", OS_NUM_OF_CORES, NUM_OF_CHANNELS, NUM_OF_MESSAGES, MESSAGE_LENGTH);
       OS_TEST_PASSED;
   }

}

/**************************************************************************
    mccDemoBackground :
    Background task - Loops indefinitely.
**************************************************************************/
void appBackground(void)
{

    uint32_t        i;
    void            *channel;
    uint32_t        **message_begin_ptr, *message_begin,  *data;
    os_het_status_t status;
 
    for ( i=0 ; i < MESSAGE_LENGTH/4 ; i++ )
        message[i] = i;
    if (osGetCoreID() == 0)
    {
        channel = (void*)channel_handles[0];
#ifdef POINTER_CHANNELS
        status =  demoPointerChannelMessageSend(channel, (void*)&message[0]);
#else
        status =  demoMessageChannelMessageSend(channel, (void*)&message[0]);
#endif
        OS_ASSERT_COND(status == OS_HETERO_SUCCESS);
    }
 
    while (1)
    {
        if ((tx_counter[0] != rx_counter[0]) || (tx_counter[0] != rx_counter[0]))
            status = osIpcIndicationSend((void*)channel_handles[0]);
    }

}

os_status appInit()
{
    os_het_status_t   status;
    void            *channel;
    os_ipc_channel_consumer_open_params_t consumer_params;
    os_ipc_channel_producer_open_params_t producer_params;
 
 
    /* create the buffers pool */
    buffers_pool = osMemPartCreate(MESSAGE_LENGTH,
                                   NUM_OF_BUFFERS,
                                   buffers_space,
                                   ALIGNED_64_BYTES,
                                   OFFSET_0_BYTES,
                                   (os_mem_part_t *)mem_manager
#if OS_MULTICORE
                                   ,1
#endif
                                   );


    //find channel
    channel = osIpcChannelIdFind(CHANNEL_ID_1 + osGetCoreID());
    OS_ASSERT_COND(channel != NULL);
    channel_handles[osGetCoreID()] = (uint32_t)channel;

    //open channel - producer
    producer_params.additional_params = NULL;
    producer_params.ch = (void*)channel_handles[osGetCoreID()];
    producer_params.cache_policy = L1_L2_CACHED;
    status = osIpcChannelProducerOpen(&producer_params);
 
    //open channel - consumer
    consumer_params.bd_ring_size = BD_RING_SIZE;
    consumer_params.buffers_pool = buffers_pool;
    consumer_params.callback = receiveMessageCallback;
    consumer_params.ch = (void*)channel_handles[osGetCoreID()];
#ifdef POINTER_CHANNELS
    consumer_params.channel_type = OS_HET_IPC_POINTER_CH;
#else //MESSAGE_CHANNEL
    consumer_params.channel_type = OS_HET_IPC_MESSAGE_CH;
#endif //CHANNELS TYPE
#ifdef VIRQ_INTERRUPT
    consumer_params.indication_type = OS_HET_VIRTUAL_INT;
#else//MESH_INTERRUPT
    consumer_params.indication_type = OS_HET_DSP_MESH;
#endif//INTERRUPT_TYPE
    consumer_params.int_argument = (os_hwi_arg)channel_handles[osGetCoreID()];
    consumer_params.int_priority = OS_HWI_PRIORITY0;
    consumer_params.max_msg_size = MESSAGE_LENGTH;
    consumer_params.cache_policy = L1_L2_CACHED;
    status = osIpcChannelConsumerOpen(&consumer_params);

    osWaitForAllCores();
    return OS_SUCCESS;
}

void os_smartdsp_init()
{
    os_status status;   // The only local variable in main()
 
#if !defined(PSC9X3X_SC_ONLY_BSP) && (OS_MIMIC_PA_ARCH == ON)
    // Simulate PA
    psc913xHetBspStubInitialize((void *)SOC_HET_CTRL_BASE, &het_control);
#endif // PSC9X3X_SC_ONLY_BSP && OS_MIMIC_PA_ARCH
 
    /* OS Initialization - call before any other OS API calls. */
    status = osInitialize();
    if (status != OS_SUCCESS) OS_ASSERT;

    /* Interrupts are disabled until osStart() is called.
       You must not enable interrupts at this point !!! */

    /* Place any required initialization code within appInit().
       Using local variables inside main() is NOT recommended, because
       its stack is never restored - osStart() never returns !!! */
    status = appInit();
    if (status != OS_SUCCESS) OS_ASSERT;

    /* Start the OS with the background task. OS ticks now start.
       appBackground() should not return unless there is an error. */
     //status = osStart(appBackground);
    //if (status != OS_SUCCESS) OS_ASSERT;

    /* Execution reaches this point only if an error occurs. */
}




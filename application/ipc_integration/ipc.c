/******************************************************************************
 Copyright �1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/05/08 08:30:45 $
 $Id: ipc.c,v 1.12 2013/05/08 08:30:45 b08551 Exp $
 $Source: /cvsdata/SmartDSP/demos/starcore/psc9x3x/ipc_integration/ipc.c,v $
 $Revision: 1.12 $
******************************************************************************/

/******************************************************************************

 @File          ipc.c

 @Description   PSC9X3X demo for IPC.

*//***************************************************************************/

//#define OS_PRINT_MALLOC_STATISTICS 1
#include "smartdsp_os.h"
#include "os_config.h"
#include "os_cache.h"
#include "os_mmu.h"
#include "sc3x00_cache.h"
#include "psc913x_heterogeneous_ipc.h"
#include "psc913x_ipc_init.h"
#include "psc913x_ipc.h"
#ifndef PSC9X3X_SC_ONLY_BSP
#include "psc9x3x_het_bsp_stub.h"
#endif

#include <stdlib.h>

#define RETURN_ASSERT_STATUS(STATUS) \
    if (STATUS != OS_SUCCESS) {OS_ASSERT; RETURN_ERROR(STATUS);}

#define MAX_MSG_SIZE        1024    // Size of messages in the message channels
#define BD_RING_SIZE        16       // Size of the BD ring
#define LOG_SIZE            10000   // Number of entries in log
#define NUM_BUFFERS         1000    // Number of buffers

enum
{
  BEGIN_APP_INIT_CMD = 1,
  END_APP_INIT_CMD
};

extern os_het_control_t  *g_os_het_control;
volatile int hold = 0;

enum
{
    SC_TO_PA_DEDICATED = 0,
    RESERVED,
    PA_TO_SC_IND,
    SC_TO_PA_IND,
 
    SC_TO_PA_NO_IND,
    PA_TO_SC_NO_IND,
 
    PA_TO_SC_TX_CONFIG,
    SC_TO_PA_TX_CONFIG,
    NUM_IPC_CH
};
void *ipc_ch[NUM_IPC_CH];

os_queue_handle queue;
os_queue_handle tx_config_queue;
uint8_t         buf_manager[MEM_PART_SIZE(NUM_BUFFERS)];
uint8_t         *buf_space;
os_mem_part_t   *buf_pool;

uintptr_t       system_log ;
uint32_t        system_log_size;

char logData[10] = {0,1,2,3,4,5,6,7,8,9};
#pragma align logData 4

struct ipc_log
{
    uint64_t time;
    uint32_t ch_num;
    void*    ptr;
    uint32_t len;
};

/*struct test_data
{
    uint64_t time;
    uint32_t index;
    uint32_t size;
    uint8_t  data[];
};*/

struct tx_config_msg
{
    uint32_t   data[(MAX_MSG_SIZE-sizeof(uint32_t *)) / sizeof(uint32_t)];
    uint32_t  *ptr;
};


volatile unsigned int log_entry = 0;
struct ipc_log        *logging;


static void validate_rx_pointer(void *ptr)
{
    os_status status;
 
    status = osMmuDataVirtProbe((const void *)ptr);
    if (status != OS_SUCCESS)
    {
        REPORT_ERROR_MESSAGE(status, "Rx pointer is not accessible");
        OS_ASSERT;
    }
}

static void loop_send(void* ch, void* data, uint32_t length)
{
    os_het_status_t status;
    int num_failures = 0;
 
    while (num_failures++ < 100)
    {
        //printf ("\nsending SC_TO_PA_IND message\n");
        //status = osIpcMessageSendPtr(ch, data, 0, TRUE);
        status = osIpcMessageSendPtr(ch, data, length, FALSE);
        if(status != OS_HETERO_SUCCESS)
            OS_WAIT(10000)
        else
            break;
    }
 
    if(status != OS_HETERO_SUCCESS)
        REPORT_ERROR_MESSAGE(OS_ERR_FULL, "Failed to send to PA channel");
}

static void appRxEcho(void* ch, void* data, uint32_t length)
{	
    VAR_UNUSED(ch);
    OS_ASSERT_COND(data != NULL);
    validate_rx_pointer(data);
    
    //loop_send(ipc_ch[SC_TO_PA_IND], data, length);
    //loop_send(ipc_ch[SC_TO_PA_IND], logData, 5);
    //printf("channel 2 and 3 is running\n");
    hold = 1;
}

static void appRxValidate(void* ch, void* data, uint32_t length)
{
    /*os_status status;
    //struct test_data *tx_datum;
    //struct test_data *rx_datum = (struct test_data *)data;
    int i;
 
    VAR_UNUSED(ch);
    VAR_UNUSED(length);
    OS_ASSERT_COND(data != NULL);
 
    //validate_rx_pointer(data);
    status = osQueueDequeue(queue, (uint32_t*)&tx_datum);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    OS_ASSERT_COND(tx_datum->time  == rx_datum->time);
    OS_ASSERT_COND(tx_datum->index == rx_datum->index);
    OS_ASSERT_COND(tx_datum->size  == rx_datum->size);
 
    for (i=0 ; i<rx_datum->size ; i++)
    {
        OS_ASSERT_COND(tx_datum->data[i] == rx_datum->data[i]);
    }
 
    //osMemBlockFree(buf_pool, tx_datum);
    //printf("channel 4 and 5 is running\n");*/
}

/*static void appTxConfigCb(void* ch, void* data, uint32_t length)
{
    struct tx_config_msg * msg = (struct tx_config_msg *)data;
    uint32_t old_consumed, new_consumed;
    uint32_t  *new_buf;
    int i, idx=0;
    os_status status;
 
    OS_ASSERT_COND(data != NULL);
    VAR_UNUSED(ch);
    VAR_UNUSED(length);
    validate_rx_pointer(data);
 
    new_buf = osMemBlockGet(buf_pool);
    OS_ASSERT_COND(new_buf != NULL);
 
    new_buf[idx++] = (uint32_t)msg->ptr;
    for (i=0 ; i<4 ; i++)
        new_buf[idx++] = msg->data[i];
    for (i=0 ; i<4 ; i++)
        new_buf[idx++] = msg->ptr[i];

    status = osQueueEnqueue(tx_config_queue, (uint32_t)new_buf);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    status = osIpcConsumerIndexGet(ipc_ch[SC_TO_PA_TX_CONFIG], &old_consumed);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    loop_send(ipc_ch[SC_TO_PA_TX_CONFIG], (void*)new_buf, 36);
 
    status = osIpcConsumerIndexGet(ipc_ch[SC_TO_PA_TX_CONFIG], &new_consumed);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    while ((old_consumed+1) != new_consumed)
    {
        status = osIpcConsumerIndexGet(ipc_ch[SC_TO_PA_TX_CONFIG], &new_consumed);
        OS_ASSERT_COND(status == OS_SUCCESS);
    }

    status = osQueueDequeue(tx_config_queue, (uint32_t *)&new_buf);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    osMemBlockFree(buf_pool, new_buf);
    
    //printf("channel 6 and 7 is running\n");
}*/

static void appBackground(void)
{
    os_het_status_t  het_status;
    os_status        status;
    struct test_data *datum;
    uint32_t         index = 0;
    int              i;
    int              num_failures;
 
    /* Send indication to PA that SC is ready */
    status = osIpcMessageSendPtr(ipc_ch[SC_TO_PA_DEDICATED], logging, 0, FALSE);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    //waiting until getting a message back. hold will be released by appRxEcho
    while(hold == 0){}
    
    //while(hold == 1){}
 
    while(1)
    {
        num_failures = 0;
        /* Send our message */
        //if (osQueueStatus(queue) != OS_QUEUE_FULL)
        {
            //datum = (struct test_data*)osMemBlockGet(buf_pool);
            //if (datum != NULL)
            //{
                //osTickTime(&datum->time);
                //datum->index = index++;
                //datum->size = rand() % (MAX_MSG_SIZE-16);
                //for (i=0 ; i<datum->size ; i++)
                    //datum->data[i] = rand();
 
                //status = osQueueEnqueue(queue, (uint32_t)datum);
            	//status = osQueueEnqueue(queue, (uint32_t)logData);
                //OS_ASSERT_COND(status == OS_SUCCESS);
 
                do
                {
                    //printf ("\nsending SC_TO_PA_NO_IND message\n");
                    het_status = osIpcMessageSendPtr(ipc_ch[SC_TO_PA_NO_IND],
                    								(void *)logData, 10, FALSE);
                                                     //(void *)datum, datum->size + 16, FALSE);
                    if (het_status != OS_HETERO_SUCCESS)
                    {
                        num_failures++;
                        OS_WAIT(10000);
                    }               
                } while (het_status != OS_HETERO_SUCCESS);
            //}
                OS_WAIT(1000);
        }
 
        /* Poll PA message */
        //osIpcMessageReceiveCb((os_hwi_arg)ipc_ch[PA_TO_SC_NO_IND]);
    }
}

/*static void debug_hook(debug_hook_arg ch, debug_hook_arg bd)
{
    os_het_ipc_bd_t *curr_bd = (os_het_ipc_bd_t*)bd;
    os_het_status_t status;
    struct ipc_log  log;
 
    OS_ASSERT_COND(curr_bd != NULL);
 
    log.time = osTickTime();
    log.ptr = (void *)curr_bd->msg_ptr;
    log.len = curr_bd->msg_len;
    status = osIpcChannelNumGet(ch, &log.ch_num);
    OS_ASSERT_COND(status == OS_HETERO_SUCCESS);
 
    logging[log_entry++] = log;
    if (log_entry == LOG_SIZE)
        log_entry = 0;
}*/

static os_het_status_t open_procducer_ch(void *ch)
{
    os_het_status_t het_status;
    os_ipc_channel_producer_open_params_t producer = {NULL, NULL, L1_L2_CACHED};

    producer.ch = ch;
    het_status = osIpcChannelProducerOpen(&producer);
    RETURN_ASSERT_STATUS(het_status);
    return het_status;
}

static os_het_status_t open_consumer_ch(void *ch, os_het_ipc_ind_t indication, void *callback)
{
    os_het_status_t het_status;
    os_ipc_channel_consumer_open_params_t consumer =
    {
         NULL,
         /**< Pointer to the relevant channel as received from osIpcChannelIdFind*/
         OS_HET_IPC_MESSAGE_CH,
         /**< type of channel - message channel or pointer channel*/
         OS_HET_VIRTUAL_INT,
         /**< the indication type to use*/
         OS_HWI_PRIORITY3,
         /**< priority of interrupt, if used*/
         0,
         /**< argument of interrupt, if used*/
         BD_RING_SIZE,
         /**< size of the mailbox BD ring*/
         MAX_MSG_SIZE,
         /**< maximal size of the message that can be passed on this channel*/
         NULL,
         /**< callback function to call when receiving a message*/
         NULL,
         /**< only for a message channel - the buffer pool to allocate the buffers from*/
         L1_L2_CACHED
         /**< caching policy to use in the cannel. according to os_cache.h defines*/
    };

    consumer.ch = ch;
    consumer.indication_type = indication;
    consumer.callback = callback;
    consumer.buffers_pool = buf_pool;
 
    het_status = osIpcChannelConsumerOpen(&consumer);
    RETURN_ASSERT_STATUS(het_status);
    return het_status;
}

static os_status appInit()
{
    os_status       status;
    int i;
 
    system_log = (*g_os_het_control->smartdsp_debug)[0].base_address;
    system_log_size = (*g_os_het_control->smartdsp_debug)[0].size;
    *((uint32_t*)system_log) = BEGIN_APP_INIT_CMD;
    system_log += 4;
 
    logging = osMalloc(sizeof(struct ipc_log) * LOG_SIZE, OS_MEM_HET_DDR1_CACHEABLE);
    OS_ASSERT_COND(logging != NULL);
 
    buf_space = osMalloc(MEM_PART_DATA_SIZE(NUM_BUFFERS, MAX_MSG_SIZE, ALIGNED_8_BYTES),
            OS_MEM_HET_DDR1_CACHEABLE);
    OS_ASSERT_COND(buf_space != NULL);

    buf_pool  = osMemPartCreate(MAX_MSG_SIZE, NUM_BUFFERS, buf_space,
                                 ALIGNED_8_BYTES, OFFSET_0_BYTES,
                                 (os_mem_part_t *)buf_manager
#if (OS_MULTICORE == 1)
                                ,0
#endif
    );
    OS_ASSERT_COND(buf_pool != NULL);

    for (i=0 ; i<NUM_IPC_CH ; i++)
    {
#ifdef MULTIMODE
        ipc_ch[i] = osIpcMultimodeChannelIdFind(i, osGetCoreID());
#else //single mode
        ipc_ch[i] = osIpcChannelIdFind(i);
#endif//MULTIMODE
        OS_ASSERT_COND(ipc_ch[i] != NULL);
    }
 
    /* Open the producer channels */
    open_procducer_ch(ipc_ch[SC_TO_PA_DEDICATED]);
    //open_procducer_ch(ipc_ch[SC_TO_PA_IND]);
    open_procducer_ch(ipc_ch[SC_TO_PA_NO_IND]);
    //open_procducer_ch(ipc_ch[SC_TO_PA_TX_CONFIG]);
 
    /* Open the consumer channels */
    open_consumer_ch(ipc_ch[PA_TO_SC_IND], OS_HET_VIRTUAL_INT, appRxEcho);
    //open_consumer_ch(ipc_ch[PA_TO_SC_NO_IND], OS_HET_NO_INT, appRxValidate);
    //open_consumer_ch(ipc_ch[PA_TO_SC_TX_CONFIG], OS_HET_VIRTUAL_INT, appTxConfigCb);

    /*status = osDebugHookCreate(OS_DEBUG_IPC_BASIC_SEND, debug_hook);
    RETURN_ASSERT_STATUS(status);
    status = osDebugHookCreate(OS_DEBUG_IPC_BASIC_RECEIVE, debug_hook);
    RETURN_ASSERT_STATUS(status);
 
    status = osQueueFind(&queue, FALSE);
    RETURN_ASSERT_STATUS(status);
 
    status = osQueueCreate(queue, BD_RING_SIZE);
    RETURN_ASSERT_STATUS(status);*/

    //status = osQueueFind(&tx_config_queue, FALSE);
    //RETURN_ASSERT_STATUS(status);

    //status = osQueueCreate(tx_config_queue, BD_RING_SIZE);
    //RETURN_ASSERT_STATUS(status);

    *((uint32_t*)system_log) = END_APP_INIT_CMD;
    system_log += 4;
    return OS_SUCCESS;
}


void main()
{
    os_status status;   // The only local variable in main()
 
//#if !defined(PSC9X3X_SC_ONLY_BSP) && (OS_MIMIC_PA_ARCH == ON)
    // Simulate PA
    psc913xHetBspStubInitialize((void *)SOC_HET_CTRL_BASE, NULL);
//#endif // PSC9X3X_SC_ONLY_BSP && OS_MIMIC_PA_ARCH
 
    *((uint32_t*)0xb0020dbc) = 0x1;
    system_log += 4;
 
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
    status = osStart(appBackground);
    if (status != OS_SUCCESS) OS_ASSERT;

    /* Execution reaches this point only if an error occurs. */
}


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

uint8_t         buf_manager[MEM_PART_SIZE(NUM_BUFFERS)];
uint8_t         *buf_space;
os_mem_part_t   *buf_pool;

char logData[10] __attribute__ ((section(".os_shared_data_bss"))); 
#pragma align logData 4

/*struct ipc_log
{
    uint64_t time;
    uint32_t ch_num;
    void*    ptr;
    uint32_t len;
};*/

//struct ipc_log *logging;

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
    
    hold++;
}

void appBackground(void)
{
    os_status status;
    int i;
    
    for(i=0; i<sizeof(logData); i++)
    	logData[i] = i;
 
    /* Send indication to PA that SC is ready */
    //status = osIpcMessageSendPtr(ipc_ch[SC_TO_PA_DEDICATED], logging, 0, FALSE);
    status = osIpcMessageSendPtr(ipc_ch[SC_TO_PA_DEDICATED], logData, 0, FALSE);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    //waiting until getting a message back. hold will be released by appRxEcho
    while(hold == 0){}
 
    while(1)
    {
        loop_send(ipc_ch[SC_TO_PA_NO_IND], (void *)0x1f000000, 10);
   
		OS_WAIT(10000);       
    }
}

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
    //os_status       status;
    int i;
 
    //logging = osMalloc(sizeof(struct ipc_log) * LOG_SIZE, OS_MEM_HET_DDR1_CACHEABLE);
    //OS_ASSERT_COND(logging != NULL);
 
    buf_space = osMalloc(MEM_PART_DATA_SIZE(NUM_BUFFERS, MAX_MSG_SIZE, ALIGNED_8_BYTES),
            OS_MEM_HET_DDR1_CACHEABLE);
    OS_ASSERT_COND(buf_space != NULL);

    buf_pool  = osMemPartCreate(MAX_MSG_SIZE, NUM_BUFFERS, buf_space,
                                 ALIGNED_8_BYTES, OFFSET_0_BYTES,
                                 (os_mem_part_t *)buf_manager);
    OS_ASSERT_COND(buf_pool != NULL);

    for (i=0 ; i<NUM_IPC_CH ; i++)
    {
    	//single mode
        ipc_ch[i] = osIpcChannelIdFind(i);
        OS_ASSERT_COND(ipc_ch[i] != NULL);
    }
 
    /* Open the producer channels */
    open_procducer_ch(ipc_ch[SC_TO_PA_DEDICATED]);
    open_procducer_ch(ipc_ch[SC_TO_PA_NO_IND]);
 
    /* Open the consumer channels */
    open_consumer_ch(ipc_ch[PA_TO_SC_IND], OS_HET_VIRTUAL_INT, appRxEcho);

    return OS_SUCCESS;
}

void os_smartdsp_init()
{
    os_status status;   // The only local variable in main()
 
#if !defined(PSC9X3X_SC_ONLY_BSP) && (OS_MIMIC_PA_ARCH == ON)
    // Simulate PA
    psc913xHetBspStubInitialize((void *)SOC_HET_CTRL_BASE, NULL);
#endif // PSC9X3X_SC_ONLY_BSP && OS_MIMIC_PA_ARCH
 
    *((uint32_t*)0xb0020dbc) = 0x1;
 
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


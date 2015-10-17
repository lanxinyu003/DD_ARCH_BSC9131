/*******************************************************************************
* COPYRIGHT BH
********************************************************************************
* FileName: os.h
* Function: OS api.
* Author:   Andy
* Date:     2011/09/01
* History:
*
* Modify Date   Owner   BugID/CRID      Contents
* ------------------------------------------------------------------------------
* 2011/09/01    Andy                     Draft
*******************************************************************************/
/************************* Protection For Header File *************************/
#ifndef OS_H
#define OS_H
/************************* Include File Declaration ****************************/
#include "os_basictypes.h"
#include "os_types.h"
#include "os_err.h"
#include "os_cpu.h"

/***************************** Macro And Constant *****************************/
#define OS_TASK(x)                        void x(void)
#define OS_SYSTEM_POOL_ID                 0x0U

/******************* Type Of External Function Definition *********************/
extern struct OS_DEBUG_STRUCT os_debug_info_sys;

extern void os_report_error(OS_ERRORCODE errorcode,
                            OS_ERROROPTION erroropt,
                            void *errormsgptr);

extern OS_BOOLEAN os_wait_event(OS_EVESELECT evesel,
                                OS_SELMODE eveselmode,
                                OS_TIMEOUT timeout,
                                OS_EVENT *event);

extern void os_signal_event(OS_EVENT event,OS_TASKID tid);

extern void os_query_event(OS_EVENT *event);

extern void os_flush_event(void);

extern OS_POOLID os_create_message_pool(OS_POOLID poolid,
                                        OS_MEMADDR *baseaddr,
                                        OS_POOLSIZE poolsize,
                                        OS_MSGSIZE const *msgsizes);

extern void os_query_message_pool(OS_POOLID poolid, OS_MSGPOOLSTATUS *status);

extern void os_entrust_mem(union OS_MESSAGE *mem,OS_TASKID tid);

extern union OS_MESSAGE *os_alloc_message(OS_POOLID poolid,
                                          OS_MSGSIZE msgsize,
                                          OS_MSGID msgid);

extern union OS_MESSAGE *os_alloc_message_nil(OS_POOLID poolid,
                                              OS_MSGSIZE msgsize,
                                              OS_MSGID msgid);

extern void os_free_message(union OS_MESSAGE **msg);

extern void os_query_message(union OS_MESSAGE *msg,OS_MSGSTATUS *msgstatus);

extern OS_MSGSIZE os_message_size(union OS_MESSAGE *msg);

extern OS_MSGID os_message_signo(union OS_MESSAGE *msg);

extern OS_POOLID os_message_poolid(union OS_MESSAGE *msg);

extern void os_recommend_message(union OS_MESSAGE *msg, OS_TASKID tid);

extern union OS_MESSAGE *os_receive_message(OS_MSGSELECT const*msgselect);

extern union OS_MESSAGE *os_receive_message_tm(OS_MSGSELECT const*msgselect, OS_TIMEOUT timeout);

extern void os_send_message(union OS_MESSAGE **msg,OS_TASKID tid);

extern OS_TASKID os_get_message_senderid(union OS_MESSAGE **msg);

extern OS_SEMAPHORE *os_create_semaphore(OS_SEMVAL init_sem_val);

extern void os_delete_semaphore(OS_SEMAPHORE *sem);

extern OS_BOOLEAN os_wait_semaphore(OS_SEMAPHORE *sem,OS_TIMEOUT timeout);

extern void os_signal_semaphore(OS_SEMAPHORE *sem);

extern void os_query_semaphore(OS_SEMAPHORE *sem,OS_SEMSTATUS *semstatus);

extern u32  os_wakeup(void);

extern void os_query_timer_list(OS_TIMERSTATUS *timerstatus);

extern void os_tick(void);

extern void os_create_idle(OS_TSKFUNC entry_function,
                           void * para_data,
                           OS_STKSIZE stack_size);

extern OS_TASKID os_create_task(OS_TASKTYPE task_type,
                                OS_TSKFUNC entry_function,
                                void * para_data,
                                OS_TASKID task_id,
                                OS_STKSIZE stack_size,
                                OS_TASKPRI task_priority,
                                OS_INTVECTOR task_vector);

extern void os_suspend_task(OS_TASKID taskid);

extern void os_resume_task(OS_TASKID taskid);

extern void os_delay_task(OS_TIME ticks);

extern void os_lock_scheduler(void);

extern void os_unlock_scheduler(void);

extern u32 os_check_task_stack(OS_TASKID task_id);

extern u32 os_check_system_stack(void);

extern void os_query_task(OS_TASKID task_id,OS_TSKSTATUS *taskstatus);

extern OS_TASKID os_get_current_taskid(void);

extern void os_default_idle(void);

extern void os_create_sys_message_pool(OS_MEMADDR *baseaddr,
                                       OS_POOLSIZE poolsize,
                                       OS_MSGSIZE const *msgsizes);

extern OS_TIME os_get_ticks(void);
/********************* End of Protection For Header File **********************/
#endif /* OS_H */
/*********************************End Of File *********************************/


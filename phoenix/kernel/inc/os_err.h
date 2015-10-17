/*******************************************************************************
* COPYRIGHT BH
********************************************************************************
* FileName: os_err.h
* Function: Error code and error options.
* Author:   Andy
* Date:     2011/09/01
* History:
*
* Modify Date   Owner   BugID/CRID      Contents
* ------------------------------------------------------------------------------
* 2011/09/01    Andy                     Draft
*******************************************************************************/
#ifndef OS_ERR_H
#define OS_ERR_H

#define OS_FATAL_FLAG 0x80000000

typedef enum
{
    OS_NO_ERROR,/*0*/
    OS_ERROR,/*1*/

    /* Task management module */
    OS_TSK_PARA_FUNC_ERR,/*2*/
    OS_TSK_PARA_PRIORITY_ERR,/*3*/
    OS_TSK_PARA_TASKID_ERR,/*4*/
    OS_TSK_PARA_STACK_SIZE_ERR,/*5*/
    OS_TSK_PARA_VECTOR_ERR,/*6*/
    OS_TSK_PARA_TASK_TYPE_ERR,/*7*/
    OS_TSK_OS_NOT_RUNNING_ERR,/*8*/
    OS_TSK_CALLED_FROM_INT_ERR,/*9*/
    OS_TASK_NOT_SUSPENDED,/*10*/
    OS_TSK_MULTITASK_ALREADY_STARTED,/*11*/
    OS_TSK_PARA_SUSPEND_PRITSK_ERR,/*12*/
    OS_TSK_PARA_RESUME_PRITSK_ERR,/*13*/
    OS_TSK_ALREADY_SUSPENDED_ERR,/*14*/
    OS_TASK_STK_OVERFLOW, /*15*/

    /* Inter task communication module */

    /* Semaphore module */
    OS_SEM_ECREATE_SEM_IN_IT,/*16*/
    OS_SEM_ILLEGAL_INIT_SEM_VAL,/*17*/
    OS_SEM_EDELETE_NULLPTR,/*18*/
    OS_SEM_IN_USE,/*19*/
    OS_SEM_EWAIT_NULLPTR,/*20*/
    OS_SEM_EWAIT_IN_IT,/*21*/
    OS_SEM_ESIGNAL_NULLPTR,/*22*/
    OS_SEM_EQUERY_NULLPTR,/*23*/
    OS_SEM_EPARAMETER,/*24*/
    OS_SEM_WAIT_EXT_LIMIT,/*25*/
    OS_SEM_SIGNAL_EXT_LIMIT,/*26*/

    /* Message module */
    OS_MSG_UNANTICIPATED,/*27*/
    OS_RECEIVE_MSG_EHEAD,/*28*/
    OS_SEND_MSG_NULLPTR,/*29*/
    OS_SEND_MSG_EHEAD,/*30*/
    OS_SEND_MSG_NOT_OWNER,/*31*/
    OS_ESEND_MSG_FROM_IT_TO_IT,/*32*/
    OS_SEND_MSG_DTASK_NOT_EXIST,/*33*/
    OS_GET_MSGID_NULLPTR,/*34*/
    OS_EWAIT_MSG_IN_IT,/*35*/

    /* Event module */
    OS_EVE_EUSE_NULLPTR,/*36*/
    OS_ESEND_EVE_FROM_IT_TO_IT,/*37*/
    OS_SEND_EVE_DTASK_NOT_EXIST,/*38*/
    OS_EVE_ERR_SELECT_MODE,/*39*/


    /* Memory management module */
    OS_MEM_SYSMSGPOOL_EBASEADDR,/*40*/
    OS_MEM_SYSMSGPOOL_EMSGSIZES,/*41*/
    OS_MEM_SYSMSGPOOL_EPOOLSIZE,/*42*/
    OS_MEM_TOO_MANY_MSGPOOL,/*43*/
    OS_MEM_MSGPOOLID_TOO_BIG,/*44*/
    OS_MEM_MSGPOOLID_IN_USE,/*45*/
    OS_MEM_MSGPOOL_EBASEADDR,/*46*/
    OS_MEM_MSGPOOL_EMSGSIZES,/*47*/
    OS_MEM_MSGPOOL_EPOOLSIZE,/*48*/
    OS_MEM_MSGPOOL_NOT_EXIST,/*49*/
    OS_MEM_EMSGSIZE,/*50*/
    OS_MEM_TOO_BIG_BLOCK,/*51*/
    OS_MEM_ALLOC_EMSGHEAD,/*52*/
    OS_MEM_NO_MSGPOOL_SPACE,/*53*/
    OS_MEM_FREE_MSG_NULLPTR,/*54*/
    OS_FREE_MSG_NOT_OWNER,/*55*/
    OS_FREE_MSG_EHEAD,/*56*/
    OS_MEM_QUERY_MSG_NULLPTR,/*57*/

    OS_MEM_ECREATE_MSGPOOL_IN_IT,/*58*/
    OS_QUERY_POOL_MSG_EHEAD,/*59*/
    OS_MSGPTR_SLOP_OVER,/*60*/

    /* Interrupt management module */
    OS_WAKEUP_BY_UNKNOWN_ERR,/*61*/
    OS_UNKNOWN_INTERRUPT_TASK_ERR,/*62*/
    OS_IS_NOT_PRIORITY_TSK,/*63*/
    OS_TSK_READY_QUEUE_IS_EMPTY,/*64*/
    OS_INTTASK_STK_OVERFLOW,/*65*/
    OS_LOCK_SCHEDULER_IN_ITASK,/*66*/
    OS_UNLOCK_SCHEDULER_IN_ITASK,/*67*/

    OS_TSK_RETURN_From_RT_ERR,/*68*/
    OS_TMC_TICK_CALLED_BY_PRITASK_ERR,/*69*/
    OS_ITC_WAKEUP_CALLED_BY_PRITASK_ERR,/*70*/
    OS_TSK_DELAY_TASK_CALLED_BY_ITASK_ERR,/*71*/
    OS_TSK_TOBESUSPEND_NOT_EXIST_ERR,/*72*/
    OS_TSK_TOBERESUME_NOT_EXIST_ERR,/*73*/
    OS_TSK_TOBECHECK_NOT_EXIST_ERR,/*74*/
    OS_TSK_TOBEQUERY_NOT_EXIST_ERR,/*75*/
    OS_TSK_PARA_DATAMEM_ERR,/*76*/
    OS_LOCK_SCHEDULER_TOO_MANY_TIMES,/*77*/
    OS_TSK_PARA_USE_RSVTSKID_ERR,/*78*/

    OS_TSK_PARA_REUSE_TASKID_ERR,/*79*/
    OS_TSK_PARA_USE_RSVTSKPRI_ERR,/*80*/
    OS_TSK_SUSPEND_IDLE_ERR,/*81*/
    OS_TMC_CLEAR_TIMER_ERR,/*82*/
    OS_TMC_TICK_FIRST_TIMER_ERR,/*83*/
    OS_SEM_NOT_EXIST,/*84*/
    OS_TSK_REDELETE_TSK_ERR,/*85*/
    OS_MEM_QUERY_MSGPOOL_NULLPTR,/*86*/
    OS_MEM_QUERY_MSGSTATUS_NULLPTR,/*87*/
    OS_EBLOCK_TASK_WHEN_LOCK_SCHEDLUE,/*88*/
    OS_TMC_TICK_TASK_ST_ERR,/*89*/
    OS_QUERY_MESSAGE_SIZE_NULLPTR,/*90*/
    OS_QUERY_MESSAGE_SIZE_EHEAD,/*91*/
    OS_QUERY_MESSAGE_SIGNO_NULLPTR,/*92*/
    OS_QUERY_MESSAGE_SIGNO_EHEAD,/*93*/    
    OS_RECOMMEND_MSG_NULLPTR,/*94*/
    OS_RECOMMEND_MSG_TASKID_ERR,/*95*/
    OS_RECOMMEND_MSG_TASK_NOT_EXIST,/*96*/
    OS_RECOMMEND_MSG_EHEAD,/*97*/
    OS_RECOMMEND_MSG_NOT_OWNER,/*98*/
    OS_FREE_MSG_ETYPE/*99*/
}OS_KERNAL_ERRORCODE;

typedef enum
{
    OS_CREATE_TASK,/*0*/

    /* Task management module */
    OS_TSK_CREATE_TASK,/*1*/
    OS_TSK_RESUME_TASK,/*2*/
    OS_TSK_START_MULTITASK,/*3*/
    OS_TSK_SUSPEND_TASK,/*4*/
    OS_TSK_RESCHEDULE,/*5*/

    /* Inter task communication module */

    /* Semaphore module */
    SEM_CREATE_SEMAPHORE,/*6*/
    SEM_DELETE_SEMAPHORE,/*7*/
    SEM_WAIT_SEMAPHORE,/*8*/
    SEM_SIGNAL_SEMAPHORE,/*9*/
    SEM_QUERY_SEMAPHORE,/*10*/

    /* Message module */
    MSG_RECEIVE_MESSAGE,/*11*/
    MSG_RECEIVE_MESSAGE_TM,/*12*/
    MSG_SEND_MESSAGE,/*13*/
    MSG_GET_MESSAGE_SENDERID,/*14*/

    /* Event module */
    EVE_WAIT_EVENT,/*15*/
    EVE_SIGNAL_EVENT,/*16*/
    EVE_QUERY_EVENT,/*17*/

    /* Memory management module */
    MEM_CREATE_SYS_MESSAGE_POOL,/*18*/
    MEM_CREATE_MESSAGE_POOL,/*19*/
    MEM_QUERY_MESSAGE_POOL,/*20*/
    MEM_ALLOC_MESSAGE,/*21*/
    MEM_ALLOC_MESSAGE_NIL,/*22*/
    MEM_FREE_MESSAGE,/*23*/
    MEM_FREE_MESSAGE_FORCE,/*24*/
    MEM_QUERY_MESSAGE,/*25*/

    /* Interrupt management module */
    OS_ITC_WAKEUP,/*26*/
    OS_ITC_UNKNOWN_INTERRUPT_TASK,/*27*/
    OS_TSK_QUERY_TASK,/*28*/
    OS_TSK_READY_QUEUE_HIGHEST,/*29*/
    OS_TSK_CHECK_TASK_STACK,/*30*/
    OS_TSK_RET_FROM_INTERRUPT,/*31*/
    OS_TSK_LOCK_SCHEDULER,/*32*/
    OS_TSK_UNLOCK_SCHEDULER,/*33*/

    OS_TSK_RETURN_From_RT,/*34*/
    OS_TMC_TICK,/*35*/
    OS_TSK_DELAY_TASK,/*36*/
    OS_TMC_QUERY_TIMER_LIST,/*37*/
    OS_TSK_STACK_OVERFLOW,/*38*/
    OS_TSK_ENTER_INTERRUPT,/*39*/
    OS_TMC_CLEAR_TIMER,/*40*/
    OS_TSK_PTQ_DELETE,/*41*/
    OS_MESSAGE_SIZE,/*42*/
    OS_RECOMMEND_MESSAGE,/*43*/
    OS_ENTRUST_MESSAGE/*44*/
}OS_KERNAL_FUNCCODE;

/********************* End of Protection For Header File **********************/
#endif
/*********************************End Of File *********************************/
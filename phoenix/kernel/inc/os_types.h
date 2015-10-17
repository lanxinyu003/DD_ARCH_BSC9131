/*******************************************************************************
* COPYRIGHT BH
********************************************************************************
* FileName: os_type.h
* Function: 
* Author:   Andy
* Date:     2011/09/01
* History:
*
* Modify Date   Owner   BugID/CRID      Contents
* ------------------------------------------------------------------------------
* 2011/09/01    Andy                     Draft
*******************************************************************************/
#ifndef OS_TYPES_H
#define OS_TYPES_H

#define OS_MEM_BLK_MAX_NUM   8

typedef  enum OS_BOOLEAN{OS_TRUE=1, OS_FALSE=0}   OS_BOOLEAN;
/*-----------------------------------------------------------*/
union OS_MESSAGE;
typedef u32                                 OS_ERRORCODE;
typedef u32                                 OS_ERROROPTION;

typedef s32                                 OS_STATUS;

typedef enum OS_ITC_INTVECTOR
{/* This type will be redefined in kernel. But for using easily, maybe we can locate them here also. */
    NULLVECTOR = -1,
}OS_INTVECTOR;

typedef u16                                 OS_POOLID;
typedef u32                                 OS_POOLSIZE;
typedef u32                                 OS_MEMADDR;
typedef u16                                 OS_MSGSIZE;
typedef u16                                 OS_MSGID;
typedef u16                                 OS_MSGSELECT;
typedef u32                                 OS_EVESELECT;
typedef s32                                 OS_SEMVAL;
typedef u32                                 OS_SENDOPT;

typedef struct
{
    OS_POOLSIZE mem_pool_size;
    OS_MSGSIZE mem_block_size[OS_MEM_BLK_MAX_NUM];
    u16 mem_free_block_count[OS_MEM_BLK_MAX_NUM];
    u32 mem_fragment_count[OS_MEM_BLK_MAX_NUM];
    u32 mem_block_alloc_count[OS_MEM_BLK_MAX_NUM];
    u32 mem_block_free_count[OS_MEM_BLK_MAX_NUM];
    u32 mem_free_mem_count;
}OS_MSGPOOLSTATUS;

typedef struct tag_OSP_POOL_DB                                                                                                
{                                                                                                                             
	OS_POOLID pool_id; 
	OS_MEMADDR pool_addr; 
	OS_POOLSIZE pool_size; 
	OS_MSGSIZE sizes_sel[OS_MEM_BLK_MAX_NUM]; 
}OS_POOL_DB; 

typedef struct
{
    OS_SEMVAL   sem_count;
    void        *head;
    void        *tail;
}OS_SEMSTATUS;

typedef struct
{
    OS_SEMVAL   sem_count;
    void        *head;
    void        *tail;
    u32 flag;
}OS_SEMAPHORE;

typedef u32                                OS_EVENT;
typedef u16                                OS_STKSIZE;

typedef enum
{
    EVE_OR,
    EVE_AND
}OS_SELMODE;

typedef s32                                 OS_TIMEOUT;
typedef u32                                 OS_TIME;

typedef struct
{
    u32 timers_number;
}OS_TIMERSTATUS;

typedef enum tasktype
{
    OS_TSK_PRITASK,
    OS_TSK_INTTASK
}OS_TASKTYPE;

typedef void (* OS_TSKFUNC)(void);

typedef u8                                  OS_TASKPRI;
typedef u16                                 OS_TASKID;

typedef struct
{
    OS_TASKTYPE                    task_type;
    OS_TASKID                      task_id;
    OS_TASKPRI                     task_priority;/* Task priority (0 == highest, 31 == lowest) */
    u8                             rsv1;
    u16                            pri_task_status;
    u16                            int_task_status;
    u32                            int_task_IER;
    OS_INTVECTOR                   int_task_vector;
    /* Event related */
    OS_EVESELECT                   event_select_bits;
    OS_SELMODE                     event_wait_mode;
    OS_EVESELECT                   event_received;
    /* Message related */
    OS_MSGSELECT                   *msg_select;
    void                           *msg_accepted;
    /* Semaphore related */
    OS_SEMVAL                      sem_count;
    /* Timer related */
    OS_BOOLEAN                     task_wait_timer;
    OS_TIME                        tick_remain;
}OS_TSKSTATUS;

typedef struct
{
    OS_TASKID   mem_owner;
    OS_TASKID   mem_sender;
    OS_MSGSIZE  mem_block_size;
    OS_MSGSIZE  mem_data_size;
    OS_POOLID   mem_poolid;
    u8          mem_bufsizeid;
    u8          mem_reserved;
    u8          mem_pool_type;
    u16         mem_mark;
} OS_MSGSTATUS;

typedef OS_BOOLEAN (*OS_FUNCTION_USR_TYPE)( OS_ERRORCODE        error_code,
                                            OS_ERROROPTION      error_opt,
                                            void                *error_msgptr,
                                            OS_TASKID           taskid,
                                            OS_BOOLEAN          user_called);

struct OS_CFG_CONFIG
{
    OS_FUNCTION_USR_TYPE    usrerrorfunction;
    u32                     nr_taskes;
    OS_POOLID               max_pool_number;
    u16                     rsv16;

    void (*swap_hook)(OS_TASKID tid);
    void (*send_hook)(union OS_MESSAGE const *msg);
    void (*receive_hook)(union OS_MESSAGE const *msg);
    void (*alloc_hook)(OS_POOLID poolid,  union OS_MESSAGE const *msg);
    void (*free_hook)(OS_POOLID poolid,  union OS_MESSAGE const *msg);
    void (*create_task_hook)(OS_TASKID tid);
    void (*create_pool_hook)(OS_POOLID poolid);
};

struct OS_ERRORMSG
{
    OS_ERRORCODE    error_code;
    OS_ERROROPTION  error_opt;
    void            *error_msgptr;
    OS_TASKID       tid;
    OS_BOOLEAN      user_called;
    u32             err_func_reentrant_count;
    u32             err_func_reentrant_flagword;
};

struct OS_DEBUG_STRUCT
{
    OS_TASKID                   tsk_previous_task_id;
    OS_TASKID                   tsk_running_task_id;

    struct OS_ERRORMSG          *err_msg;

    u32                         err_tsk_priority_task_created;/* Number of priority task created */
    u32                         err_tsk_soft_interrupt_task_created;/* Number of soft interrupt task created */
    u32                         err_tsk_hw_interrupt_task_created;/* Number of hard interrupt task  created */
    u32                         err_tsk_pritsk_switch_counter;/* Priority task switch counter */
    struct CFG_CONFIG const     *config;
    u32                         version;

    void *                      *tcb_list;
    void *                      *pool_list;
};

/********************* End of Protection For Header File **********************/
#endif
/*********************************End Of File *********************************/

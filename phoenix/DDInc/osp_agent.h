/*******************************************************************************
********************************************************************************
* 文件名称: dsp_dd_interface.h
* 功能描述: 定义DD与外部的接口函数及消息等
*
* 编写日期: 2013/12/7
* 修改历史:
* [格式要求：
*
* 修改版本  修改日期   修改人  修改内容
* ------------------------------------------------------------------------------
* 01       
*******************************************************************************/
/******************************** 头文件保护开头 ******************************/
#ifndef OSP_AGENT_H
#define OSP_AGENT_H
/******************************** 包含文件声明 ********************************/
#include <dsp_dd_interface.h>

/******************************** 类型定义 ************************************/
typedef enum
{// Define the state of the link
	IP_LINK_STATE_LINK_RESET,// Powered on
	IP_LINK_STATE_LINK_INITIALIZED, // LLD is ready
	IP_LINK_STATE_LINK_TCP_CONNECTED,	
	IP_LINK_STATE_LINK_TCP_DISCONNECTED,
	IP_LINK_STATE_LINK_UDP_CONNECTED,
	IP_LINK_STATE_LINK_UDP_DISCONNECTED
} IP_LINK_STATES_t;


// Definition of the debug command.
typedef struct OSP_DEBUG_CMD_READ_MULTI_PKT_ADD_tag
{
	u32 response_id;
	u16 block_count;
	u16 block_index;
} OSP_DEBUG_CMD_READ_MULTI_PKT_ADD;

typedef struct OSP_DEBUG_CMD_MEM_OPERATION_tag
{
	u32 buffer_address;
	u32 memory_type; // virtual or physical
	u32 buffer_size; // in bytes
	
	OSP_DEBUG_CMD_READ_MULTI_PKT_ADD read_multi_info;
} OSP_DEBUG_CMD_MEM_OPERATION;

typedef struct //__attribute__((__packed__)) OSP_DEBUG_CMD_tag
{
	OSP_STRU_MSGHEAD         		osp_header;
	OSP_DEBUG_CMD_MEM_OPERATION		memory;
} OSP_DEBUG_CMD;

typedef struct //__attribute__((__packed__)) OSP_DEBUG_CMD_tag
{
	OSP_STRU_MSGHEAD         		osp_header;
	u32		trace_msg_mapping_address;
} OSP_LOGIN_ACK_CMD;

#define CXX_LEN_OF_ALARM_INF_LEN            100 
#define CXX_LEN_OF_ALARM_OPT_LEN			8

typedef struct Msg_xxom_alarm_s
{
    OSP_STRU_MSGHEAD         msg_header;   
	u16						 half_sfn;
	u16						 tsn;
	s32	     				 opt[CXX_LEN_OF_ALARM_OPT_LEN];	
	s16        				 alarm_type; //subsystem of this reporting
	s16         			 alarm_value;
	s16        				 alarm_level;
	s16        				 error_id;	
	u8   				     alarm_info_string[CXX_LEN_OF_ALARM_INF_LEN];
}Msg_xxom_alarm;

typedef struct STRU_IPNET_DEBUG_INFO_tag
{
	u32 tcp_status;
	u32 tcp_cb_cnt;	
	u32 tcp_send_cnt;
	u32 udp_status;	
	u32 udp_cb_cnt;
	u32 udp_send_cnt;
	u32 expected_alarm_cnt;
	u32 failed_alarm_cnt;
	u32 successed_alarm_cnt;	
	u32 trace_send_ok_cnt;
	u32 trace_send_fail_cnt;	
	u32 trace_rece_ok_cnt;
	u32 trace_rece_fail_cnt;		
}STRU_IPNET_DEBUG_INFO;
/******************************** 宏和常量定义 ********************************/
#define O_DD_AGENT_MSGID                               100    /*OSP AGENT任务接收的消息ID*/

#define O_DD_AGENT_LOGIN_MSGID                         101    /*板卡登录的消息ID*/ //0x65
#define O_DD_AGENT_LOGIN_ACK_MSGID                     102 	  /*OSP AGENT板卡登录确认的消息ID*/

#define O_DD_AGENT_READ_MEM_MSG_ID                     103    /*读内存的消息ID*/
#define O_DD_AGENT_READ_MEM_ACK_MSG_ID                 104    /*OSP AGENT回应读内存的消息ID*/

#define O_DD_AGENT_WRITE_MEM_MSG_ID                	   105    /*写内存的消息ID*/
#define O_DD_AGENT_WRITE_MEM_ACK_MSG_ID                106    /*OSP AGENT回应写内存的消息ID*/

#define O_DD_ALARM_MSGID                               107    /*驱动告警上报的消息ID*/

#define O_DD_TRACE_LOG_MSGID                           108    /*驱动LOG上报的消息ID*/

#define O_DD_TRACE_DATA_MSGID                          109    /*驱动LOG上报的消息ID*/

#define O_DD_AGENT_LOGOUT_MSGID                        110    /*板卡退出登录的消息ID*/ //0x6E
#define O_DD_AGENT_LOGOUT_ACK_MSGID                    111 	  /*OSP AGENT板卡退出登录确认的消息ID*/

// Define the maximum IP packet size in byte.
#define OSP_MAX_IPNET_PACKET_SIZE_IN_BYTE              61400 

#define OSP_TRACEID_AS_THE_FLAG                    0xEB

#define OSP_DEBUG_CMD_MEMORY_VIRTUAL_TYPE          0x01 
#define OSP_DEBUG_CMD_MEMORY_PHYSICAL_TYPE         0x02

#define OSP_DEBUG_TRACE_MSG_SEND_MASK			0x01
#define OSP_DEBUG_TRACE_MSG_RECV_MASK			0x02
#define OSP_DEBUG_TRACE_MSG_ALLOC_MASK			0x04
/*----------------------------------- DSP侧告警消息相关定义 --------------------------------------*/
/*------------------------------------ 全NodeB公共告警定义 ---------------------------------------*/
/*告警类型宏定义*/
#define NB_ALARM_TYPE_COM               1                  /*通信告警*/          
#define NB_ALARM_TYPE_QOS               2                  /*服务质量告警*/                
#define NB_ALARM_TYPE_PROC              3                  /*处理机告警*/                
#define NB_ALARM_TYPE_EQUIP             4                  /*设备告警*/                
#define NB_ALARM_TYPE_ENVIR             5                  /*环境告警*/
#define NB_ALARM_TYPE_DEBUG             6                  /*调试告警*/

/*告警级别宏定义*/                                    
#define NB_ALARM_SEVERITY_CRITICAL      1                  /*紧急告警*/          
#define NB_ALARM_SEVERITY_MAJOR         2                  /*主要告警*/                
#define NB_ALARM_SEVERITY_MINOR         3                  /*次要告警*/                
#define NB_ALARM_SEVERITY_WARN          4                  /*提示告警*/ 
#define NB_ALARM_SEVERITY_INDETERMINATE 5                  /*未确定告警*/ 
#define NB_ALARM_SEVERITY_CLEAR         6                  /*已清除告警*/ 


/******************************** 外部函数原形声明 ****************************/
// 告警上报函数
OSP_STATUS fdd_report_alarm_udp(s16        alarm_type,
                          		s16        alarm_value,
                          		s16        alarm_level,
                      	  		s16        error_id,
                      	  		s32	       *opt,					  
                          		const u8   *alarm_info_string);
// IPNET initialization
OSP_STATUS fdd_dspeth_init();

// Send msg to server. The function will not free msg to the caller.
OSP_STATUS fdd_ipnet_tcp_send_msg(OSP_STRU_MSGHEAD **ppstruMsg);
OSP_STATUS fdd_ipnet_udp_send_msg(OSP_STRU_MSGHEAD **ppstruMsg);

OSP_STATUS fdd_trace_over_udp(OSP_STRU_MSGHEAD **ppstruMsg);

// Send msg log to the server.
OSP_STATUS fdd_log_over_udp(OSP_STRU_MSGHEAD **ppstruMsg);
OSP_STATUS fdd_log_over_tcp(OSP_STRU_MSGHEAD **ppstruMsg);

// Send data to the server. Those functions will alloc and free the msg for the caller.
OSP_STATUS fdd_data_over_tcp(u8 *pdata, u16 size_in_byte);
OSP_STATUS fdd_data_over_udp(u8 *pdata, u16 size_in_byte);

extern u8 g_u8_msg_trace_mapping[NB_SUPPORT_MAX_MSGID_NUM];
extern STRU_IPNET_DEBUG_INFO g_stru_ipnet_info;
/******************************** 头文件保护结尾 ******************************/
#endif /* DSP_DD_INTERFACE_H */
/******************************** 头文件结束 **********************************/

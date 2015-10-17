/*******************************************************************************
********************************************************************************
* �ļ�����: dsp_dd_interface.h
* ��������: ����DD���ⲿ�Ľӿں�������Ϣ��
*
* ��д����: 2013/12/7
* �޸���ʷ:
* [��ʽҪ��
*
* �޸İ汾  �޸�����   �޸���  �޸�����
* ------------------------------------------------------------------------------
* 01
*******************************************************************************/

/******************************** ͷ�ļ�������ͷ ******************************/
#ifndef DSP_DD_INTERFACE_H
#define DSP_DD_INTERFACE_H
/******************************** �����ļ����� ********************************/
#include <os.h>

/******************************** ���Ͷ��� ************************************/
typedef s32                                 OSP_STATUS;                         /* ��������ֵ����                     */
typedef u16                                 OSP_MSGSELECT;                      /* ��Ϣ���Ͷ���                       */
typedef u16                                 OSP_MSGSIZE;                        /* ��Ϣ���ȶ���                       */
typedef u32                                 OSP_MUTEXID;                        /* ������ID����                       */
typedef u32                                 OSP_CSEMID;                         /* �����ź���ID����                   */
typedef s32                                 OSP_CSEMVAL;                        /* �����ź�����ֵ����                 */
typedef u32                                 OSP_TASKID;                         /* ����ID����                         */
typedef s32                                 OSP_TASKSTAT;                       /* ����״̬����                       */
typedef u16                                 OSP_POOLID;                         /* �ڴ��ID����                       */
typedef u32                                 OSP_BLKSIZE;                        /* �ڴ���С����                     */
typedef u16                                 OSP_BLKID;                          /* �ڴ��ID����                       */
typedef s32                                 OSP_FTPCONNID;                      /* FTP����ID                          */
typedef s32                                 OSP_FILE;                           /* �ļ�����                           */
typedef u32                                 OSP_FILE_POS;                       /* �ļ�λ�ö���                       */
typedef u32                                 OSP_TIME;                           /* ����ʱ�䶨��                       */
typedef u16                                 OSP_DDID;                           /* ����ID����                         */
typedef u16                                 OSP_LINKID;                         /* ��·ID����                         */
typedef u32                                 OSP_TMRID;                          /* ��ʱ��ID����                       */
typedef s32                                 OSP_TMRVAL;                         /* ��ʱ��ʱ�䶨��                     */
typedef u32                                 OSP_TMRTYPE;                        /* ��ʱ�����Ͷ���                     */
typedef u8                                  OSP_SFUTYPE;                        /* SFU���Ͷ���                        */

/******************************** ��ͳ������� ********************************/
/*--------------����ֵ---------------*/
#define OSP_OK                              0                                   /* �ɹ�                               */
#define OSP_ERROR                           (-1)                                /* ʧ��                               */

/*--------------��ָ��---------------*/
#define OSP_NULLP                           0

/*-------------��������--------------*/
#define OSP_TRUE                            1                                   /* ��                                 */
#define OSP_FALSE                           0                                   /* ��                                 */

/*-------------�ȴ���ʽ--------------*/
#define OSP_NO_WAIT                         0                                   /* ���ȴ�                             */
#define OSP_WAIT_FOREVER                    (-1)                                /* ���õȴ�                           */

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/

/*******************************************************************************/
/* ����DSP�ϵĹ�����Դ��Χ */
/*******************************************************************************/
/* ��������(Task)�����뷶Χ */
#define NB_SUPPORT_MAX_TASK_NUM                 300
#define NB_DD_TASKID_BASE                         0
#define NB_DD_TASKID_MAX                          60                            /* ����ʹ��0~60������           */


/* ������Ϣ(Msg)�����뷶Χ */
#define NB_SUPPORT_MAX_MSGID_NUM                65535
#define NB_DD_MSGID_BASE                          0
#define NB_DD_MSGID_MAX                         400                             /* ����ʹ�õ���ϢID����: ����0~400    */


/* �����ڴ��(Pool)�����뷶Χ */
#define NB_POOL_TOTAL_NUM                              10                       /* �ڴ������                         */
#define NB_POOL_SHARE_NUM                              1                        /* �����ڴ�ظ���                     */
#define NB_POOLID_SYS                                  0                        /* ϵͳ�ڴ��                         */
#define NB_POOLID_DD                                   1                        /* DD�ڴ��                           */
#define NB_POOLID_PL                                   2                        /* PL�ڴ��                           */
#define NB_POOLID_IPNET                                3                        /* IPNET��                            */
#define NB_POOLID_SHARE                                4                        /* �����ڴ��                         */


/* ����澯ֵ(Alarm Value)��Χ */
#define NB_SUPPORT_ALARM_VALUD_MAX                65535
#define NB_DD_ALARM_VALUD_BASE                    100
#define NB_DD_ALARM_VALUD_MAX                     1000


/* ������󷵻�ֵ(Error Return ID)��Χ */
#define NB_SUPPORT_ERROR_RET_VALUE_MAX            (-1)
#define NB_DD_ERROR_RET_VALUE_BASE                (-10000)
#define NB_DD_ERROR_RET_VALUE_MAX                 (-20000)


/* ��������崻����û��Զ���Code(Fatal Error Code)��Χ */
#define NB_SUPPORT_FATAL_ERROR_CODE_VALUE_MAX     (0x8FFFFFFF)
#define NB_OS_FATAL_ERROR_CODE_BASE               (0x80000000)
#define NB_OS_FATAL_ERROR_CODE_MAX                (0x80000100)

#define NB_DD_FATAL_ERROR_CODE_BASE               (0x80000101)
#define NB_DD_FATAL_ERROR_CODE_MAX                (0x80000200)

/*******************************************************************************/
/*******************************************************************************/
/* ��������崻�Code(Fatal Error Code)���� */
#define CDD_DSPICC_MEM_EMSGSIZE                  NB_DD_FATAL_ERROR_CODE_BASE
#define CDD_DSPICC_MEM_NO_MSGPOOL_SPACE         (CDD_DSPICC_MEM_EMSGSIZE + 1)
#define CDD_DSPICC_FREE_MSG_EHEAD               (CDD_DSPICC_MEM_EMSGSIZE + 2)
#define CDD_DSPICC_FREE_MSG_NOT_OWNER           (CDD_DSPICC_MEM_EMSGSIZE + 3)

#define CDD_DSPBSP_MMU_EXCEPTION                (NB_DD_FATAL_ERROR_CODE_BASE + 10)


/*******************************************************************************/
/*******************************************************************************/
/* �����澯ֵ���� */
/* BSP */
#define A_DSPBSP_TIMING_ERR							  (100)                     /*BSP timing error*/
#define E_DSPBSP_TIMING_80MS							0                       /* Timing validation in 80ms failed */							
#define E_DSPBSP_TIMING_FRAME_NOT_ALIGNED				1                       /* Frame interrupt is not aligned */	
                                                                                
#define A_DSPBSP_SYS_ERR							  (101)                     /*BSP system error alarm*/
#define E_DSPBSP_SYS_ALLOC_MSG_FAIL						0                       /* Alloc msg failed */

/* ICC */
#define A_DSPICC_RX_ERR                               (110)                     /*DSPICC���մ���*/
#define E_DSPICC_RX_VALIDATION_FAIL						0                       
#define E_DSPICC_RX_UNEXPECTED_NULL_MSG					1                       
                                                                                
#define A_DSPICC_SYS_ERR                              (120)                     /* DSPICC system related error */
#define E_DSPICC_SYS_MEM_EMSGSIZE						0                       /* Error msg size when alloc in share pool */
#define E_DSPICC_SYS_MEM_NO_MSGPOOL_SPACE				1
#define E_DSPICC_SYS_FREE_MSG_EHEAD						2
#define E_DSPICC_SYS_FREE_MSG_NOT_OWNER					3

/* ETH */
#define A_DSPETH_HW_ERROR                             (130)                     /*DSPETHӲ������*/
#define A_DSPETH_SW_ERROR                             (131)                     /*DSPETH�������*/
#define A_DSPETH_RX_ERROR                             (132)                     /*DSP��̫�������ݷ�������*/
#define A_DSPETH_TX_ERROR                             (133)                     /*DSP��̫�������ݷ�������*/
                                                                                
/* IPNET */                                                                     
#define A_DSPIPNET_ERR                                (140)                     /*DSPDECP(MAPLEB)Ӳ������*/
                                                                                
/* DECP */                                                                      
#define A_DSPDECP_HW_ERR                              (150)                     /*DSPDECP(MAPLEB)Ӳ������*/
                                                                                
#define A_DSPDECP_SW_ERR                              (151)                     /*DSPDECP(MAPLEB)�������*/
#define E_DSPDECP_SW_CRC_ALLOC_FAIL                     0                       
#define E_DSPDECP_SW_DEPE_ALLOC_FAIL                    1                           
#define E_DSPDECP_SW_FTPE_ALLOC_FAIL                    2                               
#define E_DSPDECP_SW_FTPE_SEND_ICC_FAIL                 3                       
#define E_DSPDECP_SW_TVPE_ALLOC_FAIL                    4                       

/* DMA */
#define A_DSPBSP_DMA_ERR                              (160)                     /*DSPDMA�������*/
#define E_DSPBSP_DMA_ALLOC_MSG_FAIL						0                       
                                                                                
/* SRIO */                                                                      
#define A_DSPSRIO_SW_ERR                              (170)                     /*DSPSRIO�������*/
#define A_DSPSRIO_HW_ERR                              (171)                     /*DSPSRIOӲ������*/
#define A_DSPSRIO_TX_ERR                              (172)                     /*DSPSRIO���ݷ��ʹ���*/
#define A_DSPSRIO_RX_ERR                              (174)                     /*DSPSRIO���ݽ��մ���*/

/* CPRI */
#define A_DSPIQ_CPRI_SW_ERR                           (180)
#define A_DSPIQ_CPRI_CLOCK_ERROR                  	  (181)
#define A_DSPIQ_CPRI_UNSYNC                           (182)
#define A_DSPIQ_CPRI_BUFFER_OVWERUN_UNDERRUN          (183)

#define E_DSPCPRI_A_ALLOC_MSG_FAIL                    1
#define E_DSPCPRI_A_SEND_MSG_FAIL                     2
#define E_DSPCPRI_A_CDR_ERROR_ALARM                   3
#define E_DSPCPRI_A_EXTERNAL_SYNC_LOST_ALARM          4
#define E_DSPCPRI_A_LLOS_ERROR_ALARM                  5
#define E_DSPCPRI_A_LLOF_ERROR_ALARM                  6
#define E_DSPCPRI_A_RLOS_ERROR_ALARM                  7
#define E_DSPCPRI_A_RLOF_ERROR_ALARM                  8
#define E_DSPCPRI_A_RSDI_ERROR_ALARM                  9
#define E_DSPCPRI_A_RRAI_ERROR_ALARM                  10
#define E_DSPCPRI_A_RIQOV_ERROR_ALARM                 11
#define E_DSPCPRI_A_TIQUN_ERROR_ALARM                 12
#define E_DSPCPRI_A_RETHOV_ERROR_ALARM                13
#define E_DSPCPRI_A_TETHUN_ERROR_ALARM                14
#define E_DSPCPRI_A_REBDUN_ERROR_ALARM                15
#define E_DSPCPRI_A_RHDLCOV_ERROR_ALARM               16
#define E_DSPCPRI_A_THDLCUN_ERROR_ALARM               17
#define E_DSPCPRI_A_RHBDUN_ERROR_ALARM                18
#define E_DSPCPRI_A_RVSSOV_ERROR_ALARM                19
#define E_DSPCPRI_A_TVSSUN_ERROR_ALARM                20
#define E_DSPCPRI_A_COMBINATION_ERROR_ALARM           21
#define E_DSPCPRI_A_OPEN_IQ_TX_FAIL                   22
#define E_DSPCPRI_A_OPEN_IQ_RX_FAIL                   23
#define E_DSPCPRI_FREQUENCY_ALARM                     24
#define E_DSPCPRI_HW_RE_INIT_FAIL_ALARM               25


/*******************************************************************************/
/*******************************************************************************/
/* ���� POOL ID ����  */
#define CDD_DSPBSP_POOLID                           NB_POOLID_DD
#define CDD_DSPICC_POOLID                           NB_POOLID_DD
#define CDD_DSPSRIO_POOLID                          NB_POOLID_DD
#define CDD_DSPCPRI_POOLID                          NB_POOLID_DD
#define CDD_DSPDECP_POOLID                          NB_POOLID_DD
#define CDD_DSPETH_POOLID                           NB_POOLID_IPNET
#define CDD_DSPIP_POOLID                            NB_POOLID_IPNET


/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/* DD ��Ŷ��� */
#define OSP_DSPBSP_ID                            1                              /* DSPBSP ID                          */
#define OSP_DSPICC_ID                            2                              /* DSPICC ID                          */
#define OSP_DSPSRIO_ID                           3                              /* DSPSRIO ID                         */
#define OSP_DSPIQ_ID                             4                              /* DSPTVCP ID                         */
#define OSP_DSPDECP_ID                           5                              /* DSPDECP ID                         */
#define OSP_DSPDMA_ID                            6                              /* DSPDMA ID                          */
#define OSP_DSPETH_ID                            7                              /* DSPETH ID                          */
#define OSP_DSPIP_ID                             8                              /* DSPIP ID                           */


/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/* DD�����ֵ���� */
#define E_DSPBSP_VAL                             (-10100)                       /* DSPBSP�����ֵ                     */
#define E_DSPICC_VAL                             (-10200)                       /* DSPICC�����ֵ                     */
#define E_DSPSRIO_VAL                            (-10300)                       /* DSPSRIO�����ֵ                    */
#define E_DSPIQ_VAL                              (-10400)                       /* DSPIQ�����ֵ                      */
#define E_DSPDECP_VAL                            (-10500)                       /* DSPDECP�����ֵ                    */
#define E_DSPDMA_VAL                             (-10600)                       /* DSPDMA�����ֵ                     */
#define E_DSPETH_VAL                             (-10700)                       /* DSPETH�����ֵ                     */
#define E_DSPIP_VAL                              (-10800)                       /* DSPIP�����ֵ                      */



/* DSPBSP ���󷵻�ֵ���� */
#define EDD_DSPBSP_ALLOC_FAIL                   (E_DSPBSP_VAL - 0)
#define EDD_DSPBSP_SUBSFN_SET_INVALID_VAL       (E_DSPBSP_VAL - 1)
#define EDD_DSPBSP_SUBSFN_SET_INVALID_COREID    (E_DSPBSP_VAL - 2)
#define EDD_DSPBSP_TSCFG_ENTRY_NUM_ERROR        (E_DSPBSP_VAL - 3)
#define EDD_DSPBSP_TSCFG_NULLPTR_ERROR          (E_DSPBSP_VAL - 4)
#define EDD_DSPBSP_TSCFG_NO_TIMER               (E_DSPBSP_VAL - 5)
#define EDD_DSPBSP_TSCFG_NO_CFG                 (E_DSPBSP_VAL - 6)
#define EDD_DSPBSP_IOCTL_LED                    (E_DSPBSP_VAL - 7)
#define OSP_DSPBSP_INVALID_IOCTL_CMD            (E_DSPBSP_VAL - 8)
#define EDD_DSPBSP_INVALID_HSID                 (E_DSPBSP_VAL - 9)
#define EDD_DSPBSP_SEND_FAIL                    (E_DSPBSP_VAL - 10)
#define EDD_DSPBSP_CPU_OVLD_TIME_QUERY_FAIL     (E_DSPBSP_VAL - 11)
#define EDD_DSPBSP_MMU_VIRT2PHYS_ERROR          (E_DSPBSP_VAL - 12)
#define EDD_DSPBSP_MMU_PHYS2VIRT_ERROR          (E_DSPBSP_VAL - 13)
#define EDD_DSPBSP_NO_SYNCLK_SOURCE             (E_DSPBSP_VAL - 14)
#define EDD_DSPBSP_SUBSFN_SET_NOT_ON_CORE0      (E_DSPBSP_VAL - 15)
#define EDD_DSPBSP_IOCTL_CACHE_FLUSH_CMD_ERR    (E_DSPBSP_VAL - 16)
#define EDD_DSPBSP_IOCTL_CACHE_INVALID_CMD_ERR  (E_DSPBSP_VAL - 17)
#define EDD_DSPBSP_IOCTL_CACHE_PFETCH_CMD_ERR   (E_DSPBSP_VAL - 18)
#define EDD_DSPBSP_TSCFG_DSTSFU_ERR             (E_DSPBSP_VAL - 19)



/* DSP DMA ���󷵻�ֵ���� */
#define EDD_DSPDMA_IOCTL_CMD_ERR                (E_DSPDMA_VAL - 0)
#define EDD_DSPDMA_IOCTL_ARG_NULL               (E_DSPDMA_VAL - 1)
#define EDD_DSPDMA_PARAM_ERR                    (E_DSPDMA_VAL - 2)
#define EDD_DSPDMA_QUEUE_FULL                   (E_DSPDMA_VAL - 3)



/* DSP ICC ���󷵻�ֵ���� */
#define EDD_DSPICC_DST_INVAL                    (E_DSPICC_VAL - 0)
#define EDD_DSPICC_QUEUE_FULL                   (E_DSPICC_VAL - 1)
#define EDD_DSPICC_ADDR_INVAL                   (E_DSPICC_VAL - 2)
#define EDD_DSPICC_SIZE_INVAL                   (E_DSPICC_VAL - 3)
#define EDD_DSPICC_POOL_ERROR                   (E_DSPICC_VAL - 4)
#define EDD_DSPICC_NO_SUIT_SIZE_CB              (E_DSPICC_VAL - 5)
#define EDD_DSPICC_WRONG_POOL_SIZE              (E_DSPICC_VAL - 6)
#define EDD_DSPICC_POOL_WAS_EXIST               (E_DSPICC_VAL - 7)


/* DSP MAPLE ���󷵻�ֵ���� */
#define EDD_DSPDECP_ALLOC_FAIL                  (E_DSPDECP_VAL - 0)             /*����Ϣʧ��                          */
#define EDD_DSPDECP_INIT_TURBO_TYPE_ERR         (E_DSPDECP_VAL - 1)             /*��turbo����ģʽ����                 */
#define EDD_DSPDECP_IOCTL_CMD_ERR               (E_DSPDECP_VAL - 2)             /*�����������                        */
#define EDD_DSPDECP_IOCTL_NOT_SUPPORT_THIS_CORE (E_DSPDECP_VAL - 3)             /*�ù��ܲ�֧���ڴ˺��ϵ���            */
#define EDD_DSPDECP_IOCTL_BD_CNT_ZERO           (E_DSPDECP_VAL - 4)             /*һ�����õ�bd����Ϊ0                 */
#define EDD_DSPDECP_IOCTL_CFG_LEN_TOO_LARGE     (E_DSPDECP_VAL - 5)             /*һ�����õĲ���̫��                  */
#define EDD_DSPDECP_IOCTL_CFG_BDADDR_ERR        (E_DSPDECP_VAL - 6)             /*���õ�BD��ַ����                    */
#define EDD_DSPDECP_DBG_MODULE_ERR              (E_DSPDECP_VAL - 7)
#define EDD_DSPDECP_IOCTL_CFG_PROCID_ERR        (E_DSPDECP_VAL - 8)             /* ���ò����д�����ID����             */
#define EDD_DSPDECP_BD_LOAP_ERR                 (E_DSPDECP_VAL - 9)             /*BD����������                        */
#define EDD_DSPDECP_NOTGET_MMU_DATA_DESC_REG    (E_DSPDECP_VAL - 10)            /*δ�ܻ��mmu�Ĵ���                   */
#define EDD_DSPDECP_TB_BUF_OVERFLOW             (E_DSPDECP_VAL - 11)            /*���� buffer��������                 */


/* DSP ETH ���󷵻�ֵ���� */
#define EDD_DSPETH_INIT_FAIL                    (E_DSPETH_VAL - 0)


/* DSP IP ���󷵻�ֵ���� */
#define EDD_DSPIP_INIT_FAIL                     (E_DSPIP_VAL - 0)
#define EDD_DSPIP_NOT_READY                     (E_DSPIP_VAL - 1)
#define EDD_DSPIP_ERROR_PRAMS                   (E_DSPIP_VAL - 2)
#define EDD_DSPIP_GET_TCP_FRAME_FAIL            (E_DSPIP_VAL - 3)
#define EDD_DSPIP_GET_UDP_FRAME_FAIL            (E_DSPIP_VAL - 4)
#define EDD_DSPIP_SEND_FAIL           			(E_DSPIP_VAL - 5)


/* DSP IQ ���󷵻�ֵ���� */
#define EDD_DSPCPRI_IOCTL_FID                                 (E_DSPIQ_VAL - 0) /* ioctl errflag                  */
#define EDD_DSPCPRI_IOCTL_WRONG_PARM_FID                      (E_DSPIQ_VAL - 1) /* ioctl errflag                  */
#define EDD_DSPCPRI_LINK_RATE_AUTO_NEGOTIATE_FID              (E_DSPIQ_VAL - 2) /* CPRI: linkrate of autonegotiate errflag */
#define EDD_DSPCPRI_CHECK_HSSI_CPRI_RESET_DONE_FID            (E_DSPIQ_VAL - 3) /* HSSI: reset status errflag     */
#define EDD_DSPCPRI_IQ_INIT_FID                               (E_DSPIQ_VAL - 4) /* CPRI: IQ init errflag          */
#define EDD_DSPCPRI_ETH_INIT_FID                              (E_DSPIQ_VAL - 5) /* CPRI: eth init errflag         */
#define EDD_DSPCPRI_HDLC_INIT_FID                             (E_DSPIQ_VAL - 6) /* CPRI: hdlc init errflag        */
#define EDD_DSPCPRI_HDLC_INIT_WRONG_TX_BD_SIZE_FID            (E_DSPIQ_VAL - 7) /* CPRI: hdlc init errflag        */
#define EDD_DSPCPRI_HDLC_INIT_WRONG_RX_BD_SIZE_FID            (E_DSPIQ_VAL - 8) /* CPRI: hdlc init errflag        */
#define EDD_DSPCPRI_VSS_INIT_FID                              (E_DSPIQ_VAL - 9) /* CPRI: vss init errflag         */
#define EDD_DSPCPRI_VSS_BUFF_SIZE_WRONG_FID                   (E_DSPIQ_VAL - 10) /* CPRI: vss init errflag         */
#define EDD_DSPCPRI_VSS_BUFF_THRESHOLD_WRONG_FID              (E_DSPIQ_VAL - 11) /* CPRI: vss init errflag         */
#define EDD_DSPCPRI_CONTROL_IQ_DATA_FID                       (E_DSPIQ_VAL - 12) /* CPRI: IQ data control errflag  */
#define EDD_DSPCPRI_CONTROL_IQ_WRONG_CMD_FID                  (E_DSPIQ_VAL - 13) /* CPRI: IQ data control errflag  */
#define EDD_DSPCPRI_CONTROL_ETH_DATA_FID                      (E_DSPIQ_VAL - 14) /* CPRI: eth data control errflag */
#define EDD_DSPCPRI_CONTROL_HDLC_DATA_FID                     (E_DSPIQ_VAL - 15) /* CPRI: hdlc data control errflag*/
#define EDD_DSPCPRI_CONTROL_VSS_DATA_FID                      (E_DSPIQ_VAL - 16) /* CPRI: vss data control errflag */
#define EDD_DSPCPRI_OPEN_IQ_RX_FID                            (E_DSPIQ_VAL - 17) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_IQ_RX_MAP_FID                        (E_DSPIQ_VAL - 18) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_IQ_RX_BUFFER_FID                     (E_DSPIQ_VAL - 19) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_IQ_RX_THRESHOLD_FID                  (E_DSPIQ_VAL - 20) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_IQ_RX_AXC_FID                        (E_DSPIQ_VAL - 21) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_IQ_TX_FID                            (E_DSPIQ_VAL - 22) /* open IQ transmit errflag       */
#define EDD_DSPCPRI_OPEN_IQ_TX_MAP_FID                        (E_DSPIQ_VAL - 23) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_IQ_TX_BUFFER_FID                     (E_DSPIQ_VAL - 24) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_IQ_TX_THRESHOLD_FID                  (E_DSPIQ_VAL - 25) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_IQ_TX_AXC_FID                        (E_DSPIQ_VAL - 26) /* open IQ reveive errflag        */
#define EDD_DSPCPRI_OPEN_ETH_FID                              (E_DSPIQ_VAL - 27) /* open eth errflag               */
#define EDD_DSPCPRI_CONFIG_IQ_RX_FID                          (E_DSPIQ_VAL - 28) /* IQ receive config errflag      */
#define EDD_DSPCPRI_CONFIG_IQ_RX_NULL_PARM_FID                (E_DSPIQ_VAL - 29) /* IQ receive config errflag      */
#define EDD_DSPCPRI_CONFIG_IQ_RX_WRONG_CH_BW_FID              (E_DSPIQ_VAL - 30) /* IQ receive config errflag      */
#define EDD_DSPCPRI_CONFIG_IQ_RX_WRONG_PHY_ADDR_FID           (E_DSPIQ_VAL - 31) /* IQ receive config errflag      */
#define EDD_DSPCPRI_CONFIG_IQ_TX_FID                          (E_DSPIQ_VAL - 32) /* IQ transmit config errflag     */
#define EDD_DSPCPRI_CONFIG_IQ_TX_NULL_PARM_FID                (E_DSPIQ_VAL - 33) /* IQ receive config errflag      */
#define EDD_DSPCPRI_CONFIG_IQ_TX_WRONG_CH_BW_FID              (E_DSPIQ_VAL - 34) /* IQ receive config errflag      */
#define EDD_DSPCPRI_CONFIG_IQ_TX_WRONG_PHY_ADDR_FID           (E_DSPIQ_VAL - 35) /* IQ receive config errflag      */
#define EDD_DSPCPRI_CONFIG_CW_FID                             (E_DSPIQ_VAL - 36) /* CW config errflag              */
#define EDD_DSPCPRI_CONFIG_CW_OPEN_ETH_FID                    (E_DSPIQ_VAL - 37) /* CW config errflag              */
#define EDD_DSPCPRI_EER_ISR_FID                               (E_DSPIQ_VAL - 38) /* eer isr errflag                */
#define EDD_DSPCPRI_QT_ISR_FID                                (E_DSPIQ_VAL - 39) /* qt isr errflag                 */
#define EDD_DSPCPRI_BFN_ISR_FID                               (E_DSPIQ_VAL - 40) /* bfn cpri re-config errflag     */
#define EDD_DSPCPRI_DBG_CONFIG_FID                            (E_DSPIQ_VAL - 41) /* dbg config err*/                                       
#define EDD_DSPCPRI_HW_INIT_FID                               (E_DSPIQ_VAL - 60)  /* CPRI: hardware init errflag(1~19) */

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/* IOCTL command macro definitions */
#define OSP_B_CMD(DdId, DdCmd)                   ((DdId << 16) | DdCmd)

/* ����һ���������е�DDID�� */
#define OSP_B_CMD_DDID(u32Cmd)                   (OSP_DDID)((u32Cmd & 0xFFFF0000) >> 16)

/* DSPBSP Ioctl������ */
#define OSP_CSM_GET_SFN                          OSP_B_CMD(OSP_DSPBSP_ID,0x0001)/* ��ȡSfn��                          */
#define OSP_CSM_GET_HALFSFN                      OSP_B_CMD(OSP_DSPBSP_ID,0x0002)/* ��ȡHalfSfn��                      */
#define OSP_CSM_SET_HALFSFN_EX                   OSP_B_CMD(OSP_DSPBSP_ID,0x0003)/* ����/У׼HalfSfn�ţ�ָ��ʱ�����Ч */
#define OSP_CSM_GET_SYNCLK_STATUS                OSP_B_CMD(OSP_DSPBSP_ID,0x0004)/* ��ȡͬ��ʱ��Դ״̬                 */
#define OSP_CSM_GET_125US_CNT                    OSP_B_CMD(OSP_DSPBSP_ID,0x0005)/* ��ȡ125us counter����ֵ            */
#define OSP_CSM_GET_SLOTNUM                      OSP_B_CMD(OSP_DSPBSP_ID,0x0006)/* ��ȡʱ϶��                         */
#define OSP_CSM_CFG_TS                           OSP_B_CMD(OSP_DSPBSP_ID,0x0007)/* ����ʱ϶֪ͨ��Ϣ                   */
#define OSP_CSM_GET_SLOT_CPU_CYCLE_OFFSET        OSP_B_CMD(OSP_DSPBSP_ID,0x0008)/* ��ȡ��ǰʱ϶CYCLE��ƫ��            */
#define OSP_CSM_GET_SLOT_US_OFFSET               OSP_B_CMD(OSP_DSPBSP_ID,0x0009)/* ��ȡ��ǰʱ϶΢����ƫ��             */
#define OSP_CSM_GET_HALFSFN_US_OFFSET            OSP_B_CMD(OSP_DSPBSP_ID,0x000a)/* ��ȡ��ǰHalfSfn΢����ƫ��          */
#define OSP_CSM_GET_SUBFN                        OSP_B_CMD(OSP_DSPBSP_ID,0x000b)/* ��ȡSubFn��                        */
#define OSP_CSM_GET_SUBFN_US_OFFSET              OSP_B_CMD(OSP_DSPBSP_ID,0x000c)/* ��ȡ��ǰSubFn΢����ƫ��            */
#define OSP_BAM_GET_LOGIC_POSITION               OSP_B_CMD(OSP_DSPBSP_ID,0x000e)/* ��ȡ�߼�λ����Ϣ                   */
#define OSP_BAM_GET_PHYSICAL_POSITION            OSP_B_CMD(OSP_DSPBSP_ID,0x000f)/* ��ȡ����λ����Ϣ                   */
#define OSP_BAM_SET_LOCAL_LED_STATUS             OSP_B_CMD(OSP_DSPBSP_ID,0x0010)/* ����LED��״̬                      */
#define OSP_BAM_GET_BOARD_TYPE                   OSP_B_CMD(OSP_DSPBSP_ID,0x0011)/* ��ȡ�忨����                       */
#define OSP_BAM_GET_COREID                       OSP_B_CMD(OSP_DSPBSP_ID,0x0012)/* ��ȡ��ǰ��ID                       */
#define OSP_BAM_GET_CPU_CYCLE_NUM                OSP_B_CMD(OSP_DSPBSP_ID,0x0013)/* ��ȡ��ǰCYCLE��                    */
#define OSP_BAM_CACHE_INVALID                    OSP_B_CMD(OSP_DSPBSP_ID,0x0014)/* CACHE��Ч                          */
#define OSP_BAM_CACHE_FLUSH                      OSP_B_CMD(OSP_DSPBSP_ID,0x0015)/* CACHEˢ��                          */
#define OSP_BAM_CACHE_PFETCH                     OSP_B_CMD(OSP_DSPBSP_ID,0x0016)/* CACHEԤȡָ                        */
#define OSP_DEM_GET_CPU_OCCUPANCY                OSP_B_CMD(OSP_DSPBSP_ID,0x0017)/* ��ȡCPUռ����                      */
#define OSP_DEM_SET_CPU_COUNT_TIME               OSP_B_CMD(OSP_DSPBSP_ID,0x0018)/* ����CPUռ����ͳ��ʱ��              */
#define OSP_DEM_GET_CPU_COUNT_TIME               OSP_B_CMD(OSP_DSPBSP_ID,0x0019)/* ��ȡCPUռ����ͳ��ʱ��              */


/*******************************************************************************/
/*******************************************************************************/
/* DSP CPRI Ioctl������ */
#define OSP_TSM_DSPIQ_TX_CFG                     OSP_B_CMD(OSP_DSPIQ_ID,0x0001)
#define OSP_TSM_DSPIQ_RX_CFG                     OSP_B_CMD(OSP_DSPIQ_ID,0x0002)


/*******************************************************************************/
/*******************************************************************************/
/* Interface for MapleB2 */
#define OSP_BAM_DECP_START_CFG_EX                OSP_B_CMD(OSP_DSPDECP_ID,0x0001)


/*******************************************************************************/
/*******************************************************************************/
/* DSPDMA Ioctl������ */
#define OSP_TSM_MD_DMA_TRANSFER                  OSP_B_CMD(OSP_DSPDMA_ID,0x0001)/* ͨ��DMA��������(��ά)              */
#define OSP_TSM_DMA_TRANSFER                     OSP_B_CMD(OSP_DSPDMA_ID,0x0002)/* ͨ��DMA��������(һά)              */

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/* �������Ź滮 */
#define NB_BBU_DEFAULT_SUBBRACK_ID           0
#define NB_BBU_DEFAULT_SLOT_ID               0

#define NB_BBU_PROCID_DSP0_CORE0             0
#define NB_BBU_PROCID_DSP0_CORE1             1
#define NB_BBU_PROCID_DSP0_CORE2             2
#define NB_BBU_PROCID_DSP0_CORE3             3
#define NB_BBU_PROCID_DSP0_CORE4             4
#define NB_BBU_PROCID_DSP0_CORE5             5

#define NB_BBU_PROCID_REMOTE_IPNET_SERVER_BASE     100

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/* Operation System Platform header definition */
typedef u8                                  OSP_MSGOPT;                         /* ��Ϣѡ���������                   */
typedef u8                                  OSP_MSGPRIO;                        /* ��Ϣ���ȼ�����                     */
typedef u8                                  OSP_TRACEID;                        /* ��Ϣ���ٿ��ض���                   */
typedef u8                                  OSP_RESERV;                         /* �����ֶζ���                       */

/*-----------SFUID���ݽṹ-----------*/
typedef struct tag_OSP_SFUID
{
    u8 SubrackId;                                                               /* �����                             */
    u8 SlotId;                                                                  /* ��λ��                             */
    u8 ProcId;                                                                  /* ���������ˣ���                     */
    u8 TaskId;                                                                  /* Task Id                            */
}OSP_SFUID;

/*---------OSP��Ϣͷ���ݽṹ---------*/
typedef struct tag_OSP_STRU_MSGHEAD
{
    OS_MSGID      Msgid;                                                        /* ��ϢID                             */
    OS_MSGSIZE    MsgSize;                                                      /* ��Ϣ�峤�ȣ����ֽ�Ϊ��λ           */
    OSP_SFUID     SrcDesc;                                                      /* Դ Desc                            */
    OSP_SFUID     DstDesc;                                                      /* Ŀ�� Desc                          */
    OSP_MSGOPT    MsgOpt;                                                       /* ��Ϣѡ�����                       */
    OSP_MSGPRIO   MsgPrio;                                                      /* ��Ϣ���ȼ�(0-4)����ԽС���ȼ�Խ��  */
    OSP_TRACEID   TraceId;                                                      /* ��Ϣ���ٿ���                       */
    OSP_RESERV    Reserv;                                                       /* �����ֶ�                           */
}OSP_STRU_MSGHEAD;

#define OSP_MSGOPT_SAFE_MODE               1									/* ��ĳЩ·�������ʹ�ÿɿ�����,��TCP */
#define OSP_MSGOPT_UNSAFE_MODE             0

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/* CACHE���� */
#define OSP_CACHE_TYPE_L1D                       0x00000001                     /* L1������CACHE                      */
#define OSP_CACHE_TYPE_L1P                       0x00000002                     /* L1��ָ��CACHE                      */
#define OSP_CACHE_TYPE_L2P                       0x00000004                     /* L2������CACHE                      */
#define OSP_CACHE_TYPE_L2D                       0x00000008                     /* L2��ָ��CACHE                      */

/* CACHE���������ṹ���� */
typedef struct tag_OSP_STRU_CACHE_OP_ARG
{
    u32 u32Addr;                                                                /* Cache��������ʼ��ַ,��MMU�Ĵ�������*/
                                                                                /* д���ַ,Ҫ����Cache line����    */
    u32 u32Len;                                                                 /* Cache���������ݳ���,���ֽ�Ϊ��λ,  */
                                                                                /* ΪCache line��������               */
    u32 u32CacheType;                                                           /* ������Cache����                    */
}OSP_STRU_CACHE_OP_ARG;


/* BSP ���ֵĶ������ݽṹ */
#define OSP_STRU_SUBSFN                          OSP_STRU_HALFSFN
/* HALFSFN�ṹ���� */
typedef struct tag_OSP_STRU_HALFSFN
{
    u32 u32TimeOut;                                                             /* HalfSfn����Чʱ��������λms      */
    u32 u32HalfSfn;                                                             /* ��Ч��HalfSfn��                    */
}OSP_STRU_HALFSFN;


/* ʱ϶֪ͨ������Ϣ�ṹ���� */
typedef struct tag_OSP_STRU_TS_CFG_ENTRY
{
    u32              u32Slot;                                                   /* ������Ϣʱ����slot��             */
    u32              u32DelayTime;                                              /* ������Ϣʱ��������slot���ӳ�ʱ�� */
    OSP_STRU_MSGHEAD struMsgHead;                                               /* OSP��Ϣͷ                          */
}OSP_STRU_TS_CFG_ENTRY;


/* ʱ϶֪ͨ���ýṹ���� */
typedef struct tag_OSP_STRU_TS_CFG
{
    u32                   u32CfgEntryCnt;                                       /* ʱ϶֪ͨ������Ϣ��Ч��Ŀ           */
    OSP_STRU_TS_CFG_ENTRY *pstruCfgEntry;                                       /* ָ��ʱ϶֪ͨ������Ϣ�����ָ��     */
}OSP_STRU_TS_CFG;


/* ʱ϶֪ͨ��Ϣ���ݽṹ���� */
typedef struct tag_DDAPP_TS_NOTIFY_MSG
{
    OSP_STRU_MSGHEAD struMsgHead;                                               /* OSP��Ϣͷ                          */
    u32              u32HalfSfn;                                                /* HalfSfn                            */
    u32              u32SubFn;                                                  /* SubFn(TD��Ч)                      */
    u32              u32SlotNum;                                                /* SlotNum                            */
    u32              u32DelayTime;                                              /* ������Ϣʱ��������slot���ӳ�ʱ�� */
}DDAPP_TS_NOTIFY_MSG;


/* LED�Ʋ�����ʽ */
#define OSP_LED_OFF                              0                              /* ��                                 */
#define OSP_LED_ON                               1                              /* ��                                 */
#define OSP_LED_OPP                              2                              /* ȡ��                               */
#define OSP_LED_SLOW                             100                            /* ����                               */
#define OSP_LED_FAST                             25                             /* ����                               */

/* LED�ƽṹ���� */
typedef struct tag_OSP_STRU_LED
{
    u32 u32LedId;                                                               /* LED��ID                            */
    u32 u32Operation;                                                           /* ������ʽ                           */
    s32 s32GlintTime;                                                           /* ������ʽΪ��ʱ��������˸ʱ��       */
}OSP_STRU_LED;

/* CPUռ���ʽṹ���� */
typedef struct tag_OSP_STRU_CPU_OCCUPANCY
{
    u8 u8CpuOccupancy;                                                          /* ��ǰCPUռ����                      */
    u8 u8MaxCpuOccupancy;                                                       /* ���CPUռ����                      */
    u8 u8MinCpuOccupancy;                                                       /* ��СCPUռ����                      */
    u8 u8AveCpuOccupancy;                                                       /* ƽ��CPUռ����                      */
}OSP_STRU_CPU_OCCUPANCY;

/* λ����Ϣ�ṹ���� */
typedef struct tag_OSP_STRU_POSITION_INFO
{
    u8 u8SubRackId;                                                             /* �����                             */
    u8 u8SlotId;                                                                /* ��λ��                             */
    u8 u8ProcId;                                                                /* ��������                           */
    u8 u8ProductType;                                                           /* ��Ʒ����                           */
}OSP_STRU_POSITION_INFO;



/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/* DD DMA interface for app. */
/* DMA����֧�ֵ����ά�� */
#define OSP_DMA_MAX_DIMENSION                    4

/* DMA���Ʋ����ṹ����(һά) */
typedef struct tag_OSP_STRU_TSM_DMA_TRANSFER_ARG
{
    OSP_STRU_MSGHEAD struMsgHead;                                               /* OSP��Ϣͷ                          */
    u32              u32ReportMsgFlag;                                          /* ���ݰ�����ɺ���֪ͨ��Ϣ�ı�־   */
    s32              *ps32OkFlag;                                               /* ָ�����ݰ�����ɱ�־��ָ��         */
    u32              u32SrcPhyAddr;                                             /* �������ݵ�Դ�����ַ               */
    u32              u32DstPhyAddr;                                             /* �������ݵ�Ŀ�������ַ             */
    u32              u32Len;                                                    /* �������ݵĳ���                     */
}OSP_STRU_TSM_DMA_TRANSFER_ARG;

/* ��άDMA�����ṹ���� */
typedef struct tag_OSP_STRU_TSM_MD_DMA_PARAM
{
    u32 u32SrcCnt;                                                              /* ��Ҫ�����Դ���ݿ����             */
    u32 u32DstCnt;                                                              /* ��Ҫ�����Ŀ�����ݿ����           */
    s32 s32SrcOffset;                                                           /* Դ�ӵ�N����β����N+1��ͷ֮��ļ�� */
    s32 s32DstOffset;                                                           /* Ŀ�Ĵӵ�N����β����N+1��ͷ��ļ�� */
}OSP_STRU_TSM_MD_DMA_PARAM;

/* DMA���Ʋ����ṹ����(��ά) */
typedef struct tag_OSP_STRU_TSM_MD_DMA_TRANSFER_ARG
{
    OSP_STRU_MSGHEAD          struMsgHead;                                      /* OSP��Ϣͷ                          */
    u32                       u32ReportMsgFlag;                                 /* ���ݰ�����ɺ���֪ͨ��Ϣ�ı�־   */
    s32                       *ps32OkFlag;                                      /* ָ�����ݰ�����ɱ�־��ָ��         */
    u32                       u32SrcPhyAddr;                                    /* �������ݵ�Դ�����ַ               */
    u32                       u32DstPhyAddr;                                    /* �������ݵ�Ŀ�������ַ             */
    u32                       u32SrcDimension;                                  /* Դ�˴���ʹ�õ�ά��                 */
    u32                       u32DstDimension;                                  /* Ŀ�Ķ˴���ʹ�õ�ά��               */
    u32                       u32SrcLen;                                        /* ��һάԴ�����ݿ�ĳ���             */
    u32                       u32DstLen;                                        /* ��һάĿ�Ķ����ݿ�ĳ���           */
    OSP_STRU_TSM_MD_DMA_PARAM struMdDmaParam[OSP_DMA_MAX_DIMENSION-1];          /* ��άDMA�������飬�ӵڶ�ά��ʼ      */
}OSP_STRU_TSM_MD_DMA_TRANSFER_ARG;

typedef struct tag_MSG_STRU_DMA_TX_STATUS
{
    OSP_STRU_MSGHEAD            struMsgHead;
    u32                         u32Status;
    u32                         u32Info;
}STRU_MSG_DMA_TX_STATUS;



/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
// Command type of DECP
#define OSP_DECP_CMDTYPE_TVCP          1
#define OSP_DECP_CMDTYPE_DEPE          2
#define OSP_DECP_CMDTYPE_EQPE          3
#define OSP_DECP_CMDTYPE_CRC           4
#define OSP_DECP_CMDTYPE_FTPE(n)      (5 + n)


#define OSP_DECP_BD_NUM_EX   32

#define OSP_DECP_BD_PRI_HIGHEST         0
#define OSP_DECP_BD_PRI_LOWEST          1

#define OSP_DECP_APP_CHK_BD_STATUS // Check the BD status
#define OSP_DECP_APP_CHK_FLAG_WAIT_MSG // Check flags or waiting msg

typedef struct tag_OSP_STRU_DECP_REG_CFG
{
    u32 u32RegAddr;
    u32 u32RegValue;
}OSP_STRU_DECP_REG_CFG;

typedef struct tag_OSP_STRU_DECP_START_CFG_EX_ARG
{
    u32 u32CmdType;
    u32 u32DirectDecode; // Confirmation type of decoding finish of DECP. Only DFT supports direct decoding
    OSP_STRU_MSGHEAD   struMsgHead;// Osp header
    u32 u32ReportMsgFlag; // Input arg. The flag in completed msg of DECP. OSP_TRUE, send a msg ;OSP_FLASE, no msg.
    s32 *ps32OkFlag; // Input arg. Point to the flag of DECP completion.
                     // OSP_TRUE: OK; negative: failure; Ongoing: OSP_FALSE; OSP_NULLP, don't use it.
    u32 u32DecpRegCnt; // Configuration register of DECP
    OSP_STRU_DECP_REG_CFG *pstruDecpRegCfg; // DECP configuration register pointer
    u32 u32BdCnt; // BD number in a block
    u32 (*pu32BdWord)[OSP_DECP_BD_NUM_EX] ;
    u32 u32BdPri; // BD ring priority
    u32 u32BdAddr; // BD ring address: 0-driver get the address;
    u32 *pu32InfoAddr;
    u32 u32InfoLen;
}OSP_STRU_DECP_START_CFG_EX_ARG;

// Notice msg when decp complete
typedef struct tag_DDAPP_DECP_NOTIFY_MSG
{
    OSP_STRU_MSGHEAD   struMsgHead;// Osp header
    u32 u32CmdType;
    u32 u32Result;
    u32 u32BdId;
}DDAPP_DECP_NOTIFY_MSG;



/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/* Interface for CPRI */

#define OSP_DSPIQ_LTE_BW_1_4M             1u
#define OSP_DSPIQ_LTE_BW_3M               2u
#define OSP_DSPIQ_LTE_BW_5M               3u
#define OSP_DSPIQ_LTE_BW_10M              4u
#define OSP_DSPIQ_LTE_BW_15M              5u
#define OSP_DSPIQ_LTE_BW_20M              6u
#define OSP_DSPIQ_LTE_BW_INVALID        0xFF

typedef struct tag_OSP_STRU_DSPIQ_TX_CFG_ENTRY
{
    u32 u32TSNum;
    u32 u32LocalPhyAddr;
    u32 u32BufferLen;
}OSP_STRU_DSPIQ_TX_CFG_ENTRY;

typedef struct tag_OSP_STRU_DSPIQ_TX_CFG
{
    u32 u32LinkId; // link number start from 0. 0 - serdes lane J, 1 - serdes lane I and so forth.
    u32 u32CarrierId;
    u32 u32TxOffset; // Framer map offset
    u32 u32ChannelBW;
    u32 u32CfgEntryCnt;
    OSP_STRU_DSPIQ_TX_CFG_ENTRY *pstruCfgEntry;
}OSP_STRU_DSPIQ_TX_CFG;

typedef struct tag_OSP_STRU_DSPIQ_RX_CFG_ENTRY
{
    OSP_STRU_MSGHEAD   struMsgHead;
    u32 u32TSNum;
    u32 u32LocalPhyAddr;
    u32 u32BufferLen;
}OSP_STRU_DSPIQ_RX_CFG_ENTRY;

typedef struct tag_OSP_STRU_DSPIQ_RX_CFG
{
    u32 u32LinkId; // link number start from 0
    u32 u32CarrierId;
    u32 u32RxOffset;
    u32 u32ChannelBW;
    u32 u32TsConfig;
    u32 u32CfgEntryCnt;
    OSP_STRU_DSPIQ_RX_CFG_ENTRY *pstruCfgEntry;
}OSP_STRU_DSPIQ_RX_CFG;

typedef struct tag_DDAPP_DSPIQ_NOTIFY_MSG
{
    OSP_STRU_MSGHEAD   struMsgHead;// Osp header
    u32 u32HalfSfn;
    u32 u32TSNum;
    u32 u32CarrierId;
    u32 u32MsgNum;
}DDAPP_DSPIQ_NOTIFY_MSG;

/******************************** �ⲿ����ԭ������ ****************************/
extern OSP_STATUS Osp_Ioctl(u32 u32Cmd, u32 u32Arg);

extern OSP_STRU_MSGHEAD *Osp_Alloc_Msg(OS_POOLID PoolId,  OS_MSGSIZE DataSize, OS_MSGSELECT MsgType);

extern OSP_STATUS Osp_Free_Msg(OSP_STRU_MSGHEAD **pstruMsg);

extern OSP_STRU_MSGHEAD *Osp_Receive_Msg(OS_MSGSELECT *pMsgSel);

extern OSP_STATUS Osp_Send_Msg(OSP_STRU_MSGHEAD **ppstruMsg,OS_SENDOPT *pstruOpt);

// ����ֱ�ӵ������º���
//BSP
extern OSP_STATUS fdd_dspbsp_hw_init(void);
extern OSP_STATUS fdd_dspbsp_init_hook_end(void);
extern OS_STATUS  fdd_dspbsp_init();
extern OS_STATUS fdd_dspbsp_osp_ioctl (u32 cmd, u32 arg);

//DMA
extern OSP_STATUS fdd_dspdma_ioctl(u32 u32_cmd, u32 u32_arg);
extern OSP_STATUS fdd_dspdma_init(void);

//CPRI
extern OSP_STATUS fdd_dspcpri_init(void);
extern OSP_STATUS fdd_dspcpri_ioctl(u32 u32_cmd, u32 u32_arg);

//DECP
extern OSP_STATUS fdd_dspdecp_init(void);
extern OSP_STATUS  fdd_dspdecp_ioctl(u32 u32cmd, u32 u32arg);

//CSM
extern void fdd_dspbsp_csm_init(void);

//DSPIP
extern OSP_STATUS fdd_dspeth_init(void);

//ICC
extern OSP_STATUS fdd_dspicc_init(OS_POOL_DB *pstru_shared_pool);
extern OS_STATUS fdd_dspicc_send(union OS_MESSAGE **msg,u32 dst_cid,OS_TASKID dst_tid);
extern void DR_Free_Share_Message(union OS_MESSAGE **msg);
extern union OS_MESSAGE *DR_Alloc_Share_Message_Nil(OS_POOLID poolid, OS_MSGSIZE msgsize, OS_MSGID msgid);
/******************************** ͷ�ļ�������β ******************************/
#endif /* DSP_DD_INTERFACE_H */
/******************************** ͷ�ļ����� **********************************/

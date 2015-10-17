/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:53 $
 $Id: tdm_init.h,v 1.5 2012/11/22 16:28:53 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/tdm/include/tdm_init.h,v $
 $Revision: 1.5 $
******************************************************************************/

/******************************************************************************
 
 @File          tdm_init.h

 @Description   TDM Initialization header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __TDM_INIT_H
#define __TDM_INIT_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/***********************************************************//**
 @Group         tdm_id TDM Module API

 @{
*//************************************************************/

/***********************************************************//**
 @Group         tdm_init_id TDM Initialization

 @{
*//************************************************************/


/**************************************************************************//**
 @Description   Initialization structure of TDM.
*//***************************************************************************/
typedef struct
{
    uint8_t     cts;                /* Sharing of signals with other TDM */
    uint8_t     rtsal;              /* Receive and transmit sharing and active links */
    uint8_t     rfsd;               /* Receive frame sync delay */
    uint8_t     rsl;                /* Receive sync level */
    uint8_t     rde;                /* Receive data edge */
    uint8_t     rfse;               /* Receive frame sync edge */
    uint8_t     rrdo;               /* Receive reversed data order */
    uint8_t     tso;                /* Transmit sync output */
    uint16_t    tao;                /* Transmit always output */
    uint8_t     sol;                /* Sync out length */
    uint8_t     soe;                /* Sync out edge */
    uint8_t     tfsd;               /* Transmit frame sync delay */
    uint8_t     tsl;                /* Transmit sync level */
    uint8_t     tde;                /* Transmit data edge */
    uint8_t     trdo;               /* Receive reversed data order */
    uint8_t     tfse;               /* Transmit frame sync edge */
    uint32_t    rncf;               /* Receive number of channels - not even, also is function of rtsal */
    uint8_t     rcdbl;              /* Receive channel data bit latency - check here for minimum num of frames */
    uint8_t     rcs;                /* Receive chennel size - 1 */
    bool        rt1;                /* 1 if the receive frame is T1 - check num of channels */
    bool        rubm;               /* receive unified buffer mode */
    uint32_t    tncf;               /* Transmit number of channels - not even, also is a function of ttsal */
    uint8_t     tcdbl;              /* Transmit channel data bit latency - check here for minimum num of frames */
    uint8_t     tcs;                /* Transmit channel size - 1 */
    bool        tt1;                /* 1 if the receive frame is T1 */
    bool        tubm;               /* Transmit unified buffer mode */
    uint32_t    rdbs;               /* Receive data buffers size */
    uint32_t    tdbs;               /* Transmit data buffers size */
    uint32_t    rgba;               /* Receive global base address - THIS FIELD IS NOT USED ANYMORE (DONTCARE)!!!*/
    uint32_t    tgba;               /* Transmit global base address - THIS FIELD IS NOT USED ANYMORE (DONTCARE)!!!*/
    bool        ame;                /* Adaptation machine anable */
    bool        lts;                /* Learn transmit sync  */
    bool        ren;                /* Receive enable */
    bool        ten;                /* Transmit enable */
    uint8_t     irq_priority;       /* Interrupt input Line of PIC for threshold interrupts */
    uint8_t     irq_err_priority;   /* Interrupt input Line of PIC for error interrupts */
 
} tdm_init_params_t;


/**************************************************************************//**
 @Function      tdmInitialize

 @Description   Initializes the TDM driver for CIO.

 @Param         tdm_num     - The TDM ID.
 @Param         init_params - TDM initialization parameters.

 @Retval        OS_ERR_NO_MEMORY    :   No sufficient memory to allocate internal structures.
 @Retval        OS_ERR_PARAM_INVALID:   Buffer size is not alligned to 8 bytes.
 @Retval        OS_FAIL             :   Error registering core to TDM device.
 @Retval        OS_SUCCESS          :   TDM initialization succeeded.

 @Cautions      Not multicore safe.
*//***************************************************************************/
os_status   tdmInitialize(uint8_t tdm_num, tdm_init_params_t *init_params);


/** @} */ /* end of tdm_init_id */
/** @} */ /* end of tdm_id */


_OS_END_EXTERN_C

#endif // __TDM_INIT_H

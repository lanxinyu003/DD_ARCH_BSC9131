/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_hwi.c,v 1.4 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_hwi.c,v $
 $Revision: 1.4 $
******************************************************************************/

/******************************************************************************
 
 @File          starcore_hwi.c

 @Description   StarCore-specific Hardware Interrupts functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtimecritical.h"

#include "starcore_hwi_.h"


#ifndef readSR
#define readSR      _readSR
int readSR(void);
#endif

#ifndef writeSR
#define writeSR     _writeSR
void writeSR(int);
#endif



/* osActivate() will call osHwiSwiftDisable() to pull this down to 0 -
   prevents the user from calling osHwiSwiftDisable() during the application
   bring up and accidentally enabling interrupts */
volatile int                        g_int_disable_counter = 1;
uint32_t *          g_base_int_vect_add;
volatile uint16_t   g_os_interrupt_count;



/*****************************************************************************/
//void osHwiNullHandler(os_hwi_arg arg)
void osHwiNullHandler(void)
{
    //VAR_UNUSED(arg);
    return;
}


/*****************************************************************************/
os_hwi_status osHwiDisable()
{
    os_hwi_status int_status = 0;


    osHwiSwiftDisable();
    int_status = (os_hwi_status)readSR();
    writeSR((int)(int_status | SR_PRIORITY_MASK));
    osHwiSwiftEnable();
 
#ifdef HWI_PRIORITY_LOGGING
    osLog(LOG_HWI_DISABLE, 0);
#endif // HWI_LOGGING

    /* SCpriority = SR_PRIORITIES_NUMBER - OSpriority
       => OSpriority = SR_PRIORITIES_NUMBER - SCpriority */
    int_status = (int_status & SR_PRIORITY_MASK) >> 21;
    int_status = SR_PRIORITIES_NUMBER - int_status;

    return int_status;
}


/*****************************************************************************/
void osHwiEnable(os_hwi_status prev_status)
{
    os_hwi_status cur_status;

#ifdef HWI_PRIORITY_LOGGING
    osLog(LOG_HWI_ENABLE, prev_status);
#endif // HWI_LOGGING

    /* SCpriority = SR_PRIORITIES_NUMBER - OSpriority
       => OSpriority = SR_PRIORITIES_NUMBER - SCpriority */
    prev_status = SR_PRIORITIES_NUMBER - prev_status;
    prev_status <<= 21;

    osHwiSwiftDisable();
    cur_status = (os_hwi_status)readSR();
    writeSR((int)((cur_status & (~SR_PRIORITY_MASK)) | prev_status));
    osHwiSwiftEnable();
 
}



/*****************************************************************************/
asm void osHwiSetVBA(uint32_t vector_address)
{
asm_header
.arg
    _vector_address in $d0;

asm_body
    move.l d0,vba
asm_end
}

#if 0

void osHwiMultiplexedHandler(os_hwi_arg arg)
{
    multiplexed_hwi_t *this_interrupt = (multiplexed_hwi_t *)arg;
    os_status status;
 
    OS_ASSERT_COND(this_interrupt != NULL);
 
    /* Edge mode */
    if (this_interrupt->mode == 1)
    {
        status = osHwiPendingClear(this_interrupt->int_num);
        OS_ASSERT_COND(status == OS_SUCCESS);
    }
 
 
    /* Stops at first non valid entry - requires proper insertion and
     * removal from list! */
    while ((this_interrupt != NULL) && (this_interrupt->valid == TRUE))
    {
        this_interrupt->handler(this_interrupt->argument);
        this_interrupt = this_interrupt->next;
    }
 

    return;
}

#endif
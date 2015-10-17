/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/03/12 11:00:48 $
 $Id: os_sync_init.c,v 1.29 2014/03/12 11:00:48 b08551 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_sync_init.c,v $
 $Revision: 1.29 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sync_init.c

 @Description   OS Synchronization module initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"
#if (OS_MULTICORE == 1)
#include "os_l1_defense.h"
#endif //OS_MULTICORE

#include "os_sync_shared_.h"


extern uint8_t  g_max_num_of_cores;

#if (OS_MULTICORE == 1)

/*****************************************************************************/
os_status osMultiCoreSyncInitialize()
{
    os_status status = OS_SUCCESS;

    if(osL1dGetResetMode() != OS_L1D_MODE_1)
    {
        if (osGetCoreID() == osGetMasterCore())
        {
            /* the master core initializes the central barrier */
            status = osCentralBarrierInit(&shared_cores_barrier,
                                          osGetNumberOfCores());
        }
        else
        {
            /* other cores wait for initialization of central barrier */
            while (shared_cores_barrier.init_flag == FALSE)
            {
 
            }
 
        }
    }
    return status;
}


/*****************************************************************************/
os_status osCentralBarrierInit(volatile os_barrier_t *barrier,
                               uint8_t               num_of_cores)
{
    int i;
 
    OS_ASSERT_COND(barrier != NULL);
    OS_ASSERT_COMPILER_COND(g_max_num_of_cores > 0);
    OS_ASSERT_COND(osL1dGetResetMode() != OS_L1D_MODE_1);

    barrier->comm_sensor = TRUE;
    barrier->counter = 0;
    barrier->height = num_of_cores;
 
    osSpinLockInitialize((os_spinlock_handle *)&barrier->guard);
 
    if(osL1dGetResetMode() != OS_L1D_MODE_2)
    {
        uint8_t * sensors;
 
        sensors = (uint8_t *)osMalloc((sizeof(uint8_t) * g_max_num_of_cores), OS_MEM_SHARED);
        if (sensors == NULL)
        {
            RETURN_ERROR(OS_ERR_NO_MEMORY);
        }
 
        barrier->sensors = sensors;
    }

    for ( i=0; i < g_max_num_of_cores; i++ )
    {
        barrier->sensors[i] = TRUE;
    }
 
    barrier->init_flag = TRUE;
    return OS_SUCCESS;
}

#endif /* (OS_MULTICORE == 1) */

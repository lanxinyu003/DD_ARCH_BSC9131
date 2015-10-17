/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/07/01 16:29:12 $
 $Id: psc9x3x_hwi_init.c,v 1.12 2013/07/01 16:29:12 b33105 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_hwi_init.c,v $
 $Revision: 1.12 $
******************************************************************************/

/******************************************************************************
 
 @File          psc9x3x_hwi_init.c

 @Description   PSC9X3X Hardware Interrupts initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_.h"
#include "os_init.h"

#include "starcore_hwi_.h"
#include "psc9x3x_hwi.h"
#include "hwi_generic_.h"

#include "psc9x3x_memmap.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "sc3x00_mmu_.h"

#define NUM_VNMI    8
#define HWI_INVALID_MODE(mode)  \
(((mode != LEVEL_MODE) && (mode != EDGE_MODE)) || (((hwi_num >= OS_INT_ICAES) && (hwi_num <= OS_INT_DCNCH)) && (mode != EDGE_MODE)))

os_hwi_function     interrupt_pointer[MAX_INTERRUPTS];
os_hwi_arg          interrupt_argument[MAX_INTERRUPTS];


extern uint32_t            *g_base_int_vect_add;
extern volatile uint16_t    g_os_interrupt_count;

extern general_interrupt_t general_ored_interrupt[];
extern general_interrupt_t debug_ored_interrupt[];
extern general_interrupt_t maple_ored_interrupt[];


/**************************************************************************/
/* utility function: prioritySet                    */
/* called by osHwiCreate and osHwiPrioritySet       */
/* Caution: call only when interrupts are disabled  */
static void prioritySet(os_hwi_handle     hwi_num,
                 os_hwi_priority   priority)
{
    uint16_t        index;
    uint32_t        priority_mask;
    uint32_t        *priority_reg_ptr;
    uint32_t        current_reg;
    /* EPIC interrupt:
    Figure out which IPL register it is in. There are four interrupts
    per IPL register. */

    index = (uint16_t)(hwi_num >> 2);           /* /4 */

    priority_reg_ptr = (uint32_t *)((g_dsp_plat_map->epic).p_ipl);
    priority_reg_ptr += index;

    /* Figure out what the priority mask looks like */
    priority_mask = 0x000000FFUL << ((hwi_num & 0x0003) * 8);

    /* For NMI priority, the IPL bits do not matter in the p_ipl */
    /* register, only need to set the INC bit to NMI. */
    if (priority == OS_HWI_PRIORITY_NMI)
        priority = 0x80;
    /* Set priority */
    READ_UINT32(current_reg, *priority_reg_ptr);
    current_reg &= (priority_mask ^ 0xFFFFFFFF);
    /* Set priority to zeroed bits in IPL */
    current_reg |= ((uint32_t)priority) << ((hwi_num & 0x0003) * 8);
    WRITE_UINT32(*priority_reg_ptr,current_reg);
}

/**************************************************************************/
/* Note: for now this is a static function, */
/* not exposed to the user. */
/* Set enable_interrupt to TRUE to enable the interrupt */
/* in the ENDISx register, FALSE to disable the interrupt */
static os_status setInterruptEnable(os_hwi_handle hwi_num,
                                    bool enable_interrupt)
{
    uint16_t        index;
    uint32_t        endis_reg = 0;
    uint32_t        *endis_reg_ptr;
    os_hwi_handle   epic_hwi_num = 0;
    uint32_t        interrupt_bit = 0;

    /* Don't handle Non-EPIC interrupts don't require any more handling */
    if (hwi_num < EPIC_INTERRUPTS_OFFSET)
        RETURN_ERROR(OS_ERR_HWI_INVALID);
    else /* get the EPIC interrupt zero-based index */
        epic_hwi_num = (os_hwi_handle)(hwi_num - EPIC_INTERRUPTS_OFFSET);

    /* handle the Enable/Disable Interrupts register ENDISx */
    index = (os_hwi_handle)(epic_hwi_num >> 5);           /* /32 , there are 8 ENDISx registers*/
    endis_reg_ptr = (uint32_t *)((g_dsp_plat_map->epic).p_endis);
    endis_reg_ptr += index;

    /* we need to set or clear the corresponding interrupt bit */
    interrupt_bit = (uint32_t)(0x00000001 << (epic_hwi_num & 0x001F));

    READ_UINT32(endis_reg, *endis_reg_ptr);
    if (enable_interrupt)
        endis_reg |= interrupt_bit;
    else
        endis_reg &= ~interrupt_bit;
    WRITE_UINT32(*endis_reg_ptr, endis_reg);

    return OS_SUCCESS;
}

/**************************************************************************/
/*
 * Has to be called only once, not multithreaded protected.
 */
os_status osHwiInitialize(void *int_vec_add)
{
    uint16_t     i;
    os_status status;
 
    g_base_int_vect_add = (uint32_t *)int_vec_add;
    g_os_interrupt_count = 0;
    /* Disable ALL interrupts (except NMI) */
    osHwiSwiftDisable();

    /* Set the VBA */
    osHwiSetVBA((uint32_t) g_base_int_vect_add);

    /* Set Dispatchers interrupt table entries to osHwiNullHandler() */
    for (i = 0; i < MAX_INTERRUPTS; i++)
    {
        interrupt_pointer[i] = &osHwiNullHandler;
        interrupt_argument[i] = i;
    }

    /* EPIC initialization:
    1. Disable all interrupts.
    2. Clear any pending interrupts. */

    /* Note: P_IPL0 is read only, so loop starts from 1. */
    for (i = 1; i < NUMBER_OF_IPL_REGISTERS; i++)
    {
        WRITE_UINT32((g_dsp_plat_map->epic).p_ipl[i], 0x00000000);
    }

    /* Clear pending interrupts */
    for (i = 0; i < NUMBER_OF_EPIC_REGS; i++)
    {
        WRITE_UINT32((g_dsp_plat_map->epic).p_ipr[i], 0xFFFFFFFF);
    }

    /* map default MMU exceptions */
    status = osHwiCreate(OS_INT_DMMUAE,OS_HWI_PRIORITY0, EDGE_MODE, osHwiDMMUError,0);
    OS_ASSERT_COND(status == OS_SUCCESS);
    status = osHwiCreate(OS_INT_IMMUAE,OS_HWI_PRIORITY0, EDGE_MODE, osHwiIMMUError,0);
    OS_ASSERT_COND(status == OS_SUCCESS);

    /* map general exception, only on . */
    status = osHwiCreate(OS_INT_ORED_GENERAL, OS_HWI_PRIORITY0, LEVEL_MODE, osHwiGeneralIsr, (os_hwi_arg)general_ored_interrupt);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    /* map debug exception */
    status = osHwiCreate(OS_INT_ORED_DEBUG, OS_HWI_PRIORITY0, LEVEL_MODE, osHwiGeneralIsr, (os_hwi_arg)debug_ored_interrupt);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    /* map MAPLE general error */
    status = osHwiCreate(OS_INT_ORED_MAPLE, OS_HWI_PRIORITY0, LEVEL_MODE, osHwiGeneralIsr, (os_hwi_arg)maple_ored_interrupt);
    OS_ASSERT_COND(status == OS_SUCCESS);

    /* Enable interrupts */
    osHwiSwiftEnable();

    /* Enable all the EPIC controlled interrupts.
     * It is stronger than the core DI/EI instruction as it also blocks NMIs which the core DI does not. */
    CLEAR_SYNCIO_UINT32((g_dsp_plat_map->epic).p_di, 0x00000001);

    return OS_SUCCESS;
}

/**************************************************************************/
os_status osHwiCreate(os_hwi_handle     hwi_num,
                      os_hwi_priority   priority,
                      os_hwi_mode       mode,
                      os_hwi_function   handler,
                      os_hwi_arg        argument)
{
    os_status       status = OS_SUCCESS;
    os_hwi_status   int_status;
    uint16_t        index;
    uint32_t        pending_bit, mode_bit;
    uint32_t        *pending_reg_ptr, *trigger_reg_ptr;
    uint32_t        trigger_reg;
    os_hwi_handle   epic_hwi_num = 0;
 
#ifdef HWI_ERROR_CHECKING
    if ( (priority > OS_HWI_LAST_PRIORITY) && (priority != OS_HWI_PRIORITY_NMI) )
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_PRIORITY_INVALID);
    }
    if (handler == NULL)
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_FUNCTION_INVALID);
    }
    if (HWI_INVALID_MODE(mode))
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        if(priority != 0) //meaning disable only
        {
            RETURN_ERROR(OS_ERR_HWI_MODE_INVALID);
        }
    }
#endif /* HWI_ERROR_CHECKING */

    if (hwi_num > OS_INT_LAST)
    {
        if(hwi_num > OS_INT_GEN_LAST)
        {
#ifdef HWI_ERROR_ASSERT
            OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
            RETURN_ERROR(OS_ERR_HWI_INVALID);
        }
        else
        {
            return osHwiCreateGeneral(hwi_num,handler,argument);
        }
    }

    if ( (priority > 0) && (priority != OS_HWI_PRIORITY_NMI) )
        priority = (os_hwi_priority)((OS_HWI_LAST_PRIORITY + 1) - priority);

    int_status = osHwiDisable();

#ifdef HWI_LOGGING
    if (priority != OS_HWI_PRIORITY_NMI)
        osLog(LOG_HWI_CREATE, (uint32_t)(((OS_HWI_LAST_PRIORITY - priority)<<16) | (hwi_num)));
    else
        osLog(LOG_HWI_CREATE, (uint32_t)(((0xFFFF)<<16) | (hwi_num))); /* for NMI interrupts */
#endif

    /* Store handler into table */
    interrupt_pointer[hwi_num] = handler;
    if (hwi_num == OS_INT_ORED_GENERAL)
        interrupt_argument[hwi_num] = (os_hwi_arg)general_ored_interrupt;
    else if (hwi_num == OS_INT_ORED_DEBUG)
        interrupt_argument[hwi_num] = (os_hwi_arg)debug_ored_interrupt;
    else if (hwi_num == OS_INT_ORED_MAPLE)
        interrupt_argument[hwi_num] = (os_hwi_arg)maple_ored_interrupt;
    else
        interrupt_argument[hwi_num] = argument;
 
    /* Non-EPIC interrupts don't require any more handling */
    if (hwi_num < EPIC_INTERRUPTS_OFFSET)
    {
        osHwiEnable(int_status);
        return status;
    }
    else /* get the EPIC interrupt zero-based index */
        epic_hwi_num = (os_hwi_handle)(hwi_num - EPIC_INTERRUPTS_OFFSET);

    prioritySet(epic_hwi_num, priority);

    /* handle the mode register ELRx */
    index = (uint16_t)(epic_hwi_num >> 5);           /* /32 */
    trigger_reg_ptr = (uint32_t *)((g_dsp_plat_map->epic).p_elr);
    trigger_reg_ptr += index;

    mode_bit = (uint32_t)(0x00000001 << (epic_hwi_num & 0x001F));
    if (mode == LEVEL_MODE)
    {
        READ_UINT32(trigger_reg, *trigger_reg_ptr);
        trigger_reg &= ~mode_bit;
        WRITE_UINT32(*trigger_reg_ptr, trigger_reg);
    }
    else // mode == EDGE_MODE
    {
        READ_UINT32(trigger_reg, *trigger_reg_ptr);
        trigger_reg |= mode_bit;
        WRITE_UINT32(*trigger_reg_ptr, trigger_reg);
    }
 
    /* handle the pending interrupt register */
    index = (uint16_t)(epic_hwi_num >> 5);               /* /32 */
    pending_reg_ptr = (uint32_t *)((g_dsp_plat_map->epic).p_ipr);
    pending_reg_ptr += index;

    /* Figure out what the pending mask looks like */
    pending_bit = (uint32_t)(0x00000001 << (epic_hwi_num & 0x001F));

    /* Clear any pending interrupts */
    WRITE_UINT32(*pending_reg_ptr,pending_bit);

    /* enable the interrupt in the ENDIS register */
    setInterruptEnable(hwi_num, TRUE);
 
    osHwiEnable(int_status);

    return status;
}

/*************************************************************************/
os_status osHwiIsCreated(os_hwi_handle   hwi_num, bool *created)
{
    os_hwi_function function;
 
    if (hwi_num > OS_INT_LAST)
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_INVALID);
    }
    function = interrupt_pointer[hwi_num];
 
    *created = (function != &osHwiNullHandler);
    return OS_SUCCESS;
}

/*************************************************************************/
os_status osHwiPendingClear(os_hwi_handle     hwi_num)
{

    os_status       status = OS_SUCCESS;
    uint16_t        index;
    uint32_t        pending_bit;
    uint32_t        *pending_reg_ptr;
    os_hwi_handle   epic_hwi_num = 0;
 
#ifdef HWI_ERROR_CHECKING
    if (hwi_num > OS_INT_LAST)
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_INVALID);
    }
#endif /* HWI_ERROR_CHECKING */

    /* Non-EPIC interrupts don't require any more handling */
    if (hwi_num < EPIC_INTERRUPTS_OFFSET)
        return status;
    else /* get the EPIC interrupt zero-based index */
        epic_hwi_num = (os_hwi_handle)(hwi_num - EPIC_INTERRUPTS_OFFSET);
 
    /* handle the pending interrupt register */
    index = (uint16_t)(epic_hwi_num >> 5);               /* /32 */
    pending_reg_ptr = (uint32_t *)((g_dsp_plat_map->epic).p_ipr);
    pending_reg_ptr += index;

    /* Figure out what the pending mask looks like */
    pending_bit = (uint32_t)(0x00000001 << (epic_hwi_num & 0x001F));

    /* Clear any pending interrupts */
    WRITE_UINT32(*pending_reg_ptr,pending_bit);

    return status;
}

/***********************************************************************//**
 @page          hwi_msc815x             MSC815X Hardware Interrupts
 @section       hwisetpriority_msc815x  MSC815X::osHwiPrioritySet

                To API documentaion of osHwiPrioritySet()
 
                Multithread: Safe

 @Cautions      None.
 
*//*************************************************************************/
os_status osHwiPrioritySet(os_hwi_handle    hwi_num,
                           os_hwi_priority  priority)
{
    os_status       status = OS_SUCCESS;
    os_hwi_status   int_status;
    uint16_t        index;
    uint32_t        pending_bit;
    uint32_t        *pending_reg_ptr;
    os_hwi_handle   epic_hwi_num = 0;
 
#ifdef HWI_ERROR_CHECKING
    if ( (priority > OS_HWI_LAST_PRIORITY) && (priority != OS_HWI_PRIORITY_NMI))
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_PRIORITY_INVALID);
    }
    if ( (hwi_num > OS_INT_LAST) || (hwi_num < EPIC_INTERRUPTS_OFFSET) )
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_INVALID);
    }
#endif /* HWI_ERROR_CHECKING */

    if ( (priority > 0) && (priority != OS_HWI_PRIORITY_NMI) )
        priority = (os_hwi_priority)((OS_HWI_LAST_PRIORITY + 1) - priority);

    int_status = osHwiDisable();
 
    /* get the EPIC interrupt zero-based index */
    epic_hwi_num = (os_hwi_handle)(hwi_num - EPIC_INTERRUPTS_OFFSET);

    prioritySet(epic_hwi_num, priority);

    /* handle the pending interrupt register */
    index = (uint16_t)(epic_hwi_num >> 5);               /* /32 */
    pending_reg_ptr = (uint32_t *)((g_dsp_plat_map->epic).p_ipr);
    pending_reg_ptr += index;

    /* Figure out what the pending mask looks like */
    pending_bit = (uint32_t)(0x00000001 << (epic_hwi_num & 0x001F));

    /* Clear any pending interrupts */
    *pending_reg_ptr = pending_bit;

    osHwiEnable(int_status);

    return status;
}


/*************************************************************************/
os_status osHwiVectorInstall(os_hwi_handle      hwi_num,
                             os_hwi_priority    priority,
                             os_hwi_function    handler)
{
    VAR_UNUSED(hwi_num);
    VAR_UNUSED(priority);
    VAR_UNUSED(handler);
    RETURN_ERROR(OS_ERR_COMMAND_UNSUPPORTED);
}


/*************************************************************************/
os_status osHwiDelete(os_hwi_handle hwi_num)
{
    /* Use osHwiCreate to set priority to zero and handler to NULL Handler. */
    return osHwiCreate(hwi_num, 0, LEVEL_MODE, &osHwiNullHandler, 0);
}

/*************************************************************************/
os_status osHwiMultiplexedCreate(os_hwi_handle     hwi_num,
                      os_hwi_priority   priority,
                      os_hwi_mode       mode,
                      os_hwi_function   handler,
                      os_hwi_arg        argument)
{

    multiplexed_hwi_t *this_int = NULL, *head = NULL, *fixed_head = NULL;
    os_status          status;
    bool               reusing_non_valid_entry = FALSE;


#ifdef HWI_ERROR_CHECKING
    if ( (priority > OS_HWI_LAST_PRIORITY) && (priority != OS_HWI_PRIORITY_NMI) )
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_PRIORITY_INVALID);
    }
 
    if (handler == NULL)
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_FUNCTION_INVALID);
    }
 
    if (HWI_INVALID_MODE(mode))
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        if(priority != 0) //meaning disable only
        {
            RETURN_ERROR(OS_ERR_HWI_MODE_INVALID);
        }
    }
#endif /* HWI_ERROR_CHECKING */
 

    osHwiSwiftDisable();
 
    // If multiplexed interrupt list already registered
    if (interrupt_pointer[hwi_num] == osHwiMultiplexedHandler)
    {
        fixed_head = (multiplexed_hwi_t *)interrupt_argument[hwi_num];
 
        head = fixed_head;
 
        // Error in registration
#ifdef HWI_ERROR_CHECKING
      if (head == NULL)
      {
#ifdef HWI_ERROR_ASSERT
          OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
          status = OS_ERR_HWI_INVALID;
          REPORT_ERROR(status);
          goto exit_func;
      }
 
        // Verify that the mode is consistent with other
        this_int = head;

        do
        {
            if ((this_int->mode != mode) && (this_int->valid == TRUE))
            {
#ifdef HWI_ERROR_ASSERT
                OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
                status = OS_ERR_HWI_MODE_INVALID;
                REPORT_ERROR(status);
                goto exit_func;
            }
            this_int = this_int->next;
        } while (this_int != NULL);
#endif /* HWI_ERROR_CHECKING */
 
        this_int = NULL;
        while (head != NULL)
        {
            if (head->valid == FALSE)
            {
                this_int = head;
                reusing_non_valid_entry = TRUE;
                break;
            }
            head = head->next;
        }
 
    }
 
    if (this_int == NULL)
    {
        this_int = (multiplexed_hwi_t *)osMalloc(sizeof(multiplexed_hwi_t), OS_MEM_LOCAL);
        if (this_int == NULL)
        {
#ifdef HWI_ERROR_ASSERT
                OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
                status = OS_ERR_NO_MEMORY;
                REPORT_ERROR(status);
                goto exit_func;
        }

        this_int->next = NULL;
    }

    this_int->argument = argument;
    this_int->handler = handler;
    this_int->mode = mode;
    this_int->valid = TRUE;

    // This is the first registration of this multiplexed interrupt
    if (fixed_head == NULL)
        status = osHwiCreate(hwi_num, priority, mode, osHwiMultiplexedHandler, (os_hwi_arg)this_int);
    else
    {
        // If necessary to append this_int to linked list
        if (reusing_non_valid_entry == FALSE)
        {
            head = fixed_head;
            while (head->next != NULL)
                head = head->next;
            head->next = this_int;
        }
        // Set priority to new priority
        status = osHwiPrioritySet(hwi_num, priority);
        // Enable the interrupt
        setInterruptEnable(hwi_num, TRUE);
    }
 
exit_func:
    osHwiSwiftEnable();
    return status;
}
 

 
/*************************************************************************/
static inline void move_to_end(multiplexed_hwi_t *iterator, multiplexed_hwi_t *this_int)
{
    while (iterator->next != NULL)
        iterator = iterator->next;
    iterator->next = this_int;
    this_int->next = NULL;
}

/*************************************************************************/
os_status osHwiMultiplexedDelete(os_hwi_handle     hwi_num,
                      os_hwi_function   handler,
                      os_hwi_arg        argument)
{

    os_status status = OS_SUCCESS;
    multiplexed_hwi_t *this_int = NULL, *iterator, *head;
 
#ifdef HWI_ERROR_CHECKING
 
    // Can't remove during active hwi - risk of corrupting active list
    if (g_os_flags & OS_FLG_HWI_ACTIVE)
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_FAIL);
    }
    if (hwi_num > OS_INT_LAST)
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_INVALID);
    }
 
    if (interrupt_pointer[hwi_num] != osHwiMultiplexedHandler)
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HWI_COMMAND_INVALID);
    }
#endif // HWI_ERROR_CHECKING
 
    osHwiSwiftDisable();
    head = (multiplexed_hwi_t *)interrupt_argument[hwi_num];
 
    for (iterator = head ; iterator != NULL ; iterator = iterator->next)
    {
        if ((iterator->argument == argument) && (iterator->handler == handler))
        {
#ifdef HWI_ERROR_CHECKING
            if (iterator->valid != TRUE)
            {
#ifdef HWI_ERROR_ASSERT
                OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
                status = OS_ERR_ALREADY_FREE;
                REPORT_ERROR(status);
                goto exit_func;
            }
#endif // HWI_ERROR_CHECKING
            this_int = iterator;
            this_int->valid = FALSE;
 
            /* Put this_int at the end of the list. Ensures requirement
             * in osHwiMultiplexedHandler() */
 
            // If first in list
            if (this_int == head)
            {
                // If not single entry in list
                if (this_int->next != NULL)
                {
                    interrupt_argument[hwi_num] = (os_hwi_arg)(this_int->next);
                    move_to_end(iterator, this_int);
                }
                else
                {
                    setInterruptEnable(hwi_num, FALSE);
                }
            }
            // If not already last in list
            if (this_int->next != NULL)
            {
                iterator = head;
                while (iterator->next != this_int)
                {
                    iterator = iterator->next;
                }
                iterator->next = this_int->next;
                move_to_end(iterator, this_int);
            }
            break;
 
        }
    }
 
 
exit_func:
    osHwiSwiftEnable();
    return status;
 
}


/*************************************************************************/
os_status osHwiGpioMap(os_hwi_handle hwi_num, unsigned int *gpio_pin)
{
 
    VAR_UNUSED(hwi_num);
    VAR_UNUSED(gpio_pin);
    OS_ASSERT;
    RETURN_ERROR(OS_ERR_COMMAND_UNSUPPORTED); // No GPIO in SC 913x
}

/*************************************************************************/
os_status osHwiGpioIrqConfigure(os_hwi_handle hwi_num)
{
    VAR_UNUSED(hwi_num);
    OS_ASSERT;
    RETURN_ERROR(OS_ERR_COMMAND_UNSUPPORTED); // No GPIO in SC 913x
}

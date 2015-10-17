/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/07/01 16:29:12 $
 $Id: starcore_hwi_.h,v 1.19 2013/07/01 16:29:12 b33105 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/common/include/starcore_hwi_.h,v $
 $Revision: 1.19 $
******************************************************************************/

/******************************************************************************
 
 @File          starcore_hwi_.h

 @Description   StarCore-specific Hardware Interrupts header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __STARCORE_HWI_H_
#define __STARCORE_HWI_H_


/**************************************************************************//*
 @Description   ISR Entry structure.
 
*//***************************************************************************/
typedef struct
{
    os_hwi_function isr;
    os_hwi_arg      param;
 
} isr_entry_t;



typedef struct multiplexed_hwi_t
{
    bool                        valid;
    os_hwi_mode                 mode;
    uint16_t                    int_num;
    os_hwi_function             handler;
    os_hwi_arg                  argument;
#ifdef SC39XX
    os_hwi_dispatcher           dispatcher;
#endif
    struct multiplexed_hwi_t    *next;
} multiplexed_hwi_t;


/**************************************************************************//**
 @Function      osHwiSetVBA

 @Description   Sets the interrupts Vector Base Address register.
 
 @Param[in]    vector_address -  value to set to the VBA register.
 
*//***************************************************************************/
void osHwiSetVBA(uint32_t vector_address);


/**************************************************************************//**
 @Function      osHwiNullHandler

 @Description   Dummy interrupt handler for unassigned interrupts.
 
 @Param[in]    arg -  Hardware interrupt argument (unused).
 
*//***************************************************************************/
void osHwiNullHandler(os_hwi_arg arg);


/**************************************************************************//**
 @Function      osHwiMultiplexedHandler

 @Description   Interrupt handler for multiplexed interrupts.
 
 @Param[in]    arg -  Hardware interrupt argument.
 
*//***************************************************************************/
void osHwiMultiplexedHandler(os_hwi_arg arg);

/**************************************************************************//**
 @Function      osHwiIsPreciseException

 @Description   Returns whether the interrupt is preceise.
 
*//***************************************************************************/
bool osHwiIsPreciseException(os_hwi_handle hwi_num);


/**************************************************************************//**
 @Function      osSwiTrap

 @Description   Generate trap interrupts.
 
*//***************************************************************************/
void osSwiTrap(void);


/**************************************************************************//**
 @Function      socEpicBaseAddressGet

 @Description   Get the base address of the EPIC memory map .
 
*//***************************************************************************/
void * socEpicBaseAddressGet();


/**************************************************************************//**
 @Function      socHwiInitialize

 @Description   Initialize any SoC dependent initializations of the EPIC.
 
*//***************************************************************************/
os_status socHwiInitialize();

#endif // __STARCORE_HWI_H_



/*******************************************************************************
* COPYRIGHT BH
********************************************************************************
* FileName: os_cpu.h
* Function: Dependent function of the CPU.
* Author:   Andy
* Date:     2011/09/01
* History:
*
* Modify Date   Owner   BugID/CRID      Contents
* ------------------------------------------------------------------------------
* 2011/09/01    Andy                     Draft
*******************************************************************************/
#ifndef OS_CPU_H
#define OS_CPU_H

extern void os_disable_global_int(void);
extern void os_enable_global_int(void);
extern unsigned int os_disable_global_int_save(void);
extern void os_enable_global_restore(unsigned int flag);

#define OS_DISABLE_GLOBAL_INT() os_disable_global_int()
#define OS_ENABLE_GLOBAL_INT() os_enable_global_int()

#define OS_DISABLE_GLOBAL_INT_SAVE(flag) do{flag = os_disable_global_int_save();}while(0)
#define OS_ENABLE_GLOBAL_INT_RESTORE(flag) os_enable_global_restore(flag)

/********************* End of Protection For Header File **********************/
#endif
/*********************************End Of File *********************************/

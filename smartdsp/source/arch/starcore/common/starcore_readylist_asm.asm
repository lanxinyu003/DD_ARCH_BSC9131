;/******************************************************************************
; Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
; All Rights Reserved
; 
; This is proprietary source code of Freescale Semiconductor Inc., and its use
; is subject to the CodeWarrior EULA.  The copyright notice above does not 
; evidence any actual or intended publication of such source code.
;  
;*******************************************************************************/
;/******************************************************************************
; $Date: 2012/04/09 13:07:26 $
; $Id: starcore_readylist_asm.asm,v 1.10 2012/04/09 13:07:26 b20727 Exp $
; $Source: /cvsdata/SmartDSP/source/arch/starcore/common/starcore_readylist_asm.asm,v $
; $Revision: 1.10 $
; **************************************************************************//*
;
; @File          starcore_readylist_asm.asm
;
; @Description   StarCore-specific Ready-List Assembly functions.
;
;                These functions are not public.
;
; @Cautions      None.
;
;*//***************************************************************************/

    PAGE 128,0,0,0,0
    SECTION .oskernel_text_run_time
	if @def('MMU_SUPPORT')
	SECTYPE overlay
    endif

;    IF  (SC3X00==1)

;	ENDIF
;#define     ANY_TASK                        255
;#define     NO_TASK                         0xFFFF

NO_TASK                             EQU      $FFFF

;typedef struct os_ready_list_
;{
;    uint16_t number;
;    uint16_t priority;
;} os_ready_list_;

;typedef volatile struct os_ready_list
;{
;    uint16_t ready_cnt;
;    uint16_t size;
;    os_ready_list_ *list;
;} os_ready_list;

OSREADYLISTCNTOFFSET       equ 0
OSREADYLISTSIZEOFFSET      equ 2
OSREADYLISTLISTOFFSET      equ 4




;*************************************************************************
;
;     Function:           osReadyListPush
;
;     Inputs:             os_ready_list *ready - ready list of interest
;                         uint16_t number      - number associated with
;                                                object to be pushed.
;                         uint16_t priority    - priority associated with
;                                                object to be pushed.
;
;     Outputs:            os_status      - currently always returns
;                                                 OS_SUCCESS
;
;     Description:        Places object on associated ready list in
;                         order of priority.
;
;     Cautions:           Optimized routine tailored towards layout and
;                         size of ready list struct and object descriptors.
;                         uint16_t needs to be 16 bits!
;
;*************************************************************************
;os_status osReadyListPush(os_ready_list *ready,
;     uint16_t number, uint16_t priority)
;{
     global _osReadyListPush
     align $10
_osReadyListPush	type	func
; r0 = ready
; d1 = number
; SP-10 = priority


;    // THE TASK_READY LIST IS LOADED ON INITIALIZATION WITH 0x1FFF
;    // THIS WAY THE FOR LOOP WILL ALWAYS BREAK OUT
;    // THE TASK_READY TASK LIST IS g_max_swi + 1 IN SIZE SO IT WILL ALWAYS PASS

	[
		moveu.w (r0),r3	; r3 = ready_cnt
		move.l (r0+4),r1 ; r1 = list
	]
		move.w #1,d0
	[
		move.w (sp-10),d9
		tfr     d1,d8
		addl2a    r3,r1		
	]
	[
		rtsd
		inca    r3		
	]
	[
		move.w r3,(r0)
		move.2w d8:d9,(r1)
	]
		


	global	F_osReadyListPush_end
	align	16
F_osReadyListPush_end
	
;*************************************************************************
;
;     Function:           osReadyListPop
;
;     Inputs:             os_ready_list *ready  - ready list of interest
;                         uint16_t priority     - lowest priority to be
;                                                 popped.
;
;     Outputs:            uint16_t              - returns object number
;                                                 with highest priority
;                                                 on list.  If no object
;                                                 on list has priority
;                                                 higher than passed
;                                                 in priority, NO_TASK
;                                                 returned.
;
;     Description:        Pops an object off ready list if priority
;                         is greater than passed in priority.
;
;     Cautions:           Optimized routine tailored towards layout and
;                         size of ready list struct and object descriptors.
;                         uint16_t needs to be 16 bits!
;
;*************************************************************************
; uint16_t osReadyListPop(os_ready_list *ready, uint16_t priority)
; {
      GLOBAL _osReadyListPop
     align $10

_osReadyListPop	type	func
; r0=ready
; d1=priority

	[
		move.w (r0),d15 ; d15 = ready_cnt
		move.l (r0+4),r1 ; r1 = list
		aslw d1,d13
		clr d10		
	]
	[
		tsteq d15
		doen3 d15
		sub #1,d10
	]
	[
		moveu.l  #65535,d0 
		sub #1,d15
	]	
	[
		or #$ffff,d13.l
		dosetup3 _startOfLoop	
	]
	[
		tfra r1,r14
	]
	[	
		IFT rts
		IFA move.l d15,r2 ; offset cnt-1
	]
		loopstart3
_startOfLoop
	[
		inc d10
		move.2f (r1)+,d8:d9		
	]
		add d10,d9,d9
		min d9,d13
_endOfLoop
		loopend3

	[
		tfra r14,r13
	]
	[
		impy.w	#1,d13
		asrw    d13,d2
		move.l d13,r11
	]
	[
		cmpeq.w #$ffff,d13	; if no priority more important found, return
		cmpeq d1,d2 ; if(current priority recieved as param == min priority found) return
	]
	[				
	]
		zxta.w r11
		addl2a r2,r13
	[
		IFF move.w  d15,(r0)
		IFA addl2a r11,r14
	]
	[
		IFF move.2w (r13),d8:d9
		IFA rtsd
	]
	[
		IFF move.w (r14),d0
		IFF move.2w d8:d9,(r14)
	]

    
	global	F_osReadyListPop_end
	align	16
F_osReadyListPop_end

    endsec

;/******************************************************************************
; Copyright �1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
; All Rights Reserved
; 
; This is proprietary source code of Freescale Semiconductor Inc., and its use
; is subject to the CodeWarrior EULA.  The copyright notice above does not 
; evidence any actual or intended publication of such source code.
;  
;*******************************************************************************/
;/******************************************************************************
; $Date: 2012/12/25 09:01:58 $
; $Id: interrupt_dispatcher_asm.asm,v 1.14 2012/12/25 09:01:58 b41932 Exp $
; $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/interrupt_dispatcher_asm.asm,v $
; $Revision: 1.14 $
; **************************************************************************//*
;
; @File          interrupt_dispatcher_asm.asm
;
; @Description   StarCore-specific Hardware Interrupts Assembly functions.
;
;                These functions are not public.
;
; @Cautions      None.
;
;*//***************************************************************************/

    PAGE 128,0,0,0,0

   SECTION    .oskernel_text_run_time_critical
	sectype overlay


;*****************************************************************************
; THESE ARE THE DEFINES FOR THE OS_FLAGS
;*****************************************************************************

OS_FLG_HWI_ACTIVE                   EQU      $0001
OS_FLG_BGD_ACTIVE                   EQU      $0002
OS_FLG_SWI_ACTIVE                   EQU      $0004
OS_FLG_SWI_ENABLE                   EQU      $0100
OS_FLG_SWI_REQ                      EQU      $0200
OS_FLG_TSK_REQ                      EQU      $1000
OS_TASK_RUNNING					    EQU      $0080

OS_SWI_STRUCT_COUNT_OFFSET          EQU      4
OS_SWI_STRUCT_STATUS_OFFSET         EQU      8
OS_SWI_STRUCT_PRIORITY_OFFSET       EQU      10
OS_SWI_STRUCT_USER_ID_OFFSET        EQU      12
OS_TASK_STRUCT_STATUS_OFFSET        EQU      4
OS_TASK_STRUCT_SR_OFFSET            EQU      4


	if @def('MMU_SUPPORT')
OS_SWI_STRUCT_MMU_DATA              EQU      16
OS_SWI_STRUCT_MMU_PROG              EQU      20
OS_SWI_STRUCT_OFFSET                EQU      24
	else
OS_SWI_STRUCT_OFFSET                EQU      16
	endif
OS_RESOURCE_USED                    EQU      3
OS_RESOURCE_ACTIVE                  EQU      4
OS_SWI_PRIORITY_ALL                 EQU      16
OS_SR_EXP                           EQU      0x0004

ANY_TASK                            EQU      $FF
NO_TASK                             EQU      $FFFF


; Defined in this file
    GLOBAL     _osHwiDispatcher
    GLOBAL     _osHwiPreciseDispatcher
    GLOBAL     _osHwiRestoreContext
    GLOBAL     _osFirstTimeSwitch
    GLOBAL     _osHwiIsPreciseException

; Defined in external file
    GLOBAL     _interrupt_pointer
    GLOBAL     _g_time_slice_left
    GLOBAL     _g_os_flags
    GLOBAL     _g_interrupt_stack
    GLOBAL     _g_max_tasks
    GLOBAL     _g_num_ready_tasks
    GLOBAL     _g_os_interrupt_count
    GLOBAL     _g_os_timer
    GLOBAL     _g_temp_stack_holder
    GLOBAL     _os_interrupt_priority
    GLOBAL     _swi_interrupt_vector
    GLOBAL     _g_swi_priority_level
    GLOBAL     _osHwiNullHandler

    ALIGN 8

;*************************************************************************
;
;     Function:           osHwiDispatcher
;
;     Inputs:             None
;
;     Outputs:            None
;
;     Description:
;
;     Cautions:  We assume that interrupts are disabled at this point
;
;*************************************************************************


_osHwiDispatcher  type	func


;   Test EXP bit of SR. If it is 1 then we are nested
;	and we should push to exception stack, otherwise push to
;	normal stack

	bmtsts.w  #OS_SR_EXP,(sp-$14)
	move.l (sp-$14),d1
	bf from_normal_stack

; All peripheral interrupts go through this function.  We need
;  to know the int vector that made the call to here.  Because
;  this could be a nested interrupt, the values in IPRx don't
;  help.  So at the interrupt vector and before the call to
;  here, we push d0 and d1 and put the int number into d0 and the
;  value of SR into d1.

; We are pushing onto the interrupt stack.
; Return address, SR, d0, and d1 are already there.

;   push    d0.e:d1.e   push.2l d0:d1      ; 1,2


    push.2l    r0:r1    push.2l    r2:r3   ; 3,4
    push.2l    r4:r5    push.2l    r6:r7   ; 5,6
    push.2l    b0:b1    push.2l    b2:b3   ; 7,8
    push.2l    b4:b5    push.2l    b6:b7   ; 9,10

    ; Breaking the context save pipeline in order to have
    ;  the data ready in time to make good use of the
    ;  movep.l opcodes in SC3X50
    if @def('MMU_SUPPORT')
    if @def('SC3X50')
    move.l #>$fff06100,r4           ; MMU Data Control and ID Registers
    move.l #>$fff07000,r5           ; MMU Program Control and ID Registers
    endif
    endif

	push.2l    n0:n1    push.2l    n2:n3   ; 11,12
	push.2l    m0:m1    push.2l    m2:m3   ; 13,14

	push.2l    sa0:lc0  push.2l    sa1:lc1 ; 15,16
	push.2l    sa2:lc2  push.2l    sa3:lc3 ; 17,18

   	push    d2.e:d3.e              push.2l d4.e:d5.e:d6.e:d7.e     ; 19, 20
    push.2l d8.e:d9.e:d10.e:d11.e  push.2l d12.e:d13.e:d14.e:d15.e ; 21, 22

    push.2l d2:d3       push.2l gcr:mctl		           ; 26

    push.2l d4:d5       push.2l d6:d7      ; 27,28
    push.2l d8:d9       push.2l d10:d11    ; 29,30
    push.2l d12:d13     push.2l d14:d15    ; 31,32
	push.2l btr0:btr1  ; 33,34
	; zerofy mctl, must be here before any move (Rx),Dx instruction
    move.l    #0,mctl

    if @def('MMU_SUPPORT')
    ; Read the current context from the MMU and push on the stack

    if @def('SC3X50')
    [
        movep.l  (r5+0),d5      ; current enabled prog segments -> d5
        movep.l  (r4+0),d3      ; current enabled data segments -> d3
    ]
    [
        movep.l  (r5+4),d4      ; current program ID -> d4
        movep.l  (r4+4),d2      ; current data ID -> d2
    ]
    else   ; @def('SC3X50')

    move.l #>$fff06100,r4       ; MMU Data Control and ID Registers
    move.l #>$fff07000,r5       ; MMU Program Control and ID Registers
    [
        move.l  (r5)+,d5        ; current enabled prog segments -> d5
        move.l  (r4)+,d3        ; current enabled data segments -> d3
    ]
    [
        move.l  (r5),d4         ; current program ID -> d4
        move.l  (r4),d2         ; current data ID -> d2
    ]
    endif   ; @def('SC3X50')

    push.2l d2:d3  push.2l d4:d5

    endif   ; MMU_SUPPORT

    bra     continue_hwi

from_normal_stack

    pushn.2l    r0:r1    pushn.2l    r2:r3   ; 3,4
    pushn.2l    r4:r5    pushn.2l    r6:r7   ; 5,6
    pushn.2l    b0:b1    pushn.2l    b2:b3   ; 7,8
    pushn.2l    b4:b5    pushn.2l    b6:b7   ; 9,10

    ; Breaking the context save pipeline in order to have
    ;  the data ready in time to make good use of the
    ;  movep.l opcodes in SC3X50
    if @def('MMU_SUPPORT')
    if @def('SC3X50')
    move.l #>$fff06100,r4           ; MMU Data Control and ID Registers
    move.l #>$fff07000,r5           ; MMU Program Control and ID Registers
    endif
    endif

	pushn.2l    n0:n1    pushn.2l    n2:n3   ; 11,12
	pushn.2l    m0:m1    pushn.2l    m2:m3   ; 13,14

	pushn.2l    sa0:lc0  pushn.2l    sa1:lc1 ; 15,16
	pushn.2l    sa2:lc2  pushn.2l    sa3:lc3 ; 17,18

    pushn    d2.e:d3.e              pushn.2l d4.e:d5.e:d6.e:d7.e     ; 19, 20
    pushn.2l d8.e:d9.e:d10.e:d11.e  pushn.2l d12.e:d13.e:d14.e:d15.e ; 21, 22

    pushn.2l d2:d3       pushn.2l gcr:mctl					 ; 26

    pushn.2l d4:d5       pushn.2l d6:d7      ; 27,28
    pushn.2l d8:d9       pushn.2l d10:d11    ; 29,30
    pushn.2l d12:d13     pushn.2l d14:d15    ; 31,32
	pushn.2l btr0:btr1  ; 33,34
	
	; zerofy mctl, must be here before any move (Rx),Dx instruction
    move.l    #0,mctl


    if @def('MMU_SUPPORT')
    ; Read the current context from the MMU and push on the stack

    if @def('SC3X50')
    [
        movep.l  (r5+0),d5        ; current enabled prog segments -> d5
        movep.l  (r4+0),d3        ; current enabled data segments -> d3
    ]
    [
        movep.l  (r5+4),d4         ; current program ID -> d4
        movep.l  (r4+4),d2         ; current data ID -> d2
    ]
    else   ; @def('SC3X50')

    move.l #>$fff06100,r4           ; MMU Data Control and ID Registers
    move.l #>$fff07000,r5           ; MMU Program Control and ID Registers
    [
        move.l  (r5)+,d5            ; current enabled prog segments -> d5
        move.l  (r4)+,d3            ; current enabled data segments -> d3
    ]
    [
        move.l  (r5),d4         ; current program ID -> d4
        move.l  (r4),d2         ; current data ID -> d2
    ]
    endif   ; @def('SC3X50')

    pushn.2l d2:d3  pushn.2l d4:d5

    endif   ; MMU_SUPPORT

continue_hwi


   	move.l  #_g_os_flags,r0     ; r0 = &g_os_flags
    bmset   #$c,SR.L            ; rounding and saturation
    bmset   #$c,d1.l            ; rounding and saturation


; Need to know what interrupt we are in.  Value in d0


;     g_os_flags |= OS_FLG_HWI_ACTIVE;
    [
		move.w    (r0),d2
		zxt.w     d0,d0             ; D0 = exception number
		bmclr     #$f800,d1.h		; Clear SR register for SWI
    ]

    bmclr   #$0033,d1.l       ; Clear SR register for SWI
    bmset   #$0004,d1.h             ; Set exception mode in SR
    or      #OS_FLG_HWI_ACTIVE,d2.l

    [
        move.w d2,(r0)              ; d2 = g_os_flags w/ HWI_ACTIVE
                                    ;  set.
        tfr d0,d2                   ; d2 = exception number, needed
									;  to determine is exeception
									;  is SWI activation.
        move.l d0,r1                ; r1 = exception number, needed
    ]                               ;  below for interrupt lookup.


    move.l #_g_os_interrupt_count,r6        ;  Needed below
                                            ;  r6 = &g_os_interrupt_count

    move.l #_interrupt_pointer,r5           ; Needed below
                                            ;  r5 = &interrupt_pointer
    [
        move.w (r6),d6              ; Needed below:
                                    ;  d6 = g_os_interrupt_count
        move.l #_interrupt_argument,r7
    ]

    [
        ; // Call the correct interrupt handler.  Index is in d0.  Value of 0xffff in
        ; //  index indicates we are handling an SWI TRAP.
        ; if(index != 0xffff)
        ;     (interrupt_pointer[index])(interrupt_argument[index]);
        cmpeqa.w #$0fff,r1

        ; g_os_interrupt_count++;
        add #1,d6

        addl2a r1,r5                ; interrup_pointer[index]
    ]

    [
	    addl2a r1,r7                ; interrupt_argument[index]
	    btd osHwiNoDispatch
    ]
    tfr d1,d7                       ; Save return ILP over the ISR call

    move.l (r7),d0

    [
        move.w d6,(r6)              ; g_os_interrupt_count store
                                    ;  from above.
        move.l (r5),r0              ; Read interrupt_pointer[index]
    ]


    ;Zero pending bit
    ;We do it disregarding if interrupt is level or edge
    ;index = hwi_num >> 5;
    move.l r1,d1

	[
        cmpgt.w #<$11,d1
        sub     #<$12,d1
        move.l  #>$ffae0,r7
        move.w  #<$1,d2
    ]

    [
	   and #$1f,d1,d4
	   bf  <after_epic_hwi
	]

    [
        extract #$1b,#$5,d1,d1
        asll    d4,d2
    ]

	move.l   d1,r5
    ; If SC3x50, make use of this PCU slot for pfetch.
    if @def('SC3X50')
        pfetch r0
    else
        nop
    endif
	asl2a    r5
	zxta.w   r5
	suba     r7,r5

	move.l   d2,(r5)

after_epic_hwi

	if @def('MMU_SUPPORT') ; set MMU global context

    move.l _data_context_list,r2
    move.l _prog_context_list,r3

    move.l #>$fff07000,r8       ; MMU Program Control and ID Registers
    move.l #>$fff06100,r7       ; MMU Data Control and ID Registers
    move.l #>1,d5

	[
	    move.l (r3),d4          ; d4 = system context program segment mask
        move.l (r2),d3          ; d3 = system context data segment mask
    ]


    if @def('SC3X50')
    ; Configure the MMU Program attributes
    [
        movep.l d4,(r8+0)       ; Program segments
        movep.l d5,(r8+4)       ; PID
    ]

    ; Configure the MMU Data attributes
    [
        movep.l d3,(r7+0)       ; Data segments
        movep.l d5,(r7+4)       ; DID
    ]

    else ; SC3X50
    ; Configure the MMU Program attributes
    [
        move.l d3,(r7)+             ; Data segments
        move.l d4,(r8)+             ; Program segments
    ]

    ; Configure the MMU Data attributes
    [
        move.l d5,(r8)          ; PID
        move.l d5,(r7)          ; DID
    ]

    endif ; SC3X50

    syncio

    endif ; MMU_SUPPORT


    ei                              ; Allow nested interrupts.  If
                                    ;  we got here, interrupts were
                                    ;  enabled when we came into the
                                    ;  ISR.


	if @def('LOG_SUPPORT')
	push r1
	push d0
    push r0


	[
		move.l  r1,d1
		move.l #$01001001,d0
	]

	jsr _osLog

	pop r0
	pop d0

    jsr r0                          ; Perform ISR (with Kernel Awareness)

	pop r1
	nop
	[
		move.l  r1,d1
		move.l #$02001002,d0
	]
	jsr _osLog

	else ;not LOG_SUPPORT
	jsr r0                         ; Perform ISR (without Kernel Awareness)
	endif ; LOG_SUPPORT

    di                              ; Disable all interrupts again.
    falign

osHwiNoDispatch

    ; // See if stack needs adjustment.
    ; g_os_interrupt_count--;
    sub #1,d6                       ; d6 = g_os_interrupt_count
                                    ;  from above.  It should
                                    ;  be preserved across the
                                    ;  interrupt handler call.

    [
        tsteq  d6
        move.w d6,(r6)              ; r6 = &g_os_interrupt_count
                                    ;  from above.
    ]


    ; // If count is zero need to context switch back to the normal
    ; //  stack.  Otherwise, a low priority int is waiting so we will
    ; //  context switch out of this interrupt (staying on exception
    ; //  stack).  The restore will lower the interrupt priority mask
    ; //  and kick off the pending ints.

    ; if(g_os_interrupt_count!=0)
    ;     Restore_Context()


	bt osHwiDispatch_SWI            ; We are not nested.

 falign

_osHwiRestoreContext:

                                    ; Some flow comes here with int
                                    ;  disabled.  Restoring SR
                                    ;  will put interrupts enable to
                                    ;  original setting.
	if @def('MMU_SUPPORT') ;Restore MMU context

    move.l #>$fff07000,r1           ; MMU Program Control and ID Registers
    move.l #>$fff06100,r3           ; MMU Data Control and ID Registers

	pop.2l d4:d5       pop.2l d2:d3

    ; d2 = current data ID
    ; d3 = current enabled data segments
    ; d4 = current program ID
    ; d5 = current enabled prog segments

	if @def('SC3X50')

    [
        movep.l d5,(r1+0)
        movep.l d4,(r1+4)
    ]

    [
        movep.l d3,(r3+0)
        movep.l d2,(r3+4)
    ]

    else    ; SC3X50

    [
	    move.l d5,(r1)+
	    move.l d3,(r3)+
    ]
    [
        move.l   d4,(r1)
        move.l   d2,(r3)
    ]

	endif  ; SC3X50

    syncio

    endif   ; MMU_SUPPORT

	pop.2l 	  btr0:btr1
    pop.2l    d14:d15  pop.2l    d12:d13
    pop.2l    d10:d11  pop.2l    d8:d9
    pop.2l    d6:d7    pop.2l    d4:d5

    pop.2l 	  gcr:mctl pop.2l    d2:d3

    pop.2l    d12.e:d13.e:d14.e:d15.e pop.2l  d8.e:d9.e:d10.e:d11.e
    pop.2l    d4.e:d5.e:d6.e:d7.e     pop 	  d2.e:d3.e

	pop.2l    sa3:lc3  pop.2l    sa2:lc2
	pop.2l    sa1:lc1  pop.2l    sa0:lc0

	pop.2l    m2:m3    pop.2l    m0:m1
	pop.2l    n2:n3    pop.2l    n0:n1

    pop.2l    b6:b7    pop.2l    b4:b5
    pop.2l    b2:b3    pop.2l    b0:b1
    pop.2l    r6:r7    pop.2l    r4:r5
    pop.2l    r2:r3    pop.2l    r0:r1

    pop.2l d0:d1
    pop    d0.e:d1.e

    ; RTE will restore status register and thus the correct
    ;  priority.
    rte

_osHwiRestoreContextToNormal:
                                    ; Some flow comes here with int
                                    ;  disabled.  Restoring SR
                                    ;  will put interrupts enable to
                                    ;  original setting.

	if @def('MMU_SUPPORT')          ; Restore MMU context

    move.l #>$fff07000,r1           ; MMU Program Control and ID Registers
    move.l #>$fff06100,r3           ; MMU Data Control and ID Registers

	popn.2l  d4:d5  popn.2l d2:d3
    ; d2 = current data ID
    ; d3 = current enabled data segments
    ; d4 = current program ID
    ; d5 = current enabled prog segments

    if @def('SC3X50')

    [
        movep.l d5,(r1+0)
        movep.l d4,(r1+4)
    ]

    [
        movep.l d3,(r3+0)
        movep.l d2,(r3+4)
    ]

    else    ; SC3X50

	[
	    move.l d5,(r1)+
	    move.l d3,(r3)+
	]
    [
        move.l d4,(r1)
        move.l d2,(r3)
    ]

    endif  ; SC3X50

    syncio

    endif   ; MMU_SUPPORT
	popn.2l    btr0:btr1
    popn.2l    d14:d15  popn.2l    d12:d13
    popn.2l    d10:d11  popn.2l    d8:d9
    popn.2l    d6:d7    popn.2l    d4:d5

    popn.2l    gcr:mctl popn.2l    d2:d3

    popn.2l    d12.e:d13.e:d14.e:d15.e popn.2l    d8.e:d9.e:d10.e:d11.e
    popn.2l    d4.e:d5.e:d6.e:d7.e     popn 	  d2.e:d3.e

	popn.2l    sa3:lc3  popn.2l    sa2:lc2
	popn.2l    sa1:lc1  popn.2l    sa0:lc0

	popn.2l    m2:m3    popn.2l    m0:m1
	popn.2l    n2:n3    popn.2l    n0:n1

    popn.2l    b6:b7    popn.2l    b4:b5
    popn.2l    b2:b3    popn.2l    b0:b1
    popn.2l    r6:r7    popn.2l    r4:r5
    popn.2l    r2:r3    popn.2l    r0:r1

    pop.2l d0:d1
    pop    d0.e:d1.e

    ; RTERI will restore status register and thus the correct
    ;  priority + RAS is invalidated
    rteri


	falign
osHwiDispatch_SWI:

    ; // We are not nested so time to finalize.  While we are on the interrupt stack
    ; // we need to see if there are any SWI to handle.  If we came into the HWI
    ; // while servicing an SWI, we only need to see if there are any higher
    ; // priority SWI to handle.  The lower priority SWI will be
    ; // handled after the interrupted SWI is complete.
    ; if(g_os_flags & (OS_FLG_SWI_REQ|OS_FLG_SWI_ENABLE))
    ; {
    ;     check_priority = g_swi_priority_level;
	
    move.l #_g_os_flags,r6
    bmclr.w  #OS_FLG_HWI_ACTIVE,(r6) ; clear HWI flag in 
    move.w _g_swi_priority_level,d0
    [
        move.w (r6),d1
        move.w (r6),d2
    ]
    and #OS_FLG_SWI_ENABLE,d1.l
    [
        tsteq d1
        tfr d2,d1
        move.l #_g_active_swi_ptr,r7
    ]
    [
        and #OS_FLG_SWI_ACTIVE,d2.l
        ifa move.l (r7),r2
    ]
    iff and #OS_FLG_SWI_REQ,d1.l
    [
        iff tsteq d1
        ifa move.l (r7),r0
    ]
    btd osHwiReturnStack
    [
        tstgt d2
        adda #OS_SWI_STRUCT_PRIORITY_OFFSET,r2
    ]

    ;     if((g_os_flags & OS_FLG_SWI_ACTIVE)
    ;          && (SWI_Data[g_active_swi].priority < check_priority))
    ;     {
    ;         check_priority = SWI_Data[g_active_swi].priority;
    ;     }
    ;     *(sp)++ = g_active_swi_ptr;
    ;     *(sp)++ =
    ;          ((g_os_flags & OS_FLG_SWI_ACTIVE) ?
    ;          SWI_Data[g_active_swi].priority:OS_SWI_PRIORITY_ALL);

    move.w #OS_SWI_PRIORITY_ALL,d1

    ift move.w (r2),d1
    ift cmpgt d1,d0

    [
        push r0
        push d1
    ]
    iff tfr d0,d1

osHwiFind_SWI
    ;     while((active_swi = Search_For_New_Task(READY_SWI, check_priority))!=NO_TASK)
    ;     {

    jsrd _osReadyListPop
    move.l #_swi_ready,r0

    [
        eor #NO_TASK,d0.l
        tfr d0,d1
    ]
    [
        tsteq d0
        move.l #OS_SWI_Return_Point,r0
    ]
    bt osHwi_No_SWI

    ;         g_os_flags |= OS_FLG_SWI_ACTIVE;
    ;         *(sp++) = OS_SWI_Return_Point  // Setup return point for SWI return.
    ;         sp++;
    [
        bmset.w #OS_FLG_SWI_ACTIVE,(r6)
        push r0
    ]

	if @def('LOG_SUPPORT')
	push r0		push r1
	push r2		push r3
    push d2     push d1

	move.l   >_g_int_disable_counter,d0
	add      #<$1,d0
	move.l   d0,>_g_int_disable_counter

	[
		move.l #$01002002,d0
		tfr  d1,d5
	]

	jsr _osLog

	move.l   >_g_int_disable_counter,d0
	sub      #<$1,d0
	move.l   d0,>_g_int_disable_counter


    pop d2      pop d1
	pop r2		pop r3
	pop r0		pop r1
	endif ;LOG_SUPPORT


    ;         g_active_ptr = &(swi_data[active_swi]);
    ;         *(sp++) = SWI_DATA.function;      // We will act like we are at the end of
    ;         *(sp++) = HWI_sr;                 //  of the interrupt.  This will enable
    ;                                           //  interrupts in the SWI.

    impy.w #OS_SWI_STRUCT_OFFSET,d1
    move.l _swi_data,d0
	add d0,d1,d0
    [
        move.l d0,r0
        move.l d0,(r7)
        tfr d7,d1
    ]
    move.w #OS_RESOURCE_USED,d2
    [
        move.l (r0),d0
        tfra r0,r2
    ]
    [
        push d0
        push d7
    ]

    if @def('MMU_SUPPORT') ; Set MMU context for SWI

	syncio

    ; This whole flow assumes that the system segments are enabled
    ; for all PID/DID and thus it is safe to switch the PID/DID and
    ; segments thereof without concern of order
    move.l #>$fff06100,r1

    if @def('SC3X50')
    [
	    movep.l (r0+OS_SWI_STRUCT_MMU_DATA),r3
	    movep.l (r0+OS_SWI_STRUCT_MMU_PROG),r4
    ]

    [
        movep.l (r3+4),d3            ; DID -> d3
        movep.l (r4+4),d1            ; PID -> d1
    ]
    [
        movep.l d3,(r1+4)            ; set DID: d3 -> fff06104
        movep.l d1,(r1+$F04)         ; set PID: d3 -> fff07004
    ]
    [
        move.l (r3),d3              ; active data segments -> d3
        move.l (r4),d1              ; active program segments -> d1
    ]

    [
        move.l d3,(r1)              ; set active data segments: d3 -> fff06100
        movep.l d1,(r1+$F00)         ; set active program segments: d1 -> fff07000
    ]
    else    ; @def('SC3X50')
    [
        move.l (r0+OS_SWI_STRUCT_MMU_DATA),r3
        move.l (r0+OS_SWI_STRUCT_MMU_PROG),r4
    ]

    [
        move.l (r3+4),d3            ; DID -> d3
        move.l (r4+4),d1            ; PID -> d1
    ]
    [
        move.l d3,(r1+4)            ; set DID: d3 -> fff06104
        move.l d1,(r1+$F04)         ; set PID: d3 -> fff07004
    ]
    [
        move.l (r3),d3              ; active data segments -> d3
        move.l (r4),d1              ; active program segments -> d1
    ]

    [
        move.l d3,(r1)              ; set active data segments: d3 -> fff06100
        move.l d1,(r1+$F00)         ; set active program segments: d1 -> fff07000
    ]
    endif   ; @def('SC3X50')

    syncio

    endif



    ;         g_active_ptr->status = OS_RESOURCE_USED;
    ;         // Put count into d0 as part of C calling conventions.
    ;         RTE();
	[
        move.w d2,(r2+OS_SWI_STRUCT_STATUS_OFFSET)
        move.l (r0+OS_SWI_STRUCT_COUNT_OFFSET),d0
    ]
    ; RTERID will invalidate the RAS. In 3850, this is important in order to ensure that
    ; the return from ther swi handler won't be according to the RAS. For 3400 this is
    ; irrelevant since the RAS was effectivly invalidated by the call to osReadyListPop
    rterid
	zxt.l d0


OS_SWI_Return_Point:
    ; ReturnPoint:
    ;         DI();
    ;         check_priority = g_swi_priority_level;
    ;         if((g_os_flags & OS_FLG_SWI_ACTIVE)
    ;              && (SWI_Data[g_active_swi].priority < check_priority))
    ;         {
    ;             check_priority = SWI_Data[g_active_swi].priority;
    ;         }
    di

	if @def('LOG_SUPPORT')

    ; Add counter to insure that osLog does not enable interrupts
	move.l   >_g_int_disable_counter,d0
	add      #<$1,d0
	move.l   d0,>_g_int_disable_counter


	[
	    move.l #$02002003,d0
	    tfr  d6,d1
	]
	jsr _osLog

	move.l   >_g_int_disable_counter,d0
	sub      #<$1,d0
	move.l   d0,>_g_int_disable_counter

	endif   ;LOG_SUPPORT


    move.l (SP-4),d1                ; d1=SWI_Data[g_active_swi].priority

    [
        sxt.l d1
        move.w _g_swi_priority_level,d0 ; d0=check_priority
    ]
    cmpgt d1,d0
    iff tfr d0,d1

;     }
    bra osHwiFind_SWI

osHwi_No_SWI:
    ;     --sp;
    ;     g_active_swi = *(--sp);
    pop d0

    ;     if(g_active_swi == 0)
    ;         g_os_flags &= ~(OS_FLG_SWI_ACTIVE|OS_FLG_SWI_REQ);
    tsteq d0
    move.l d0,(r7)
    ift bmclr.w #(OS_FLG_SWI_ACTIVE|OS_FLG_SWI_REQ),(r6)


; }

osHwiReturnStack
    ; // We are not nested.  Need to see if we should switch SP.  SP
    ; // gets switched only if we are not in an SWI and a Task is
    ; // active.
    ; if(!(g_os_flags & (OS_FLG_SWI_ACTIVE|OS_FLG_BGD_ACTIVE)))
    ; {
    ;     interrupt_stack = SP;
    ;     SP = g_temp_stack_holder;
    ; }

    ; at this point r6 = &g_os_flags
    ; move.l #_g_os_flags,r6        ; Needed below:
                                    ;  r1 = &g_os_flags

    ;move.w (r6),d0                 ; Needed below:
                                    ;  d0 = g_os_flags

    ; // Turn off OS_FLG_HWI_ACTIVE flag.
    ; g_os_flags ^= OS_FLG_HWI_ACTIVE;

    bmtsts.w #OS_FLG_SWI_ACTIVE,(r6)
    bmclr.w  #OS_FLG_HWI_ACTIVE,(r6)

    bt _osHwiRestoreContext


    ; // Get out of here if we don't have to do a SWI switch or task
    ; //  switch.
    ; if((g_os_flags & OS_FLG_TSK_REQ) != 0)
    ;     Restore_Context()
    bmtsts.w #OS_FLG_TSK_REQ,(r6)
    bf _osHwiRestoreContextToNormal


; TASK SWITCH DISPATCHER CALL GOES HERE

_osHwiContextSwitch:


	; move.l #_g_os_flags,r6                         ; from osHwiDispatch_SWI
                                                     ; r6 = &g_os_flags


; TASK SWITCH DISPATCHER CALL GOES HERE

	if @def('LOG_SUPPORT')
	push r0		push r1
	push r2		push r3
    push d2     push d1

    move.l (_g_highest_task),r1
	move.l   >_g_int_disable_counter,d0


	adda     #>$54,r1,r1
	add      #<$1,d0

	move.l   d0,>_g_int_disable_counter
	move.l (_g_running_task),r2
	[
		move.l #$00005002,d0
		move.l  (r1),d1
	]
	adda     #>$54,r2,r2
	[
		move.l  (r2),d2
		aslw     d1,d1
	]
	or d2,d1

	jsr _osLog

	move.l   >_g_int_disable_counter,d0
	sub      #<$1,d0
	move.l   d0,>_g_int_disable_counter

    pop d2      pop d1
	pop r2		pop r3
	pop r0		pop r1

	endif ;LOG_SUPPORT

	if @def('DEBUG_HOOKS')
	move.l   >_g_int_disable_counter,d0
	add      #<$1,d0
	move.l   d0,>_g_int_disable_counter

	jsr _taskSwitchDebugHandler

	move.l   >_g_int_disable_counter,d0
	sub      #<$1,d0
	move.l   d0,>_g_int_disable_counter
	endif

	move.l (_g_running_task),r1        ;save SP

	[
		bmclr.w #OS_FLG_TSK_REQ,(r6)   ; clear task request
		tfra osp,r2                    ; move task's stack pointer
	]                                  ; in order to save it

	[
		move.l r2,(r1)                ;save task's stack pointer
		tfra sp,r3
	]
	[
		adda #OS_TASK_STRUCT_STATUS_OFFSET,r1
		suba #24,r3
	]
	[
		bmclr.w #OS_TASK_RUNNING,(r1) ; clear current running
		adda #OS_TASK_STRUCT_SR_OFFSET,r1
	]

	move.2l (r3)+,d0:d1  ; SR and PC

	[
		move.2l d0:d1,(r1)+
		move.2l (r3)+,d0:d1  ; D0e_D1e
	]
	[
		move.2l d0:d1,(r1)+
		move.2l (r3),d0:d1   ; D0,  D1
	]


    [
    	move.2l d0:d1,(r1)
        move.l (_g_highest_task),r1  ;sp = _g_highest_task
    ]

	[
	    tfra sp,r3
    ]

	move.l (r1),r2
	[
		move.l r1,_g_running_task
		suba #24,r3
	]
	[
		adda #OS_TASK_STRUCT_STATUS_OFFSET,r1
		tfra r2,osp
	]
	[
		bmset.w #OS_TASK_RUNNING,(r1) ; set new running
		adda #OS_TASK_STRUCT_SR_OFFSET,r1
	]

	move.2l (r1)+,d0:d1  ; SR and PC
	[
		move.2l d0:d1,(r3)+
		move.2l (r1)+,d0:d1  ; D0e_D1e
	]
	[
		move.2l d0:d1,(r3)+
		move.2l (r1),d0:d1   ; D0,  D1
	]

	[
		move.2l d0:d1,(r3)
		bra _osHwiRestoreContextToNormal
	]


F_osHwiDispatcher_end

; // And away we go...


    ALIGN 8

;*************************************************************************
;
;     Function:           osHwiPreciseDispatcher
;
;     Inputs:             None
;
;     Outputs:            None
;
;     Description:
;
;     Cautions:
;
;*************************************************************************
; R0 AND R1 CANNOT BE USED. THEY CONTAIN PASSED IN COMPONENTS
; FOR THE SWI INTERRUPT


_osHwiPreciseDispatcher type func

    push.2l    r0:r1    push.2l    r2:r3   ; 3,4
    push.2l    r4:r5    push.2l    r6:r7   ; 5,6
    push.2l    b0:b1    push.2l    b2:b3   ; 7,8
    push.2l    b4:b5    push.2l    b6:b7   ; 9,10

	push.2l    n0:n1    push.2l    n2:n3   ; 11,12
	push.2l    m0:m1    push.2l    m2:m3   ; 13,14

	push.2l    sa0:lc0  push.2l    sa1:lc1 ; 15,16
	push.2l    sa2:lc2  push.2l    sa3:lc3 ; 17,18

    push       d2.e:d3.e              push.2l d4.e:d5.e:d6.e:d7.e     ; 19, 20
    push.2l    d8.e:d9.e:d10.e:d11.e  push.2l d12.e:d13.e:d14.e:d15.e ; 21, 22

    push.2l    d2:d3    push.2l    gcr:mctl				      ; 26

    push.2l    d4:d5    push.2l    d6:d7      ; 27,28
    push.2l    d8:d9    push.2l    d10:d11    ; 29,30
    push.2l    d12:d13  push.2l    d14:d15    ; 31,32
	push.2l	   btr0:btr1  ; 33,34

    bmset     #$c,SR.L			; rounding and saturation

    [
	    move.l    #0,mctl
	    zxt.w d0,d0
	]

    move.l #_g_os_interrupt_count,r6            ;  Needed below
                                                ;  r6 = &g_os_interrupt_count

    [
        move.l #_interrupt_pointer,r5           ; Needed below
                                                ;  r5 = &interrupt_pointer
        move.l d0,r1                            ; r1 = exception number, needed
    ]                                           ;  below for interrupt lookup.

    [
      move.w (r6),d6                             ; Needed below:
                                                 ;  d6 = g_os_interrupt_count
      move.l #_interrupt_argument,r7
    ]

    ; g_os_interrupt_count++;
    [
        add #1,d6
        addl2a r1,r5                             ; interrup_pointer[index]
    ]

    addl2a r1,r7                                 ;  calculation.
    [
	    tfr d1,d7                                ; Save return ILP over the ISR
	    move.l (r7),d0                           ;  call.
    ]

    [
        move.w d6,(r6)                           ; g_os_interrupt_count store
                                                 ;  from above.
        move.l (r5),r0                           ; Read interrupt_pointer[index]
    ]

    move.l   >_g_int_disable_counter,d2              ; don't allow nested interrupts.  If
    add      #<$1,d2                                 ;  we got here, interrupts were
    move.l   d2,>_g_int_disable_counter              ;  enabled when we came into the
                                                     ;  ISR.

    push r1
	if @def('LOG_SUPPORT')
	push r1
	push d0
    push r0


	[
	move.l  r1,d1
	move.l #$01001001,d0
	]

	jsr _osLog



	pop r0
	pop d0




    jsr r0


	pop r1
	nop
	[
	move.l  r1,d1
	move.l #$02001002,d0
	]
	jsr _osLog
	else ;not LOG_SUPPORT
	jsr r0
	endif ; LOG_SUPPORT

    ; back from the exception
    di
    move.l   >_g_int_disable_counter,d2              ; don't allow nested interrupts.  If
    sub      #<$1,d2                                 ;  we got here, interrupts were
    move.l   d2,>_g_int_disable_counter              ;  enabled when we came into the
                                                     ;  ISR.

    ; g_os_interrupt_count--;
    sub #1,d6                                        ; d6 = g_os_interrupt_count
                                                     ;  from above.  It should
                                                     ;  be preserved across the
                                                     ;  interrupt handler call.
    move.w d6,(r6)                                 ; r6 = &g_os_interrupt_count
                                                     ;  from above.
                                                     ; d7 = return SR with IPL
	pop r1
    jsrd _osHwiIsPreciseException                       ; Is regular NMI or precise
    move.l r1,d0
    tsteq d0
	pop.2l    btr0:btr1
    pop.2l    d14:d15  pop.2l    d12:d13
    pop.2l    d10:d11  pop.2l    d8:d9
    pop.2l    d6:d7    pop.2l    d4:d5

    pop.2l	  gcr:mctl pop.2l    d2:d3

    pop.2l    d12.e:d13.e:d14.e:d15.e pop.2l    d8.e:d9.e:d10.e:d11.e
    pop.2l    d4.e:d5.e:d6.e:d7.e     pop 	  d2.e:d3.e

	pop.2l    sa3:lc3  pop.2l    sa2:lc2
	pop.2l    sa1:lc1  pop.2l    sa0:lc0

	pop.2l    m2:m3    pop.2l    m0:m1
	pop.2l    n2:n3    pop.2l    n0:n1

    pop.2l    b6:b7    pop.2l    b4:b5
    pop.2l    b2:b3    pop.2l    b0:b1																																																																																																												
    pop.2l    r6:r7    pop.2l    r4:r5
    pop.2l    r2:r3    pop.2l    r0:r1

    pop.2l d0:d1
    pop    d0.e:d1.e

; RTPE will restore status register and thus the correct
;  priority. If regular NMI, will call RTE
    ift rte
    rtpe

F_osHwiPreciseDispatcher_end

_osFirstTimeSwitch type	func


	move.l  #_StackStart+24,r0       ; initialize the stack pointer
	move.l  #_g_os_flags,r6          ; r6 = &g_os_flags from osHwiDispatch_SWI
    tfra    r0,sp
    bra _osHwiContextSwitch

F_osFirstTimeSwitch_end


 endsec

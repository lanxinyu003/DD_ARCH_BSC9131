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
; $Date: 2010/03/21 12:48:15 $
; $Id: starcore_sync_asm_log.asm,v 1.14 2010/03/21 12:48:15 b06368 Exp $
; $Source: /cvsdata/SmartDSP/source/arch/starcore/common/starcore_sync_asm_log.asm,v $
; $Revision: 1.14 $
; **************************************************************************//*
;
; @File          starcore_sync_asm_log.asm
;
; @Description   Starcore-specific synchronization module assembly functions,
;                with logging features.
;
; @Cautions      None.
;
;*//***************************************************************************/


	section .oskernel_text_run_time_critical local
	if @def('MMU_SUPPORT')
	SECTYPE overlay
    endif


	global	_osSpinLockTryGet
	global  _osSpinLockSwiftTryGet
_osSpinLockTryGet type	func

	if @def('SC3X00')||@def('SC3X50')
	bmtsts.w #$FF00,(r0)                      ; Check if guard is disabled
	ift sub d0,d0,d0 rts
	endif

_osSpinLockSwiftTryGet type  func
	bmtset.w    #1,(r0)                     ; Try to apply atomic operation
	ift sub     d0,d0,d0                    ; on address received in r0
	rtsd
	iff move.w  #<1,d0                      ; Return result
F_osSpinLockTryGet_end
F_osSpinLockSwiftTryGet_end


    global  _osSpinLockGet
_osSpinLockGet type func
    push r0
    [
    move.l #$3001,d0
    move.l  r0,d1
    ]
    jsr _osLog
    pop r0
    nop

    if @def('SC3X00')||@def('SC3X50')
    bmtsts.w #$FF00,(r0)                      ; Check if guard is disabled
    ift rts
    endif

_osSpinLockGet_loop
    bmtset.w    #1,(r0)                    ; Apply atomic operation
                                           ; on address received in r0
    iff rts
    bmtsts.w    #1,(r0)                       ; spin (non atomically) untill free
    bt          <*-$4
    bra         _osSpinLockGet_loop
F_osSpinLockGet_end


    global  _osSpinLockSwiftGet
_osSpinLockSwiftGet type func
    push r0
    [
    move.l #$3001,d0
    move.l  r0,d1
    ]
    jsr _osLog
    pop r0
    nop

_osSpinLockSwiftGet_loop
    bmtset.w    #1,(r0)                    ; Apply atomic operation
                                           ; on address received in r0
    iff rts
    bmtsts.w #1,(r0)                       ; spin (non atomically) untill free
    bt          <*-$4
    bra         _osSpinLockSwiftGet_loop
F_osSpinLockSwiftGet_end


	global	_osSpinLockRelease
_osSpinLockRelease type	func
	push r0
	[
    move.l #$3002,d0
    move.l  r0,d1
	]
	jsr _osLog
	pop r0
	nop

	if @def('SC3X00')||@def('SC3X50')
	sub d0,d0,d0 adda #1,r0
	move.b      d0,(r0)
	else
	move.w      #0,(r0)
	endif

	rts
F_osSpinLockRelease_end


    global  _osSpinLockIrqGet
_osSpinLockIrqGet type  func
    di
    move.l      >_g_int_disable_counter,d0  ; to check g_int_disable_counter
    add         #<$1,d0                     ; at release functions.
    move.l      d0,>_g_int_disable_counter
    push r0
    [
    move.l #$3003,d0
    move.l  r0,d1
    ]
    jsr _osLog
    pop r0
    nop

    if @def('SC3X00')||@def('SC3X50')
    bmtsts.w #$FF00,(r0)                      ; Check if guard is disabled
    ift rts
    endif

_osSpinLockIrqGet_loop
    bmtset.w    #1,(r0)                    ; Apply atomic operation
                                           ; on address received in r0
    iff rts
    bmtsts.w #1,(r0)                       ; spin (non atomically) untill free
    bt          <*-$4
    bra         _osSpinLockIrqGet_loop
F_osSpinLockIrqGet_end


    global  _osSpinLockSwiftIrqGet
_osSpinLockSwiftIrqGet type  func
    di
    move.l      >_g_int_disable_counter,d0  ; to check g_int_disable_counter
    add         #<$1,d0                     ; at release functions.
    move.l      d0,>_g_int_disable_counter
    push r0
    [
    move.l #$3003,d0
    move.l  r0,d1
    ]
    jsr _osLog
    pop r0
    nop

_osSpinLockSwiftIrqGet_loop
    bmtset.w    #1,(r0)                    ; Apply atomic operation
                                           ; on address received in r0
    iff rts
    bmtsts.w #1,(r0)                       ; spin (non atomically) untill free
    bt          <*-$4
    bra         _osSpinLockSwiftIrqGet_loop
F_osSpinLockSwiftIrqGet_end


	global	_osSpinLockIrqRelease
_osSpinLockIrqRelease type	func

	if @def('SC3X00')||@def('SC3X50')
	sub d0,d0,d0 adda #1,r0
	move.b      d0,(r0)
	else
	move.w      #0,(r0)
	endif

	[                                       ; when interrupts are disabled
    move.l #$3004,d0
    move.l  r0,d1
	]
	jsr _osLog
	move.l      >_g_int_disable_counter,d0
	sub         #<$1,d0
	tsteq       d0 & move.l d0,>_g_int_disable_counter
    bf          <*+$4
	ei
	rts
F_osSpinLockIrqRelease_end

	endsec



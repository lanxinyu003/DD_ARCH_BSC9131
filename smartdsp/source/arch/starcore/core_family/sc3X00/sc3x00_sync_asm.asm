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
; $Date: 2013/04/25 13:09:48 $
; $Id: sc3x00_sync_asm.asm,v 1.4 2013/04/25 13:09:48 b41932 Exp $
; $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_sync_asm.asm,v $
; $Revision: 1.4 $
; **************************************************************************//*
;
; @File          starcore_sync_asm.asm
;
; @Description   Starcore-specific synchronization module assembly functions.
;
; @Cautions      None.
;
;*//***************************************************************************/

; Keep in sync with os_multicore_sync.h
OS_GUARD_DISABLE	equ	 $FF00

	section .oskernel_text_run_time_critical local
	if @def('MMU_SUPPORT')
	SECTYPE overlay
    endif

    global  _osSpinLockTryGet
    global  _osSpinLockSwiftTryGet
_osSpinLockTryGet type	func

	if @def('SC3X00')||@def('SC3X50')
	bmtsts.w #OS_GUARD_DISABLE,(r0)                   ; Check if guard is disabled
	ift sub d0,d0,d0 rts
	endif

_osSpinLockSwiftTryGet type  func
	bmtset.w    #1,(r0)                    ; Try to apply atomic operation
	ift sub     d0,d0,d0                   ; on address received in r0
	rtsd
	iff move.w  #<1,d0                     ; Return result
	SIZE _osSpinLockTryGet,(*-_osSpinLockTryGet)
F_osSpinLockTryGet_end
	SIZE _osSpinLockSwiftTryGet,(*-_osSpinLockSwiftTryGet)
F_osSpinLockSwiftTryGet_end


	global	_osSpinLockGet
    global  _osSpinLockSwiftGet
_osSpinLockGet type	func

	if @def('SC3X00')||@def('SC3X50')
	bmtsts.w #OS_GUARD_DISABLE,(r0)                   ; Check if guard is disabled
	ift rts
	endif

_osSpinLockSwiftGet type  func
_osSpinLockGet_loop
	bmtset.w    #1,(r0)                    ; Apply atomic operation
                                           ; on address received in r0
    iff rts
    bmtsts.w    #1,(r0)                       ; spin (non atomically) untill free
    bt          <*-$4
	bra         _osSpinLockGet_loop
	SIZE _osSpinLockGet,(*-_osSpinLockGet)
F_osSpinLockGet_end
	SIZE _osSpinLockSwiftGet,(*-_osSpinLockSwiftGet)
F_osSpinLockSwiftGet_end


	global	_osSpinLockRelease
_osSpinLockRelease type	func

	if @def('SC3X00')||@def('SC3X50')
	sub d0,d0,d0 adda #1,r0
	move.b      d0,(r0)
	else
	move.w      #0,(r0)
	endif

	rts
	SIZE _osSpinLockRelease,(*-_osSpinLockRelease)
F_osSpinLockRelease_end


    global  _osSpinLockIrqGet
_osSpinLockIrqGet type  func
    di
    move.l      >_g_int_disable_counter,d0 ; to check g_int_disable_counter
    add         #<$1,d0                    ; at release functions.
    move.l      d0,>_g_int_disable_counter

    if @def('SC3X00')||@def('SC3X50')
    bmtsts.w #OS_GUARD_DISABLE,(r0)                   ; Check if guard is disabled
    ift rts
    endif

_osSpinLockIrqGet_loop
    bmtset.w    #1,(r0)                    ; Apply atomic operation
                                           ; on address received in r0
    iff rts
    bmtsts.w    #1,(r0)                       ; spin (non atomically) untill free
    bt          <*-$4
    bra         _osSpinLockIrqGet_loop
    	SIZE _osSpinLockIrqGet,(*-_osSpinLockIrqGet)
F_osSpinLockIrqGet_end


    global  _osSpinLockSwiftIrqGet
_osSpinLockSwiftIrqGet type  func
    di
    move.l      >_g_int_disable_counter,d0 ; to check g_int_disable_counter
    add         #<$1,d0                    ; at release functions.
    move.l      d0,>_g_int_disable_counter

_osSpinLockSwiftIrqGet_loop
    bmtset.w    #1,(r0)                    ; Apply atomic operation
                                           ; on address received in r0
    iff rts
    bmtsts.w    #1,(r0)                       ; spin (non atomically) untill free
    bt          <*-$4
    bra         _osSpinLockSwiftIrqGet_loop
        	SIZE _osSpinLockSwiftIrqGet,(*-_osSpinLockSwiftIrqGet)
F_osSpinLockSwiftIrqGet_end


	global	_osSpinLockIrqRelease
_osSpinLockIrqRelease type	func			; This function has to be called

	if @def('SC3X00')||@def('SC3X50')
	sub d0,d0,d0 adda #1,r0
	move.b      d0,(r0)
	else
	move.w      #0,(r0)
	endif

	move.l      >_g_int_disable_counter,d0
	sub         #<$1,d0
	tsteq       d0 & move.l d0,>_g_int_disable_counter
    bf          <*+$4
	ei
	rts
	    	SIZE _osSpinLockIrqRelease,(*-_osSpinLockIrqRelease)
F_osSpinLockIrqRelease_end

	endsec



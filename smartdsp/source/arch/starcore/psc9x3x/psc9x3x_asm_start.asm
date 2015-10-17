;/******************************************************************************
; Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
; All Rights Reserved
; 
; This is proprietary source code of Freescale Semiconductor Inc., and its use
; is subject to the CodeWarrior EULA.  The copyright notice above does not 
; evidence any actual or intended publication of such source code.
;  
;*******************************************************************************/
;*******************************************************************************
; $Date: 2011/06/01 09:02:34 $
; $Id: psc9x3x_asm_start.asm,v 1.2 2011/06/01 09:02:34 b17975 Exp $
; $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_asm_start.asm,v $
; $Revision: 1.2 $
;*******************************************************************************

	section  .oskernel_text_initialization
	sectype  overlay

	align 4
	global ___target_asm_start
___target_asm_start type func
    SIZE ___target_asm_start,F___target_asm_start_end-___target_asm_start

          ; get current core id
    move.l #>$FFF06028,r0
    move.l #>$00FF0000,d1
    move.l (r0),d0
          ; load in d0 the size of a table entry: each core uses (4x4) bytes of data
    and d0,d1  move.l #>16,d0
    lsrw     d1,d2

          ; compute the offset in the table to get the specific data for
          ; MMU initialization

		  ; d14 will hold core number with an offset
    impyuu d2,d0,d14
    move.l #>__asm__LocalData_c0_b,r1
    move.l d14,r0
          ; r0 will point to the start of core's MMU initialization data
    adda r1,r0

          ; d3 <= __asm__LocalData_cX_size
    move.l (r0+4),d3
          ; d0 <= __asm__LocalData_cX_b & d1<= __asm__LocalData_cX_size
    move.l (r0),d0        tfr d3,d1
          ; d1 <= __asm__LocalData_cX_size>>1
    move.l #>((SYSTEM_DATA_MMU_DEF&$0000003F)|$1),d2  lsr d1
          ; d0 <= __asm__LocalData_cX_b | ((SYSTEM_DATA_MMU_DEF&$0000003F)|$1)
    or d2,d0
          ; d0<= __asm__LocalData_cX_b | ((SYSTEM_DATA_MMU_DEF&$0000003F)|$1) | (__asm__LocalData_cX_size>>1)
          ; d2 <= __asm__LocalData_cX_Phys_b
          ; d1 <= __asm__LocalData_cX_FSM
                  move.l (r0+8),d2    move.l (r0+12),d1

	; Setup FSM is requested by user
    ; d3 register must contain the size of segment
    tsteq d1
    ift or d1,d0
    jt skip_FSM_setup

do_FSM_setup
    ; r1 <= M_DSDC0
    move.l #$FFF09008,r1

    ; d3 <= size_of_segment/256 (d3 is the DSS field of DSDC0 register)
    clr d1        asrr #8,d3
    ; set the DSPM bit in DSDC0 register
    bmset #$8000,d1.h
    or d3,d1
    move.l d1,(r1) syncio
	move.l #>((SYSTEM_DATA_MMU_DEF&$0000003F)|$1),d1
    move.l (r0),d0
    ;lsr d0
    or d1,d0

skip_FSM_setup
	      ; Initialize MMU according to Linker settings
          ; r0 <= address of M_DSDA0 (Data Segment Descriptor Registers A0)
	move.l #>$FFF09000,r0
	      ; next instruction avoids a nop
    tfr  d2,d1
          ; r0 <= address of M_DSDB0 (Data Segment Descriptor Registers B0)
	move.l d0,(r0)   move.l #>$FFF09004,r0

	move.l #>(((SYSTEM_DATA_MMU_DEF&$FFFF0000)>>$10)),d0
	;iff lsr d1
	or d1,d0
	move.l d0,(r0)
	
	; Enable translation, disable protection
	move.l #>$FFF06000,r0 
	nop	
	move.l   (r0),d0
	bmset    #$4,d0.l
	bmclr    #$8,d0.l
	move.l   d0,(r0) syncio
L2  		

	; check if the stack is mapped

	asrr #2,d14 ; adjust offset to core id * 4 byte
 [
	move.l #__asm_StackStart_c0,r0
	move.l d14,r1
 ]
	adda r1,r0
	move.l (r0),r0 ; r0 programmed to __Local_StackStart_cX
	move.l r0,d8   ; save the address of the stack of the correct core
 [
		dquery (r0)
		syncm
 ]

	move.l   #$FFF06108,r0 ;//Status register
	move.l   (r0),d0	   ;//status  -> d3
	and      #$70000,d0,d1 ;//check if fetch succeded
	tsteq.l  d1			   ;//check status register
	jt 		 stack_mapped
	
	; stack is not mapped, so we have to map it.
	; first, we have to find the entry of the stack in the mmu table.
	; d0 - virtual address of the stack of the specific core

	move.l d8,d0 
 [
    clr      d2                         
    tfr      d0,d1                     
    move.l   >__address_translation_table_mmu_count,d3
    suba     r1,r1                      
 ]
    tstgt    d3                   
    jf       L12                  
    FALIGN                       
L11
 [
    tfr      d1,d4                  
    move.l   #>__address_translation_table_mmu,r0 
 ]
    adda     r1,r0                
    moveu.w  (r0+18),d0          
    and      #<8,d0,d0            
    tsteq.l  d0                   
    jf       L13                 
 [
    move.l   (r0),d0                 
    movep.l  (r0+8),d5              
 ]
 [
    isub     d0,d4                   
    move.l   r0,d0                
 ]
    cmphi    d4,d5          
    jt       found_entry              
L13
 [
    inc      d2                  
    adda     #<20,r1                  
 ]
    cmpgt    d2,d3              
    jt       L11                 
L12
	debug ; we can't find the entry

	; now, map the segment.	
	; r0 is the pointer to the entry in the mmu table.
	; this snippet is the function mmuMapProgramSegment() with some modifications.
	; the stack will be in the second segment.

found_entry
 [
    clr      d5                        
    move.l   (r0),d0                 
 ]
    and      #255,d0,d0              
 [
    tsteq.l  d0                       
    movep.l  (r0+8),d0                 
 ]
 [
    isub     #<1,d0                  
    jf       L38                  
 ]
    movep.l  (r0+4),d1          
    and      #255,d1,d1      
    tsteq.l  d1                 
    jt       L40              
L38
	debug                      
L40
 [
    moveu.w  (r0+18),d3                
    movep.l  (r0+12),d4                
 ]
    and      #32,d3,d3          
 [
    tsteq.l  d3                        
    moveu.l  #16777217,d1             
 ]
 [
    or       d4,d1                      
    moveu.l  #8323072,d3               
 ]
    jtd      L42                 
    movep.l  d1,(r0+12)        
    movep.l  (r0+8),d0          
    zxt.l    d0                  
 [
    cmpgtd   d3,d0,d3                  
    cmpltd.w #256,d0,d4               
 ]
    or       d4,d3               
    tsteq.l  d3                   
    jt       L44               
 	debug                        
L44
    and      #255,d0,d3          
    tsteq.l  d3                   
    jt       L46                 
 debug                       
L46
 [
    and      #-268435456,d1,d5          
    lsrr.l   #<8,d0                     
    move.l   (r0),d4                    
 ]
 [
    or       d0,d5                      
    jmpd     L47                      
 ]
    bmset    #32768,d5.h          
L42
 [
    tfr      d0,d6                      
    movep.l  (r0+8),d3                  
 ]
    zxt.l    d3                  
 [
    and      d3,d6                     
    cmpltd.w #256,d3,d4                
 ]
    zxt.l    d6                  
    cmpned.w #<0,d6,d6            
    or       d4,d6               
    tsteq.l  d6                   
    jt       L49                  
	debug                        
L49
 [
    tfr      d0,d6                      
    move.l   (r0),d4                    
 ]
    and      d4,d6                
    tsteq.l  d6                   
    jf       L50                  
    movep.l  (r0+4),d6            
    and      d6,d0                
    tsteq.l  d0                   
    jt       L52                  
L50
	debug                        
L52
    lsr      d3                   
    or       d3,d4                
L47
 [
    and      #63,d1,d0                  
    extractu #<8,#<16,d1,d1             
    move.l   d5,d2                      
 ]
 [
    zxt.l    d2                         
    movep.l  (r0+4),d3                  
    move.l   d4,d4                      
 ]
 [
    or       d4,d0                      
    or       d3,d1                      
    move.l   #-1011672,r2    ;2nd segment in the data matt
 ]
 [
    zxt.l    d0                         
    zxt.l    d1                         
    move.l   #-1011676,r1     ;2nd segment 
 ]
    move.l   #-1011680,r0     ;2nd segment 
	move.l d0,(r0)               
	move.l d1,(r1)               
	move.l d2,(r2)               
	syncio                       

	
stack_mapped
	
	jmp ___default_asm_start
	
	Global F___target_asm_start_end
F___target_asm_start_end

    align 4
    Global _startup_symbols_tbl

_startup_symbols_tbl
; core 0
__asm__LocalData_c0_b      dcl __LNK_LocalData_c0_b
__asm__LocalData_c0_size   dcl __LNK_LocalData_c0_size
__asm__LocalData_c0_Phys_b dcl __LNK_LocalData_c0_Phys_b
__asm__LocalData_c0_FSM    dcl __LNK_LocalData_c0_FSM
; core 1
__asm__LocalData_c1_b      dcl __LNK_LocalData_c1_b
__asm__LocalData_c1_size   dcl __LNK_LocalData_c1_size
__asm__LocalData_c1_Phys_b dcl __LNK_LocalData_c1_Phys_b
__asm__LocalData_c1_FSM    dcl __LNK_LocalData_c1_FSM
; core 2
__asm__LocalData_c2_b      dcl __LNK_LocalData_c2_b
__asm__LocalData_c2_size   dcl __LNK_LocalData_c2_size
__asm__LocalData_c2_Phys_b dcl __LNK_LocalData_c2_Phys_b
__asm__LocalData_c2_FSM    dcl __LNK_LocalData_c2_FSM
; core 3
__asm__LocalData_c3_b      dcl __LNK_LocalData_c3_b
__asm__LocalData_c3_size   dcl __LNK_LocalData_c3_size
__asm__LocalData_c3_Phys_b dcl __LNK_LocalData_c3_Phys_b
__asm__LocalData_c3_FSM    dcl __LNK_LocalData_c3_FSM
; core 4
__asm__LocalData_c4_b      dcl __LNK_LocalData_c4_b
__asm__LocalData_c4_size   dcl __LNK_LocalData_c4_size
__asm__LocalData_c4_Phys_b dcl __LNK_LocalData_c4_Phys_b
__asm__LocalData_c4_FSM    dcl __LNK_LocalData_c4_FSM
; core 5
__asm__LocalData_c5_b      dcl __LNK_LocalData_c5_b
__asm__LocalData_c5_size   dcl __LNK_LocalData_c5_size
__asm__LocalData_c5_Phys_b dcl __LNK_LocalData_c5_Phys_b
__asm__LocalData_c5_FSM    dcl __LNK_LocalData_c5_FSM

; att_mmu start addr
__asm_att_mmu_c0_virtual_start   dcl __LNK_att_mmu_virtual_c0_start_addr
__asm_att_mmu_c1_virtual_start   dcl __LNK_att_mmu_virtual_c1_start_addr
__asm_att_mmu_c2_virtual_start   dcl __LNK_att_mmu_virtual_c2_start_addr
__asm_att_mmu_c3_virtual_start   dcl __LNK_att_mmu_virtual_c3_start_addr
__asm_att_mmu_c4_virtual_start   dcl __LNK_att_mmu_virtual_c4_start_addr
__asm_att_mmu_c5_virtual_start   dcl __LNK_att_mmu_virtual_c5_start_addr

; att_mmu count
__asm_att_mmu_count_c0   dcl __LNK_address_translation_table_mmu_count_c0
__asm_att_mmu_count_c1   dcl __LNK_address_translation_table_mmu_count_c1
__asm_att_mmu_count_c2   dcl __LNK_address_translation_table_mmu_count_c2
__asm_att_mmu_count_c3   dcl __LNK_address_translation_table_mmu_count_c3
__asm_att_mmu_count_c4   dcl __LNK_address_translation_table_mmu_count_c4
__asm_att_mmu_count_c5   dcl __LNK_address_translation_table_mmu_count_c5

; task_table start addr
__asm_task_table_c0_virtual_start   dcl __LNK_task_table_c0_start_addr
__asm_task_table_c1_virtual_start   dcl __LNK_task_table_c1_start_addr
__asm_task_table_c2_virtual_start   dcl __LNK_task_table_c2_start_addr
__asm_task_table_c3_virtual_start   dcl __LNK_task_table_c3_start_addr
__asm_task_table_c4_virtual_start   dcl __LNK_task_table_c4_start_addr
__asm_task_table_c5_virtual_start   dcl __LNK_task_table_c5_start_addr

; task_table count
__asm_task_table_count_c0   dcl __LNK_task_table_count_c0
__asm_task_table_count_c1   dcl __LNK_task_table_count_c1
__asm_task_table_count_c2   dcl __LNK_task_table_count_c2
__asm_task_table_count_c3   dcl __LNK_task_table_count_c3
__asm_task_table_count_c4   dcl __LNK_task_table_count_c4
__asm_task_table_count_c5   dcl __LNK_task_table_count_c5

; Stack start symbols
__asm_StackStart_c0 dcl __LNK_Local_StackStart_c0
__asm_StackStart_c1 dcl __LNK_Local_StackStart_c1
__asm_StackStart_c2 dcl __LNK_Local_StackStart_c2
__asm_StackStart_c3 dcl __LNK_Local_StackStart_c3
__asm_StackStart_c4 dcl __LNK_Local_StackStart_c4
__asm_StackStart_c5 dcl __LNK_Local_StackStart_c5


; following dummy data is put here to protect against a debugger issue
_nop_1 dcb 0xff,0xff
_nop_2 dcb 0xff,0xff

	endsec

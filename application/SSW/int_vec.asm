;************************************************************************
;
; Interrupt handler table and interrupt handlers.
; This section is still TBD.
;
; This section should vary pending the exact configuration of
; SC3850 devices.
;
;************************************************************************
	section .os_intvec

    ; Entry 0x00: Trap 0. Trap 0 is noted with 0x0FFF value

    di
    move.l #>$fffdffff,d1
    pop.2l d2:d3
    and d1,d3
    push.2l d2:d3
    rte

	;
    ; Entry 0x01: Trap 1
    ;
	di
	jmp >_OS_Exp_Vector1
    dup 4
    nop
    endm

	;
    ; Entry 0x02: Trap 2
    ;
	di
	jmp >_OS_Exp_Vector2
    dup 4
    nop
    endm

	;
    ; Entry 0x03: Trap 3
    ;
	di
	jmp >_OS_Exp_Vector3
    dup 4
    nop
    endm

    ;
    ; Entry 0x04: Reserved
    ;
	di
	jmp >_OS_Exp_Vector4
	dup 28
    nop
	endm

    ;
    ; Entry 0x05: Illegal Instruction
    ;
	di
	jmp >_OS_Exp_Vector5
	dup 28
    nop
	endm

    ;
    ; Entry 0x06: Debug
    ;
	di
	jmp >_OS_Exp_Vector6
	dup 28
    nop
	endm

    ;
    ; Entry 0x07: DALU Overflow
    ;
	di
	jmp >_OS_Exp_Vector7
	dup 28
    nop
	endm

	;
	; Entry 0x08: Reserved (8 bytes entry)
	;
	di
	jmp >_OS_Exp_Vector8

	;
	; Entry 0x09: OCE - OCE exception
	;
	di
	jmp >_OS_Exp_Vector9

	;
	; Entry 0x0A: IMMUAE - Instruction MMU Address Error
	;
	di
	jmp >_OS_Exp_Vector10

	;
	; Entry 0x0B: DMMUAE - Data MMU Address Error
	;
	di
	jmp >_OS_Exp_Vector11

	;
	; Entry 0x0C: Reserved
	;
	debug
	dup 3
    nop
	endm

	;
	; Entry 0x0D: IEDC - Instruction EDC Error
	;
	di
	jmp >_OS_Exp_Vector13

	;
	; Entry 0x0E: DEDC - Data EDC Error
	;
	di
	jmp >_OS_Exp_Vector14

	;
	; Entry 0x0F: Reserved
	;
    debug
	dup 3
    nop
	endm

    ;
    ; Entry 0x10: Reserved
    ;
	debug
	dup 31
    nop
	endm

    ;
    ; Entry 0x11: Reserved
    ;
	debug
	dup 31
    nop
	endm



	; epic index 0
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(0),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 1
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(1),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 2
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(2),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 3
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(3),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 4
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(4),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 5
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(5),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 6
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(6),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 7
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(7),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 8
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(8),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 9
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(9),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 10
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(10),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 11
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(11),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 12
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(12),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 13
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(13),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 14
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(14),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 15
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(15),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 16
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(16),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 17
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(17),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 18
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(18),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 19
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(19),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 20
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(20),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 21
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(21),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 22
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(22),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 23
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(23),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 24
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(24),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 25
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(25),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 26
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(26),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 27
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(27),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 28
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(28),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 29
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(29),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 30
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(30),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 31
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(31),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 32
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(32),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 33
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(33),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 34
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(34),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 35
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(35),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 36
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(36),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 37
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(37),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 38
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(38),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 39
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(39),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 40
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(40),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 41
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(41),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 42
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(42),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 43
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(43),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 44
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(44),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 45
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(45),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 46
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(46),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 47
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(47),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 48
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(48),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 49
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(49),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 50
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(50),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 51
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(51),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 52
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(52),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 53
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(53),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 54
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(54),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 55
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(55),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 56
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(56),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 57
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(57),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 58
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(58),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 59
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(59),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 60
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(60),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 61
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(61),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 62
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(62),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 63
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(63),d0
	jmp	>_os_itc_int_stub
	nop
    ;
	; epic index 64
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(64),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 65
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(65),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 66
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(66),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 67
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(67),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 68
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(68),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 69
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(69),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 70
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(70),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 71
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(71),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 72
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(72),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 73
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(73),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 74
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(74),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 75
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(75),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 76
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(76),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 77
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(77),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 78
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(78),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 79
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(79),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 80
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(80),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 81
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(81),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 82
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(82),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 83
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(83),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 84
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(84),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 85
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(85),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 86
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(86),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 87
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(87),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 88
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(88),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 89
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(89),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 90
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(90),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 91
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(91),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 92
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(92),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 93
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(93),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 94
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(94),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 95
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(95),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 96
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(96),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 97
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(97),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 98
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(98),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 99
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(99),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 100
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(100),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 101
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(101),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 102
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(102),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 103
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(103),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 104
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(104),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 105
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(105),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 106
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(106),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 107
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(107),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 108
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(108),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 109
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(109),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 110
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(110),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 111
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(111),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 112
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(112),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 113
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(113),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 114
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(114),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 115
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(115),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 116
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(116),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 117
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(117),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 118
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(118),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 119
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(119),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 120
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(120),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 121
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(121),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 122
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(122),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 123
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(123),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 124
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(124),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 125
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(125),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 126
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(126),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 127
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(127),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 128
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(128),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 129
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(129),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 130
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(130),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 131
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(131),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 132
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(132),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 133
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(133),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 134
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(134),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 135
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(135),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 136
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(136),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 137
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(137),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 138
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(138),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 139
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(139),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 140
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(140),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 141
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(141),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 142
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(142),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 143
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(143),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 144
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(144),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 145
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(145),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 146
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(146),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 147
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(147),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 148
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(148),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 149
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(149),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 150
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(150),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 151
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(151),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 152
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(152),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 153
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(153),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 154
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(154),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 155
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(155),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 156
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(156),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 157
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(157),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 158
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(158),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 159
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(159),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 160
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(160),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 161
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(161),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 162
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(162),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 163
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(163),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 164
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(164),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 165
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(165),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 166
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(166),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 167
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(167),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 168
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(168),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 169
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(169),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 170
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(170),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 171
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(171),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 172
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(172),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 173
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(173),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 174
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(174),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 175
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(175),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 176
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(176),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 177
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(177),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 178
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(178),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 179
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(179),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 180
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(180),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 181
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(181),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 182
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(182),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 183
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(183),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 184
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(184),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 185
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(185),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 186
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(186),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 187
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(187),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 188
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(188),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 189
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(189),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 190
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(190),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 191
	;
	di						; disable interrupt
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move	#(191),d0
	jmp	>_os_itc_int_stub
    ;
	; epic index 192
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_192
    ;
	; epic index 193
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_193
    ;
	; epic index 194
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_194
    ;
	; epic index 195
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_195
    ;
	; epic index 196
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_196
    ;
	; epic index 197
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_197
    ;
	; epic index 198
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_198
    ;
	; epic index 199
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_199
    ;
	; epic index 200
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_200
    ;
	; epic index 201
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_201
    ;
	; epic index 202
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_202
    ;
	; epic index 203
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_203
    ;
	; epic index 204
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_204
    ;
	; epic index 205
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_205
    ;
	; epic index 206
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_206
    ;
	; epic index 207
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_207
    ;
	; epic index 208
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_208
    ;
	; epic index 209
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_209
    ;
	; epic index 210
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_210
    ;
	; epic index 211
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_211
    ;
	; epic index 212
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_212
    ;
	; epic index 213
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_213
    ;
	; epic index 214
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_214
    ;
	; epic index 215
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_215
    ;
	; epic index 216
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_216
    ;
	; epic index 217
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_217
    ;
	; epic index 218
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_218
    ;
	; epic index 219
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_219
    ;
	; epic index 220
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_220
    ;
	; epic index 221
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_221
    ;
	; epic index 222
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_222
    ;
	; epic index 223
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_223
    ;
	; epic index 224
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_224
    ;
	; epic index 225
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_225
    ;
	; epic index 226
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_226
    ;
	; epic index 227
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_227
    ;
	; epic index 228
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_228
    ;
	; epic index 229
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_229
    ;
	; epic index 230
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_230
    ;
	; epic index 231
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_231
    ;
	; epic index 232
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_232
    ;
	; epic index 233
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_233
    ;
	; epic index 234
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_234
    ;
	; epic index 235
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_235
    ;
	; epic index 236
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_236
    ;
	; epic index 237
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_237
    ;
	; epic index 238
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_238
    ;
	; epic index 239
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_239
    ;
	; epic index 240
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_240
    ;
	; epic index 241
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_241
    ;
	; epic index 242
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_242
    ;
	; epic index 243
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_243
    ;
	; epic index 244
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_244
    ;
	; epic index 245
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_245
    ;
	; epic index 246
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_246
    ;
	; epic index 247
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_247
    ;
	; epic index 248
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_248
    ;
	; epic index 249
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_249
    ;
	; epic index 250
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_250
    ;
	; epic index 251
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_251
    ;
	; epic index 252
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_252
    ;
	; epic index 253
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_253
    ;
	; epic index 254
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_254
    ;
	; epic index 255
	;
	di						; disable interrupt
	jmp	>_OS_Vector2_255
	endsec

	;
;
;************************************************************************
; text segment.
;************************************************************************
	section  .dd_intvec_text

	global  ___EmptyIntHandler_Pharos__

___EmptyIntHandler_Pharos__
	push d2
	move.l (sp-16),d2
	move.l d2,>_os_debug_nmi_pc
	pop d2
	rte

  	;
_OS_Vector2_192:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(192),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_193:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(193),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_194:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(194),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_195:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(195),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_196:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(196),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_197:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(197),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_198:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(198),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_199:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(199),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_200:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(200),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_201:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(201),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_202:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(202),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_203:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(203),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_204:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(204),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_205:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(205),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_206:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(206),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_207:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(207),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_208:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(208),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_209:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(209),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_210:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(210),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_211:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(211),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_212:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(212),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_213:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(213),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_214:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(214),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_215:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(215),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_216:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(216),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_217:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(217),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_218:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(218),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_219:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(219),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_220:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(220),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_221:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(221),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_222:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(222),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_223:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(223),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_224:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(224),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_225:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(225),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_226:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(226),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_227:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(227),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_228:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(228),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_229:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(229),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_230:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(230),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_231:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(231),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_232:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(232),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_233:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(233),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_234:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(234),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_235:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(235),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_236:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(236),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_237:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(237),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_238:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(238),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_239:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(239),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_240:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(240),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_241:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(241),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_242:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(242),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_243:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(243),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_244:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(244),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_245:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(245),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_246:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(246),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_247:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(247),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_248:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(248),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_249:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(249),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_250:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(250),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_251:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(251),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_252:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(252),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_253:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(253),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_254:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(254),d0
	jmp	>_os_itc_int_stub
  	;
_OS_Vector2_255:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(255),d0
	jmp	>_os_itc_int_stub

;add exception handler jump table
    ;Trap0
_OS_Exp_Vector0:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(0),d0
	jmp	>_os_excep_handler
	;Trap1
_OS_Exp_Vector1:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(1),d0
	jmp	>_os_excep_handler
	;Trap2
_OS_Exp_Vector2:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(2),d0
	jmp	>_os_excep_handler
	;Trap3
_OS_Exp_Vector3:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(3),d0
	jmp	>_os_excep_handler
	;
_OS_Exp_Vector4:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(4),d0
	jmp	>_os_excep_handler
	;
_OS_Exp_Vector5:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(5),d0
	jmp	>_os_excep_handler
	;
_OS_Exp_Vector6:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(6),d0
	jmp	>_os_excep_handler
    ;
_OS_Exp_Vector7:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(7),d0
	jmp	>_os_excep_handler
	;
_OS_Exp_Vector8:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(8),d0
	jmp	>_os_excep_handler
    ;
_OS_Exp_Vector9:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(9),d0
	jmp	>_os_excep_handler
	;
_OS_Exp_Vector10:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(10),d0
	jmp	>_os_excep_handler
    ;
_OS_Exp_Vector11:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(11),d0
	jmp	>_os_excep_handler
	;
_OS_Exp_Vector12:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(12),d0
	jmp	>_os_excep_handler
    ;
_OS_Exp_Vector13:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(13),d0
	jmp	>_os_excep_handler
	;
_OS_Exp_Vector14:
	push	d0.e:d1.e		;
	push.2l	d0:d1       	; push d0,d1 into esp
	move.l	#(14),d0
	jmp	>_os_excep_handler

	endsec

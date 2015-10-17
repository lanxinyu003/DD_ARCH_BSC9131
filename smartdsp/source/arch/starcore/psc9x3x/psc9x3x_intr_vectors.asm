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
; $Date: 2012/07/15 16:46:58 $
; $Id: psc9x3x_intr_vectors.asm,v 1.4 2012/07/15 16:46:58 b05127 Exp $
; $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_intr_vectors.asm,v $
; $Revision: 1.4 $
; **************************************************************************//*
;
; @File          psc9x3x_intr_vectors.asm
;
; @Description   Interrupt vectors for PSC9X3X.
;
; @Cautions      None.
;
;*//***************************************************************************/

	section .osvecb
	SECTYPE overlay

	ALIGN 4096
ISR MACRO IRQ

	   di
	   push d0.e:d1.e
       push.2l d0:d1
; Need to get off the stack the value IPL (in saved SR). We will need this to allow interrupts to be
; on while we work through SWI's

       jmpd _osHwiDispatcher
       moveu.w  #\?IRQ,d0.l

	 ENDM

ISR_PRECISE MACRO IRQ

       di
       push d0.e:d1.e
       push.2l d0:d1
; Need to get off the stack the value IPL (in saved SR). We will need this to allow interrupts to be
; on while we work through SWI's

       jmpd _osHwiPreciseDispatcher
       moveu.w  #\?IRQ,d0.l


	 ENDM


	global _interr_vector_file
	global _VBAddr

_VBAddr	            ; backward compatibility
_interr_vector_file


    ;
    ; Entry 0x00: Trap 0. Trap 0 is noted with 0x0FFF value
    ;
    ISR $0FFF

	;
    ; Entry 0x01: Trap 1
    ;
    ISR $1

	;
    ; Entry 0x02: Trap 2
    ;
    ISR $2

	;
    ; Entry 0x03: Trap 3
    ;
    ISR $3

    ;
    ; Entry 0x04: Reserved
    ;
    debug
	dup 31
    nop
	endm

    ;
    ; Entry 0x05: Illegal Instruction
    ;
    ISR_PRECISE $5
    debug
	dup 23
    nop
	endm

    ;
    ; Entry 0x06: Debug
    ;
    ISR_PRECISE $6
    debug
    dup 23
    nop
    endm

    ;
    ; Entry 0x07: DALU Overflow
    ;
	debug
	dup 31
    nop
	endm

	;
	; Entry 0x08: Reserved (8 bytes entry)
	;
	debug
	dup 3
    nop
	endm

	;
	; Entry 0x09: OCE - OCE exception
	;
	di
	jmp _OCE

	;
	; Entry 0x0A: IMMUAE - Instruction MMU Address Error
	;
	di
	jmp _IMMUAE

	;
	; Entry 0x0B: DMMUAE - Data MMU Address Error
	;
	di
	jmp _DMMUAE

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
	jmp _IEDC

	;
	; Entry 0x0E: DEDC - Data EDC Error
	;
	di
	jmp _DEDC

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

	;
    ; Entry 0x12: IRQ0 - I_MIFER: Master IF errors from the MMU (NMI)
    ;
	ISR $12

	;
    ; Entry 0x13: IRQ1 - I_SIFER: Slave IF errors from the MMU (NMI)
    ;
	ISR $13

	;
    ; Entry 0x14: IRQ2 - I_WBBEDC: WBB soft data error (NMI)
    ;
	ISR $14

	;
    ; Entry 0x15: IRQ3 - I_DMAEDC: DMA soft data error (NMI)
    ;
	ISR $15

	;
    ; Entry 0x16: IRQ4 - I_ICDME: ICache double match error (NMI)
    ;
	ISR $16

	;
    ; Entry 0x17: IRQ5 - I_DCDME: DCache double match error (NMI)
    ;
	ISR $17

	;
    ; Entry 0x18: IRQ6 - I_L2NM2: L2 non mapped M2 access
    ;
	ISR $18

	;
    ; Entry 0x19: IRQ7 - I_L2NAE: L2 non-aligned non-allocated access
    ;
	ISR $19

    ;
    ; Entrie 0x1A: IRQ8 - Reserved
    ;
	debug
	dup 7
    nop
	endm

	;
    ; Entry 0x1B: IRQ9 - I_ICAES: ICache end-of-sweep operation exception
    ;
	ISR $1B

	;
    ; Entry 0x1C: IRQ10 - I_DCAES: DCache end-of-sweep operation exception
    ;
	ISR $1C

	;
    ; Entry 0x1D: IRQ11 - I__L2AES: L2 Cache end-of-sweep operation exception. Operational in Titanium with L2 cache
    ;

	ISR $1D

	;
    ; Entry 0x1E: IRQ12 - I_TM0: Timer 0 interrupt
    ;
	ISR $1E

	;
    ; Entry 0x1F: IRQ13 - I_TM1: Timer 1 interrupt
    ;
	ISR $1F

	;
    ; Entry 0x20: IRQ14 - I_DPUA: DPU interrupt A
    ;
    if @def('VTB_HEAT')
    ISR_PRECISE $20 ; Uses NMI for triggering VBB overflow interrupts
    else
	ISR $20 ; default
	endif

	;
    ; Entry 0x21: IRQ15 - I_DPUB: DPU interrupt B
    ;
	ISR $21

	;
    ; Entry 0x22: IRQ16 - I_ICNCH: ICache non-cacheable hit exception
    ;
	ISR $22

	;
    ; Entry 0x23: IRQ17 - I_DCNCH: DCache non-cacheable hit exception
    ;
	ISR $23

	;
    ; Entry 0x24: IRQ18 - I_L2NCH: L2 Cache non-cacheable hit exception
    ;
	ISR $24

	;
    ; Entry 0x25: IRQ19 - I_L2ESP: L2 Cache end-of-software pre-fetch
    ;
	ISR $25

	;
    ; IRQ20-IRQ33 Reserved for internal platform use
    ;
	debug
	dup 111
    nop
	endm


	; IRQs 34-49 are used for mesh of interrupts between cores
	;
    ; Entry 0x34: IRQ34
    ;
	ISR $34

	;
    ; Entry 0x35 IRQ35
    ;
	ISR $35

	;
    ; Entry 0x36 IRQ36
    ;
	ISR $36

	;
    ; Entry 0x37 IRQ37
    ;
	ISR $37

	;
    ; Entry 0x38 IRQ38
    ;
	ISR $38

	;
    ; Entry 0x39 IRQ39
    ;
	ISR $39

	;
    ; Entry 0x3A IRQ40
    ;
	ISR $3A

	;
    ; Entry 0x3B IRQ41
    ;
	ISR $3B

	;
    ; Entry 0x3C IRQ42
    ;
	ISR $3C

	;
    ; Entry 0x3D IRQ43
    ;
	ISR $3D

	;
    ; Entry 0x3E IRQ44
    ;
	ISR $3E

	;
    ; Entry 0x3F IRQ45
    ;
	ISR $3F

	;
    ; Entry 0x40 IRQ46
    ;
	ISR $40

	;
    ; Entry 0x41 IRQ47
    ;
	ISR $41

	;
    ; Entry 0x42 IRQ48
    ;
	ISR $42

	;
    ; Entry 0x43 IRQ49
    ;
	ISR $43

	;
    ; Entry 0x44 IRQ50
    ;
	ISR $44

	;
    ; Entry 0x45 IRQ51
    ;
	ISR $45

	;
    ; Entry 0x46 IRQ52
    ;
	ISR $46

	;
    ; Entry 0x47 IRQ53
    ;
	ISR $47

	;
    ; Entry 0x48 IRQ54
    ;
	ISR $48

	;
    ; Entry 0x49 IRQ55
    ;
	ISR $49

	;
    ; Entry 0x4A IRQ56
    ;
	ISR $4A

	;
    ; Entry 0x4B IRQ57
    ;
	ISR $4B

	;
    ; Entry 0x4C IRQ58
    ;
	ISR $4C

	;
    ; Entry 0x4D IRQ59
    ;
	ISR $4D

	;
    ; Entry 0x4E IRQ60
    ;
	ISR $4E

	;
    ; Entry 0x4F IRQ61
    ;
	ISR $4F

	;
    ; Entry 0x50 IRQ62
    ;
	ISR $50

	;
    ; Entry 0x51 IRQ63
    ;
	ISR $51

	;
    ; Entry 0x52 IRQ64
    ;
	ISR $52

	;
    ; Entry 0x53 IRQ65
    ;
	ISR $53

	;
    ; Entry 0x54 IRQ66
    ;
	ISR $54

	;
    ; Entry 0x55 IRQ67
    ;
	ISR $55

	;
    ; Entry 0x56 IRQ68
    ;
	ISR $56

	;
    ; Entry 0x57 IRQ69
    ;
	ISR $57

	;
    ; Entry 0x58 IRQ70
    ;
	ISR $58

	;
    ; Entry 0x59 IRQ71
    ;
	ISR $59

	;
    ; Entry 0x5A IRQ72
    ;
	ISR $5A

	;
    ; Entry 0x5B IRQ73
    ;
	ISR $5B

	;
    ; Entry 0x5C IRQ74
    ;
	ISR $5C

	;
    ; Entry 0x5D IRQ75
    ;
	ISR $5D

	;
    ; Entry 0x5E IRQ76
    ;
	ISR $5E

	;
    ; Entry 0x5F IRQ77
    ;
	ISR $5F

	;
    ; Entry 0x60 IRQ78
    ;
	ISR $60

	;
    ; Entry 0x61 IRQ79
    ;
	ISR $61

	;
    ; Entry 0x62 IRQ80
    ;
	ISR $62

	;
    ; Entry 0x63 IRQ81
    ;
	ISR $63

	;
    ; Entry 0x64 IRQ82
    ;
	ISR $64

	;
    ; Entry 0x65 IRQ83
    ;
	ISR $65

	;
    ; Entry 0x66 IRQ84
    ;
	ISR $66

	;
    ; Entry 0x67 IRQ85
    ;
	ISR $67

	;
    ; Entry 0x68 IRQ86
    ;
	ISR $68

	;
    ; Entry 0x69 IRQ87
    ;
	ISR $69

	;
    ; Entry 0x6A IRQ88
    ;
	ISR $6A

	;
    ; Entry 0x6B IRQ89
    ;
	ISR $6B

	;
    ; Entry 0x6C IRQ90
    ;
	ISR $6C

	; IRQs 91 till 108 are used for Ethernet Communication Controller 0
	;
    ; Entry 0x6D IRQ91
    ;
	ISR $6D

	;
    ; Entry 0x6E IRQ92
    ;
	ISR $6E

	;
    ; Entry 0x6F IRQ93
    ;
	ISR $6F

	;
    ; Entry 0x70 IRQ94
    ;
	ISR $70

	;
    ; Entry 0x71 IRQ95
    ;
	ISR $71

	;
    ; Entry 0x72 IRQ96
    ;
	ISR $72

	;
    ; Entry 0x73 IRQ97
    ;
	ISR $73

	;
    ; Entry 0x74 IRQ98
    ;
	ISR $74

	;
    ; Entry 0x75 IRQ99
    ;
	ISR $75

	;
    ; Entry 0x76 IRQ100
    ;
	ISR $76

	;
    ; Entry 0x77 IRQ101
    ;
	ISR $77

	;
    ; Entry 0x78 IRQ102
    ;
	ISR $78

	;
    ; Entry 0x79 IRQ103
    ;
	ISR $79

	;
    ; Entry 0x7A IRQ104
    ;
	ISR $7A

	;
    ; Entry 0x7B IRQ105
    ;
	ISR $7B

	;
    ; Entry 0x7C IRQ106
    ;
	ISR $7C

	;
    ; Entry 0x7D IRQ107
    ;
	ISR $7D

	;
    ; Entry 0x7E IRQ108
    ;
	ISR $7E

	;
    ; Entry 0x7F IRQ109
    ;
	ISR $7F

	;
    ; Entry 0x80 IRQ110
    ;
	ISR $80

	;
    ; Entry 0x81 IRQ111
    ;
	ISR $81

	;
    ; Entry 0x82 IRQ112
    ;
	ISR $82

	;
    ; Entry 0x83 IRQ113
    ;
	ISR $83

	;
    ; Entry 0x84 IRQ114
    ;
	ISR $84

	;
    ; Entry 0x85 IRQ115
    ;
	ISR $85

	;
    ; Entry 0x86 IRQ116
    ;
	ISR $86

	;
    ; Entry 0x87 IRQ117
    ;
	ISR $87

	;
    ; Entry 0x88 IRQ118
    ;
	ISR $88

	;
    ; Entry 0x89 IRQ119
    ;
	ISR $89

	;
    ; Entry 0x8A IRQ120
    ;
	ISR $8A

	;
    ; Entry 0x8B IRQ121
    ;
	ISR $8B

	;
    ; Entry 0x8C IRQ122
    ;
	ISR $8C

	;
    ; Entry 0x8D IRQ123
    ;
	ISR $8D

	;
    ; Entry 0x8E IRQ124
    ;
	ISR $8E

	;
    ; Entry 0x8F IRQ125
    ;
	ISR $8F

	;
    ; Entry 0x90 IRQ126
    ;
	ISR $90

	;
    ; Entry 0x91 IRQ127
    ;
	ISR $91

	;
    ; Entry 0x92 IRQ128
    ;
	ISR $92

	;
    ; Entry 0x93 IRQ129
    ;
	ISR $93

	;
    ; Entry 0x94 IRQ130
    ;
	ISR $94

	;
    ; Entry 0x95 IRQ131
    ;
	ISR $95

	;
    ; Entry 0x96 IRQ132
    ;
	ISR $96

	;
    ; Entry 0x97 IRQ133
    ;
	ISR $97

	;
    ; Entry 0x98 IRQ134
    ;
	ISR $98

	;
    ; Entry 0x99 IRQ135
    ;
	ISR $99

	;
    ; Entry 0x9A IRQ136
    ;
	ISR $9A

	;
    ; Entry 0x9B IRQ137
    ;
	ISR $9B

	;
    ; Entry 0x9C IRQ138
    ;
	ISR $9C

	;
    ; Entry 0x9D IRQ139
    ;
	ISR $9D

	;
    ; Entry 0x9E IRQ140
    ;
	ISR $9E

	;
    ; Entry 0x9F IRQ141
    ;
	ISR $9F

	;
    ; Entry 0xA0 IRQ142
    ;
	ISR $A0

	;
    ; Entry 0xA1 IRQ143
    ;
	ISR $A1

	; IRQs 144 till 159 are used for DMA interrupts
	;
    ; Entry 0xA2 IRQ144 - DMA ch 0
    ;
	ISR $A2

	;
    ; Entry 0xA3 IRQ145 - DMA ch 1
    ;
	ISR $A3

	;
    ; Entry 0xA4 IRQ146 - DMA ch 2
    ;
	ISR $A4

	;
    ; Entry 0xA5 IRQ147 - DMA ch 3
    ;
	ISR $A5

	;
    ; Entry 0xA6 IRQ148 - DMA ch 4
    ;
	ISR $A6

	;
    ; Entry 0xA7 IRQ149 - DMA ch 5
    ;
	ISR $A7

	;
    ; Entry 0xA8 IRQ150 - DMA ch 6
    ;
	ISR $A8

	;
    ; Entry 0xA9 IRQ151 - DMA ch 7
    ;
	ISR $A9

	;
    ; Entry 0xAA IRQ152 - DMA ch 8
    ;
	ISR $AA

	;
    ; Entry 0xAB IRQ153 - DMA ch 9
    ;
	ISR $AB

	;
    ; Entry 0xAC IRQ154 - DMA ch 10
    ;
	ISR $AC

	;
    ; Entry 0xAD IRQ155 - DMA ch 11
    ;
	ISR $AD

	;
    ; Entry 0xAE IRQ156 - DMA ch 12
    ;
	ISR $AE

	;
    ; Entry 0xAF IRQ157 - DMA ch 13
    ;
	ISR $AF

	;
    ; Entry 0xB0 IRQ158 - DMA ch 14
    ;
	ISR $B0

	;
    ; Entry 0xB1 IRQ159 - DMA ch 15
    ;
	ISR $B1

	;
    ; Entry 0xB2 IRQ160 
    ;
	ISR $B2

	;
    ; Entry 0xB3 IRQ161
    ;
	ISR $B3

	;
    ; Entry 0xB4 IRQ162 
    ;
	ISR $B4

	;
    ; Entry 0xB5 IRQ163
    ;
	ISR $B5

	;
    ; Entry 0xB6 IRQ164 
    ;
	ISR $B6

	;
    ; Entry 0xB7 IRQ165 
    ;
	ISR $B7

	;
    ; Entry 0xB8 IRQ166 
    ;
	ISR $B8

	;
    ; Entry 0xB9 IRQ167 
    ;
	ISR $B9

	;
    ; Entry 0xBA IRQ168 
    ;
	ISR $BA

	;
    ; Entry 0xBB IRQ169 
    ;
	ISR $BB

	;
    ; Entry 0xBC IRQ170 
    ;
	ISR $BC

	;
    ; Entry 0xBD IRQ171 
    ;
	ISR $BD

	;
    ; Entry 0xBE IRQ172 
    ;
	ISR $BE

	;
    ; Entry 0xBF IRQ173 
    ;
	ISR $BF

	;
    ; Entry 0xC0 IRQ174 
    ;
	ISR $C0

	;
    ; Entry 0xC1 IRQ175 
    ;
	ISR $C1

	;
    ; Entry 0xC2 IRQ176
    ;
	ISR $C2

	; IRQs 177 till 192 are used for Virtual IRQ interrupts
	;
    ; Entry 0xC3 IRQ177 - VIRQ0
    ;
	ISR $C3

	;
    ; Entry 0xC4 IRQ178 - VIRQ1
    ;
	ISR $C4

	;
    ; Entry 0xC5 IRQ179 - VIRQ2
    ;
	ISR $C5

	;
    ; Entry 0xC6 IRQ180 - VIRQ3
    ;
	ISR $C6

	;
    ; Entry 0xC7 IRQ181 - VIRQ4
    ;
	ISR $C7

	;
    ; Entry 0xC8 IRQ182 - VIRQ5
    ;
	ISR $C8

	;
    ; Entry 0xC9 IRQ183 - VIRQ6
    ;
	ISR $C9

	;
    ; Entry 0xCA IRQ184 - VIRQ7
    ;
	ISR $CA

	;
    ; Entry 0xCB IRQ185 - VIRQ8
    ;
	ISR $CB

	;
    ; Entry 0xCC IRQ186 - VIRQ9
    ;
	ISR $CC

	;
    ; Entry 0xCD IRQ187 - VIRQ10
    ;
	ISR $CD

	;
    ; Entry 0xCE IRQ188 - VIRQ11
    ;
	ISR $CE

	;
    ; Entry 0xCF IRQ189 - VIRQ12
    ;
	ISR $CF

	;
    ; Entry 0xD0 IRQ190 - VIRQ13
    ;
	ISR $D0

	;
    ; Entry 0xD1 IRQ191 - VIRQ14
    ;
	ISR $D1

	;;; Entries 0xD2-0x111 (IRQs 192-255). Note: they have only 8 bytes per entry

	;
    ; Entry 0xD2 IRQ192 - VIRQ15
    ;
    di
	jmp _IRQ192

	; IRQs 193 till 200 are used for CPRI Interrupts
	;
    ; Entry 0xD3 IRQ193 - CPRI_5
    ;
    di
	jmp _IRQ193

	;
    ; Entry 0xD4 IRQ194 - CPRI_6
    ;
    di
	jmp _IRQ194

	;
    ; Entry 0xD5 IRQ195 - CPRI_7
    ;
    di
	jmp _IRQ195

	;
    ; Entry 0xD6 IRQ196 - CPRI_8
    ;
    di
	jmp _IRQ196

	;
    ; Entry 0xD7 IRQ197 - CPRI_9
    ;
    di
	jmp _IRQ197

	;
    ; Entry 0xD8 IRQ198 - CPRI_10
    ;
    di
	jmp _IRQ198

	;
    ; Entry 0xD9 IRQ199 - CPRI_11
    ;
    di
	jmp _IRQ199

	;
    ; Entry 0xDA IRQ200 - CPRI_12
    ;
    di
	jmp _IRQ200

	;
    ; Entry 0xDB IRQ201
    ;
    di
	jmp _IRQ201

	;
    ; Entry 0xDC IRQ202
    ;
    di
	jmp _IRQ202

	; IRQs 203 till 206 are used for OCNDMA0 interrupts
	;
    ; Entry 0xDD IRQ203
    ;
    di
	jmp _IRQ203

	;
    ; Entry 0xDE IRQ204
    ;
    di
	jmp _IRQ204

	;
    ; Entry 0xDF IRQ205
    ;
    di
	jmp _IRQ205

	;
    ; Entry 0xE0 IRQ206
    ;
    di
	jmp _IRQ206

	;
    ; Entry 0xE1 IRQ207
    ;
    di
	jmp _IRQ207

	;
    ; Entry 0xE2 IRQ208 
    ;
    di
	jmp _IRQ208

	; IRQs 209 till 224 are used for MAPLE interrupts
	;
    ; Entry 0xE3 IRQ209 - MAPLE ch 0
    ;
    di
	jmp _IRQ209

	;
    ; Entry 0xE4 IRQ210 - MAPLE ch 1
    ;
    di
	jmp _IRQ210

	;
    ; Entry 0xE5 IRQ211 - MAPLE ch 2
    ;
    di
	jmp _IRQ211

	;
    ; Entry 0xE6 IRQ212 - MAPLE ch 3
    ;
    di
	jmp _IRQ212

	;
    ; Entry 0xE7 IRQ213 - MAPLE ch 4
    ;
    di
	jmp _IRQ213

	;
    ; Entry 0xE8 IRQ214 - MAPLE ch 5
    ;
    di
	jmp _IRQ214

	;
    ; Entry 0xE9 IRQ215 - MAPLE ch 6
    ;
    di
	jmp _IRQ215

	;
    ; Entry 0xEA IRQ216 - MAPLE ch 7
    ;
    di
	jmp _IRQ216

	;
    ; Entry 0xEB IRQ217 - MAPLE ch 8
    ;
    di
	jmp _IRQ217

	;
    ; Entry 0xEC IRQ218 - MAPLE ch 6
    ;
    di
	jmp _IRQ218

	;
    ; Entry 0xED IRQ219 - MAPLE ch 10
    ;
    di
	jmp _IRQ219

	;
    ; Entry 0xEE IRQ220 - MAPLE ch 11
    ;
    di
	jmp _IRQ220

	;
    ; Entry 0xEF IRQ221 - MAPLE ch 12
    ;
    di
	jmp _IRQ221

	;
    ; Entry 0xF0 IRQ222 - MAPLE ch 13
    ;
    di
	jmp _IRQ222

	;
    ; Entry 0xF1 IRQ223 - MAPLE ch 14
    ;
    di
	jmp _IRQ223

	;
    ; Entry 0xF2 IRQ224 - MAPLE ch 15
    ;
    di
	jmp _IRQ224

	;
    ; Entry 0xF3 IRQ225
    ;
    di
	jmp _IRQ225

	; IRQs 226 till 242 are used for External interrupts
	;
    ; Entry 0xF4 IRQ226 - IRQ0
    ;
    di
	jmp _IRQ226

	;
    ; Entry 0xF5 IRQ227 - IRQ1
    ;
    di
	jmp _IRQ227

	;
    ; Entry 0xF6 IRQ228 - IRQ2
    ;
    di
	jmp _IRQ228

	;
    ; Entry 0xF7 IRQ229 - IRQ3
    ;
    di
	jmp _IRQ229

	;
    ; Entry 0xF8 IRQ230 - IRQ4
    ;
    di
	jmp _IRQ230

	;
    ; Entry 0xF9 IRQ231 - IRQ5
    ;
    di
	jmp _IRQ231

	;
    ; Entry 0xFA IRQ232 - IRQ6
    ;
    di
	jmp _IRQ232

	;
    ; Entry 0xFB IRQ233 - IRQ7
    ;
    di
	jmp _IRQ233

	;
    ; Entry 0xFC IRQ234 
    ;
    di
	jmp _IRQ234

	;
    ; Entry 0xFD IRQ235 
    ;
    di
	jmp _IRQ235

	;
    ; Entry 0xFE IRQ236 
    ;
    di
	jmp _IRQ236

	;
    ; Entry 0xFF IRQ237 - IRQ8
    ;
    di
	jmp _IRQ237

	;
    ; Entry 0x100 IRQ238 - IRQ9
    ;
    di
	jmp _IRQ238

	;
    ; Entry 0x101 IRQ239 - IRQ10
    ;
    di
	jmp _IRQ239

	;
    ; Entry 0x102 IRQ240 - IRQ11
    ;
    di
	jmp _IRQ240

	;
    ; Entry 0x103 IRQ241
    ;
    di
	jmp _IRQ241

	;
    ; Entry 0x104 IRQ242
    ;
    di
	jmp _IRQ242

	;
    ; Entry 0x105 IRQ243 
    ;
    di
	jmp _IRQ243

	;
    ; Entry 0x106 IRQ244 - Debug
    ;
    di
	jmp _IRQ244

	;
    ; Entry 0x107 IRQ245 - General error
    ;
    di
	jmp _IRQ245

	;
    ; Entry 0x108 IRQ246 - WDT (set to be NMI)
    ;
    di
	jmp _IRQ246

	;
    ; Entry 0x109 IRQ247 - MAPLE
    ;
    di
	jmp _IRQ247

	;
    ; Entry 0x10A IRQ248
    ;
    di
	jmp _IRQ248

	;
    ; Entry 0x10B IRQ249
    ;
    di
	jmp _IRQ249

	;
    ; Entry 0x10C IRQ250
    ;
    di
	jmp _IRQ250

	;
    ; Entry 0x10D IRQ251
    ;
    di
	jmp _IRQ251

	;
    ; Entry 0x10E IRQ252
    ;
    di
	jmp _IRQ252

	; IRQs 253 till 254 are used for CPRI_RX_TIMING
	;
    ; Entry 0x10F IRQ253
    ;
    di
	jmp _IRQ253

	;
    ; Entry 0x110 IRQ254
    ;
    di
	jmp _IRQ254

	;
    ; Entry 0x111 IRQ255
    ;
    di
	jmp _IRQ255

	;;;
	;;; The following entries are reached from previous entries, that are too small (only 8 bytes)
	;;; to contain the ISR macro.
	;;;

	;;; OCE entry
_OCE:
	ISR_PRECISE $9

	;;; IMMUAE entry
_IMMUAE:
	ISR_PRECISE $A

	;;; DMMUAE entry
_DMMUAE:
	ISR_PRECISE $B

	;;; IEDC entry
_IEDC:
	ISR_PRECISE $D

	;;; DEDC entry
_DEDC:
	ISR_PRECISE $E

	;;; IRQ192 entry
_IRQ192:
	ISR $D2

	;;; IRQ193 entry
_IRQ193:
	ISR $D3

	;;; IRQ194 entry
_IRQ194:
	ISR $D4

	;;; IRQ195 entry
_IRQ195:
	ISR $D5

	;;; IRQ196 entry
_IRQ196:
	ISR $D6

	;;; IRQ197 entry
_IRQ197:
	ISR $D7

	;;; IRQ198 entry
_IRQ198:
	ISR $D8

	;;; IRQ199 entry
_IRQ199:
	ISR $D9

	;;; IRQ200 entry
_IRQ200:
	ISR $DA

	;;; IRQ201 entry
_IRQ201:
	ISR $DB

	;;; IRQ202 entry
_IRQ202:
	ISR $DC

	;;; IRQ203 entry
_IRQ203:
	ISR $DD

	;;; IRQ204 entry
_IRQ204:
	ISR $DE

	;;; IRQ205 entry
_IRQ205:
	ISR $DF

	;;; IRQ206 entry
_IRQ206:
	ISR $E0

	;;; IRQ207 entry
_IRQ207:
	ISR $E1

	;;; IRQ208 entry
_IRQ208:
	ISR $E2

	;;; IRQ209 entry
_IRQ209:
	ISR $E3

	;;; IRQ210 entry
_IRQ210:
	ISR $E4

	;;; IRQ211 entry
_IRQ211:
	ISR $E5

	;;; IRQ212 entry
_IRQ212:
	ISR $E6

	;;; IRQ213 entry
_IRQ213:
	ISR $E7

	;;; IRQ214 entry
_IRQ214:
	ISR $E8

	;;; IRQ215 entry
_IRQ215:
	ISR $E9

	;;; IRQ216 entry
_IRQ216:
	ISR $EA

	;;; IRQ217 entry
_IRQ217:
	ISR $EB

	;;; IRQ218 entry
_IRQ218:
	ISR $EC

	;;; IRQ219 entry
_IRQ219:
	ISR $ED

	;;; IRQ220 entry
_IRQ220:
	ISR $EE

	;;; IRQ221 entry
_IRQ221:
	ISR $EF

	;;; IRQ222 entry
_IRQ222:
	ISR $F0

	;;; IRQ223 entry
_IRQ223:
	ISR $F1

	;;; IRQ224 entry
_IRQ224:
	ISR $F2

	;;; IRQ225 entry
_IRQ225:
	ISR $F3

	;;; IRQ226 entry
_IRQ226:
	ISR $F4

	;;; IRQ227 entry
_IRQ227:
	ISR $F5

	;;; IRQ228 entry
_IRQ228:
	ISR $F6

	;;; IRQ229 entry
_IRQ229:
	ISR $F7

	;;; IRQ230 entry
_IRQ230:
	ISR $F8

	;;; IRQ231 entry
_IRQ231:
	ISR $F9

	;;; IRQ232 entry
_IRQ232:
	ISR $FA

	;;; IRQ233 entry
_IRQ233:
	ISR $FB

	;;; IRQ234 entry
_IRQ234:
	ISR $FC

	;;; IRQ235 entry
_IRQ235:
	ISR $FD

	;;; IRQ236 entry
_IRQ236:
	ISR $FE

	;;; IRQ237 entry
_IRQ237:
	ISR $FF

	;;; IRQ238 entry
_IRQ238:
	ISR $100

	;;; IRQ239 entry
_IRQ239:
	ISR $101

	;;; IRQ240 entry
_IRQ240:
	ISR $102

	;;; IRQ241 entry
_IRQ241:
	ISR $103

	;;; IRQ242 entry
_IRQ242:
	ISR $104

	;;; IRQ243 entry
_IRQ243:
	ISR $105

	;;; IRQ244 entry
_IRQ244:
	ISR $106

	;;; IRQ245 entry
_IRQ245:
	ISR $107

	;;; IRQ246 entry
_IRQ246:
	ISR_PRECISE $108

	;;; IRQ247 entry
_IRQ247:
	ISR $109

	;;; IRQ248 entry
_IRQ248:
	ISR $10A

	;;; IRQ249 entry
_IRQ249:
	ISR $10B

	;;; IRQ250 entry
_IRQ250:
	ISR $10C

	;;; IRQ251 entry
_IRQ251:
	ISR $10D

	;;; IRQ252 entry
_IRQ252:
	ISR $10E

	;;; IRQ253 entry
_IRQ253:
	ISR $10F

	;;; IRQ254 entry
_IRQ254:
	ISR $110

	;;; IRQ255 entry
_IRQ255:
	ISR $111

	endsec

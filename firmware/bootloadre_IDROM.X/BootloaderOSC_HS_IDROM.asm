    radix DEC

; change these lines accordingly to your application	
#include "p18f4620.inc"
IdTypePIC = 0x4F		    ; must exists in "piccodes.ini"
#define max_flash 0x10000	    ; in BYTES!!! (= 'max flash memory' from "piccodes.ini")

xtal EQU 16384000		    ; you may want to change: _XT_OSC_1H  _HS_OSC_1H  _HSPLL_OSC_1H		    ; you may want to change: _XT_OSC_1H  _HS_OSC_1H  _HSPLL_OSC_1H
baud EQU 38400			    ; standard baud rates: 115200 or 19200
; The above 6 lines can be changed and built a bootloader for the desired frequency (and PIC type)
	
; This source file is for PIC18F2620 and 4620.

; Copy these include files to your project directory (i.e. they are in the same
; directory with your .asm source file), if necessary

#include "spbrgselect.inc"	    ; RoundResult and baud_rate

#define first_address max_flash-200 ;100 words

#define app_init 0x0032
		

;----- CONFIG1H Options -----
    CONFIG	OSC = HS, FCMEN = OFF, IESO = OFF
;----- CONFIG2L Options -----
    CONFIG	PWRT = ON, BOREN = ON, BORV = 2
;----- CONFIG2H Options -----
    CONFIG	WDT = OFF, WDTPS = 128
;----- CONFIG3H Options -----
    CONFIG	MCLRE = ON, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC
;----- CONFIG4L Options -----
    CONFIG	STVREN = ON, LVP = OFF, DEBUG = OFF, XINST = OFF


;----------------------------- PROGRAM ---------------------------------
	cblock 0
	crc
	i
	cnt1
	cnt2
	cnt3
	counter_hi
	counter_lo
	flag
	endc
	cblock 10
	buffer:64
	endc
	
SendL macro car
	movlw car
	movwf TXREG
	endm
	
;0000000000000000000000000 RESET 00000000000000000000000000

    ORG     0x0000
    GOTO    IntrareBootloader

;view with TabSize=4
;&&&&&&&&&&&&&&&&&&&&&&&   START     &&&&&&&&&&&&&&&&&&&&&&
;----------------------  Bootloader  ----------------------
;PC_flash:	C1h	U	    H		L	x  ...<64 bytes>... crc	
;PC_eeprom:	C1h	40h	    EEADRH  EEADR	1       EEDATA	    crc					
;PC_cfg		C1h	U OR 80h    H		L	1	byte	    crc
;PIC_response:	type `K`
	
	ORG first_address		;space to deposit first 4 instr. of user prog.
	nop
	nop
	nop
	nop
	org first_address+8
IntrareBootloader
	
				    ; init serial port
	movlw b'00100100'
	movwf TXSTA
	movlw spbrg_value
	movwf SPBRG
	movlw b'10010000'
	movwf RCSTA
					; wait for computer
	rcall Receive			
	sublw 0xC1			; Expect C1h
	bnz way_to_exit
	SendL IdTypePIC			; send PIC type
MainLoop
	SendL 'K'			; "-Everything OK, ready and waiting."
mainl
	clrf crc
	rcall Receive			; Upper
	movwf TBLPTRU
	movwf flag			; (for EEPROM and CFG cases)
	rcall Receive			; Hi
	movwf TBLPTRH
	movwf EEADRH			; (for EEPROM case)
	rcall Receive			; Lo
	movwf TBLPTRL
	movwf EEADR			; (for EEPROM case)

	rcall Receive			; count
	movwf i
	incf i
	lfsr FSR0, (buffer-1)
rcvoct					; read 64+1 bytes
	movwf TABLAT			; prepare for cfg; => store byte before crc
	rcall Receive
	movwf PREINC0
	btfss i,0			; don't know for the moment but in case of EEPROM data presence...
	movwf EEDATA			; ...then store the data byte (and not the CRC!)
	decfsz i
	bra rcvoct
	
	tstfsz crc			; check crc
	bra ziieroare
	btfss flag,6			; is EEPROM data?
	bra noeeprom
	movlw b'00000100'		; Setup eeprom
	rcall Write
	bra waitwre
noeeprom
;----no CFG write in "Tiny PIC Bootloader+"
;		btfss flag,7		;is CFG data?
;		bra noconfig
;		tblwt*			;write TABLAT(byte before crc) to TBLPTR***
;		movlw b'11000100'	;Setup cfg
;		rcall Write
;		bra waitwre
;noconfig
;----
					;write
eraseloop
	movlw b'10010100'		; Setup erase
	rcall Write
	TBLRD*-				; point to adr-1
	
;writebigloop				; don't need the outter loop for
;	movlw 1				; 1 group
;	movwf counter_hi
	lfsr FSR0,buffer
;writesloop
	movlw 64			; 64bytes = 32instr
	movwf counter_lo
writebyte
	movf POSTINC0,w			; put 1 byte
	movwf TABLAT
	tblwt+*
	decfsz counter_lo
	bra writebyte
	
	movlw	b'10000100'		; Setup writes
	rcall Write
;	decfsz counter_hi
;	bra writesloop
waitwre	
	;btfsc EECON1,WR		; for eeprom writes (wait to finish write)
	;bra waitwre			; no need: round trip time with PC bigger than 4ms
	
	bcf EECON1,WREN			; disable writes
	bra MainLoop
	
ziieroare				; CRC failed
	SendL 'N'
	bra mainl
	  
;******** procedures ******************

Write
	movwf EECON1
	movlw 0x55
	movwf EECON2
	movlw 0xAA
	movwf EECON2
	bsf EECON1,WR		; WRITE
	nop
	;nop
	return


Receive
	movlw xtal/2000000+1	; for 20MHz => 11 => 1second delay
				; for 18F2xxx chips, this should be xtal/1000000+1
	movwf cnt1
rpt2						
	clrf cnt2
rpt3
	clrf cnt3
rptc
	btfss PIR1,RCIF		; test RX
	bra notrcv  
	movf RCREG,w		; return read data in W
	addwf crc,f		; compute crc
	return
	
notrcv
	decfsz cnt3
	bra rptc
	decfsz cnt2
	bra rpt3
	decfsz cnt1
	bra rpt2
	;timeout:
way_to_exit
	bcf RCSTA,SPEN		; deactivate UART
	bra first_address
;*************************************************************
; After reset
; Do not expect the memory to be zero,
; Do not expect registers to be initialised like in catalog.

            END

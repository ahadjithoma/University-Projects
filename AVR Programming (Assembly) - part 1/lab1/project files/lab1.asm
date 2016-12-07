;lab1

;Χατζηθωμά Αντρέας 8026
;Νήρας Δημήτρης 8057

;Στο τμήμα 1 αρχικοποιούμε τον stack pointer, εισάγουμε τα αεμ και υπολογίζουμε την πρόσθεση. 
;Ακολούθως στα LEDs εμφανίζονται τα ΑΕΜ μας και το αποτέλεσμα της πρόσθεσης τους, ανα 10 δευτερόλεπτα το καθένα.

;Στο τμήμα 2 τα ΑΕΜ και το άθροισμα τους, εμφανίζονται στα LEDs αν πατηθούν οι διακόπτες SW1, SW2 και SW3 αντίστοιχα. 
 
.include "m16def.inc"
.org 0
.cseg

rjmp part1

.def aem1a = r27
.def aem1b = r26
.def aem2a = r29
.def aem2b = r28

part1:
;initialize the stack
sp_init:
	ldi r16, low(RAMEND)
	out spl, r16
	ldi r16, high(RAMEND)
	out sph, r16

main:
	ldi aem1a, 0b10000000	 ;BCD num 80
	ldi aem1b, 0b00100110	 ;BCD num 26
	ldi aem2a, 0b10000000	 ;BCD num 80
	ldi aem2b, 0b01010111	 ;BCD num 57
	ldi r20, 0b00000110   ;6 number to add if BCD addition is upper 9 in second byte array addition
	ldi r21, 0b01100000   ;6 number to add if BCD addition is upper 9 in fisrt byte array addition 
	
	mov r17, aem1b		
	mov r18, aem2b
	add r17, r18		;add 26+57 	
	add r17, r20		;add 6 to result of (26+57)
	sts 0x0060, r17		;store result to SRAM address 0x0060
	
	mov r17, aem2a
	mov r18, aem2a
	adc r17, r18		;add 80+80 and the contents of C flag (carry)
	
	ldi r24, $00		;load 0 to register
	ldi r25, $00		;load 0 to register
	adc r24, r25		;add zero to keep the carry
	sts 0x0070, r24		;store result to SRAM address 0x0070
	
	add r17, r21		;add 6 to result of (80+80)
	sts 0x0080, r17		;store result to SRAM address 0x0080

setDDRBouput:
	ser r16				;all "1"
	out DDRB, r16		;set portB (exit) 

led_blinking:
	ser r16				;all "1"
	out PORTB, r16		;turn off LEDs
	rcall aem_1b
	rcall aem_1a
	rcall aem_2b
	rcall aem_2a
	rcall add0
	rcall add1
	rcall add2
	ser r16
	out PORTB, r16		
	rjmp part2

;LED functions
aem_1b:
	mov r30, aem1b
	com r30			;NOT(register) because of the ACTIVE LOW logic of LEDs
	out PORTB, r30
	rcall delay
	ret

aem_1a:
	mov r30, aem1a
	com r30
	out PORTB, r30
	rcall delay
	ret

aem_2b:
	mov r30, aem2b
	com r30
	out PORTB, r30
	rcall delay
	ret

aem_2a:
	mov r30, aem2a
	com r30
	out PORTB, r30
	rcall delay
	ret

add0:
	lds r30, 0x0060
	com r30
	out PORTB, r30
	rcall delay
	ret

add1:
	lds r30, 0x0080
	com r30
	out PORTB, r30
	rcall delay
	ret

add2:
	lds r30, 0x0070
	com r30
	out PORTB, r30
	rcall delay
	ret

delay:
	ldi r17, 0x2
d7:
	dec r17
	brne d7
	ret


bdelay:
	ldi r17, 0xed			;237x237x238 loops for 10sec delay (cycle time = 0.25us) 
d3:
	ldi r18, 0xed
d2:
	ldi r19, 0xee
d1:
	dec r19
	brne d1
	dec r18
	brne d2
	dec r17
	brne d3
	ret
	

part2:
	clr r16				;all "0"
	out DDRD, r16		;set portD (input)

	ser r16
	out PIND, r16		;turn off leds
	
;switch 1
;checks if switch is unpressed and waits until is pressed
sw1_unpressed:
	sbic PIND, 0x01		;Skip next instruction if (Port D, pin1 == 0)
	rjmp sw1_unpressed
;checks if switch is pressed and waits until is released
sw1_pressed:
	sbis PIND, 0x01		;If (Port D, pin1 == 0) do
	rjmp sw1_pressed
	
	rcall aem_1b
	rcall aem_1a

	ser r16
	out PORTB, r16

;switch 2
sw2_unpressed:
	sbic PIND, 0x02		;Skip next instruction if (Port D, pin2 == 0)
	rjmp sw2_unpressed	
sw2_pressed:
	sbis PIND, 0x02		;If (Port D, pin2 == 0) do
	rjmp sw2_pressed

	rcall aem_2b 
	rcall aem_2a

	ser r16
	out PORTB, r16

	rcall press 
	lds r30, 0x0060
	rcall left_right

	rcall press
	lds r30, 0x0060
	rcall right

	rcall press
	lds r30, 0x0080
	rcall left_right

	rcall press
	lds r30, 0x0080
	rcall right

	rcall press
	rcall add2

	ser r16
	out PORTB, r16

end:
	rjmp end

;release variables
.undef aem1a
.undef aem1b
.undef aem2a
.undef aem2b

;switch 3
press:
sw3_unpressed:
	sbic PIND, 0x03		;Skip next instruction if (Port D, pin3 == 0)
	rjmp sw3_unpressed
sw3_pressed:
	sbis PIND, 0x03		;If (Port D, pin3 == 0) do
	rjmp sw3_pressed	
	ret	

;shift register 4 times left/right inj order to light up leds 0-3 only
left_right:
	com r30
	lsl r30
	lsl r30
	lsl r30
	lsl r30
	lsr r30
	lsr r30
	lsr r30
	lsr r30
	ori r30, 0b11110000
	out PORTB, r30
	ret

right:
	com r30
	lsr r30
	lsr r30
	lsr r30
	lsr r30
	ori r30, 0b11110000
	out PORTB, r30
	ret



.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

//==================================================================
// ECE 362 Lab Experiment 3
// General Purpose I/O
//==================================================================

.equ  RCC,      0x40021000
.equ  AHBENR,   0x14
.equ  GPIOCEN,  0x00080000
.equ  GPIOBEN,  0x00040000
.equ  GPIOAEN,  0x00020000
.equ  GPIOC,    0x48000800
.equ  GPIOB,    0x48000400
.equ  GPIOA,    0x48000000
.equ  MODER,    0x00 
.equ  PUPDR,    0x0c
.equ  IDR,      0x10 
.equ  ODR,      0x14
.equ  BSRR,     0x18 
.equ  BRR,      0x28 

//==========================================================
// initb: Autotest check ??
// Enable Port B in the RCC AHBENR register and configure
// the pins as described in section 2.1 of the lab
// No parameters.
// No expected return value.
.global initb
initb:
    push    {lr}
    // Student code goes here
	ldr  r0,=RCC			//Enable RCC clock for GPIOB
	ldr  r1,[r0,#AHBENR]
	ldr  r2,=GPIOBEN
	orrs r1,r2
	str  r1,[r0,#AHBENR]

	ldr  r0,=GPIOB
	ldr  r1,[r0,#MODER]
	ldr  r2,=0xFF0303       // Clear PB0 & PB4 and PB8-11
	bics r1,r2
	ldr  r2,=0x550000		// Enable PB8-11
	orrs r1,r2
	str  r1,[r0,#MODER]
    // End of student code
    pop     {pc}

//==========================================================
// initc: Autotest check ??
// Enable Port C in the RCC AHBENR register and configure
// the pins as described in section 2.2 of the lab
// No parameters.
// No expected return value.
.global initc
initc:
    push    {lr}
    // Student code goes here
	ldr  r0,=RCC			//Enable RCC clock for GPIOC
	ldr  r1,[r0,#AHBENR]
	ldr  r2,=GPIOCEN
	orrs r1,r2
	str  r1,[r0,#AHBENR]

	ldr  r0,=GPIOC
	ldr  r1,[r0,#MODER]
	ldr  r2,=0xFFFF			// Clear PC0-PC3 and PC4-PC7
	bics r1,r2
	ldr  r2,=0x5500			// Set PC4-PC7 input
	orrs r1,r2
	str  r1,[r0,#MODER]

	ldr  r1,[r0,#PUPDR]
	ldr  r2,=0xFF			// Clear PC0-PC3 PUPDR
	bics r1,r2
	ldr  r2,=0xaa			// Set PC0-PC3 pull down
	orrs r1,r2
	str  r1,[r0,#PUPDR]

    // End of student code
    pop     {pc}

//==========================================================
// setn: Autotest check ??
// Set given pin in GPIOB to given value in ODR
// Param 1 - pin number
// param 2 - value [zero or non-zero]
// No expected retern value.
.global setn
setn:
    push    {lr}
    // Student code goes here
    if1:
    	cmp  r1,#0
    	beq  else1
	then1:			//Set pin n high
		ldr  r2,=GPIOB
		ldr  r1,[r2,#ODR]
		movs r3,#1
		lsls r3,r0
		orrs r1,r3
		str  r1,[r2,#ODR]
		b    endif1
	else1:
		ldr  r2,=GPIOB
		ldr  r1,[r2,#ODR]
		movs r3,#1
		lsls r3,r0
		bics r1,r3
		str  r1,[r2,#ODR]
		b	 endif1
	endif1:
	// ldr  r0,=GPIOB
	// ldr  r1,[r0,#ODR]
    // End of student code
    pop     {pc}

//==========================================================
// readpin: Autotest check ??
// read the pin given in param 1 from GPIOB_IDR
// Param 1 - pin to read
// No expected return value.
.global readpin
readpin:
    push    {lr}
    // Student code goes here
	movs r2,#1
	lsls r2,r0
	ldr  r3,=GPIOB
	ldr  r1,[r3,#IDR]
	ands r2,r1
	lsrs r2,r0
	movs r0,r2
    // End of student code
    pop     {pc}

//==========================================================
// buttons: Autotest check ??
// Check the pushbuttons and turn a light on or off as 
// described in section 2.6 of the lab
// No parameters.
// No return value
.global buttons
buttons:
    push    {lr}
    // Student code goes here
	movs r0,#0			// setn(8, readpin(0));
	bl   readpin
	movs r1,r0
	movs r0,#8
	bl   setn

	movs r0,#4			// setn(8, readpin(4));
	bl   readpin
	movs r1,r0
	movs r0,#9
	bl   setn
    // End of student code
    pop     {pc}

//==========================================================
// keypad: Autotest check ??
// Cycle through columns and check rows of keypad to turn
// LEDs on or off as described in section 2.7 of the lab
// No parameters.
// No expected return value.
.global keypad
keypad:
    push    {lr}
    // Student code goes here
    movs r1,#8			// c=8
	loop3:
		cmp  r1,#0		// c > 0
		ble  done3

		lsls r3,r1,#4	// c<<4
		ldr  r0,=GPIOC	// GPIOC->ODR = r0
		ldr  r2,[r0,#ODR]
		str  r3,[r0,#ODR]

		push {r1,r2}
		bl   mysleep
		pop  {r1,r2}
		ldr  r2,[r0,#IDR]
		movs r3,#0xf
		ands r2,r3		// r2 = GPIOC->IDR & 0xf

		if2:
			cmp  r1,#8
			bne  elseif2a
		then2:
			movs r0,#8
			push {r1}
			movs r3,#1
			ands r2,r3
			movs r1,r2
			bl   setn
			pop  {r1}
			b    endif2
		elseif2a:
			cmp  r1,#4
			bne  elseif2b
		elsethen2a:
			movs r0,#9
			push {r1}
			movs r3,#2
			ands r2,r3
			movs r1,r2
			bl   setn
			pop  {r1}
			b    endif2
		elseif2b:
			cmp  r1,#2
			bne  else
		elsethen2b:
			movs r0,#10
			push {r1}
			movs r3,#4
			ands r2,r3
			movs r1,r2
			bl   setn
			pop  {r1}
			b	 endif2
		else:
			movs r0,#11
			push {r1}
			movs r3,#8
			ands r2,r3
			movs r1,r2
			bl   setn
			pop  {r1}
			b    endif2
		endif2:

		lsrs r1,#1		// c>>1
	 	b    loop3
	done3:
    // End of student code
    pop     {pc}

//==========================================================
// mysleep: Autotest check ??
// a do nothing loop so that row lines can be charged
// as described in section 2.7 of the lab
// No parameters.
// No expected return value.
.global mysleep
mysleep:
    push    {lr}
    // Student code goes here
	movs r1,#0			// n = 0
	ldr  r2,=0x3e8
	loop4:
		cmp  r1,r2
		bge  done4
		adds r1,#1
		b	 loop4
	done4:
    // End of student code
    pop     {pc}

//==========================================================
// The main subroutine calls everything else.
// It never returns.
.global main
main:
        push {lr}
	bl   autotest // Uncomment when most things are working
	bl   initb
        bl   initc
// uncomment one loop when ready
loop1:
	bl   buttons
        b    loop1
loop2:
	bl   keypad
	b    loop2

        wfi
        pop {pc}

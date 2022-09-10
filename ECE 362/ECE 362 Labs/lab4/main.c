.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

//==================================================================
// ECE 362 Lab Experiment 4
// Interrupts
//==================================================================

// RCC config registers
.equ  RCC,      0x40021000
.equ  AHBENR,   0x14
.equ  GPIOCEN,  0x00080000
.equ  GPIOBEN,  0x00040000
.equ  GPIOAEN,  0x00020000
.equ  APB2ENR,  0x18
.equ  SYSCFGCOMPEN, 1

// GPIO config registers
.equ  GPIOC,    0x48000800
.equ  GPIOB,    0x48000400
.equ  GPIOA,    0x48000000
.equ  MODER,    0x00
.equ  PUPDR,    0x0c
.equ  IDR,      0x10
.equ  ODR,      0x14
.equ  BSRR,     0x18
.equ  BRR,      0x28

// SYSCFG config registers
.equ SYSCFG, 0x40010000
.equ EXTICR1, 0x08
.equ EXTICR2, 0x0c
.equ EXTICR3, 0x10
.equ EXTICR4, 0x14

// External interrupt config registers
.equ EXTI,  0x40010400
.equ IMR,   0
.equ EMR,   0x4
.equ RTSR,  0x8
.equ FTSR,  0xc
.equ SWIER, 0x10
.equ PR,    0x14

// Variables to register things for EXTI on pin 0
.equ EXTI_RTSR_TR0, 1<<0
.equ EXTI_IMR_MR0,  1<<0
.equ EXTI_PR_PR0,   1<<0
// Variables to register things for EXTI on pin 1
.equ EXTI_RTSR_TR1, 1<<1
.equ EXTI_IMR_MR1,  1<<1
.equ EXTI_PR_PR1,   1<<1
// Variables to register things for EXTI on pin 2
.equ EXTI_RTSR_TR2, 1<<2
.equ EXTI_IMR_MR2,  1<<2
.equ EXTI_PR_PR2,   1<<2
// Variables to register things for EXTI on pin 3
.equ EXTI_RTSR_TR3, 1<<3
.equ EXTI_IMR_MR3,  1<<3
.equ EXTI_PR_PR3,   1<<3
// Variables to register things for EXTI on pin 4
.equ EXTI_RTSR_TR4, 1<<4
.equ EXTI_IMR_MR4,  1<<4
.equ EXTI_PR_PR4,   1<<4

// SysTick counter variables....
.equ STK, 0xe000e010
.equ CSR, 0x0
.equ RVR, 0x4
.equ CVR, 0x8

// NVIC config registers
.equ NVIC, 0xe000e000
.equ ISER, 0x100
.equ ICER, 0x180
.equ ISPR, 0x200
.equ ICPR, 0x280
.equ IPR,  0x400
.equ EXTI0_1_IRQn,5  // External interrupt number for pins 0 and 1 is IRQ 5.
.equ EXTI2_3_IRQn,6  // External interrupt number for pins 2 and 3 is IRQ 6.
.equ EXTI4_15_IRQn,7 // External interrupt number for pins 4 - 15 is IRQ 7.

// GPIO config registers
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
// nano_wait
// Loop for approximately the specified number of nanoseconds.
// Write the entire subroutine below.
.global nano_wait
nano_wait:
    subs r0,#83
    bgt  nano_wait
    bx lr

//==========================================================
// initc
// Enable the RCC clock for GPIO C and configure pins as 
// described in section 2.2.1.
// Do not modify any other pin configuration.
// Parameters: none
// Write the entire subroutine below.
.global initc
initc:
	ldr  r0,=RCC			// Enabling RCC clock to GPIO C
	ldr  r1,[r0,#AHBENR]
	ldr  r2,=GPIOCEN
	orrs r1,r2
	str  r1,[r0,#AHBENR]

	ldr  r0,=GPIOC
	ldr  r1,[r0,#MODER]
	ldr  r2,=0xfffff	    // Clear PC0-PC9
	bics r1,r2
	ldr  r2,=0x55500	    // Set PC4-PC9 Output
	orrs r1,r2
	str  r1,[r0,#MODER]

	ldr  r1,[r0,#PUPDR]
	ldr  r2,=0xff
	bics r1,r2
	ldr  r2,=0xaa			// Set PC0-PC3 pull down
	orrs r1,r2
	str  r1,[r0,#PUPDR]
	bx   lr

//==========================================================
// initb
// Enable the RCC clock for GPIO B and configure pins as 
// described in section 2.2.2
// Do not modify any other pin configuration.
// Parameters: none
// Write the entire subroutine below.
.global initb
initb:
	ldr  r0,=RCC			// Enabling RCC clock to GPIO B
	ldr  r1,[r0,#AHBENR]
	ldr  r2,=GPIOBEN
	orrs r1,r2
	str  r1,[r0,#AHBENR]

	ldr  r0,=GPIOB
	ldr  r1,[r0,#MODER]
	ldr  r2,=0xFF03F3	    // Clear PC0,PC2-PC4,PC8-PC11
	bics r1,r2
	ldr  r2,=0x550000		// Set PC8-PC11 Output
	orrs r1,r2
	str  r1,[r0,#MODER]

	ldr  r1,[r0,#PUPDR]
	ldr  r2,=0xf0
	bics r1,r2
	ldr  r2,=0xa0			// Set PC0-PC3 pull down
	orrs r1,r2
	str  r1,[r0,#PUPDR]
	bx   lr

//==========================================================
// togglexn
// Change the ODR value from 0 to 1 or 1 to 0 for a specified
// pin of Port C.
// Parameters: r0 holds the base address of the GPIO port
//                to use
//             r1 holds the pin number to toggle
// Write the entire subroutine below.
.global togglexn
togglexn:
	ldr  r2,[r0,#ODR]
	movs r3,#1
	lsls r3,r1
	eors r2,r3
	str  r2,[r0,#ODR]
	bx   lr

//==========================================================
//  (autotest #??)
// Write the EXTI interrupt handler for pins 0 and 1 below.
// Copy the name from startup/startup_stm32.s, create a label
// of that name below, declare it to be global, and declare
// it to be a function.
// It acknowledge the pending bit for pin 0, and it should
// call togglexn(GPIOB, 8).
.global EXTI0_1_IRQHandler
.type EXTI0_1_IRQHandler, %function
EXTI0_1_IRQHandler:
	push {lr}
	ldr  r0,=EXTI			// Acknowledge pending bit for pin 0
	ldr  r1,=EXTI_PR_PR0
	str  r1,[r0,#PR]

	ldr  r0,=GPIOB			// Call togglexn(GPIOB, 8)
	movs r1,#8
	bl   togglexn
	pop  {pc}


//==========================================================
//  (autotest #??)
// Write the EXTI interrupt handler for pins 2-3 below.
// It should acknowledge the pending bit for pin2, and it
// should call togglexn(GPIOB, 9).
.global EXTI2_3_IRQHandler
.type EXTI2_3_IRQHandler, %function
EXTI2_3_IRQHandler:
	push {lr}
	ldr  r0,=EXTI			// Acknowledge pending bit for pin 2
	ldr  r1,=EXTI_PR_PR2
	str  r1,[r0,#PR]

	ldr  r0,=GPIOB			// Call togglexn(GPIOB, 9)
	movs r1,#9
	bl   togglexn
	pop  {pc}

//==========================================================
//  (autotest #??)
// Write the EXTI interrupt handler for pins 4-15 below.
// It should acknowledge the pending bit for pin4, and it
// should call togglxn(GPIOB, 10).
.global EXTI4_15_IRQHandler
.type EXTI4_15_IRQHandler, %function
EXTI4_15_IRQHandler:
	push {lr}
	ldr  r0,=EXTI			// Acknowledge pending bit for pin 4
	ldr  r1,=EXTI_PR_PR4
	str  r1,[r0,#PR]

	ldr  r0,=GPIOB			// Call togglexn(GPIOB, 10)
	movs r1,#10
	bl   togglexn
	pop  {pc}

//==========================================================
// init_exti  (autotest #??)
// (1) Enable the SYSCFG subsystem, and select Port B for
//     pins 0, 2, 3, and 4.
// (2) Configure the EXTI_RTSR register so that an EXTI
//     interrupt is generated on the rising edge of
//     pins 0, 2, 3, and 4.
// (3) Configure the EXTI_IMR register so that the EXTI
//     interrupts are unmasked for pins 2, 3, and 4.
// (4) Enable the three interupts for EXTI pins 0-1, 2-3 and 
//     4-15. Don't enable any other interrupts.
// Parameters: none
.global init_exti
init_exti:
	push {lr}
	// Student code goes below

	// (1)
	ldr  r0,=RCC			// Enable SYSCFGCOMPEN
	ldr  r1,[r0,#APB2ENR]
	ldr  r2,=SYSCFGCOMPEN
	orrs r1,r2
	str  r1,[r0,#APB2ENR]

	ldr  r0,=SYSCFG			// Port B for 0,2,3
	ldr  r1,[r0,#EXTICR1]
	ldr  r2,=0xff0f			// Clear 0,2,3
	bics r1,r2
	ldr  r2,=0x1101			// set 0,2,3 to Port B
	orrs r1,r2
	str  r1,[r0,#EXTICR1]

	ldr  r1,[r0,#EXTICR2]	// Port B for 4
	ldr  r2,=0xf			// Clear 4
	bics r1,r2
	ldr  r2,=0x1			// Set 4 to Port B
	orrs r1,r2
	str  r1,[r0,#EXTICR2]

	// (2)
	ldr  r0,=EXTI			// Set 0,2,3,4 to rising edge
	ldr  r1,[r0,#RTSR]
	ldr  r2,=EXTI_RTSR_TR0	// Set 0,2,3,4 rising
	orrs r1,r2
	ldr  r2,=EXTI_RTSR_TR2
	orrs r1,r2
	ldr  r2,=EXTI_RTSR_TR3
	orrs r1,r2
	ldr  r2,=EXTI_RTSR_TR4
	orrs r1,r2
	str  r1,[r0,#RTSR]

	// (3)
	ldr  r1,[r0,#IMR]		// Unmask 0,2,3,4
	ldr  r2,=EXTI_IMR_MR0	// Set 0,2,3,4 rising
	orrs r1,r2
	ldr  r2,=EXTI_IMR_MR2
	orrs r1,r2
	ldr  r2,=EXTI_IMR_MR3
	orrs r1,r2
	ldr  r2,=EXTI_IMR_MR4
	orrs r1,r2
	str  r1,[r0,#IMR]

	// (4)
    ldr  r2,=1<<EXTI0_1_IRQn
    ldr  r3,=1<<EXTI2_3_IRQn
    orrs r2,r3
    ldr  r3,=1<<EXTI4_15_IRQn
    orrs r2,r3
    ldr  r0,=NVIC
    ldr  r1,=ISER
    str  r2,[r0,r1]

	// Student code goes above
	pop  {pc}


//==========================================================
// set_col
// Set the specified column level to logic "high.
// Set the other three three columns to logic "low".
.global set_col
set_col:
	ldr  r3,=GPIOC			// GPIOC->BSRR = (0xf << (4 + 16));
	ldr  r1,=0xf
	lsls r1,#20
	str  r1,[r3,#BSRR]

	movs r1,#1				// GPIOC->BSRR = (1 << (8-col));
	movs r2,#8
	subs r2,r0
	lsls r1,r2
	str  r1,[r3,#BSRR]
	bx   lr

//==========================================================
// The current_col variable.
.data
.global current_col
current_col:
        .word 1
.text


//==========================================================
// SysTick_Handler
// The ISR for the SysTick interrupt.
//
.global SysTick_Handler
.type SysTick_Handler, %function
SysTick_Handler:
	push {lr}
	// Student code goes below
	ldr  r0,=GPIOC				// int row_val = GPIOC->IDR & 0xf;
	ldr  r1,[r0,#IDR]
	ldr  r2,=0xf
	ands r1,r2					// r1 = row_val

	// r0 = current_col, r1 = row_val
	if1:
		ldr  r0,=current_col
		ldr  r0,[r0]
		cmp  r0,#1
		bne  elseif1a
		ldr  r2,=0x8
		ands r2,r1
		cmp  r2,#0
		beq  elseif1a
	then1:
		push {r0,r1}
		ldr  r0,=GPIOB
		movs r1,#8
		bl   togglexn
		pop  {r0,r1}
		b    endif1
	elseif1a:
		cmp  r0,#2
		bne  elseif1b
		ldr  r2,=0x4
		ands r2,r1
		cmp  r2,#0
		beq  elseif1b
	then1a:
		push {r0,r1}
		ldr  r0,=GPIOB
		movs r1,#9
		bl   togglexn
		pop  {r0,r1}
		b    endif1
	elseif1b:
		cmp  r0,#3
		bne  elseif1c
		ldr  r2,=0x2
		ands r2,r1
		cmp  r2,#0
		beq  elseif1c
	then1b:
		push {r0,r1}
		ldr  r0,=GPIOB
		movs r1,#10
		bl   togglexn
		pop  {r0,r1}
		b    endif1
	elseif1c:
		cmp  r0,#4
		bne  endif1
		ldr  r2,=0x1
		ands r2,r1
		cmp  r2,#0
		beq  endif1
	then1c:
		push {r0,r1}
		ldr  r0,=GPIOB
		movs r1,#11
		bl   togglexn
		pop  {r0,r1}
		b    endif1
	endif1:

	adds r0,#1
	if2:
		cmp  r0,#4
		ble  endif2
	then2:
		movs r0,#1
		b	 endif2
	endif2:

	ldr  r3,=current_col
	str  r0,[r3]
	bl	 set_col

	// Student code goes above
	pop  {pc}

//==========================================================
// init_systick
// Enable the SysTick interrupt to occur every 0.5 seconds.
// Parameters: none
.global init_systick
init_systick:
	push {lr}
	// Student code goes below
	ldr  r3,=STK
	ldr  r0,=375000-1
	str  r0,[r3,#RVR]
	movs r0,#3
	str  r0,[r3,#CSR]
	// Student code goes above
	pop  {pc}


//==========================================================
// adjust_priorities
// Set the priority for EXTI pins 2-3 interrupt to 192.
// Set the priority for EXTI pins 4-15 interrupt to 128.
// Do not adjust the priority for any other interrupts.
.global adjust_priorities
adjust_priorities:
	push {lr}
	// Student code goes below

	// Student code goes above
	pop  {pc}

//==========================================================
// The main subroutine will call everything else.
// It will never return.
.global main
main:
	bl autotest // Uncomment when most things are working
	bl initb
	bl initc
	bl init_exti
	bl init_systick
	bl adjust_priorities

endless_loop:
	ldr  r0,=GPIOC
	movs r1,#9
	bl   togglexn
	ldr  r0,=500000000
	bl   nano_wait
	b    endless_loop

//============================================================================
// ECE 362 Lab Experiment 5
// Basic Timers
//============================================================================
.cpu cortex-m0
.thumb
.syntax unified

// RCC configuration registers
.equ  RCC,      0x40021000
.equ  AHBENR,   0x14
.equ  GPIOCEN,  0x00080000
.equ  GPIOBEN,  0x00040000
.equ  GPIOAEN,  0x00020000
.equ  APB1ENR,  0x1c
.equ  TIM6EN,   1<<4
.equ  TIM7EN,   1<<5

// NVIC configuration registers
.equ NVIC, 0xe000e000
.equ ISER, 0x100
.equ ICER, 0x180
.equ ISPR, 0x200
.equ ICPR, 0x280
.equ IPR,  0x400
.equ TIM6_DAC_IRQn, 17
.equ TIM7_IRQn,     18

// Timer configuration registers
.equ TIM6, 0x40001000
.equ TIM7, 0x40001400
.equ TIM_CR1,  0x00
.equ TIM_CR2,  0x04
.equ TIM_DIER, 0x0c
.equ TIM_SR,   0x10
.equ TIM_EGR,  0x14
.equ TIM_CNT,  0x24
.equ TIM_PSC,  0x28
.equ TIM_ARR,  0x2c

// Timer configuration register bits
.equ TIM_CR1_CEN,  1<<0
.equ TIM_DIER_UDE, 1<<8
.equ TIM_DIER_UIE, 1<<0
.equ TIM_SR_UIF,   1<<0

// GPIO configuration registers
.equ  GPIOC,    0x48000800
.equ  GPIOB,    0x48000400
.equ  GPIOA,    0x48000000
.equ  MODER,    0x00
.equ  PUPDR,    0x0c
.equ  IDR,      0x10
.equ  ODR,      0x14
.equ  BSRR,     0x18
.equ  BRR,      0x28

//============================================================================
// void enable_ports(void) {
//   // Set up the ports and pins exactly as directed.
// }
.global enable_ports
enable_ports:
	// Enable RCC clock to GPIOB and GPIOC
	ldr  r0,=RCC
	ldr  r1,[r0,#AHBENR]
	ldr  r3,=GPIOBEN
	ldr  r2,=GPIOCEN
	orrs r2,r3
	orrs r1,r2
	str  r1,[r0,#AHBENR]

	// Configure pins PB0-PB10 outputs 0x155555
	ldr  r0,=GPIOB
	ldr  r1,[r0,#MODER]
	ldr  r2,=0x3fffff			// clear P0-10
	bics r1,r2
	ldr  r2,=0x155555
	orrs r1,r2
	str  r1,[r0,#MODER]

	// Configure pins PC4-PC7 and PC9 outputs 0x45500
	ldr  r0,=GPIOC
	ldr  r1,[r0,#MODER]
	ldr  r2,=0xcffff			// clear P4-7 and 9 (also set P0-3 input)
	bics r1,r2
	ldr  r2,=0x45500
	orrs r1,r2
	str  r1,[r0,#MODER]

	// Configure pins PC0-PC3 pulled low
	ldr  r1,[r0,#PUPDR]
	ldr  r2,=0xff
	bics r1,r2
	ldr  r2,=0xaa			// Set PC0-PC3 pull down
	orrs r1,r2
	str  r1,[r0,#PUPDR]
	bx   lr

//============================================================================
// TIM6_ISR() {
//   TIM6->SR &= ~TIM_SR_UIF
//   if (GPIOC->ODR & (1<<9))
//     GPIOC->BRR = 1<<9;
//   else
//     GPIOC->BSRR = 1<<9;
// }
.global TIM6_DAC_IRQHandler
.type TIM6_DAC_IRQHandler, %function
TIM6_DAC_IRQHandler:
	push {lr}
	ldr  r0,=TIM6			// Acknowledge interrupt
	ldr  r1,[r0,#TIM_SR]
	ldr  r2,=TIM_SR_UIF
	bics r1,r2
	str  r1,[r0,#TIM_SR]

	if1:
		ldr  r0,=GPIOC
		ldr  r1,[r0,#ODR]
		ldr  r2,=1<<9
		ands r1,r2
		cmp  r1,#0
		beq  else1
	then1:
		str  r2,[r0,#BRR]
		b	 endif1
	else1:
		str  r2,[r0,#BSRR]
		b    endif1
	endif1:

	pop  {pc}

//============================================================================
// Implement the setup_tim6 subroutine below.  Follow the instructions in the
// lab text.
.global setup_tim6
setup_tim6:
	// Enable RCC to tim6
	ldr  r0,=RCC
	ldr  r1,[r0,#APB1ENR]
	ldr  r2,=TIM6EN
	orrs r1,r2
	str  r1,[r0,#APB1ENR]

	// Set PSC to 47999 and ARR to 499
	ldr  r0,=TIM6
	ldr  r1,=48000-1
	str  r1,[r0,#TIM_PSC]

	ldr  r1,=500-1
	str  r1,[r0,#TIM_ARR]

	// Enable UIE in DIER
	ldr  r1,[r0,#TIM_DIER]
	ldr  r2,=TIM_DIER_UIE
	orrs r1,r2
	str  r1,[r0,#TIM_DIER]

	// Enable CEN bit in Tim6
	ldr  r1,[r0,#TIM_CR1]
	ldr  r2,=TIM_CR1_CEN
	orrs r1,r2
	str  r1,[r0,#TIM_CR1]

	// Unmask interrupt
	ldr  r0,=NVIC
	ldr  r1,=ISER
	ldr  r2,=(1<<TIM6_DAC_IRQn)
	str  r2,[r0,r1]
	bx   lr

//============================================================================
// void show_char(int col, char ch) {
//   GPIOB->ODR = ((col & 7) << 8) | font[ch];
// }
.global show_char
show_char:
	ldr  r2,=7
	ands r0,r2			// r0 = r0 | 7

	ldr  r2,=font		// r3 = font[r1]
	ldrb r3,[r2,r1]
	lsls r0,#8
	orrs r0,r3			// r0 = (r0<<8) | font[r1]
	ldr  r3,=GPIOB
	// ldr  r1,[r3,#ODR]
	// orrs r1,r0
	str  r0,[r3,#ODR]
	bx   lr


//============================================================================
// nano_wait(int x)
// Wait the number of nanoseconds specified by x.
.global nano_wait
nano_wait:
	subs r0,#83
	bgt nano_wait
	bx lr

//============================================================================
// This subroutine is provided for you to fill the LED matrix with "AbCdEFgH".
// It is a very useful subroutine for debugging.  Study it carefully.
.global fill_alpha
fill_alpha:
	push {r4,r5,lr}
	movs r4,#0
fillloop:
	movs r5,#'A' // load the character 'A' (integer value 65)
	adds r5,r4
	movs r0,r4
	movs r1,r5
	bl   show_char
	adds r4,#1
	movs r0,#7
	ands r4,r0
	ldr  r0,=1000000
	bl   nano_wait
	b    fillloop
	pop {r4,r5,pc} // not actually reached

//============================================================================
// void drive_column(int c) {
//   c = c & 3;
//   GPIOC->BSRR = 0xf00000 | (1 << (c + 4));
// }
.global drive_column
drive_column:
	movs r1,#3
	ands r0,r1

	ldr  r1,=0xf00000
	adds r0,#4
	ldr  r2,=1
	lsls r2,r0
	orrs r1,r2

	ldr  r2,=GPIOC
	str  r1,[r2,#BSRR]
	bx   lr

//============================================================================
// int read_rows(void) {
//   return GPIOC->IDR & 0xf;
// }
.global read_rows
read_rows:
	ldr  r0,=GPIOC
	ldr  r1,[r0,#IDR]
	ldr  r2,=0xf
	ands r1,r2
	movs r0,r1
	bx   lr

//============================================================================
// void update_history(int c, int r) {
//   c = c & 3;
//   for(int n=0; n<4; n++) {
//     history[4*c + n] <<= 1;
//     history[4*c + n] |= (r>>n) & 1;
//   }
// }
.global update_history
update_history:
	push {r4-r7, lr}
	movs r2,#3
	ands r0,r2				// c = c & 3

	movs r2,#0				// n = 0
	for1:
		movs r3,#4			// n<4
		cmp  r2,r3
		bge  done1

		movs r4,#4			// r4 = 4*c + n
		muls r4,r0
		adds r4,r2

		ldr  r5,=hist		// history[4*c + n] <<= 1
		ldrb r6,[r5,r4]
		lsls r6,#1
		strb r6,[r5,r4]

		ldrb r6,[r5,r4]		// r7 = (r>>n) & 1
		movs r7,r1
		lsrs r7,r2
		movs r3,#1
		ands r7,r3
		orrs r6,r7
		strb r6,[r5,r4]

		adds r2,#1			// n++
		b    for1
	done1:
	pop {r4-r7, pc}


//============================================================================
// TIM7_ISR() {
//    TIM7->SR &= ~TIM_SR_UIF
//    update_history(col);
//    show_char(col, disp[col])
//    col = col + 1;
//    drive_column(col);
// }
.global TIM7_IRQHandler
.type TIM7_IRQHandler, %function
TIM7_IRQHandler:
	push {lr}

	ldr  r0,=TIM7			// Acknowledge interrupt
	ldr  r1,[r0,#TIM_SR]
	ldr  r2,=TIM_SR_UIF
	bics r1,r2
	str  r1,[r0,#TIM_SR]

	bl   read_rows
	movs r1,r0

	ldr  r0,=col			// r0 = col
	ldrb r0,[r0]

	push {r0}
	bl   update_history		// update_history(col)
	pop  {r0}

	ldr  r1,=disp			// r1 = disp[col]
	ldrb r1,[r1,r0]

	push {r0}
	bl   show_char			// show_char(col, disp[col])
	pop  {r0}

	ldr  r1,=col			// col = (col + 1) & 7
	adds r0,#1
	movs r2,#7
	ands r0,r2
	strb r0,[r1]

	bl   drive_column		// drive_column(col)

	pop  {pc}


//============================================================================
// Implement the setup_tim7 subroutine below.  Follow the instructions
// in the lab text.
.global setup_tim7
setup_tim7:
	// Enable RCC to tim7
	ldr  r0,=RCC
	ldr  r1,[r0,#APB1ENR]
	ldr  r2,=TIM7EN
	orrs r1,r2
	str  r1,[r0,#APB1ENR]

	// Set PSC to 4799 and ARR to 9
	ldr  r0,=TIM7
	ldr  r1,=4800-1
	str  r1,[r0,#TIM_PSC]

	ldr  r1,=10-1
	str  r1,[r0,#TIM_ARR]

	// Enable UIE in DIER
	ldr  r1,[r0,#TIM_DIER]
	ldr  r2,=TIM_DIER_UIE
	orrs r1,r2
	str  r1,[r0,#TIM_DIER]

	// Unmask interrupt
	ldr  r0,=NVIC
	ldr  r1,=ISER
	ldr  r2,=(1<<TIM7_IRQn)
	str  r2,[r0,r1]

	// Enable CEN bit in Tim7
	ldr  r0,=TIM7
	ldr  r1,[r0,#TIM_CR1]
	ldr  r2,=TIM_CR1_CEN
	orrs r1,r2
	str  r1,[r0,#TIM_CR1]

	bx   lr

//============================================================================
// int wait_for(char val) {
//   for(;;) {
//     wfi
//     for(int n=0; n<16; n++)
//       if (hist[n] == val)
//         return n;
//   }
// }
.global wait_for
wait_for:
	forever_loop:
	wfi
	movs r1,#0				// n = 0
	for2:
		movs r2,#16			// n < 16
		cmp  r1,r2
		bge  done2

		if2:
			ldr  r2,=hist
			ldrb r2,[r2,r1]
			cmp  r2,r0
			bne  endif2
		then2:
			movs r0,r1
			b    exit_forever_loop
		endif2:

		adds r1,#1			// n++
		b    for2
	done2:
	b    forever_loop
	exit_forever_loop:
	bx   lr

//============================================================================
// void shift_display() {
//   for(int i=0; i<7; i++)
//     disp[i] = disp[i+1];
// }
.global shift_display
shift_display:
	movs r1,#0				// i = 0
	for3:
		movs r2,#7				// i < 7
		cmp  r1,r2
		bge  done3

		ldr  r0,=disp
		movs r3,r1
		adds r3,#1
		ldrb r2,[r0,r3]			// r2 = disp[i+1]
		strb r2,[r0,r1]			// disp[i] = disp[i+1]

		adds r1,#1				// i++
		b    for3
	done3:
	bx   lr

//============================================================================
// This subroutine is provided for you to call the functions you wrote.
// It waits for a key to be pressed.  When it finds one, it immediately
// shifts the display left, looks up the character for the key, and writes
// the new character in the rightmost element of disp.
// Then it waits for a key to be released.
.global display_key
display_key:
	push {r4,lr}
	movs r0,#1
	bl   wait_for

	movs r4,r0
	bl   shift_display
	ldr  r0,=disp
	ldr  r2,=keymap
	ldrb r1,[r2,r4]
	strb r1,[r0,#7]

	movs r0,#0xfe
	bl   wait_for

	pop {r4,pc}

.global login
login: .string "subbiah" // Replace with your login.
.balign 2

.global main
main:
	bl autotest
	bl enable_ports
	bl setup_tim6
	// bl fill_alpha
	bl setup_tim7
display_loop:
	bl display_key
	nop
	b  display_loop
	// Does not return.


//============================================================================
// Map the key numbers in the history array to characters.
// We just use a string for this.
.global keymap
keymap:
.string "DCBA#9630852*741"

//============================================================================
// This table is a *font*.  It provides a mapping between ASCII character
// numbers and the LED segments to illuminate for those characters.
// For instance, the character '2' has an ASCII value 50.  Element 50
// of the font array should be the 8-bit pattern to illuminate segments
// A, B, D, E, and G.  Spread out, those patterns would correspond to:
//   .GFEDCBA
//   01011011 = 0x5b
// Accessing the element 50 of the font table will retrieve the value 0x5b.
//
.global font
font:
.space 32
.byte  0x00 // 32: space
.byte  0x86 // 33: exclamation
.byte  0x22 // 34: double quote
.byte  0x76 // 35: octothorpe
.byte  0x00 // dollar
.byte  0x00 // percent
.byte  0x00 // ampersand
.byte  0x20 // 39: single quote
.byte  0x39 // 40: open paren
.byte  0x0f // 41: close paren
.byte  0x49 // 42: asterisk
.byte  0x00 // plus
.byte  0x10 // 44: comma
.byte  0x40 // 45: minus
.byte  0x80 // 46: period
.byte  0x00 // slash
.byte  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07
.byte  0x7f, 0x67
.space 7
// Uppercase alphabet
.byte  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x6f, 0x76, 0x30, 0x1e, 0x00, 0x38, 0x00
.byte  0x37, 0x3f, 0x73, 0x7b, 0x31, 0x6d, 0x78, 0x3e, 0x00, 0x00, 0x00, 0x6e, 0x00
.byte  0x39 // 91: open square bracket
.byte  0x00 // backslash
.byte  0x0f // 93: close square bracket
.byte  0x00 // circumflex
.byte  0x08 // 95: underscore
.byte  0x20 // 96: backquote
// Lowercase alphabet
.byte  0x5f, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x6f, 0x74, 0x10, 0x0e, 0x00, 0x30, 0x00
.byte  0x54, 0x5c, 0x73, 0x7b, 0x50, 0x6d, 0x78, 0x1c, 0x00, 0x00, 0x00, 0x6e, 0x00
.balign 2

//============================================================================
// Data structures for this experiment.
// Guard bytes are placed between variables so that autotest can (potentially)
// detect corruption caused by bad updates.
//
.data
.global col
.global hist
.global disp
guard1: .byte 0
disp: .string "Hello..."
guard2: .byte 0
col: .byte 0
guard3: .byte 0
hist: .byte 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
guard4: .byte 0

.cpu cortex-m0
.thumb
.syntax unified
.fpu softvfp

.data
.balign 4
// Your global variables go here
.global arr
arr: .word 23, 11, 23, 27, 19, 14, 13, 13, 13, 12, 17, 24, 21, 16, 19, 22
.global value
value: .word 0
.global str
str: .string "jKfFgP+ , oGuUcIgU - 01234 Kp . VjKu % UvTkPi!"

.text
.global intsub
intsub:
    // Your code for intsub goes here
    movs r1, #0             // i = 0
    loop1:
        cmp  r1,#15          // i < 15
        bge  done1

        if1:
            ldr  r0,=value   // value >> 4
            ldr  r0,[r0]
            lsrs r0,r0,#4
            cmp  r0,#5       // (value >> 4) < 5
            bge  else1
        then1:
            ldr  r0,=arr     // r0 = arr[i]
            lsls r3,r1,#2
            ldr  r0,[r0,r3]
            movs r2,#2       // r0 = 2 * arr[i]
            muls r0,r0,r2
            ldr  r2,=arr     // r2 = arr[i+1]
            movs r3,r1
            adds r3,#1
            lsls r3,r3,#2
            ldr  r2,[r2,r3]
            adds r0,r0,r2    // r0 = 2 * arr[i] + arr[i+1]
            ldr  r3,=value
            ldr  r2,[r3]
            adds r2,r2,r0    // r2 = value + r0
            str  r2,[r3]
            b    endif1
        else1:
            ldr  r0,=arr     // r0 = arr[i]
            lsls r3,r1,#2
            ldr  r0,[r0,r3]
            movs r2,#3
            muls r0,r0,r2    // r0 = 3 * arr[i]
            ldr  r3,=value
            ldr  r2,[r3]
            subs r2,r2,r0    // r2 = r2 - r0
            str  r2,[r3]
        endif1:
        ldr  r0,=arr     // r0 = arr[i]
        lsls r3,r1,#2
        ldr  r0,[r0,r3]
        movs r2,#2       // r0 = 2 * arr[i]
        muls r0,r0,r2
        ldr  r2,=arr     // r2 = arr[i+1]
        movs r3,r1
        adds r3,#1
        lsls r3,r3,#2
        ldr  r2,[r2,r3]
        adds r0,r0,r2    // r0 = 2 * arr[i] + arr[i+1]

        ldr  r2,=arr   // arr[i] = r0
		lsls r3,r1,#2
		str  r0,[r2,r3]

        adds r1,#1      // i++
        b    loop1
    done1:
    // You must terminate your subroutine with bx lr
    // unless you know how to use PUSH and POP.
    bx lr



.global charsub
charsub:
    // Your code for charsub goes here
    movs r1,#0             // x = 0
    loop2:
        ldr  r0,=str       // r0 = str[x]
        // lsls r3,r1,#2
        ldrb r0,[r0,r1]
        cmp  r0,#0         // str[x] != 0
        beq  done2

        if2a:
        	ldr  r0,=str     // r0 = str[x]
        	// lsls r3,r1,#2
        	ldrb r0,[r0,r1]
        	cmp  r0,#0x00000061    // r0 >= 'a'
        	blt  endif2
        if2b:
        	ldr  r0,=str     // r0 = str[x]
        	// lsls r3,r1,#2
        	ldrb r0,[r0,r1]
        	cmp  r0,#0x0000007a    // r0 <= 'z'
        	bgt  endif2
        then2:
        	ldr  r0,=str     // r0 = str[x]
        	// lsls r3,r1,#2
        	ldrb r0,[r0,r1]
        	movs r2,#0x00000020
        	bics r0,r2       // r0 = r0 & ~0x20
        	ldr  r2,=str     // str[x] = r0
        	// lsls r3,r1,#2
        	strb r0,[r2,r1]
        endif2:

        if3a:
        	ldr  r0,=str     // r0 = str[x]
        	// lsls r3,r1,#2
        	ldrb r0,[r0,r1]
        	cmp  r0,#0x00000041    // r0 >= 'A'
        	blt  endif3
        if3b:
        	ldr  r0,=str     // r0 = str[x]
        	// lsls r3,r1,#2
        	ldrb r0,[r0,r1]
        	cmp  r0,#0x0000005a    // r0 <= 'Z'
        	bgt  endif3
        then3:
        	ldr  r0,=str     // r0 = str[x]
        	// lsls r3,r1,#2
        	ldrb r0,[r0,r1]
        	subs r0,#2       // r0 = str[x] - 2
        	ldr  r2,=str     // str[x] = r0
        	// lsls r3,r1,#2
        	strb r0,[r2,r1]
        endif3:

        adds r1,#1         // x++
    	b    loop2
    done2:
    // You must terminate your subroutine with bx lr
    // unless you know how to use PUSH and POP.
    bx lr


.global login
login: .string "subbiah" // Make sure you put your login here.
.balign 2
.global main
main:
    bl autotest // uncomment AFTER you debug your subroutines
    bl intsub
    bl charsub
    bkpt

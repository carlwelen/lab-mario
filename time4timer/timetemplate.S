# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# PUSH / POP macros.                                        #
#                                                           #
# RARS and the GNU assembler (used for the DTEK-V board)    #
# use slightly different macro syntax. The file ships with  #
# the RARS-style block active, because Assignments 1-4 are  #
# done in RARS. When you port to the board in Assignment 5, #
# comment out the RARS block below and uncomment the DTEK   #
# block beneath it.                                         #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp)
#.end_macro
#
#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp)
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	# delay is declared global so the auto-grader's test harness (which
	# calls delay directly from a small labmain.S) can resolve the symbol.
	.globl timetemplate, tick, time2string, delay, siffror, display_string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 2		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hexasc, delay. #
#########################################################

time2string:

	PUSH ra
	PUSH s0
	PUSH s1
	
	add s0, zero, a0 #addressen
	add s1, zero, a1 #värdena
	
	li t0, 0x3a
	li t1, 0x00
	
	sb t1, 5(s0)
	sb t0, 2(s0)  # (X)(X)(:)(X)(X)(\0)
	
	# a0 = s0;
	
	add a0, zero, s1
	
	jal hexasc
	sb a0, 4(s0)
	
	srli a0, s1, 4 # första shiften
	
	jal hexasc
	sb a0, 3(s0)
	
	srli a0, s1, 8 # andra shiften
	
	jal hexasc
	sb a0, 1(s0)
	
	srli a0, s1, 12 # tredje shiften
	
	jal hexasc
	sb a0, 0(s0)
	
	POP s1
	POP s0
	POP ra
	
	jr ra
	
hexasc:
  	andi a0, a0, 0x0F
  	li t0, 9
  	ble a0, t0, siffror
  	
  	addi a0, a0, 0x37
  	
  	jr ra
  	
siffror:
  	addi a0, a0, 0x30
  	
  	jr ra
  	
delay:
	PUSH ra
	
	loop:
		addi a0, a0, -1
		li t6, 5239437
		
		loop2:
			addi t6, t6, -1
			ble zero, t6, loop2
			
	ble zero, a0, loop
		
	POP ra
jr ra
	


	

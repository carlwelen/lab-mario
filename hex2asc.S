  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	# Declare hexasc global too so it can be called from another file
	# (e.g. timetemplate.S, or the auto-grader's test harness).
	.globl hex2asc, hexasc

hex2asc:
	li	a0, 11		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  #
  hexasc:
  	andi a0, a0, 0x0F
  	li t0, 9
  	ble a0, t0, siffror
  	
  	addi a0, a0, 0x37
  	
  	jr ra
  	
  siffror:
  	addi a0, a0, 0x30
  	
  	jr ra
  	
  	
  	
  	
  	
  
  
  
  

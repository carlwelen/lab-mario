# main.S
# Written by W Szczerek
# Copyright abandonded - this file is in the public domain.

######################################################################################
# This file is only a "hub" for your code to run in. You can reference the functions #
# that you wrote in the other files here.                                            #
######################################################################################

.globl main, handle_interrupt
.text

	
# You can select which one of the assignments to run by uncommenting the apporpriate
# call.
main:
    # Uncomment below for assignment 1
    # jal analyze

    # Uncomment below for assignment 2
    # jal hex2asc
	
    #U Uncomment below for assignment 3 & 6
    jal timetemplate

loop: j loop

# Below is unused in lab1
handle_interrupt:
    jr ra
	

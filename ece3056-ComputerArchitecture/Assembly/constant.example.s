		# this program builds the constant 0x78965432 in register $t4
		.text
		.globl main
main: 	add $t4, $0, $0 	#initialize $t4 to 0
		lui $t4, 0x7896 	#load 0x7896 into the upper
				#16 bits of $t4
		ori $t4, $t4, 0x5432 	# perform the logical OR
				#between $t0 and the value
				#0x5432
		li $v0, 10 # code for program end
		syscall

# this program performs various logical operations

.data
label: 	.word 0x0f000000
label1: 	.word 0xffffffff
	.text
	.globl main
main:	lw $t0, label($0)
	lw $t1, label1($0)
	and $t2, $t0, $t1 	    # should produce 0x0f000000 in $t2
	andi $t3, $t1, 0xffff 	# produce 0xffffffff & 0x0000ffff
	srl $t2, $t2, 12 		# shift 12 logical (3 places in hex)
	move $t0, $0 		    # initialize $t0 to 0
	lui $t0, 0x0f00 		# store a 32-bit constant into $t0
	andi $t0, 0x0000

	li $v0, 10 		# program termination
	syscall

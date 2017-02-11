# example to sum the four bytes stored at the second word
# in  the data segment and store the result at the location
# marked by label L2

	    .data
L1:     .word 0x22334455, 0xaabbccdd, 0xff
L2:	    .space 4
        .text
        .globl main
main:   la $s0, L1
        addi $s0, $s0, 4	# $s0 contains the starting address of the 2nd word
	    la $s1, L2		    # $s1 contains the starting address of the to-be stored sum
	    lb $s3, 0($s0)
    	lb $s4, 1($s0)
    	lb $s5, 2($s0)
    	lb $s6, 3($s0)
    	add $s3, $s3, $s4
    	add $s3, $s3, $s5
    	add $s3, $s3, $s6	# Addition complete, result in $s3
    	sw $s3, 0($s1)		# Store at location L2

exit: 	li $v0, 10			#exit program
    	syscall

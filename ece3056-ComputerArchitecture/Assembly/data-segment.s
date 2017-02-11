# simple example of structuring the
# data segment

	.data
L1: 	.word 0x1234 	# some arbitrary value
L2: 	.word 0x4567	# some arbitrary value
Res:	.space 4	    # reserve 4 bytes

	.asciiz "CmpE 2030"	# store an ascii string
	.byte 0x11		    # arbitrary byte value
str:	.ascii "CmpE 2030"	# store without the null termination character
	.byte 0x22

	.align 2		            # move back to a word boundary
	.byte 0x33, 21, 0x99		# arbitrary numbers

	.align 4		    # move to a 16 byte boundary
	.byte 0x44
	.align 3
	.byte 0x55
	.align 4
	.byte 0x66
row1:	.word 11, 12, 13	# first row of the matrix
row2:	.word 21, 22, 23    # second row of the matrix
row3:	.word 31, 32, 33	# third row of the matrix

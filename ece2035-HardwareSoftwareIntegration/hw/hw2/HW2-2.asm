# HW2-2
# Student Name: Kairi Kozuma
# Date:
#
# 			Letter Histogram
#
# This program computes the letter histogram of the characters encoded as
# packed ascii codes, starting at label Text.  The histogram contains the
# number of times the ascii code for each letter of the alphabet
# (upper/lower case) occurs in the Text.  The histogram is computed and
# stored beginning at the label Letters w/ the frequency count for the
# letter 'A', then 'B', etc.
#

.data
Letters: .word   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

# These are the ascii codes for the first 256 characters in address.txt,
# packed into 64 four-byte (32-bit) words.  To view the ascii codes more
# easily, select the hexadecimal display base under Options in MiSaSiM.
Text:	.word	543516756, 1953785159, 1969386361, 1092642674, 1701995620,  168457075, 1310734641, 1835365999
	.word	 544367970,  859191345, 1866861066, 1668510325,  543519343,  543452769, 1702258035, 1702436974
	.word	 544436833,  544171873,  544372079, 1752457574,  544436837, 1970238050,  544499815, 1953656678
	.word	1965042792,  544108400, 1936287860, 1852793632, 1701734772,  170685550, 1701716065, 1634607223
	.word	1852795252, 1868767276, 1768252270,  543450486, 1814064745, 1919246953, 1629518196, 1679844462
	.word	1667851365, 1684370529,  544175136,  543516788, 1886351984, 1953067887,  544108393, 1952540788
	.word	1818305056, 1701644908, 1918967918, 1919098981, 1702125925, 1902452836,  778854773, 1309280802
	.word	1998616431, 1918967909, 1852121189, 1701273959, 1852383332, 1730175264, 1952540018, 1986618144
End:	.word	0

# $1: Byte value
# $2: Byte shift
# $4: End is reached predicate
# $5: Store letter count
# $6: Current letter
# $11: Less than or equal to 91 (Z) predicate
# $12: Less than or equal to 65 (A) predicate
# $31: Caller

.text
	# write your code here...
		add $2, $0, $0			# Initialize byte shift
		lb $1, Text($0)			# Load first byte
		addiu $7, $0, Text		# Initialize instruction start location
		addiu $8, $0, End		# Initialize end location
		sub $9, $8, $7			# Initialize increment number
Loop:	slt $4, $2, $9			# If End is reached
		beq $4, $0, Finish		# Then exit loop
		slti $12, $1, 65		# Else if not letter character
		bne $12, $0, Skip		# Then Skip to end of loop
		slti $11, $1, 91		# Else if letter is not uppercase
		bne $11, $0, Count		# Then count up
		addiu $1, $1, -32		# Else convert to lowercase 
Count:	addiu $6, $1, -65		# Letters start from 0 by location
		sll $6, $6, 2			# Multiply location by 4 bytes for 1 word
		lw $5, Letters($6)		# Load value into letter
		addi $5, $5, 1			# Increment count by 1
		sw $5, Letters($6)		# Store value into letter
Skip:	addi $2, $2, 1			# Shift 1 byte
		lb $1, Text($2)			# Load next byte
	 	j Loop					# Return to loop
Finish:	jr      $31				# return to operating system
	


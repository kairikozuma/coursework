# This program computes the factorial of input stored in memory at label Input. 
# The result is stored in memory at the location following the input.
# $1: n
# $2: running product (n!)
# $3: predicate register
# $4: address of input
.data
Input: .word 5
       .alloc 1
.text
Fact:	addi  $4, $0, Input	# put address of input in $4
	lw    $1, 0($4)		# read input into $1
	addi  $2, $0, 1		# initialize output to 1
Loop:	slti  $3, $1, 2		# if input is less than 2
	bne   $3, $0, Skip	# then skip to return
	mult  $1, $2		# else multiply input and running product
	mflo  $2		# assume small numbers
	addi  $1, $1, -1	# decrement input
	j Loop			# and loop
Skip:   sw    $2, 4($4)         # store result in word following input
	jr    $31		# return to caller


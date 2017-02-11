# Alternative factorial: input read in from label Input; result stored at label Output.
# $1: n
# $2: running product (n!)
# $3: predicate register
# $4: address of input
.data
Input: .word 5
Output:	.alloc 1
.text
Fact:	lw    $1, Input($0)	# read input into $1
	addi  $2, $0, 1		# initialize output to 1
Loop:	slti  $3, $1, 2		# if input is less than 2
	bne   $3, $0, Skip	# then skip to return
	mult  $1, $2		# else multiply input and running product
	mflo  $2		# assume small numbers
	addi  $1, $1, -1	# decrement input
	j Loop			# and loop
Skip:   sw    $2, Output($0)    # store result in word following input
	jr    $31		# return to caller


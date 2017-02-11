# HW1-3
# Student Name: Kairi Kozuma
# Date: 01/23/15
#
# This program computes and prints the span of a set of ten integers Set.

.data
Set:  .word 9, 17, -7, 3, -27, 25, 42, 26, 8, -60
Span: .alloc 1
Max: .alloc 1
Min: .alloc 1

# $1: Element value
# $2: Max
# $3: Min
# $5: Span
# $7: Max greater predicate
# $8: Min smaller predicate
# $11: Number of elements
# $12: Incrementer
# $13: Loop predicate

.text
		addi $11, $0, 10		# Initialize number of elements
		addi $12, $0, 4			# Initialize increment (skip first element)
		lw $2, Set($0)			# Initialize Max
		lw $3, Set($0)			# Intialize Min
Loop:	slti $13, $12, 40		# If $12 < 40 is false
		beq $13, $0, End		# Then exit loop
		lw $1, Set($12)			# Else load element to $1
		addi $12, $12, 4		# Increment incrementer by 1 word
		slt $7, $1, $2 		    # If element is less than Max
		bne $7, $0, Mintst		# Then skip to Mintst
		add $2,	$1, $0			# Else set Max to element
Mintst: slt $8, $3, $1			# If element is greater than Min
		bne $8, $0, Skip		# Then skip to end of loop
		add $3, $1, $0			# Else set Min to element
Skip:   j Loop					# Return to loop
End:	sub $5, $2, $3			# $5: $2 - $3 (Span = Max - Min)
		sw $5, Span($0)			# Store Span in memory
		jr $31	     			# return to OS



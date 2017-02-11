# P1-2
# Student Name: Kairi Kozuma
# Date:	02-14-15
#
#     I c o n   M a t c h
#
#
# This routine determines which of eight candidate icons matches a pattern icon.
# 1.0: Obtains correct answer
# 2.0: Starts at first nonzero value of pattern before comparison
# 3.0: Reduce instructions required
# 4.0: Use register for flag vector instead of memory
# 4.1: Change start element to element 36
# 4.2: Reduce number of registers used

.data
CandBase: 		.alloc 1152
PatternBase:	.alloc 144

.text
IconMatch: addi	$1, $0, CandBase     	# point to base of Candidates
		swi		584						# generate puzzle icons	

		# $2: Candidate Guess (Index of current candidate)
		# $3: Current Element of Pattern Being Checked
		# $4: Current Element of Candidate
		# $5: Index for Current Element of Pattern
		# $7: Index for Current Element of Candidate
		# $8: End condition for Candidate Loop
		# $9: Predicate: elements are equal
		# $10: Flag predicate that tells which candidates to skip
		# $11: Predicate: End condition for comparison
		# $12: Number of elements in pattern (144)
		# $13: Number of candidates ruled out
		# $15: Flag predicate vector that tells which candidates to skip
		# $16: Flag vector for candidate validity
		
		
		addi	$5, $0, 144				# Initialize index of pattern element (element 37)
		addi	$2, $0, 0 				# Initialize index of candidate element (first puzzle)
		addi 	$8, $0, 8				# Initialize end condition (beyond last candidate)
		addi 	$10, $0, 0				# Initialize flag for element
		addi 	$12, $0, 576			# Initialize offset of number of elements
		addi 	$13, $0, 7				# Initialize number of candidates
		addi	$15, $0, 255			# Initialize flag vector
		addi 	$16, $0, 1				# Initialize mask for flag vector

StIn:	addi	$5, $5, 4				# Increment offset of candidate element
		lw 		$3, PatternBase($5)		# Load element from candidate base
		beq		$3, $0, StIn			# If element is zero, continue loop
		
Loop:	and		$10, $15, $16			# Load flag for current candidate
		bne		$10, $16, Skip			# Skip to next candidate if flag is false
		mult	$2, $12					# Multiply candidate number with offset per candidate
		mflo	$7						# Move product to $7
		add		$7, $7, $5				# Compute offset into the candidate
		lw		$3, PatternBase($5)		# Load element of pattern
		lw		$4, CandBase($7)		# Load element of candidate
		beq		$3, $4, Skip			# If equal, skip to Skip
		xor		$15, $15, $16			# Otherwise, set flag vector to false
		addi	$13, $13, -1			# Decrement number of candidates left
		beq		$13, $0, Exit			# If 7 candidates ruled out, exit loop
	
Skip:	addi	$2, $2, 1				# Increment candidate number
		sll		$16, $16, 1				# Shift mask one to the left
		beq		$2, $8, NextIt      	# If last puzzle is reached, go to NextIt
		j		Loop					# Go to Loop (check next candidate)
		
NextIt:	addi	$5, $5, 4				# Increment Element Number
		addi	$2, $0, 0				# Reset candidate number to 0
		addi	$16, $0, 1				# Reset flag mask to 1
		j 		Loop					# Restart iteration through candidates
		
Exit:	addi 	$2, $0, -1				# Initialize index

FindIn: srl		$15, $15, 1				# Shift flag vector to the left	
		addi 	$2, $2, 1				# Increment index
		bne		$15, $0, FindIn			# If flag vector is not equal to 0, continue loop
		
SetCan: swi		544						# submit answer and check
		jr		$31						# return to caller

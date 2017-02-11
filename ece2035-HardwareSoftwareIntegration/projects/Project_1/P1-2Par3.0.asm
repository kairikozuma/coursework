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

.data
CandBase: 		.alloc 1152
PatternBase:	.alloc 144
Flag:			.word 1,1,1,1,1,1,1,1

.text
IconMatch:	addi	$1, $0, CandBase	# point to base of Candidates
		swi	584			# generate puzzle icons

		# $1: 
		# $2: Candidate Guess
		# $3: Current Element of Pattern Being Checked
		# $4: Current Element of Candidate
		# $5: Index for Current Element of Pattern
		# $6: Index for Current Candidate
		# $7: Index for Current Element of Candidate
		# $8: End condition for Candidate Loop
		# $9: Predicate: elements are equal
		# $10: Flag predicate that tells which candidates to skip
		# $11: Predicate: End condition for comparison
		# $12: Number of elements in pattern (144)
		# $13: Number of candidates ruled out
		# $15: Predicate: Whether candidate is valid
				
		addi	$5, $0, -4				# Initialize index of pattern element (element -1)
		addi	$6, $0, 0 				# Initialize index of candidate element (first puzzle)
		addi 	$8, $0, 32				# Initialize end condition (beyond last candidate)
		addi 	$10, $0, 0				# Initialize flag for element
		addi 	$12, $0, 144			# Initialize offset of number of elements
		addi 	$13, $0, 7				# Initialize number of candidates

StIn:	addi	$5, $5, 4				# Increment offset of candidate element
		lw 		$3, PatternBase($5)		# Load element from candidate base
		beq		$3, $0, StIn			# If element is zero
		
Loop:	lw		$10, Flag($6)			# Load flag for current candidate
		beq		$10, $0, Skip			# Skip to next candidate if flag is false
		mult	$6, $12					# Multiply candidate number with offset per candidate
		mflo	$7						# Move product to $7
		add		$7, $7, $5				# Offset into the candidate
		lw		$3, PatternBase($5)		# Load element of pattern
		lw		$4, CandBase($7)		# Load element of candidate
		beq		$3, $4, Skip			# If equal, skip to Skip
		sw		$0, Flag($6)			# Set flag to false
		addi	$13, $13, -1			# Decrement number of candidates left
		beq		$13, $0, Exit			# If 7 candidates ruled out, exit loop
	
Skip:	addi	$6, $6, 4				# Increment candidate number
		beq		$6, $8, NextIt      	# If last puzzle is reached, go to NextIt
		j		Loop					# Go to Loop (check next candidate)
		
NextIt:	addi	$5, $5, 4				# Increment Element Number
		addi	$6, $0, 0				# Reset candidate number to 0
		j 		Loop					# Restart iteration through candidates
		
Exit:	addi 	$6, $0, -4  			# Initialize index of candidate

FindIn: addi	$6, $6, 4				# Increment offset into flag
		lw		$15, Flag($6)			# Load flag for candidate
		beq		$15, $0, FindIn  		# Loop to next offset
		
SetCan: sra		$6, $6, 2				# Convert byte offset to index offset
		add		$2, $6, $0				# Set index offset as pattern answer        
		swi	544			# submit answer and check
		jr	$31			# return to caller
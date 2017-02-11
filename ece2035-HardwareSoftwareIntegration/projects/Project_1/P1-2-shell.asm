# P1-2
# Student Name:
# Date:
#
#     I c o n   M a t c h
#
#
# This routine determines which of eight candidate icons matches a pattern icon.

.data
CandBase: 	.alloc 1152
PatternBase:	.alloc 144

.text
IconMatch:	addi	$1, $0, CandBase	# point to base of Candidates
		swi	584			# generate puzzle icons
	
# your code goes here

        addi    $2, $0, 0               # temp: guess the first icon
		swi	544			# submit answer and check
		jr	$31			# return to caller

	# Refresher SPIM Assignment
	#
# Start with data declarations
#
	.data
str1: .asciiz "Enter the First Operand: "
str2: .asciiz "Enter the Second Operand: "
str3: .asciiz "A + B is "
str4: .asciiz "A - B is "
str5: .asciiz " Done [0 = Yes, 1 = No]: "
newline: .asciiz "\n"
    .align 2

    .globl main
    .text             # This is an I/O sequence. Print the string
       			      # labeled str1: to the console

main:   la $a0, str1  # load string address into $a0 and I/O code into $v0
	li $v0, 4
	syscall  	      # execute the syscall to perform input/output
	 		          # via the console

	li $v0, 5         # an I/O sequence to read an integer from
	   		          # the console window
	syscall
	move $t0, $v0	  # place the value read into register $t0

	la $a0, str2	  # Load address of string 2 into register $a0
	li $v0, 4	      # Load I/O code to print string to console
	syscall		      # print string

	li $v0, 5	      # an I/O sequence to read an integer from the console
	syscall
	move $t1, $v0	  # place the value read into register $t1

	add $t2, $t1, $t0 # perform the addition
	sub $t3, $t0, $t1 # perform the subtraction

	la $a0, str3	  # Load address of string 3 into register $a0
	li $v0, 4	      # Load I/O code to print string to console
	syscall		      # print string

	li $v0, 1	      # an I/O sequence to write an integer from
	   		          # the console window
	move $a0, $t2
	syscall

	la $a0, newline	  # print the new line character to
	   		          # force a carriage return
	li $v0, 4	      # on the console
	syscall

	la $a0, str4	  # Load address of string 4 into register $a0
	li $v0, 4	      # Load I/O code to print string to console
	syscall		      # print string

	li $v0, 1	      # an I/O sequence to write an integer
	   		          #from the console window
	move $a0, $t3
	syscall

	la $a0, newline	  # print the new line character to force a
	   		          # carriage return
	li $v0, 4	      # on the console
	syscall

	la $a0, str5	  # Load address of string 5 into register $a0
	li $v0, 4	      # Load I/O code to print string to console
	syscall		      # print string

	li $v0, 5	      # an I/O sequence to read an integer from the console
	syscall

    bne $v0,$0, main  # if not complete then start at the beginning

    li $v0, 10     #load code to exit program
    syscall        #exit program

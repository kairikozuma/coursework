# Kairi Kozuma
# ECE 3056 Spring 2016
# Homework 1
# 01-15-16

            .data
# Statements to print to console
strA:       .asciiz "Original Array:\n"
strB:       .asciiz "Second Array:\n:"
newLine:    .asciiz "\n"
space:      .asciiz " "

# Original array
Original:   .word   200,   270,   250,   100
            .word   205,   230,   105,   235
            .word   190,    95,    90,   205
            .word    80,   205,   110,   215

# Allocate space for 4 maximum numbers
Max:        .space 16
            .align 2
            .globl main
            .text

# Main program
main:
    la   $a0, strA      # Load address of original array string statement
    li   $v0, 4         # Load I/O code to print string to console
    syscall             # Print string statement for original array

    li   $s0, 0         # Byte offset into matrix
    li   $s1, 64        # End condition for byte offset
    li   $s2, 16        # Constant for checking if divisible by 4

# Start of printing loop for matrix
PrintLoop:
    div  $s0, $s2       # Check if divisible by four
    mfhi $t0            # Get remainder of division
    bne  $t0, $zero, PrintLoopElement
                        # Skip printing new line character if remainder exists
    la   $a0, newLine   # Load address of new line character
    li   $v0, 4         # Load I/O code to print string to console
    syscall

PrintLoopElement:
    lw   $a0, Original($s0)       # Get current element
    li   $v0, 1         # Load I/O code to print integer to console
    syscall

    la   $a0, space     # Load address of space character
    li   $v0, 4         # Load I/O code to print string to console
    syscall

    addi $s0, $s0, 4    # Increment to next element
    bne  $s0, $s1, PrintLoop # Return to loop head if last number not reached

# Find the maximum of each column of matrix
    jal  FindMax

# Print results
    la   $a0, newLine   # Load address of new line character
    li   $v0, 4         # Load I/O code to print string to console
    syscall
    syscall

    la   $a0, strB      # Load address of second array string statement
    li   $v0, 4         # Load I/O code to print string to console
    syscall

    li   $s0, 0         # Byte offset into matrix
    li   $s1, 16        # End condition for byte offset
PrintMaxLoop:
    lw   $a0, Max($s0)  # Get current element
    li   $v0, 1         # Load I/O code to print integer to console
    syscall

    la   $a0, space     # Load address of space character
    li   $v0, 4         # Load I/O code to print string to console
    syscall

    addi $s0, $s0, 4    # Increment to next element
    bne  $s0, $s1, PrintMaxLoop # Return to loop head if last number not reached

    li   $v0, 10        # Load code to exit program
    syscall             # Exit program

# Subroutine to find the maximum of each column of matrix
FindMax:
    la   $s0, Original  # Address of first index of matrix
    li   $s1, 0         # Index of current row
    li   $s3, 16        # End condition for column loop
LoopColumn:             # Loop for each column
    lw   $s4, Original($s1) # Initialize first element of column as max
    move $s2, $s1       # Copy index of current row
    addi $s5, $s2, 64   # End condition for row loop
LoopRow:                # Loop for each row
    addi $s2, $s2, 16   # Increment to next row
    lw   $t1, Original($s2) # Load the current element
    bgt  $s4, $t1, Skip # If current max is greater than or equal, continue loop
    move $s4, $t1       # Otherwise, set element to new max
Skip:
    bne  $s2, $s5, LoopRow # Loop back to row loop
    sw   $s4, Max($s1)  # Store max of column into max array
    addi $s1, $s1, 4    # Increment the column index
    bne  $s1, $s3, LoopColumn # Loop back to column loop
    jr   $31            # Return to caller

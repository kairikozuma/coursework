        .data
str1:   .asciiz "After Swapping: x = "
str2:   .asciiz ", y = "
newline: .asciiz "\n"

        .text
main:   li $s0, 10          # x = 10
        li $s1, 5           # y = 5
        add $s0, $s0, $s1   # x = x + y
        sub $s1, $s0, $s1   # y = x - y
        sub $s0, $s0, $s1   # x = x - y
        la $a0, str1        # Print str1
        li $v0, 4
        syscall
        move $a0, $s0         # Print value of x
        li $v0, 1
        syscall
        la $a0, str2        # Print str2
        li $v0, 4
        syscall
        move $a0, $s1         # Print value of y
        li $v0, 1
        syscall
        la $a0, newline     # Print newline
        li $v0, 4
        syscall

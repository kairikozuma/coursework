        .data
label:  .word 24, 28
        .byte 64, 32
        .asciiz "Example Program"

        .text
main:   jal push
        jal pop
        li $v0, 10
        syscall
pop:    lw $fp, 0($sp)
        lw $ra, 4($sp)
        add $sp, $sp, 32
ret1:   jr $ra
push:   addiu $sp, $sp, -32
        sw $fp, 0($sp)
        sw $ra, 4($sp)
ret2:   jr $ra

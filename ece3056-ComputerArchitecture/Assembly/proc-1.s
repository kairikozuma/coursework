.data
label:  .word 8, 16, 32, 64
.byte 64, 32

.text
.globl main
main:    la $4, label
					li $5, 16
					jal func
 					li $v0, 10
					syscall

func:    move $2, $4
 					move $3, $5
					add $3, $3, $2
					move $9, $0
loop:	lw $22, 0($2)
					add $9, $9, $22
					addi $2, $2, 4
					slt $8, $2, $3
					bne $8, $0, loop
					move $2, $9
					jr $31

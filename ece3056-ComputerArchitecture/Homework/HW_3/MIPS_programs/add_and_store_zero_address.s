# Simple program that executes label [3] = label[1] + label[2]
        .text
        .globl main
main:   lw  $t0, 4($zero)
        lw  $t1, 8($zero)
        add $t0, $t0, $t1
        sw  $t0, 12($zero)
exit: 	li  $v0, 10			#exit program
    	syscall

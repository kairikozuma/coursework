# Simple program that executes label [3] = label[1] + label[2]
        .data
label:  .word 0, 1, 2, 0
        .text
        .globl main
main:   la  $s0, label    # load label address
        lw  $t0, 4($s0)
        lw  $t1, 8($s0)
        add $t0, $t0, $t1
        sw  $t0, 12($s0)
exit: 	li  $v0, 10			#exit program
    	syscall

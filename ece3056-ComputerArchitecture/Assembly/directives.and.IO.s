# ECE 2030 SPIM Assignment 1
# Senyo Apewokin


	.data
        .globl initials
initials:
label1: .asciiz "***********************************"
label2: .asciiz "*                                 *"
label3: .asciiz "*          Senyo Apewokin         *"
label4:	.asciiz "*ECE 2030 Programming Assignment 1*"
label5: .asciiz "*           Section F             *"
label6: .asciiz "*            11/09/02             *"
label7: .space 7
label8: .byte 0, 1, 2, 3, 4, 5, 6, 7, 8
        .align 2
label9: .word 110, 220
label10: .asciiz "*                                 *"
label11: .asciiz "***********************************"

newline: .asciiz "\n"

        
		
# Standard startup code.  Invoke the routine main with no arguments.

	.text
	.globl __start
__start: jal main
	addu $0, $0, $0		# Nop
	addiu $v0, $0, 10
	syscall			# syscall 10 (exit)

	.globl main
main:
        li  $v0, 4           # syscall to print message at label1
	la  $a0, label1
	syscall 
        
        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 	 

        li  $v0, 4           # syscall to print message at label2
	la  $a0, label2
	syscall 
        
        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 	 

        li  $v0, 4           # syscall to print message at label3
	la  $a0, label3
	syscall 
        
        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 	 

        li  $v0, 4           # syscall to print message at label4
	la  $a0, label4
	syscall 
        
        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 	

        li  $v0, 4           # syscall to print message at label5
	la  $a0, label5
	syscall 
        
        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 	 

        li  $v0, 4           # syscall to print message at label6
	la  $a0, label6
	syscall 
        
        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 

	
        la  $s0, label8
        lb  $a0, 0($s0)
        li  $v0, 1           # syscall to print first byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 


        lb  $a0, 1($s0)
        li  $v0, 1           # syscall to print second byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 

	

        lb  $a0, 2($s0)
        li  $v0, 1           # syscall to print third byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 


        lb  $a0, 3($s0)
        li  $v0, 1           # syscall to print fourth byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 

	

        lb  $a0, 4($s0)
        li  $v0, 1           # syscall to print fifth byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 


        lb  $a0, 5($s0)
        li  $v0, 1           # syscall to print sixth byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 

	
  
        lb  $a0, 6($s0)
        li  $v0, 1           # syscall to print seventh byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 


        lb  $a0, 7($s0)
        li  $v0, 1           # syscall to print eighth byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 

        lb  $a0, 8($s0)
        li  $v0, 1           # syscall to print ninth byte at label8
	syscall 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 

        la  $s0,label9
        lw  $a0, 0($s0)
        li  $v0, 1           # syscall to print first word at label9
	syscall 		 

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 

        lw  $a0, 4($s0)
        li  $v0, 1           # syscall to print second word at label9
	syscall 	

        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 

        li  $v0, 4           # syscall to print message at label10
	la  $a0, label10
	syscall 
        
        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 	 

        li  $v0, 4           # syscall to print message at label11
	la  $a0, label11
	syscall 
        
        li  $v0, 4           # syscall to print new line character
	la  $a0, newline
	syscall 	 



exit:
       jr $31
        
        
 
        
        


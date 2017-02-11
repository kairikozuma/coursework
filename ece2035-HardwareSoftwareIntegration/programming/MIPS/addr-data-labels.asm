# Use of Mnemonic labels to refer to data memory addresses.
.data
VBase: .word 11, 22, 33, 44, 55, 66, 77, 88, 99
.text
lw $1, VBase($0)          # $1: 11
addi $4, $0, 4            # $4: 4
addi $5, $0, 20           # $5: 20

lw $2, VBase($4)          # 4 bytes (1 word) away from VBase (22)
lw $3, VBase($5)          # 20 bytes (5 words) away from VBase (66)

addi $6, $0, VBase        # VBase used as immed (addr of 1st word in vector)
lw $7, 4($6)              # Ans: $7: 22
lw $8, 20($6)             # Ans: $8: 66


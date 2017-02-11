# Differences between bitwise and logical operators (& vs &&, | vs ||)
# $1: x = 3
# $2: y = 4
# $3: z = x & y    */ bitwise AND: 0...0011 and 0...0100 = 0...0 /*
# $4: w = x && y   */ logical AND: both are nonzero, so w = 1 /*
# $5: a = x | y    */ bitwise OR: 0...0011 and 0...0100 = 0...0111 /*
# $6: b = x || y   */ logical OR: at least one is nonzero, so w = 1 /*

Bitwise:        addi $1, $0, 3
                addi $2, $0, 4
                and  $3, $1, $2         # z = x & y

                # w = x && y
                beq  $1, $0, False      # branch to False if x = 0
                beq  $2, $0, False      # branch to False if y = 0
                addi $4, $0, 1          # x and y are both nonzero, so w = 1
                j Continue
False:          addi $4, $0, 0          # x and/or y are 0, so w = 0

Continue:       or   $5, $1, $2         # a = x | y

                # w = x || y
                bne  $1, $0, True       # branch to True if x is non-zero
                bne  $2, $0, True       # branch to True if y is non-zero
                addi $6, $0, 0          # x and y are both zero, so b = 0
                j End
True:           addi $6, $0, 1          # x and/or y are non-zero, so b = 1
End:            jr $31

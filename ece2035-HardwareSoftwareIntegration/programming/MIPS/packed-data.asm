# Computes the OR of four 8-bit pixels packed in a 32-bit data word.
#
# $2: packed data word
# $3: a pixel
# $6: OR of all pixels (result)
.data
# decimal equivalent of 0xFF3BA10F
Pixels: .word  -12869361    

.text
Dilate: lw $2, Pixels($0)
        andi $6, $2, 255      # mask: extract P0 (0xFF)
        srl  $2, $2, 8        # logical shift (don't sign extend) right by 8 bits

        andi $3, $2, 255     # mask; extract P1
        srl  $2, $2, 8        # shift off P1

        or   $6, $3, $6       # $6: P1 or P0

        andi $3, $2, 255     # mask; extract P2
        srl  $2, $2, 8        # shift off P2, $2 is right with just P3

        or   $6, $3, $6       # $6: P2 or P1 or P0

        or   $6, $2, $6       # $6: P3 or P2 or P1 or P0
        jr $31

.text
# float floatmean(float *vec, unsigned len)
#   Find arithmetic mean of single-precision floating point array.
floatmean: move  $t0, $a1        # n = len
           li.s  $f0, 0.0        # sum = 0.0 ; pseudoinstruction
    
fml:       beq   $a1, $zero, fmr # if (len == 0) return

           l.s   $f1, ($a0)      # sum += *vec
           add.s $f0, $f0, $f1
    
           addi  $a0, $a0, 4     # vec++, len--
           addi  $a1, $a1, -1

           j     fml

fmr:       mtc1 $t0, $f1         # sum /= (float)n
           cvt.s.w $f1, $f1      # Convert word to single-precision
           div.s $f0, $f0, $f1
           jr   $ra

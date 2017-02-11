.text
# double dblmean(double *vec, unsigned len)
#   Find arithmetic mean of double-precision floating point array.
dblmean: move  $t0, $a1        # n = len
         li.d  $f0, 0.0        # sum = 0.0 ; pseudoinstruction

dml:     beq   $a1, $zero, dmr # if (len == 0) return

         l.d   $f2, ($a0)      # sum += *vec
         add.d $f0, $f0, $f2   # l.d is another pseudoinstruction

         addi  $a0, $a0, 8     # vec++, len--
         addi  $a1, $a1, -1

         j     dml

dmr:     mtc1 $t0, $f2         # sum /= (double)n
         mtc1 $0, $f3
         cvt.d.w $f2, $f2      # Convert word to double
         div.d $f0, $f0, $f2   # Remember that $f0 as a double includes $f1
         jr   $ra

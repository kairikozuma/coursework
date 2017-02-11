BLOCK_SIZES="4 8 16 32 64 128 256 512 1024 2048"
CACHE_SIZES="131072"
ASSOCIATIVITIES="1 2 4 8 16 32 64"
TRACES=`ls | grep -E "trace.(merge|bubble)"`
echo "Trace,Cache Size,Associativity,Block Size,Accesses,Hits,Misses,ReadMiss,ReadCount,WriteMiss,WriteCount,Writebacks"
for t in $TRACES; do
  for b in $BLOCK_SIZES; do
    for s in $CACHE_SIZES; do
      for a in $ASSOCIATIVITIES; do
        echo -n "$t, $s, $a, $b, "
        ./cachesim $t $b $s $a
      done
    done  
  done
done

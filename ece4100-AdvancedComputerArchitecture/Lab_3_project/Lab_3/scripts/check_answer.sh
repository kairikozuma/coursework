
######################################################################################
# This scripts runs all four traces
# You will need to first compile your code in ../src before launching this script
# the results are stored in the ../results/ folder
######################################################################################



########## ---------------  B.1 ---------------- ################

../src/sim -pipewidth 1 -schedpolicy 0 -loadlatency 1 ../traces/gcc.ptr.gz > ../results/B1.gcc.res
../src/sim -pipewidth 1 -schedpolicy 0 -loadlatency 1 ../traces/sml.ptr.gz > ../results/B1.sml.res

########## ---------------  B.2 ---------------- ################

../src/sim -pipewidth 1 -schedpolicy 1 -loadlatency 1 ../traces/gcc.ptr.gz > ../results/B2.gcc.res
../src/sim -pipewidth 1 -schedpolicy 1 -loadlatency 1 ../traces/sml.ptr.gz > ../results/B2.sml.res


########## ---------------  B.3 ---------------- ################

../src/sim -pipewidth 1 -schedpolicy 0 -loadlatency 4 ../traces/gcc.ptr.gz > ../results/B3.gcc.res
../src/sim -pipewidth 1 -schedpolicy 0 -loadlatency 4 ../traces/sml.ptr.gz > ../results/B3.sml.res

########## ---------------  B.4 ---------------- ################

../src/sim -pipewidth 1 -schedpolicy 1 -loadlatency 4 ../traces/gcc.ptr.gz > ../results/B4.gcc.res
../src/sim -pipewidth 1 -schedpolicy 1 -loadlatency 4 ../traces/sml.ptr.gz > ../results/B4.sml.res



########## ---------------  C.1 ---------------- ################

../src/sim -pipewidth 2 -schedpolicy 0 -loadlatency 1 ../traces/gcc.ptr.gz > ../results/C1.gcc.res
../src/sim -pipewidth 2 -schedpolicy 0 -loadlatency 1 ../traces/sml.ptr.gz > ../results/C1.sml.res

########## ---------------  C.2 ---------------- ################

../src/sim -pipewidth 2 -schedpolicy 1 -loadlatency 1 ../traces/gcc.ptr.gz > ../results/C2.gcc.res
../src/sim -pipewidth 2 -schedpolicy 1 -loadlatency 1 ../traces/sml.ptr.gz > ../results/C2.sml.res


########## ---------------  C.3 ---------------- ################

../src/sim -pipewidth 2 -schedpolicy 0 -loadlatency 4 ../traces/gcc.ptr.gz > ../results/C3.gcc.res
../src/sim -pipewidth 2 -schedpolicy 0 -loadlatency 4 ../traces/sml.ptr.gz > ../results/C3.sml.res

########## ---------------  C.4 ---------------- ################

../src/sim -pipewidth 2 -schedpolicy 1 -loadlatency 4 ../traces/gcc.ptr.gz > ../results/C4.gcc.res
../src/sim -pipewidth 2 -schedpolicy 1 -loadlatency 4 ../traces/sml.ptr.gz > ../results/C4.sml.res



########## ---------------  GenReport ---------------- ################

grep LAB3_CPI ../results/B?.*.res > report.txt
grep LAB3_CPI ../results/C?.*.res >> report.txt


######### ------- Goodbye -------- ##################

echo "Done. Check report.txt, and .res files in ../results";

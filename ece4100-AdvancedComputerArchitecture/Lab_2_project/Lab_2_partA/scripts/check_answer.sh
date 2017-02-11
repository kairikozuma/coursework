
######################################################################################
# This scripts runs gcc.ptr.gz and sml.ptr.gz and compares to the sample answer
# the results are stored in the ../results/ folder
######################################################################################


########## ---------------  A.1 ---------------- ################
../src/sim -pipewidth 1 ../traces/sml.ptr.gz   > ../results/A1.sml.res
../src/sim -pipewidth 1 ../traces/gcc.ptr.gz   > ../results/A1.gcc.res

########## ---------------  A.2 ---------------- ################

../src/sim -pipewidth 2 ../traces/sml.ptr.gz   > ../results/A2.sml.res
../src/sim -pipewidth 2 ../traces/gcc.ptr.gz   > ../results/A2.gcc.res

########## ---------------  A3 ---------------- ################

../src/sim -pipewidth 2 -enablememfwd -enableexefwd ../traces/sml.ptr.gz   > ../results/A3.sml.res
../src/sim -pipewidth 2 -enablememfwd -enableexefwd ../traces/gcc.ptr.gz   > ../results/A3.gcc.res

########## ---------------  B1 ---------------- ################

../src/sim -pipewidth 2 -enablememfwd -enableexefwd -bpredpolicy 1 ../traces/sml.ptr.gz   > ../results/B1.sml.res
../src/sim -pipewidth 2 -enablememfwd -enableexefwd -bpredpolicy 1 ../traces/gcc.ptr.gz   > ../results/B1.gcc.res

########## ---------------  B2 ---------------- ################

../src/sim -pipewidth 2 -enablememfwd -enableexefwd -bpredpolicy 2 ../traces/sml.ptr.gz   > ../results/B2.sml.res
../src/sim -pipewidth 2 -enablememfwd -enableexefwd -bpredpolicy 2 ../traces/gcc.ptr.gz   > ../results/B2.gcc.res

########## ---------------  GenReport ---------------- ################
echo "SML"
diff -y --suppress-common-lines ../results/A1.sml.res ../solution/A1.sml.res | grep '^' | wc -l
diff -y --suppress-common-lines ../results/A2.sml.res ../solution/A2.sml.res | grep '^' | wc -l
diff -y --suppress-common-lines ../results/A3.sml.res ../solution/A3.sml.res | grep '^' | wc -l
diff -y --suppress-common-lines ../results/B1.sml.res ../solution/B1.sml.res | grep '^' | wc -l
diff -y --suppress-common-lines ../results/B2.sml.res ../solution/B2.sml.res | grep '^' | wc -l
echo "GCC"
diff -y --suppress-common-lines ../results/A1.gcc.res ../solution/A1.gcc.res | grep '^' | wc -l
diff -y --suppress-common-lines ../results/A2.gcc.res ../solution/A2.gcc.res | grep '^' | wc -l
diff -y --suppress-common-lines ../results/A3.gcc.res ../solution/A3.gcc.res | grep '^' | wc -l
diff -y --suppress-common-lines ../results/B1.gcc.res ../solution/B1.gcc.res | grep '^' | wc -l
diff -y --suppress-common-lines ../results/B2.gcc.res ../solution/B2.gcc.res | grep '^' | wc -l

######### ------- Goodbye -------- ##################

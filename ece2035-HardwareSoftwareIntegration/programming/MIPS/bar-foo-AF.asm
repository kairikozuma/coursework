# Bar/Foo example from lecture.
#
# int bar(int a, int b) {
#    int x = 25;
#    int y = 16;
#    int V[] = {1, 3, 5};
#    int Foo(int, int, int *, int[]);
#    ...<body Bar1>...
#    x = Foo(125, x, &y, V);
#    ...<body Bar2>...
#    return(y);
# }
#
# int Foo(int M, int N, int *P, int A[]){
#    int T = 100;
#    *P = N + *P;
#    A[1] = A[2] + M;
#    M = T * 8;
#    return(M);
# }
				# A
Bar:	add  $30, $29, $0	# set base of FP
	addi $29, $29, -20	# allocate space for Locals (5 words)
	addi $1, $0, 25		# initialize locals
	sw   $1, 16($29)	# x = 25;
	addi $1, $0, 16
	sw   $1, 12($29)	# y = 16;
	addi $1, $0, 5
	sw   $1, 8($29)		# V[2] = 5
	addi $1, $0, 3
	sw   $1, 4($29)		# V[1] = 3
	addi $1, $0, 1
	sw   $1, 0($29)		# V[0] = 1

	#... <body Bar1> ...

	# prepare to call Foo   # B
	addi $29, $29, -28	# allocate space
	sw   $31, 24($29)	# push RA
	sw   $30, 20($29)	# push FP
	addi $1, $0, 125	# push Inputs
	sw   $1, 16($29)	# push 125
	lw   $1, -4($30)	# load in x
	sw   $1, 12($29)	# push copy of x
	addi $1, $30, -8	# compute address of y
	sw   $1, 8($29)		# push &y
	addi $1, $30, -20	# compute address of V[0]
	sw   $1, 4($29)		# push V
	     	 		# note: 0($29) is allocated for RV
	jal  Foo

	# restore state         # C
	lw   $31, 24($29)	# restore RA
	lw   $30, 20($29)	# restore FP
	lw   $1, 0($29)		# get RV
	sw   $1, -4($30)	# store RV in x.
	addi $29, $29, 28       # deallocate space

	#... <body Bar2> ...

	# prepare to return     # D
	lw   $1, -8($30)	# load y
	sw   $1, 0($30)		# store RV
	add  $29, $30, $0	# pop locals
	jr   $31
	
				# E
Foo: 	add  $30, $29, $0	# set base of FP
	addi $29, $29, -4	# allocate space for Locals (1 word)
	addi $1, $0, 100	# initialize local
	sw   $1, 0($29)		# T = 100;

	# Body of Foo:
	lw   $1, 12($30)	# load N
	lw   $2, 8($30)		# load P
	lw   $3, 0($2)		# dereference P
	add  $1, $1, $3		# N + *P
	sw   $1, 0($2)		# *P = N + *P

	lw   $1, 4($30)		# base address of V
	lw   $2, 8($1)		# load A[2]
	lw   $3, 16($30)	# load M
	add  $3, $2, $3		# A[2] + M
	sw   $3, 4($1) 		# A[1] = A[2] + M

	lw   $1, -4($30)	# load T
	sll  $1, $1, 3		# T * 8
	sw   $1, 16($30)	# M = T * 8

	# prepare to return     # F
	sw   $1, 0($30)		# write RV
	add  $29, $30, $0	# pop locals
	jr   $31

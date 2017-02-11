/*      I c o n   M a t c h
Student Name: Kairi Kozuma
Date: 02/13/15

ECE 2035 Project 1-1

This program loads 8 icons (each is a 12x12 array of pixels) into a
linear array of candidates and a single icon that serves as a pattern,
which matches exactly one candidate icon.

Add code to find which of the eight candidates matches the pattern icon.
*/

#include <stdio.h>
#include <stdlib.h>

#define IconSize (12 * 12)
#define NumCandidates 8
#define CandidateArraySize (IconSize * NumCandidates)

int Load_Mem(char *InputFileName, int Candidates[], int Pattern[]);
int Load_N_Values(FILE *FP, int N, int Array[]);
int Find_Index(int arr[], int size, int value);

int main(int argc, char *argv[]) {
  int	Candidates[CandidateArraySize];
  int	Pattern[IconSize];
  int	Length, Match=0;

  if (argc != 2) {
    printf("usage: P1-1 valuefile\n");
    exit(1);
  }
  Length = Load_Mem(argv[1], Candidates, Pattern);
  if (Length != CandidateArraySize + IconSize) {
     printf("valuefile does not contain valid data.\n");
     exit(1);
  }
  
  // Initialize File Pointer
  FILE *file = fopen(argv[1], "r");
  
  // Initialize candidates and pattern
  Load_N_Values(file, CandidateArraySize, Candidates);
  Load_N_Values(file, IconSize, Pattern);

  // Initialize array of flags that tell if puzzle matches candidate
  int flag[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  
  // Initialize counter variables
  int n;	// Number of pixel
  int m;	// Number of candidate
  
  // Initialize True variable
  int true = 1;
  
  // Iterate through each pixel of candidate
  for (n = 0; n < IconSize; n++){
  	// Iterate through each candidate
  	for (m = 0; m < NumCandidates; m++){
  		// If the flag is true and the pixels do not match, set flag to false
  		// No change if pixels match or flag is already false
  		if(flag[m] && (Candidates[m * IconSize + n] != Pattern[n])) {
  			flag[m] = 0;
  		}
  	}
  }
  
  Match = Find_Index(flag, NumCandidates, true);
        
  printf("The matching icon is %d\n", Match);
  exit(0);
}

/* This routine loads in pairs of integers of the form "Addr: Value"
from a named file in the local directory. Each pair is delimited by a
newline. The first values are placed in the passed integer arrays
Candidates and Pattern. The number of input integers is returned. */

int Load_Mem(char *InputFileName, int Candidates[], int Pattern[]) {
  int	Nc, Np = 0;
   FILE	*FP;

   FP = fopen(InputFileName, "r");
   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   } else {
     Nc = Load_N_Values(FP, CandidateArraySize, Candidates);
     if (Nc == CandidateArraySize)
       Np = Load_N_Values(FP, IconSize, Pattern);
     fclose(FP);
     return Nc+Np;
   }
}

int Load_N_Values(FILE *FP, int N, int Array[]){
  int i, NumVals, Addr, Value;
  for (i = 0; i < N; i++){
    NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
    if (NumVals == 2)
      Array[i] = Value;
    else 
      return(i);
  }
  return(N);
}

/* This subroutine returns the index of first value in the array that
matches the given value*/
int Find_Index(int arr[], int size, int value) {
	// Initialize variables
	int i = 0;	// Index in array
	int k = -1; // Index of value
	
	// Iterate through array until value is found
	while (i < size && arr[i] != value) {
		i++;
	}
	// If value is not found, return -1
	// Otherwise, return index
	return (k == size ? -1 : i);
}
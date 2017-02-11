#include <stdio.h>
#include <stdlib.h>

/*                    Letter Histogram
Student Name: Kairi Kozuma
Date: January 30 2015

ECE 2035 Homework 2-1

This program computes and displays a letter usage histogram in a text file.

(This is the only file that should be modified for the C implementation
of Homework 2.)

*/
void		LetterHistogram(FILE *);

int main(int argc, char *argv[]) {
   FILE		*FP;

   if (argc != 2) {
     printf("usage: %s valuefile\n", argv[0]);
     exit(1);
   }
   FP = fopen(argv[1], "r");
   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", argv[1]);
      exit(1);
   } else {
      LetterHistogram(FP);
      fclose(FP);
      exit(0);
   }
}

/* This routine counts the frequency of letters in a text file. The final
   count is used to print a letter usage histogram. */

void LetterHistogram(FILE *FP) {
  /* Initialize letter and alphabet array*/
  char let;
  int alph[26] = {0};
  
  while(!feof(FP)) {
    let = getc(FP);
    /* Fix lowercase to uppercase*/
    if (let >= 97 && let <= 122) {
      let -= 32;
    }
	
	/* Increment alphabet value by one if letter is found*/
    if(let >= 65 && let <= 90) {
      alph[let - 65] += 1;
    }
  }
  
  /* Sum total number of alphabet letters*/
  int total = 0, i = 0;
  for (i = 0; i < 26; i++) {
    total += alph[i];
  }
  printf("Letter Occurrence Histogram\n");
  
  int n;
  
  /* Print out results*/
  for(n = 0; n < 26; n++) {
     printf("[%c] %5d, %4.1f%%\n", (char)(n + 65), (int)alph[n], 100*(double)alph[n]/total);
     }
}
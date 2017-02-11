#include <stdio.h>
#include <stdlib.h>

/*                    Letter Histogram
Student Name:
Date:

ECE 2035 Homework 2-1

This program computes and displays a letter usage histogram in a text file.

(This is the only file that should be modified for the C implementation
of Homework 2.)

*/

#include <stdio.h>

int main(int argc, char *argv[]) {
   FILE		*FP;
   void		LetterHistogram(FILE *);
   void		exit(int);

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

   /* your code goes here and should include the printf statements below */

   printf("Letter Occurrence Histogram\n");
     /* complete and use this print statement in a loop
        to print the stats for each letter:  

     printf("[%c] %5d, %4.1f%%\n",   ,  ,  )

      */
     
}

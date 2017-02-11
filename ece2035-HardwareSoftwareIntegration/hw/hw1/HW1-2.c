#include <stdio.h>
#include <stdlib.h>

/* 
 Student Name: Kairi Kozuma
 Date:01/23/15

ECE 2035 Homework 1-2

This is the only file that should be modified for the C implementation
of Homework 1.

This program computes and prints the span of a set of ten integers, Set.
*/

int Set[] = {9, 17, -7, 3, -27, 25, 42, 26, 8, -60};

int main() {
  int Span;
  int min = Set[0];
  int max = Set[0];
  int i;
  for (i = 1; i < 10; i++) {
    if (max < Set[i]) {
      max = Set[i];
    }
    if (min > Set[i]) {
      min = Set[i];
    }
  }
  Span = max - min;
  printf("Span: %d\n", Span);
  
  return 0;
}


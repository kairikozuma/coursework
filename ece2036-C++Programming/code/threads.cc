// Illustrate using CreateThread in gthreads
// George F. Riley, Georgia Tech, ECE3090, Spring 2012

#include "gthread.h" // Must be included to use the gthreads library

void BubbleSort(int* d, int startingPoint, int length)
{ // This is the thread starting point
  // This is where, in this example, the sorting of array d will be done
  EndThread(); // Call this just before exiting
}

const int nThreads = 4; // Number of threads desired
const int maxSize = 512000; // Largest sort size

int main()
{
  int d[maxSize]; // Array to be sorted
  int start = 0; // Starting point of sub-array
  int lengthPerThread = maxSize / nThreads; // Length of sub-array
  for (int k = 0; k < nThreads; ++k)
  { // Create each of the four sorting threads
    // Address of bubblesort function passed as argument
    CreateThread(BubbleSort, d, start, lengthPerThread);
    // Does not execute bubblesort immediately
    // Only creates threads to execute some time in the future
    start = start + lengthPerThread;
  }
  // At this point all threads are created
  WaitAllThreads(); // This waits until all child threads are done
  // Perform the merge procedure to merge the separate sub-arrays
}
/*
// Mutex: make commands within mutex atomic (accessible by only one thread)
// Syntax

lock(mutex);
// does some code
unlock(mutex);

// Mutext implementation in assembly
int L; // L = 0 (unlock), L = 1 (locked)
Loop: LOAD R1, 1  // Set R1 to 1
      SWAP(&L, R1) // Swaps values in one clock cycle, L is 1 unconditionally
      BEQ R1, R0, LOCK
      JUMP Loop
LOCK: ADDI R1, 1


void* // void pointer points to some address

global variables shared
local variabels not shared (because it is on stack)

signal condition end with pthread_cond_signal(&exitCond)
 */

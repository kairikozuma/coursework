//////////////////////////////////////
// Course: ECE-3056, Spring 2016
// Instructor: Sudhakar Yalamanchili
// Assignment 2: EnergyModelPlotting.c
//
// Name: Kairi Kozuma
// GTID: 903050898
//////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARRAY_WIDTH 256  // Use 256 x 256 images for Assignment 2

int main (int argc, char *argv[])
{
  // Require two arguments
  if (argc < 2)
    {
      printf("Usage: <Input File with DCT of Image> <Name of File for writing K, PowerConsumption, ReconstructionError>");
      exit(-1);
    }

  // Initialize variables for file input
  FILE *inputfp;
  FILE *outputfp;
  double *array[ARRAY_WIDTH];
  int idx = 0;
  int j = 0;
  int i = 0;
  int k = 0;
  char *buffer = NULL;
  char *ptr = NULL;
  size_t len = 0;
  ssize_t read;

  // Open the input/file and read the DCT of the image of interest
  inputfp = fopen (argv[1], "r");      //open file as read only

  // If file is invalid, print error and return
  if (!inputfp)
    {
      fprintf (stderr, "failed to open file for reading\n");
      return 1;
    }

  // Read in file and store in two dimensional array
  while ((read = getline (&buffer, &len, inputfp)) != -1) // While not end of file
    {
        // Allocate memory for 256 elements for row
        array[idx] = (double *)malloc (sizeof(array));

        // Put input values into array
        for (j = 0, ptr = buffer; j < ARRAY_WIDTH; j++, ptr++)
            // Parse string and store in matrix
            array [idx][j] = strtod(ptr, &ptr);
        // Increment to next row
        idx++;
    }

  // Close input file
  fclose (inputfp);

  // Initialize new array for storing PC, RE values
  double *resultArr[ARRAY_WIDTH + 1];

  // Allocate memory for 256 elements per row
  for (i = 0; i < ARRAY_WIDTH + 1; i++) {
      resultArr[i] = (double *)malloc (2 * sizeof(double));
  }

  // Iterate through all possible values of k
  for (k = 0; k <= ARRAY_WIDTH; k++) {

      int dctTerms = ARRAY_WIDTH - k;

      // Calculate power consumption
      //    1e-15 J per multiplication operation
      //    Matrix multiplication of (N - K) x (N - K) matrix with a filter of same size
      //    Total multiplication instructions: (N - K)^3
      //    Processed at 30Hz
      //    Power = (N - K)^3 * 10e-15J * 30Hz
      double powerConsum = pow(dctTerms, 3.0) * 10e-15 * 30;

      // Calculate reconstruction error
      //    ReconstructionError = 100 * sqrt(diff/square)
      //    diff = SUM((original - truncated)^2)
      //    square = SUM(original^2)
      double diff = 0.0; // numerator
      double square = 0.0; // denominator
      for (i = 0; i < ARRAY_WIDTH; i++) {
         for (j = 0; j < ARRAY_WIDTH; j++) {
            if (j >= dctTerms || i >= dctTerms) {
                diff += array[i][j] * array[i][j];
            }
            square += array[i][j] * array[i][j];
         }
      }
      double reconError = sqrt(diff/square);

      // Store values in array
      resultArr[k][0] = powerConsum;
      resultArr[k][1] = reconError;
  }

  // Open the output/second file and write the contents of truncated DCT matrix
  outputfp = fopen(argv[2], "w"); // Open as write

  // If output file is invalid, print error and return
  if (outputfp == NULL)
    {
      fprintf(stderr, "Can't open output file %s!\n", argv[2]);
      exit(1);
    }

  // Iterate through array and print results to new file
  fprintf(outputfp, "Number of Terms Truncated (k), Power Consumption (W),Reconstruction Error (%%)\n");
  for (i = 0; i < ARRAY_WIDTH + 1; i++) {
      fprintf(outputfp, "%d,%e,%lf\n", i, resultArr[i][0], resultArr[i][1]);
  }

  // Close the output file
  fclose (outputfp);
  return 0;
}

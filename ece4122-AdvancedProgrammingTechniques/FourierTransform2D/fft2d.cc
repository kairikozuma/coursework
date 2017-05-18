// Distributed two-dimensional Discrete FFT transform
// Kairi Kozuma
// ECE8893 Project 1


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <signal.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>

#include "Complex.h"
#include "InputImage.h"

using namespace std;

// Rank that handles summation over columns
#define MASTER_RANK 0

// MPI send buffer
#define SEND_MSG_SIZE (512 * 256)
double sendBuf[SEND_MSG_SIZE];

// MPI receive buffer
#define RECV_MSG_SIZE (512 * 256)
double recvBuf[RECV_MSG_SIZE];

// Forward declaration of functions
void Transform1D(Complex* h, int w, Complex* H);

void transposeSquare(Complex* imageArray, int dim) {
  for (int c = 0; c < dim; c++) {
    for (int r = c; r < dim; r++) {
      Complex temp(imageArray[r * dim + c]);
      imageArray[r * dim + c] = imageArray[c * dim + r];
      imageArray[c * dim + r] = temp;
    }
  }
}

void Transform2D(const char* inputFN)
{ // Do the 2D transform here.
  // 1) Use the InputImage object to read in the Tower.txt file and
  //    find the width/height of the input image.
  // 2) Use MPI to find how many CPUs in total, and which one
  //    this process is
  // 3) Allocate an array of Complex object of sufficient size to
  //    hold the 2d DFT results (size is width * height)
  // 4) Obtain a pointer to the Complex 1d array of input data
  // 5) Do the individual 1D transforms on the rows assigned to your CPU
  // 6) Send the resultant transformed values to the appropriate
  //    other processors for the next phase.
  // 6a) To send and receive columns, you might need a separate
  //     Complex array of the correct size.
  // 7) Receive messages from other processes to collect your columns
  // 8) When all columns received, do the 1D transforms on the columns
  // 9) Send final answers to CPU 0 (unless you are CPU 0)
  //   9a) If you are CPU 0, collect all values from other processors
  //       and print out with SaveImageData().
  InputImage image(inputFN);  // Create the helper object for reading the image
  // Step (1) in the comments is the line above.
  // Your code here, steps 2-9

  // Obtain CPU count and rank of this process
  int numtasks, rank;
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks); // number of copies
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);     // id or CPU

  // Debugging printing task and rank
  // printf("Task:%d Rank:%d\n", numtasks, rank);

  // Get image properties
  int width = image.GetWidth();
  int height = image.GetHeight();

  // Initialize pointer to image array
  Complex* imageArray = image.GetImageData();

  // Calculate row per rank
  int rowPerRank = height / numtasks;

  // Array to store result of row DFT
  Complex* dftImageArray = new Complex[height * width];

  // Temporary array for computation done by this rank
  Complex* tempArray = new Complex[width * rowPerRank];

  // Compute row DFT
  for (int i = 0; i < rowPerRank; i++) {
    Transform1D(&imageArray[(rank * rowPerRank + i) * width], width, &tempArray[i * width]);
  }

  // Required for non-blocking calls
  MPI_Request* reqs = new MPI_Request[2 * (numtasks - 1)];
  MPI_Status* stats = new MPI_Status[2 * (numtasks - 1)];
  int reqIndex = 0;

  // Post non-blocking receive from all other ranks
  for (int otherRank = 0; otherRank < numtasks; otherRank++) {
    if (otherRank != rank) {
      int rc = MPI_Irecv(&recvBuf[2 * otherRank * rowPerRank * width], 2 * width * rowPerRank, MPI_LONG, otherRank, 0, MPI_COMM_WORLD, &reqs[reqIndex++]);
      if (rc != MPI_SUCCESS) {
        cout << "Rank " << rank << " receive failed, rc " << rc << endl;
        MPI_Finalize();
        exit(1);
      }
    }
  }

  // Put computed data into send and receive buffer
  for (int i = 0; i < width * rowPerRank; i++) {
    sendBuf[2 * i] = tempArray[i].real;
    sendBuf[(2 * i) + 1] = tempArray[i].imag;
    recvBuf[2 * (rank * rowPerRank * width + i)] = tempArray[i].real;
    recvBuf[2 * (rank * rowPerRank * width + i) + 1] = tempArray[i].imag;
  }

  // Send buffer through non-blocking send to all other ranks
  for (int otherRank = 0; otherRank < numtasks; otherRank++) {
    if (otherRank != rank) {
      int rc = MPI_Isend(&sendBuf[0], 2 * width * rowPerRank, MPI_LONG, otherRank, 0, MPI_COMM_WORLD, &reqs[reqIndex++]);
      if (rc != MPI_SUCCESS) {
        cout << "Rank " << rank << " send failed, rc " << rc << endl;
        MPI_Finalize();
        exit(1);
      }
    }
  }

  // Wait for all non-blocking operations to complete
  MPI_Waitall(2 * (numtasks - 1), reqs, stats);

  // Copy from buffer to complex array
  for (int i = 0; i < width * height; i++) {
    dftImageArray[i] = Complex(recvBuf[2 * i], recvBuf[(2 * i) + 1]);
  }

  
  // Save result to file if master rank
  if (rank == MASTER_RANK) {
    image.SaveImageData("MyAfter1D.txt", dftImageArray, width, height);
  }

  // Transpose matrix to do column-wise DFT
  transposeSquare(dftImageArray, width);

  // Compute column DFT
  for (int i = 0; i < rowPerRank; i++) {
    Transform1D(&dftImageArray[(rank * rowPerRank + i) * width], width, &tempArray[i * width]);
  }

  // Put computed data into buffer
  for (int i = 0; i < width * rowPerRank; i++) {
    sendBuf[2 * i] = tempArray[i].real;
    sendBuf[(2 * i) + 1] = tempArray[i].imag;
    recvBuf[2 * (rank * rowPerRank * width + i)] = tempArray[i].real;
    recvBuf[2 * (rank * rowPerRank * width + i) + 1] = tempArray[i].imag;
  }

  // Send buffer through MPI
  if (rank == MASTER_RANK) {

    // Required for non-blocking calls
    MPI_Request* masterReqs = new MPI_Request[numtasks - 1];
    MPI_Status* masterStats = new MPI_Status[numtasks - 1];
    int masterReqIndex = 0;

    // Post non-blocking receive from all other ranks
    for (int otherRank = 0; otherRank < numtasks; otherRank++) {
      if (otherRank != rank) {
        int rc = MPI_Irecv(&recvBuf[2 * otherRank * rowPerRank * width], 2 * width * rowPerRank, MPI_LONG, otherRank, 0, MPI_COMM_WORLD, &masterReqs[masterReqIndex++]);
        if (rc != MPI_SUCCESS) {
          cout << "Rank " << rank << " receive failed, rc " << rc << endl;
          MPI_Finalize();
          exit(1);
        }
      }
    }

    // Wait for all non-blocking operations to complete
    MPI_Waitall(numtasks - 1, masterReqs, masterStats);

    // Copy from buffer to complex array
    for (int i = 0; i < width * height; i++) {
      dftImageArray[i] = Complex(recvBuf[2 * i], recvBuf[(2 * i) + 1]);
    }

    // Transpose matrix
    transposeSquare(dftImageArray, width);

    // Save result to file
    image.SaveImageData("MyAfter2D.txt", dftImageArray, width, height);
  } else {
    // Blocking send for data
    int rc = MPI_Send(&sendBuf[0], 2 * width * rowPerRank, MPI_LONG, MASTER_RANK, 0, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS) {
      cout << "Rank " << rank << " send failed, rc " << rc << endl;
      MPI_Finalize();
      exit(1);
    }
  }
}

void Transform1D(Complex* h, int w, Complex* H)
{
  // Implement a simple 1-d DFT using the double summation equation
  // given in the assignment handout.  h is the time-domain input
  // data, w is the width (N), and H is the output array.

  double val = 2.0 * M_PI / w;

  // Compute DFT
  // For each output H[n]
  for (int n = 0; n < w; n++) {
    // Iterate through each input h[k]
    H[n] = Complex(0,0);
    for (int k = 0; k < w; k++) {
      H[n] = H[n] + (h[k] * Complex(cos(val * n * k), - sin(val * n * k)));
    }
  }
}

int main(int argc, char** argv)
{
  string fn("Tower.txt"); // default file name
  if (argc > 1) fn = string(argv[1]);  // if name specified on cmd line

  // MPI initialization
  int rc = MPI_Init(&argc,&argv);
  if (rc != MPI_SUCCESS) {
    printf ("Error starting MPI program. Exiting.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  // Perform transform
  Transform2D(fn.c_str());

  // Finalize MPI here
  MPI_Finalize();
}

// Implementation of the eight queens problem
// YOUR NAME HERE

#include <iostream>

using namespace std;

const int nrows = 8;
const int ncols = 8;

// For the board, zero means unoccupied and not attacked
// -1 means occupied
// Non-zero is the number of previously placed pieces attacking this space
int board[nrows][ncols];
int nSolutions = 0;

void PrintSolution()
{
  for (int r = 0; r < nrows; ++r)
    {
      for (int c = 0; c < ncols; ++c)
        {
          if (board[r][c] < 0)      cout << " Q ";
          else                      cout << " - ";
        }
      cout << endl;
    }
}

  
void Place(int r, int c)
{
  board[r][c] = -1; // Occupied
  for (int c1 = 0; c1 < ncols; ++c1)
    {
      if (c != c1) board[r][c1]++; // attacked
    }
  for (int r1 = 0; r1 < nrows; ++r1)
    {
      if (r != r1) board[r1][c]++; // attacked
    }
  int r1 = r - 1;
  int c1 = c - 1;
  // upper left diagonal
  while (r1 >= 0 && c1 >= 0)
    {
      board[r1--][c1--]++;
    }
  r1 = r - 1;
  c1 = c + 1;
  // upper right diagonal
  while (r1 >= 0 && c1 < ncols)
    {
      board[r1--][c1++]++;
    }
  r1 = r + 1;
  c1 = c - 1;
  // lower left diagonal
  while (r1 < nrows && c1 >= 0)
    {
      board[r1++][c1--]++;
    }
  r1 = r + 1;
  c1 = c + 1;
  // lower right diagonal
  while (r1 < nrows && c1 < ncols)
    {
      board[r1++][c1++]++;
    }
}

void Remove(int r, int c)
{
  board[r][c] = 0; // Not occupied
  for (int c1 = 0; c1 < ncols; ++c1)
    {
      if (c1 != c) board[r][c1]--; // no longer attacked
    }
  for (int r1 = 0; r1 < nrows; ++r1)
    {
      if (r1 != r) board[r1][c]--; // no longer attacked
    }
  int r1 = r - 1;
  int c1 = c - 1;
  // upper left diagonal
  while (r1 >= 0 && c1 >= 0)
    {
      board[r1--][c1--]--;
    }
  r1 = r - 1;
  c1 = c + 1;
  // upper right diagonal
  while (r1 >= 0 && c1 < ncols)
    {
      board[r1--][c1++]--;
    }
  r1 = r + 1;
  c1 = c - 1;
  // lower left diagonal
  while (r1 < nrows && c1 >= 0)
    {
      board[r1++][c1--]--;
    }
  r1 = r + 1;
  c1 = c + 1;
  // lower right diagonal
  while (r1 < nrows && c1 < ncols)
    {
      board[r1++][c1++]--;
    }
}

 
          
bool verbose = true;

void Try(int c)
{
  if (c == ncols)
    { // Solution found
      nSolutions++;
      if (verbose)
        {
          cout << "Found solution number " << nSolutions 
               << endl;
          PrintSolution();
        }
      return;
    }
  
  for (int r = 0; r < nrows; ++r)
    {
      // if (!verbose && c <= 1) cout << "Trying col " << c 
      //                              << " row " << r << endl;
      if (board[r][c] == 0)
        { // We can place on this square
          Place(r,c);
          Try(c + 1);
          Remove(r,c);
        }
    }
}

int main(int argc, char** argv)
{
  Try(0);
  cout << "Found " << nSolutions << " solutions" << endl;
}


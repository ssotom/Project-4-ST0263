#include <cstdio>
#include <iostream>
#include <mpi.h>
#include <string>
#include <fstream>
#include <cmath>  
#include <omp.h>
  
using namespace std;

const int nTrials = 10;
const int skipTrials = 3; // Skip first iteration as warm-up 

// function to calculate Hamming distance
int hammingDistance(string &str1, string &str2, int const myRank, int const nRanks) {
  const int rowsPerProcess = double(str1.length())/double(nRanks);
  const int myFirstRow = 1 + int(rowsPerProcess*myRank);
  const int myLastRow  = 1 + int(rowsPerProcess*(myRank+1));
 
  unsigned int count = 0;
  #pragma omp parallel for reduction(+ : count)
  for (unsigned int i = myFirstRow; i < myLastRow; i++) {
    if (str1[i] != str2[i])
      count++;
  }
  unsigned int result = 0;
  MPI_Allreduce(&count, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  return result;
}

double stats(double &x, double &dx) {
  x  /= (double)(nTrials - skipTrials);
  dx  = sqrt(dx/double(nTrials - skipTrials) - x*x);
}

int main(int argc, char** argv) {

  MPI_Init(&argc, &argv);
  int myRank, nRanks;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

  if(argc < 3) {
    printf("Usage: %s {file01 {file02}\n", argv[0]);
    exit(1);
  }

 string str1, str2;
 ifstream file1(argv[1]);
 ifstream file2(argv[2]);
 getline(file1, str1);
 getline(file2, str2);

 if (myRank == 0) {
  printf("\n\033[1mHammingDistance\033[0m\n");
  printf("\033[1m%5s %15s\033[0m\n", "Step", "Time, ms"); fflush(stdout);
 }

 double t, dt;

 unsigned int result = 0;

 for (int iTrial = 1; iTrial <= nTrials; iTrial++) {
 
  MPI_Barrier(MPI_COMM_WORLD); 
  const double t0 = omp_get_wtime();

  result = hammingDistance(str1, str2, myRank, nRanks);

  const double t1 = omp_get_wtime();
  MPI_Barrier(MPI_COMM_WORLD);

  const double ts   = t1-t0; // time in seconds
  const double tms  = ts*1.0e3; // time in milliseconds

  if (iTrial > skipTrials) { // Collect statistics
    t  += tms; 
    dt += tms*tms;
  }
  // Output performance
  if (myRank == 0) {
   printf("%5d %15.3f %s\n", iTrial, tms, (iTrial<=skipTrials?"*":""));
   fflush(stdout);
 }
 }

 if (myRank == 0) {
  stats(t, dt);   
  printf("----------------------------\n");
  printf("\033[1m%s\033[0m\n%8s   \033[42m%8.2f+-%.2f\033[0m\n",
    "Average performance:", "", t, dt);
  printf("----------------------------\n");
  printf("Result: %d\n", result);
  printf("----------------------------\n"); 
  printf("* - warm-up, not included in average\n\n");
 }

 MPI_Finalize();

}

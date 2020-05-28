#include <cstdio>
#include<iostream>
#include<string>
#include <fstream>
#include <cmath>  
#include <omp.h>
  
using namespace std;

const int nTrials = 10;
const int skipTrials = 3; // Skip first iteration as warm-up 

// function to calculate Hamming distance
int hammingDistance(string &str1, string &str2) {
  int count = 0;
  #pragma omp parallel for reduction(+ : count)
  for(int i = 0; i < str1.length(); i = i++) {
    if (str1[i] != str2[i])
      count++;
  }
  return count;
}

double stats(double &x, double &dx) {
  x  /= (double)(nTrials - skipTrials);
  dx  = sqrt(dx/double(nTrials - skipTrials) - x*x);
}

int main(int argc, char** argv) {

 if(argc < 3) {
    printf("Usage: %s {file01 {file02}\n", argv[0]);
    exit(1);
  }

 string str1, str2;
 ifstream file1(argv[1]);
 ifstream file2(argv[2]);
 getline(file1, str1);
 getline(file2, str2);

 printf("\n\033[1mHammingDistance\033[0m\n");
 printf("\033[1m%5s %15s\033[0m\n", "Step", "Time, ms"); fflush(stdout);

 double t, dt;

 int result = 0;

 for (int iTrial = 1; iTrial <= nTrials; iTrial++) {
  const double t0 = omp_get_wtime();

  result = hammingDistance(str1, str2);

  const double t1 = omp_get_wtime();

  const double ts   = t1-t0; // time in seconds
  const double tms  = ts*1.0e3; // time in milliseconds

  if (iTrial > skipTrials) { // Collect statistics
    t  += tms; 
    dt += tms*tms;
  }
  // Output performance
  printf("%5d %15.3f %s\n", iTrial, tms, (iTrial<=skipTrials?"*":""));
  fflush(stdout);
 }

 stats(t, dt);   
 printf("----------------------------\n");
 printf("\033[1m%s\033[0m\n%8s   \033[42m%8.2f+-%.2f\033[0m\n",
	"Average performance:", "", t, dt);
 printf("----------------------------\n");
 printf("Result: %d\n", result);
 printf("----------------------------\n"); 
 printf("* - warm-up, not included in average\n\n");

}
#include <cstdio>
#include<iostream>
#include<string>
#include <fstream>  
#include <omp.h>
  
using namespace std; 

// function to calculate Hamming distance
int hammingDistance(string &str1, string &str2) {
  int count = 0;
  for(int i = 0; i < str1.length(); i = i++) {
    if (str1[i] != str2[i])
      count++;
  }
  return count;
}

int main(int argc, char** argv) {
  //char str1[] = "Hella";
  //char str2[] = "Hello";
  string str1, str2;
  ifstream file1("in01");
  ifstream file2("in02");
  getline(file1, str1);
  getline(file2, str2);

  int result = hammingDistance(str1, str2);
  cout << "Result: " << result << endl;
}


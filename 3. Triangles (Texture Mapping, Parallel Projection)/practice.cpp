#include <iostream>
#include <cmath>
#include <fstream>
#include "vectorMath.hpp"
using namespace std;


int main () {
  ifstream readfile;
  readfile.open("input.txt");
  int count_v = 0;
  int count_f = 0;

  string w = "2.9";

  int hello[100] = {1,2,3,4,54,1,345,12345};

  cout << hello[stoi(w)] << endl;


  string word;
  while (readfile >> word) {
    if (word.compare("v") == 0)
      count_v ++;
    else if (word.compare("f") == 0)
      count_f ++;
  }

  cout << count_v << endl;
  cout << count_f << endl;



  return 0;
}

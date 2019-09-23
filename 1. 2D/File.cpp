#include <iostream>
#include <fstream>
#include <cmath>
#include "File.h"
#include "vectorMath.h"

using namespace std;

// check if it is a txt file
bool isTextFile (string fname) {
  return fname.find(".txt") == fname.length()-4;
}

// check if the file is empty
bool isEmptyFile (ifstream& file) {
  return file.peek() == ifstream::traits_type::eof();
}

// check whether or not command and follow up numbers have no issue.
bool isValid (string command, int numberOfArguments) {
  // TODO:
  // if the text line says eye and has 3 numbers following
  // it returns True
  return true;
}

// check whether the string is number
bool isNumber (string s) {
  int countDecimal = 0;
  for (int i = 0; i < s.length(); i++){
    if (i == 0 && s[i] == '-') {
      continue;
    }
    else if (isdigit(s[i]) == false) {
      if (s[i] != '.' || countDecimal >= 1) {
        return false;
      } else {
        countDecimal++;
      }
    }
  }
  return true;
}

// txt file to ppm file
string txtToPPM (string fname) {
  // fname is "filename.txt" format
  string ppm = ".ppm";
  fname.replace(fname.end() - 4, fname.end(), ppm);
  return fname;
}




// ERROR FUNCTIONS
void printTextFileError (string fname) {
  if (!isTextFile(fname)) {
    cout << "ERROR: Not a text file. " << endl;
    exit(1);
  }
}

void printNotNumberError (string s) {
  if (!isNumber(s)) {
    cout << "ERROR: Not enough value. Check txt file " << endl;
    exit(1);
  }
}

void printValueError (int s) {
  if (s < 0.0 || s > 1.0) {
    cout << "ERROR: Value wrong (0 - 10)" << endl;
    exit(1);
  }
}

void printZeroLengthVectorError (Vector v) {
  if (v.x == 0.0 && v.y == 0.0 && v.z == 0.0) {
    cout << "ERROR: Zero Length Vector Error" << endl;
    exit(1);
  }
}

void printParallelVectorError (Vector up, Vector view) {
  Vector v = crossProduct(up, view);
  if (v.x == 0 && v.y == 0 && v.z == 0) {
    cout << "ERROR: up and view direction parallel" << endl;
    exit(1);
  }
}

void printFOVError (float vfov) {
  if (vfov > 180 || vfov == 0) {
    cout << "ERROR: Vertical Field of View invalid" << endl;
    exit(1);
  }
}

void printImageSizeError (int x, int y) {
  if (x <= 0 || y <= 0) {
    cout << "ERROR: Image size invalid" << endl;
    exit(1);
  }
}
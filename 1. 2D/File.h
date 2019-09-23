#include <string>
#include "vectorMath.h"

#ifndef FILE_H
#define FILE_H

bool isNumber (std::string s);
bool isEmptyFile (std::ifstream& file);
bool isTextFile (std::string fname);
bool isValid (std::string command, int numberOfArguments);

std::string txtToPPM (std::string fname);

void printTextFileError (std::string fname);
void printNotNumberError (std::string s);
void printValueError (int s);
void printZeroLengthVectorError (Vector v);    // for zero length vector
void printParallelVectorError (Vector up, Vector view);
void printFOVError (float vfov);
void printImageSizeError (int x, int y);

#endif  // FILE_H

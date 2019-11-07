#include <string>
#include "VectorMath.hpp"

#ifndef FILE_HPP
#define FILE_HPP

bool isNumber (std::string s);
bool isPPMFile (std::string fname);
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
void printLightError (float x);
void printZeroToOneError (float a, float d, float s);
#endif  // FILE_HPP

#include <iostream>
#include <cmath>
#include <fstream>
#include "vectorMath.h"
using namespace std;

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


int main () {
  ifstream readfile;
  readfile.open("input.txt");
  string        word;
  Vector        eye;
  Vector        viewdir, updir;
  float         vfov;
  int           imsize_x, imsize_y;
  ColorType     bkgcolor, mtlcolor;
  SphereType    sphere;
  EllipsoidType ellipsoid;

  while (readfile >> word) {
    // Check whether or not it is valid statement. ex) eye..
    if (!isNumber(word)) {
      // starting to store data.
      if (word.compare("eye") == 0) {
        readfile >> word;
        eye.x = stof(word);
        readfile >> word;
        eye.y = stof(word);
        readfile >> word;
        eye.z = stof(word);
      } else if (word.compare("viewdir") == 0) {
        readfile >> word;
        viewdir.x = stof(word);
        readfile >> word;
        viewdir.y = stof(word);
        readfile >> word;
        viewdir.z = stof(word);
      } else if (word.compare("updir") == 0) {
        readfile >> word;
        updir.x = stof(word);
        readfile >> word;
        updir.y = stof(word);
        readfile >> word;
        updir.z = stof(word);
      } else if (word.compare("vfov") == 0) {
        readfile >> word;
        vfov = stof(word);
      } else if (word.compare("imsize") == 0) {
        readfile >> word;
        imsize_x = stof(word);
        readfile >> word;
        imsize_y = stof(word);
      } else if (word.compare("bkgcolor") == 0) {
        readfile >> word;
        bkgcolor.r = stof(word);
        readfile >> word;
        bkgcolor.g = stof(word);
        readfile >> word;
        bkgcolor.b = stof(word);
        break;    // going to next step !
      } else if (word.compare("mtlcolor") == 0) {
        readfile >> word;
        mtlcolor.r = stof(word);
        readfile >> word;
        mtlcolor.g = stof(word);
        readfile >> word;
        mtlcolor.b = stof(word);
        break;    // going to next step !
      } else {
        cout << "ERROR: incorrect information" << endl;
        cout << "       should have eye, viewdir, updir, vfov, imsize followed by numbers" << endl;
        exit(1);
      }
    } else {
      cout << "ERROR: incorrect information" << endl;
      cout << "       Should have info name first rather than numbers or sphere" << endl;
    }
  }
  // Next step
  while (readfile >> word) {
    if (!isNumber(word)) {
      if (word.compare("bkgcolor") == 0) {
        readfile >> word;
        bkgcolor.r = stof(word);
        readfile >> word;
        bkgcolor.g = stof(word);
        readfile >> word;
        bkgcolor.b = stof(word);
      } else if (word.compare("mtlcolor") == 0) {
        readfile >> word;
        mtlcolor.r = stof(word);
        readfile >> word;
        mtlcolor.g = stof(word);
        readfile >> word;
        mtlcolor.b = stof(word);
      } else if (word.compare("sphere") == 0) {
        readfile >> word;
        sphere.x = stof(word);
        readfile >> word;
        sphere.y = stof(word);
        readfile >> word;
        sphere.z = stof(word);
        readfile >> word;
        sphere.r = stof(word);
        // Color Calculation goes here.
      } else if (word.compare("ellipsoid") == 0) {
        readfile >> word;
        ellipsoid.x = stof(word);
        readfile >> word;
        ellipsoid.y = stof(word);
        readfile >> word;
        ellipsoid.z = stof(word);
        readfile >> word;
        ellipsoid.rx = stof(word);
        readfile >> word;
        ellipsoid.ry = stof(word);
        readfile >> word;
        ellipsoid.rz = stof(word);
        readfile >> word;
        ellipsoid.r = stof(word);
      } else {
        cout << "ERROR: incorrect information" << endl;
        cout << "       should have mtlcolor, sphere followed by numbers" << endl;
        cout << "       did you updated new info? " << endl;
        exit(1);
      }
    } else {
      cout << "ERROR: incorrect information" << endl;
      cout << "       Should have info name first rather than numbers or sphere" << endl;
    }
  }

  cout << eye.x << " " << eye.y << " " << eye.z << endl;
  cout << sphere.z << endl;
  Vector temp = viewdir - updir;
  cout << temp.y << endl;
  return 0;
}

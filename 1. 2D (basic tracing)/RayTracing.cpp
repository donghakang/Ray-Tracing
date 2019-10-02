// Dongha Kang
// September 11th, 2019
// CSCI 5706, University of Minnesota

#include <iostream>
#include <fstream>
#include <cmath>
#include "vectorMath.h"
#include "File.h"
using namespace std;


int main() {
  string        filename;
  ifstream      readfile;
  ofstream      savefile;

  cout << "Type file name (.txt): ";
  cin  >> filename;

  printTextFileError(filename);

  readfile.open(filename);
  // check rather readfile is empty
  if (readfile.good() == false) {
    cout << "ERROR: Text file not available. " << endl;
    exit(1);
  }
  if (isEmptyFile(readfile)) {
    cout << "ERROR: Empty File." << endl;
    exit(1);
  }


  // FILE CHECK ENDED HERE.

  string        word;
  Vector        eye;
  Vector        viewdir, updir;
  float         vfov;
  int           imsize_x, imsize_y;
  ColorType     bkgcolor, mtlcolor;
  SphereType    sphere;
  EllipsoidType ellipsoid;

  float         aspect_ratio;              // imsize_x / imsize_y;
  float         w, h;                      // length of viewing window width and height
  Vector        u, v;                      // u, v vector
  Vector        ur, ul, ll, lr;            // viewing window
  Vector        view_width, view_height;
  Vector        view_center_w, view_center_h;
  Vector        view_direction;
  ColorType     ListOfColors[100];         // store colors
  SphereType    ListOfSphere[100];
  int           color_entry = 0;
  int           sphere_entry = 0;          // sphere Entry related to List of Sphere

  RayType       ray;                       // store eye, and direction
  ColorType     returnColor;               // ColorType returnColor;




  while (readfile >> word) {
    // Check whether or not it is valid statement. ex) eye..
    if (!isNumber(word)) {
      // starting to store data.
      if (word.compare("eye") == 0) {
        readfile >> word;
        printNotNumberError(word);
        eye.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        eye.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        eye.z = stof(word);
      } else if (word.compare("viewdir") == 0) {
        readfile >> word;
        printNotNumberError(word);
        viewdir.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        viewdir.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        viewdir.z = stof(word);
      } else if (word.compare("updir") == 0) {
        readfile >> word;
        printNotNumberError(word);
        updir.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        updir.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        updir.z = stof(word);
      } else if (word.compare("vfov") == 0) {
        readfile >> word;
        printNotNumberError(word);
        vfov = stof(word);
        printFOVError(vfov);
        vfov = vfov * PI / 180;
      } else if (word.compare("imsize") == 0) {
        readfile >> word;
        printNotNumberError(word);
        imsize_x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        imsize_y = stof(word);
        printImageSizeError(imsize_x, imsize_y);
      } else if (word.compare("bkgcolor") == 0) {
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.b = int(stof(word) * 255);
        break;    // going to next step !
      } else if (word.compare("mtlcolor") == 0) {
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.b = int(stof(word) * 255);
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

  printParallelVectorError(updir, viewdir);
  printParallelVectorError(viewdir, updir);
  printZeroLengthVectorError(updir);
  printZeroLengthVectorError(viewdir);


  // Set values with the items that i have
  aspect_ratio = (float)(imsize_x) / (float)(imsize_y);   // aspect ratio
  h = 2.0 * tan(vfov / 2.0);                              // view height length
  w = h * aspect_ratio;                                   // view width length
  u = u_vector(updir, viewdir);                           // u vector
  v = v_vector(u, viewdir);                               // v vector

  // set view window .. d = 1
  ul = eye + UnitVector(viewdir) - m(u, w/2.0) + m(v, h/2.0);
  ur = eye + UnitVector(viewdir) + m(u, w/2.0) + m(v, h/2.0);
  ll = eye + UnitVector(viewdir) - m(u, w/2.0) - m(v, h/2.0);
  lr = eye + UnitVector(viewdir) + m(u, w/2.0) - m(v, h/2.0);

  view_width = d(ur - ul, imsize_x - 1.0);
  view_height = d(ll - ul, imsize_y - 1.0);
  view_center_w = m(view_width, 0.5);             // ∆ch
  view_center_h = m(view_height, 0.5);            // ∆cv



  // Next step
  while (readfile >> word) {
    if (!isNumber(word)) {
      if (word.compare("bkgcolor") == 0) {
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.b = int(stof(word) * 255);
      } else if (word.compare("mtlcolor") == 0) {
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.b = int(stof(word) * 255);
        ListOfColors[color_entry] = mtlcolor;
        color_entry ++;
      } else if (word.compare("sphere") == 0) {
        readfile >> word;
        printNotNumberError(word);
        sphere.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        sphere.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        sphere.z = stof(word);
        readfile >> word;
        printNotNumberError(word);
        sphere.r = stof(word);
        sphere.m = color_entry - 1;           // save the color data.
        ListOfSphere[sphere_entry] = sphere;
        sphere_entry ++;
      } else if (word.compare("ellipsoid") == 0) {
        readfile >> word;
        printNotNumberError(word);
        ellipsoid.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        ellipsoid.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        ellipsoid.z = stof(word);
        readfile >> word;
        printNotNumberError(word);
        ellipsoid.rx = stof(word);
        readfile >> word;
        printNotNumberError(word);
        ellipsoid.ry = stof(word);
        readfile >> word;
        printNotNumberError(word);
        ellipsoid.rz = stof(word);
        readfile >> word;
        printNotNumberError(word);
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
      exit(1);
    }
  }

  // OUTPUT
  savefile.open(txtToPPM(filename));
  savefile << "P3" << endl;
  savefile << imsize_x << " " << imsize_y << endl;     // followed by width and height (px)
  savefile << 255 << endl;                             // followed by 255 (r,g,b)


  for (int i = 0; i < imsize_y; i ++) {
    for (int j = 0; j < imsize_x; j ++ ) {
      view_direction = ul + m(view_width, j) + m(view_height, i) + view_center_w + view_center_h;
      ray = rayDirection(eye, view_direction);

      returnColor = Trace_Ray(ray, ListOfSphere, ListOfColors, bkgcolor, sphere_entry);
      savefile << returnColor.r << " " << returnColor.g << " " << returnColor.b << " ";
    }
    // enables the fun background 
    bkgcolor = BackgroundColor(bkgcolor, imsize_y, i);
  }

  return 0;
}

// Dongha Kang
// September 11th, 2019
// CSCI 5706, University of Minnesota

#include <iostream>
#include <cmath>
using namespace std;

#define PI 3.1415926535897

// *** input file info ***
// eye eye_x, eye_y, eye_z                        view origin
// viewdir vdir_x, vdir_y, vdir_z                 viewing direction
// updir up_x, up_y, up_z                         up direction
// vfov fov_v                                     vertical field of view
// imsize width height                            image size
// bkgcolor r, g, b
// mtlcolor r, g, b                               material color
// sphere c_x, c_y, c_z, r
// ellipsoid c_x, c_y, c_z, r_x, r_y, r_z


// TODO: Check the text file contains all of the information
struct Vector {
  double x;
  double y;
  double z;
};

struct Color {
  double r;
  double g;
  double b;
};

// TODO: Basic functions about the vectors
double distance (Vector v1) {
  return sqrt(pow(v1.x, 2) + pow(v1.y, 2) + pow(v1.z, 2));
}

double dotProduct (Vector v1, Vector v2) {
  return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

double angleBetweenVectors (Vector v1, Vector v2) {
  return acos(dotProduct(v1, v2) / (distance(v1) * distance(v2)));
}

Vector crossProduct (Vector s_v, Vector d_v) {
  Vector returnVector;
  returnVector.x = (s_v.y * d_v.z) - (s_v.z * d_v.y);
  returnVector.y = -((s_v.x * d_v.z) - (s_v.z * d_v.x));
  returnVector.z = (s_v.x * d_v.y) - (s_v.y * d_v.x);
  return Vector;
}

Vector unitLength (Vector)

int main() {
  Vector v;
  v.x = 10.0;
  v.y = 10.0;
  v.z = 10.0;

  cout << "x : ";
  cin >> v.x;
  cout << "y : ";
  cin >> v.y;
  cout << "z : ";
  cin >> v.z;

  cout << "Distance : " << distance(v) << endl;
}

// TODO: Define an array of sufficient size to store the color values of your image.

#include <iostream>
#include <cmath>
#include <fstream>
#include "File.hpp"
#include "VectorMath.hpp"
#include "Shading.hpp"
#include "SpecialTexture.hpp"

using namespace std;

#define EPSOLIN 0.0001
#define PI 3.1415926535897


ColorType** readTexture (string filename) {
  if (!isPPMFile(filename)) {
    cout << "ERROR: is not PPM File" << endl;
    exit(1);
  }
  int       width, height;
  ifstream  readfile;
  string    word;
  ColorType **picture;
  float     r, g, b;

  readfile.open(filename);

  readfile >> word;         // P3
  readfile >> word;         // Width
  printNotNumberError(word);
  width = stoi(word);
  readfile >> word;         // Height
  printNotNumberError(word);
  height = stoi(word);
  readfile >> word;         // 255
  printNotNumberError(word);
  if (stoi(word) != 255) {
    cout << "ERROR: PPM File has a problem (255)" << endl;
    exit(1);
  }

  picture = new ColorType*[height];
  for (int y = 0; y < height; y ++) {
    picture[y] = new ColorType[width];
    for (int x = 0; x < width; x ++) {
      readfile >> word;
      printNotNumberError(word);
      r = stof(word) / 255.0;
      readfile >> word;
      printNotNumberError(word);
      g = stof(word) / 255.0;
      readfile >> word;
      printNotNumberError(word);
      b = stof(word) / 255.0;

      picture[y][x] = {r, g, b};
    }
  }

  readfile.close();
  readfile.clear();

  return picture;
}


ColorType TexturePosition(ColorType** texture, int w, int h, Object obj) {
  if (obj.objectType == 0) {
    SphereType  sphere = obj.sphere;
    Vector p    = obj.intersection;

    float Nx    = (p.x - sphere.x) / sphere.r;
    float Ny    = (p.y - sphere.y) / sphere.r;
    float Nz    = (p.z - sphere.z) / sphere.r;
    float phi   = acos(Ny);
    float theta = atan(Nx/Nz);

    float u     = (theta + PI) / (2 * PI);
    float v     = phi / PI;

    int width   = round(u * (w - 1));
    int height  = round(v * (h - 1));

    return texture[height][width];
  } else {
    Function   function = obj.triangle;
    Vector     p   =   obj.intersection;

    float alpha = Alpha(function, p, calculateArea(function));
    float beta  = Beta(function, p, calculateArea(function));
    float gamma = Gamma(function, p, calculateArea(function));

    float u    = function.vt1.u * alpha + function.vt2.u * beta + function.vt3.u * gamma;
    float v    = function.vt1.v * alpha + function.vt2.v * beta + function.vt3.v * gamma;
    int width  = round(u * (w - 1));
    int height = round(v * (h - 1));

    return texture[height][width];
  }
}


// Fresnel Reflectance Coefficient
float Fresnel_Reflectance(float n, Vector n_vector, Vector i_vector) {
  float f0, fr;
  float cosine_theta;

  // i_vector is the intersection to eye
  cosine_theta = dotProduct(unitVector(n_vector), unitVector(i_vector));

  f0 = pow((n-1)/(n+1), 2);
  fr = f0 + ((1-f0) * pow(1-cosine_theta, 5));

  return fr;
}

Vector Specular_Reflection(Vector n_vector, Vector i_vector) {
  n_vector = unitVector(n_vector);
  i_vector = unitVector(i_vector);

  float i = 2.0 * dotProduct(n_vector, i_vector);
  return unitVector((i * n_vector) - i_vector);
}

RayType NewRayReflection(Vector n, Vector intersection, Vector reflection) {
  RayType ray;
  ray.x  = intersection.x + (EPSOLIN * n.x);
  ray.y  = intersection.y + (EPSOLIN * n.y);
  ray.z  = intersection.z + (EPSOLIN * n.z);
  ray.dx = reflection.x;
  ray.dy = reflection.y;
  ray.dz = reflection.z;
  return ray;
}



// Transparrency
Vector Transparent_Vector(float ni, float nt, Vector n_vector, Vector i_vector) {
  float refract      = nt/ni;
  float cosine_theta = dotProduct(n_vector, i_vector);

  Vector temp1 = sqrt(1.0 - (pow(refract, 2) * (1 - pow(cosine_theta, 2)))) * (-1 * n_vector);
  Vector temp2 = refract * ((cosine_theta * n_vector) - i_vector);

  return unitVector(temp1 + temp2);
}


RayType NewRayRefraction(Vector n, Vector intersection, Vector refraction) {
  RayType ray;
  ray.x  = intersection.x + (EPSOLIN * -n.x);
  ray.y  = intersection.y + (EPSOLIN * -n.y);
  ray.z  = intersection.z + (EPSOLIN * -n.z);
  ray.dx = refraction.x;
  ray.dy = refraction.y;
  ray.dz = refraction.z;
  return ray;
}

#include <iostream>
#include <cmath>
#include <fstream>
#include "File.hpp"
#include "vectorMath.hpp"
#include "Shading.hpp"
#include "Texture.hpp"

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
  int       r, g, b;

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
      r = stoi(word);
      readfile >> word;
      printNotNumberError(word);
      g = stoi(word);
      readfile >> word;
      printNotNumberError(word);
      b = stoi(word);

      picture[y][x] = {r, g, b};
    }
  }

  readfile.close();
  readfile.clear();

  return picture;
}

ColorType TexturePosition(ColorType** texture, int w, int h,
        SphereType sphere, Vector p) {
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
}


ColorType TexturePosition(ColorType** texture, int w, int h,
        Function function, float alpha, float beta, float gamma) {

  float u    = function.vt1.u * alpha + function.vt2.u * beta + function.vt3.u * gamma;
  float v    = function.vt1.v * alpha + function.vt2.v * beta + function.vt3.v * gamma;
  int width  = round(u * (w - 1));
  int height = round(v * (h - 1));

  return texture[height][width];
}

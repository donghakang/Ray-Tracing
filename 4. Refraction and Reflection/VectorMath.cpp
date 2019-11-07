#include <cmath>
#include "VectorMath.hpp"
#include <iostream>
using namespace std;

#define EPSOLIN 0.0001

// ||v1||
float distance (Vector v) {
  return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

// v1 . v2
float dotProduct (Vector v1, Vector v2) {
  return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float angleBetweenVectors (Vector v1, Vector v2) {
  return acos(dotProduct(v1, v2) / (distance(v1) * distance(v2)));
}

Vector unitVector (Vector v) {
  Vector returnVector;
  returnVector.x = v.x / distance(v);
  returnVector.y = v.y / distance(v);
  returnVector.z = v.z / distance(v);
  return returnVector;
}

// v1 X v2
Vector crossProduct (Vector v1, Vector v2) {
  Vector returnVector;
  returnVector.x = (v1.y * v2.z) - (v1.z * v2.y);
  returnVector.y = (v1.z * v2.x) - (v1.x * v2.z);
  returnVector.z = (v1.x * v2.y) - (v1.y * v2.x);
  return returnVector;
}



// Vector Operation
// Subtract between two vectors
Vector operator-(const Vector &v1, const Vector &v2) {
  return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

// Add between two vectors
Vector operator+(const Vector &v1, const Vector &v2) {
  return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

// multiply between number and a vector
Vector operator*(float num, const Vector &v) {
  return {num * v.x, num * v.y, num * v.z};
}

// divide a vector with the number
Vector operator/(const Vector &v, float num) {
  return {v.x / num, v.y / num, v.z / num};
}


// Color Operation
ColorType operator-(const ColorType &c1, const ColorType &c2) {
  return {c1.r - c2.r, c1.g - c2.g, c1.b - c2.b};
}

// Add between two vectors
ColorType operator+(const ColorType &c1, const ColorType &c2) {
  return {c1.r + c2.r, c1.g + c2.g, c1.b + c2.b};
}

// multiply between number and a vector
ColorType operator*(float num, const ColorType &c) {
  return {num * c.r, num * c.g, num * c.b};
}

ColorType operator*(const ColorType &c1, const ColorType &c2) {
  return {c1.r * c2.r, c1.g * c2.g, c1.b * c2.b};
}
// divide a vector with the number
ColorType operator/(const ColorType &c, float num) {
  return {c.r / num, c.g / num, c.b / num};
}

bool operator==(const ColorType &c1, const ColorType &c2) {
  if (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b) {
    return true;
  } else {
    return false;
  }
}


// Different Type of Vectors
// Vector nVector (Vector intersection, )
Vector uVector (Vector up, Vector view) {
  Vector returnVector = crossProduct(view, up);
  return unitVector(returnVector);
}

Vector vVector (Vector u, Vector view) {
  Vector returnVector = crossProduct(u, view);
  return unitVector(returnVector);
}

Vector nVector(Vector intersection, SphereType sphere) {
  Vector s = {sphere.x, sphere.y, sphere.z};
  return unitVector((intersection - s) / sphere.r);
}

Vector nVector (Vector v1, Vector v2, Vector v3) {
  Vector e1, e2;
  e1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
  e2 = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
  return crossProduct(e1, e2);
}

Vector nVector (Function f) {
  return unitVector(nVector(f.v1, f.v2, f.v3));
}

Vector nVector(Function f, float alpha, float beta, float gamma) {
  return unitVector((alpha * f.vn1) + (beta * f.vn2) + (gamma * f.vn3));
}

Vector nVector(Object current) {
  Function    function;
  Vector      p;
  float       alpha, beta, gamma;
  Vector      n;
  if (current.objectType == 0) {
    n = nVector(current.intersection, current.sphere);
  }
  // triangles
  else {
    function = current.triangle;
    // hard shading.
    if ((function.vn1.x == 0 && function.vn1.y == 0 && function.vn1.z == 0) ||
        (function.vn2.x == 0 && function.vn2.y == 0 && function.vn2.z == 0) ||
        (function.vn3.x == 0 && function.vn3.y == 0 && function.vn3.z == 0)) {
      n = nVector(function);
    }
    // smooth shading.
    else {
      p     = current.intersection;
      alpha = Alpha(function, p, calculateArea(function));
      beta  = Beta(function, p, calculateArea(function));
      gamma = Gamma(function, p, calculateArea(function));
      n = nVector(function, alpha, beta, gamma);
    }
  }
  return n;
}

Vector iVector(RayType ray) {
  return unitVector({-ray.dx, -ray.dy, -ray.dz});
}


Vector lVector(Vector intersection, Light l) {
  Vector light = {l.v.x, l.v.y, l.v.z};
  // directional light
  if (l.w == 0) {
    return unitVector(-1 * light);
  }
  // point light
  else {
    return unitVector(light - intersection);
  }
}



// Efficient Functions

Vector intersect(RayType ray, float t) {
  Vector eye = {ray.x, ray.y, ray.z};
  Vector i   = {ray.dx, ray.dy, ray.dz};
  return eye + (t * i);
}

float calculateD (Vector n, Vector v) {
  float returnValue;
  returnValue = -1 * dotProduct(n, v);
  return returnValue;
}

float calculateArea (Function f) {
  Vector e1, e2;
  e1 = f.v2 - f.v1;
  e2 = f.v3 - f.v1;
  return distance(crossProduct(e1, e2)) / 2.0;
}

float Alpha (Function f, Vector p, float area) {
  Vector e3 = p - f.v2;
  Vector e4 = p - f.v3;
  return (distance(crossProduct(e3, e4)) / 2.0) / area;
}

float Beta (Function f, Vector p, float area) {
  Vector e4 = p - f.v3;
  Vector e2 = f.v3 - f.v1;
  return (distance(crossProduct(e4, e2)) / 2.0) / area;
}

float Gamma (Function f, Vector p, float area) {
  Vector e1 = f.v2 - f.v1;
  Vector e3 = p - f.v2;
  return (distance(crossProduct(e1, e3)) / 2.0) / area;
}

bool checkInPlane (float alpha, float beta, float gamma) {
  bool returnValue = false;
  if (alpha >= 0 && alpha <= 1)
    if (beta >= 0 && beta <= 1)
      if (gamma >= 0 && gamma <= 1)
        if (fabs(alpha + beta + gamma - 1) < EPSOLIN)
          return true;
  return returnValue;
}

// change 2 points to a ray.
RayType rayDirection (Vector origin, Vector p) {
  Vector dir = unitVector(p - origin);
  return {origin.x, origin.y, origin.z, dir.x, dir.y, dir.z};
}

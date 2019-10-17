#include <cmath>
#include "vectorMath.hpp"
#include <iostream>
using namespace std;

#define EPSOLIN 0.0001


void pv (Vector v) {
  cout << v.x << "   " << v.y << "   " << v.z << endl;
}



// ||v1||
float distance (Vector v1) {
  return sqrt(pow(v1.x, 2) + pow(v1.y, 2) + pow(v1.z, 2));
}

// v1 . v2
float dotProduct (Vector v1, Vector v2) {
  return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float angleBetweenVectors (Vector v1, Vector v2) {
  return acos(dotProduct(v1, v2) / (distance(v1) * distance(v2)));
}

Vector UnitVector (Vector v) {
  Vector returnVector;
  returnVector.x = v.x / distance(v);
  returnVector.y = v.y / distance(v);
  returnVector.z = v.z / distance(v);
  return returnVector;
}

// s_v X d_v
Vector crossProduct (Vector s_v, Vector d_v) {
  Vector returnVector;
  returnVector.x = (s_v.y * d_v.z) - (s_v.z * d_v.y);
  returnVector.y = (s_v.z * d_v.x) - (s_v.x * d_v.z);
  returnVector.z = (s_v.x * d_v.y) - (s_v.y * d_v.x);
  return returnVector;
}



// VECTOR MATHEMATICS

// Subtract between two vectors
Vector operator-(const Vector &v1, const Vector &v2) {
	// use the Cents constructor and operator+(int, int)
	// we can access m_cents directly because this is a friend function
  Vector returnVector = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
  return returnVector;
}

// Add between two vectors
Vector operator+(const Vector &v1, const Vector &v2) {
	// use the Cents constructor and operator+(int, int)
	// we can access m_cents directly because this is a friend function
  Vector returnVector = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
  return returnVector;
}

// multiply vector by number
Vector m(const Vector &v, float num) {
  return {num * v.x, num * v.y, num * v.z};
}

// divide vector by number
Vector d(const Vector &v, float num) {
  return {v.x /(float)num, v.y / (float)num, v.z / (float)num};
}


// Subtract between two Colors
ColorType operator-(const ColorType &c1, const ColorType &c2) {
  ColorType returnColor = {c1.r - c2.r, c1.g - c2.g, c1.b - c2.b};
  return returnColor;
}

// Subtract between two Colors
ColorType operator+(const ColorType &c1, const ColorType &c2) {
  ColorType returnColor = {c1.r + c2.r, c1.g + c2.g, c1.b + c2.b};
  return returnColor;
}


// Clamps the colors
ColorType clamp(ColorType c) {
  if (c.r < 0) {
    c.r = 0;
  } else if (c.r > 255) {
    c.r = 255;
  }
  if (c.g < 0) {
    c.g = 0;
  } else if (c.g > 255) {
    c.g = 255;
  }
  if (c.b < 0) {
    c.b = 0;
  } else if (c.b > 255) {
    c.b = 255;
  }
  return c;
}


// Light
Vector intersect(RayType ray, float t) {
  Vector eye = {ray.x, ray.y, ray.z};
  Vector i   = {ray.dx, ray.dy, ray.dz};
  return eye + m(i, t);
}

Vector n_vector(Vector intersection, SphereType sphere) {
  Vector s = {sphere.x, sphere.y, sphere.z};
  return UnitVector(d(intersection - s, sphere.r));
}

// assume we doing position light
Vector l_vector (Vector intersection, Light l) {
  // Finite Light
  Vector light = {l.v.x, l.v.y, l.v.z};
  return UnitVector(light - intersection);
}




// ******************************************************
// *************  A S S I G N M E N T  1 C **************
// ******************************************************

Vector n_vector(Vector v1, Vector v2, Vector v3) {
  Vector e1, e2;
  e1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
  e2 = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
  return crossProduct(e1, e2);
}

Vector n_vector(Function f) {
  return UnitVector(n_vector(f.v1, f.v2, f.v3));
}

Vector n_vector(Function f, float alpha, float beta, float gamma) {
  return UnitVector(m(f.vn1, alpha) + m(f.vn2, beta) + m(f.vn3, gamma));
}

float calculateD (Vector n, Vector v) {
  float returnValue;
  returnValue = -(n.x * v.x + n.y * v.y + n.z * v.z);
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
  if (alpha > 0 && alpha < 1)
    if (beta > 0 && beta < 1)
      if (gamma > 0 && gamma < 1)
        if (fabs(alpha + beta + gamma - 1) < EPSOLIN)
          return true;
  return returnValue;
}

// Shading HERE

// Finding u, using Up direction and View direction
Vector u_vector (Vector up, Vector view) {
  Vector returnVector = crossProduct(view, up);
  return UnitVector(returnVector);
}

// Finding v, using u vector and view direction
Vector v_vector (Vector u, Vector view) {
  Vector returnVector = crossProduct(u, view);
  return UnitVector(returnVector);
}

// Equation of each ray
// o is for starting vector   .. origin
// p is for viewing window at the point .. point
RayType rayDirection (Vector o, Vector p) {
  Vector v = UnitVector(p - o);
  return {o.x, o.y, o.z, v.x, v.y, v.z};
}

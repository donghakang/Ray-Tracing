#include <string>

#ifndef VECTORMATH_HPP
#define VECTORMATH_HPP

#define PI 3.1415926535897

struct Vector {
  float x, y, z;
};

struct ColorType {
  int r, g, b;
};

struct MaterialColor {
  ColorType object;
  ColorType specular;
  float     a, d, s;
  float     n;
  int       texture;
};

struct SphereType {
  float x, y, z;
  float r;
  int   m;
};

struct Light {
  Vector v;
  float w;                // 0 if directional light, 1 if point light
  ColorType color;
  Vector c;             // for attlight
};

struct Spotlight {
  Vector v;
  Vector dir;
  float theta;
  ColorType color;
  Vector c;             // for attspotlight
};

struct RayType {
  float   x, y, z;        // direction origin
  float   dx, dy, dz;
};

// Assignment 1c
struct Texture {
  float u, v;
};

struct TexturePicture {
  int w, h;
  ColorType **picture;
};

struct Function {
  Vector v1, v2, v3;
  Vector vn1, vn2, vn3;
  Texture vt1, vt2, vt3;
  int m;
};



float distance (Vector v1);
float dotProduct (Vector v1, Vector v2);
float angleBetweenVectors (Vector v1, Vector v2);
Vector UnitVector (Vector v);
Vector crossProduct (Vector s_v, Vector d_v);

Vector operator-(const Vector &v1, const Vector &v2);
Vector operator+(const Vector &v1, const Vector &v2);
Vector m(const Vector &v, float num);
Vector d(const Vector &v, float num);

ColorType operator-(const ColorType &c1, const ColorType &c2);
ColorType operator+(const ColorType &c1, const ColorType &c2);
ColorType clamp(ColorType c);

Vector n_vector(Vector v1, Vector v2, Vector v3);
Vector n_vector(Function f);
Vector n_vector(Function f, float alpha, float beta, float gamma);
Vector n_vector(Vector intersection, SphereType sphere);
Vector u_vector (Vector up, Vector view);
Vector v_vector (Vector u, Vector view);
Vector l_vector (Vector intersection, Light l);

RayType rayDirection (Vector o, Vector p);
Vector intersect(RayType ray, float t);

// For Triangle
float calculateD (Vector n, Vector v);
float calculateArea (Function f);
float Alpha (Function f, Vector p, float area);
float Beta (Function f, Vector p, float area);
float Gamma (Function f, Vector p, float area);
bool checkInPlane (float alpha, float beta, float gamma);

#endif  // VECTORMATH_HPP

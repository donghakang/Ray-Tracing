#include <iostream>
#include <string>

#ifndef VECTORMATH_HPP
#define VECTORMATH_HPP

#define PI 3.1415926535897


// supports position and vector together.
struct Vector {
  float x, y, z;

  void print() {
    std::cout << x << "   " << y << "   " << z << std::endl;
  }
};


// supports color r,g,b;
struct ColorType {
  float r, g, b;

  void print() {
    // std::cout << r << ", " << g << ", " << b << "     ";
    std::cout << (int)(255.0 * r) << ", " << (int)(255.0 * g) << ", " << (int)(255.0 * b) << std::endl;
  }

  void clamp() {
    if (r < 0.0) {
      r = 0.0;
    } else if (r > 1.0) {
      r = 1.0;
    }
    if (g < 0) {
      g = 0.0;
    } else if (g > 1.0) {
      g = 1.0;
    }
    if (b < 0.0) {
      b = 0;
    } else if (b > 1.0) {
      b = 1.0;
    }
  }
};

struct Light {
  Vector v;
  int w;                // 0 if directional light, 1 if point light
  ColorType color;
};

struct MaterialColor {
  ColorType object;
  ColorType specular;
  float     a, d, s;
  float     n;
  float     opacity;
  float     index_of_refraction;
  int       texture;
};

struct SphereType {
  float x, y, z;
  float r;
};

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
};

struct RayType {
  float   x, y, z;        // direction origin
  float   dx, dy, dz;
  bool    refract;
};


struct Object {
  int       objectType;   // 0 if sphere, 1 if triangle

  // material location
  float     t;           // t value to find intersection point
  Vector    intersection;
  // float     t2;

  // Color
  MaterialColor material;
  int           texture_index;
  // Sphere
  SphereType sphere;
  // Triangle
  Function  triangle;
};



float distance (Vector v);
float dotProduct (Vector v1, Vector v2);
float angleBetweenVectors (Vector v1, Vector v2);
Vector unitVector (Vector v);
Vector crossProduct (Vector s_v, Vector d_v);

Vector operator-(const Vector &v1, const Vector &v2);
Vector operator+(const Vector &v1, const Vector &v2);
Vector operator*(float num, const Vector &v2);
Vector operator/(const Vector &v1, float num);

ColorType operator-(const ColorType &v1, const ColorType &v2);
ColorType operator+(const ColorType &c1, const ColorType &c2);
ColorType operator*(float num, const ColorType &c);
ColorType operator*(const ColorType &c1, const ColorType &c2);
ColorType operator/(const ColorType &c, float num);
bool operator==(const ColorType &c1, const ColorType &c2);


Vector uVector (Vector up, Vector view);
Vector vVector (Vector u, Vector view);
Vector nVector(Vector intersection, SphereType sphere);
Vector nVector (Vector v1, Vector v2, Vector v3);
Vector nVector (Function f);
Vector nVector (Function f, float alpha, float beta, float gamma);
Vector nVector (Object o);
Vector iVector (RayType ray);
Vector lVector (Vector intersection, Light l);

Vector intersect(RayType ray, float t);
float calculateD (Vector n, Vector v);
float calculateArea (Function f);
float Alpha (Function f, Vector p, float area);
float Beta (Function f, Vector p, float area);
float Gamma (Function f, Vector p, float area);
bool checkInPlane (float alpha, float beta, float gamma);
RayType rayDirection (Vector origin, Vector p);

#endif  // VECTORMATH_HPP

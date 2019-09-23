#ifndef VECTORMATH_H
#define VECTORMATH_H

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

struct Vector {
  float x;
  float y;
  float z;
};

struct ColorType {
  int r;
  int g;
  int b;
};

struct SphereType {
  float x, y, z;
  float r;
  int   m;
};

struct EllipsoidType {
  float x, y, z;
  float rx, ry, rz;
  float r;
  int m;
};

struct RayType {
  float   x, y, z;        // direction origin
  float   dx, dy, dz;
};

float distance (Vector v1);
float dotProduct (Vector v1, Vector v2);
float angleBetweenVectors (Vector v1, Vector v2);
Vector UnitVector (Vector v);
Vector crossProduct (Vector s_v, Vector d_v);
Vector u_vector (Vector up, Vector view);
Vector v_vector (Vector u, Vector view);

RayType rayDirection (Vector o, Vector p);
ColorType BackgroundColor(ColorType back, int x, int i);
ColorType Trace_Ray (RayType ray, SphereType *s, ColorType *m, ColorType back, int size_s);

Vector operator-(const Vector &v1, const Vector &v2);
Vector operator+(const Vector &v1, const Vector &v2);
Vector m(const Vector &v, float num);
Vector d(const Vector &v, float num);


#endif  // VECTORMATH_H

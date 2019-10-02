#ifndef VECTORMATH_HPP
#define VECTORMATH_HPP

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

// Shading
Vector intersect(RayType ray, float t);

Vector u_vector (Vector up, Vector view);
Vector v_vector (Vector u, Vector view);
Vector n_vector(Vector intersection, SphereType sphere);
Vector l_vector (Vector intersection, Light l);

ColorType PhongGenerator (MaterialColor m, Vector n, Vector l, Vector h);

bool isInShadow (Spotlight light, Vector origin, SphereType sphere);
bool isInShadow (Light light, Vector origin, SphereType sphere);
float ShadowGenerator(SphereType *sphere, SphereType s, Light l, int* size, Vector o);
float ShadowGenerator (SphereType *sphere, SphereType s, Spotlight l, int* size, Vector o);

Light SpotLightToLight (Spotlight s);

// Attenuation
bool isAttlight (Light light);
bool isAttspotlight (Spotlight spotlight);
float Attlight (Light l, float distance);
float Attspotlight (Spotlight l, float distance);

ColorType SpotLight_Sphere(MaterialColor original_color, RayType ray, float t,
  SphereType *s, SphereType sphere, Spotlight *spotlight, int *count_spotlight);
ColorType Light_Sphere(MaterialColor original_color, RayType ray, float t,
  SphereType *s, SphereType sphere, Light *light, int count_light);
ColorType Shade_Ray_Sphere (MaterialColor m, RayType ray, float t, SphereType *sphere, SphereType s,
  Light *l, Spotlight *s_l, int *size);


RayType rayDirection (Vector o, Vector p);
ColorType Trace_Ray (RayType ray, SphereType *sphere,
      MaterialColor *color, Light *light, Spotlight *spotlight, int *size);



// ColorType BackgroundColor(ColorType back, int x, int i);


#endif  // VECTORMATH_HPP

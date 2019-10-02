#include <cmath>
#include "vectorMath.hpp"
#include <iostream>
using namespace std;

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



// Calculates Phong Illumination
ColorType PhongGenerator (MaterialColor m, Vector n, Vector l, Vector h) {
  ColorType I;    // return Color
  I.r = m.d * m.object.r * dotProduct(n, l) + m.s * m.specular.r * pow(dotProduct(n, h), m.n);
  I.g = m.d * m.object.g * dotProduct(n, l) + m.s * m.specular.g * pow(dotProduct(n, h), m.n);
  I.b = m.d * m.object.b * dotProduct(n, l) + m.s * m.specular.b * pow(dotProduct(n, h), m.n);
  return I;   // Phong does not contain ambient color since it's in Light Ray.
}


bool isAttlight (Light light) {
  if (light.c.x == -1 || light.c.y == -1 || light.c.z == -1) {
    return false;
  } else {
    return true;
  }
}
bool isAttspotlight (Spotlight spotlight) {
  if (spotlight.c.x == -1 || spotlight.c.y == -1 || spotlight.c.z == -1) {
    return false;
  } else {
    return true;
  }
}

float Attlight (Light l, float d) {
  return l.c.x + (l.c.y * d) + (l.c.z * pow(d, 2));
}
float Attspotlight (Spotlight l, float d) {
  return l.c.x + (l.c.y * d) + (l.c.z * pow(d, 2));
}



// SHADOW
bool isInShadow (Spotlight light, Vector origin, SphereType sphere) {
  float  epsolin= 0.001;
  Vector direction;
  float  b, c, t1, t2;

  direction = UnitVector({light.v.x - origin.x, light.v.y - origin.y, light.v.z - origin.z});
  origin = {origin.x + direction.x * epsolin, origin.y + direction.y * epsolin, origin.z + direction.z * epsolin};

  b = direction.x * (origin.x - sphere.x) + direction.y * (origin.y - sphere.y) + direction.z * (origin.z - sphere.z);
  c = pow((origin.x - sphere.x), 2) + pow((origin.y - sphere.y), 2) + pow((origin.z - sphere.z), 2) - pow(sphere.r, 2);
  t1 = -b + sqrt(pow(b,2) - c);
  t2 = -b - sqrt(pow(b,2) - c);

  if (t1 > 0 || t2 > 0) {
    return true;
  } else {
    return false;
  }
}

bool isInShadow (Light light, Vector origin, SphereType sphere) {
  float  epsolin= 0.001;
  Vector direction;
  float  b, c, t1, t2;

  if (light.w == 1) {
    direction = UnitVector({light.v.x - origin.x, light.v.y - origin.y, light.v.z - origin.z});
  } else {
    direction = UnitVector({-light.v.x, -light.v.y, -light.v.z});
  }

  origin = {origin.x + direction.x * epsolin, origin.y + direction.y * epsolin, origin.z + direction.z * epsolin};

  b = direction.x * (origin.x - sphere.x) + direction.y * (origin.y - sphere.y) + direction.z * (origin.z - sphere.z);
  c = pow((origin.x - sphere.x), 2) + pow((origin.y - sphere.y), 2) + pow((origin.z - sphere.z), 2) - pow(sphere.r, 2);
  t1 = -b + sqrt(pow(b,2) - c);
  t2 = -b - sqrt(pow(b,2) - c);

  if (t1 > 0 || t2 > 0) {
    return true;
  } else {
    return false;
  }
}

// Returns 1 if it doesn't hit
// Returns 0 if it hits
float ShadowGenerator (SphereType *sphere, SphereType s, Spotlight l, int* size, Vector o) {
  int        count_sphere = size[1];

  float      count = 0;
  float      random_x, random_y, random_z;
  Spotlight  light = l;

  for (int i = 0; i < count_sphere; i++ ){
    if (sphere[i].x == s.x && sphere[i].y == s.y && sphere[i].z == s.z) {
      continue;
    } else {
      if (isInShadow(l, o, sphere[i])) {
        // returns 0 if it creates shadow
        for (int j = 0; j < 100; j ++) {
          random_x = rand() / (float)RAND_MAX * .002 - .001;
          random_y = rand() / (float)RAND_MAX * .002 - .001;
          random_z = rand() / (float)RAND_MAX * .002 - .001;

          // cout << random_x << endl;
          light.v = {l.v.x + random_x, l.v.y + random_y, l.v.z + random_z};
          if (isInShadow(light, o, sphere[i]))
            count ++;
          else
            continue;
        }
      }
    }
  }
  return (100 - count) / 100.0;     // returns 1 if there is no intersection with spheres
}


float ShadowGenerator (SphereType *sphere, SphereType s, Light l, int* size, Vector o) {
  int     count_sphere = size[1];
  float   count = 0;
  float   random_x, random_y, random_z;
  Light   light = l;

  for (int i = 0; i < count_sphere; i++ ){
    if (sphere[i].x == s.x && sphere[i].y == s.y && sphere[i].z == s.z) {
      continue;
    } else {
      if (isInShadow(l, o, sphere[i])) {
        // return 0;   // returns 0 if it creates shadow
        for (int j = 0; j < 100; j ++) {
          random_x = rand() / (float)RAND_MAX * .002 - .001;
          random_y = rand() / (float)RAND_MAX * .002 - .001;
          random_z = rand() / (float)RAND_MAX * .002 - .001;

          // cout << random_x << endl;
          light.v = {l.v.x + random_x, l.v.y + random_y, l.v.z + random_z};
          if (isInShadow(light, o, sphere[i]))
            count ++;
          else
            continue;
        }
      }
    }
  }   return (100 - count) / 100.0;     // returns 1 if there is no intersection with spheres
}


// LIGHTS
ColorType SpotLight_Sphere(MaterialColor original_color, RayType ray, float t,
  SphereType *s, SphereType sphere, Spotlight *spotlight, int *size) {

  int       count_spotlight = size[4];
  Vector    n = n_vector(intersect(ray, t), sphere);
  Vector    l;
  Vector    h;
  Vector    v = {ray.x, ray.y, ray.z};
  ColorType phong = {0, 0, 0};
  ColorType returnColor = {0, 0, 0};
  float     shadow;
  Light     s_light;
  MaterialColor out_color;

  Vector    dir_, obj_;

  v = UnitVector(v - intersect(ray, t));

  for (int i = 0; i < count_spotlight; i ++) {
    out_color            = original_color;
    out_color.specular.r = original_color.specular.r * spotlight[i].color.r / 255;
    out_color.specular.g = original_color.specular.g * spotlight[i].color.g / 255;
    out_color.specular.b = original_color.specular.b * spotlight[i].color.b / 255;

    s_light.v = {spotlight[i].v.x, spotlight[i].v.y, spotlight[i].v.z};
    s_light.w = 1;
    s_light.color = {spotlight[i].color.r, spotlight[i].color.g, spotlight[i].color.b};
    s_light.c = spotlight[i].c;

    dir_ = {spotlight[i].dir.x, spotlight[i].dir.y, spotlight[i].dir.z};
    dir_ = UnitVector(dir_);

    obj_ = {spotlight[i].v.x, spotlight[i].v.y, spotlight[i].v.z};
    obj_ = UnitVector(intersect(ray, t) - obj_);

    if (dotProduct(dir_, obj_) >= cos(spotlight[i].theta)) {
      // Spot Light function goes here.
      l           =   l_vector(intersect(ray, t), s_light);
      h           =   UnitVector(l + v);
      phong       =   PhongGenerator(out_color, n, l, h);
      if (isAttspotlight(spotlight[i])) {
        obj_ = {spotlight[i].v.x, spotlight[i].v.y, spotlight[i].v.z};
        phong.r = phong.r / Attspotlight(spotlight[i], distance(obj_ - intersect(ray, t)));
        phong.g = phong.g / Attspotlight(spotlight[i], distance(obj_ - intersect(ray, t)));
        phong.b = phong.b / Attspotlight(spotlight[i], distance(obj_ - intersect(ray, t)));
      }
    }

    shadow = ShadowGenerator(s, sphere, spotlight[i], size, intersect(ray, t));
    phong.r = shadow * phong.r;
    phong.g = shadow * phong.g;
    phong.b = shadow * phong.b;

    returnColor = returnColor + phong;
  }

  return returnColor;
}

ColorType Light_Sphere(MaterialColor original_color, RayType ray, float t,
  SphereType *s, SphereType sphere, Light *light, int *size) {

  int           count_light = size[3];
  Vector        n = n_vector(intersect(ray, t), sphere);
  Vector        l, ll;
  Vector        h;
  Vector        v = {ray.x, ray.y, ray.z};
  ColorType     phong       = {0, 0, 0};
  ColorType     returnColor = {0, 0, 0};
  float         shadow;
  MaterialColor out_color;

  v = UnitVector(v - intersect(ray, t));
  for (int i = 0; i < count_light; i ++) {
    out_color            = original_color;
    out_color.specular.r = original_color.specular.r * light[i].color.r / 255;
    out_color.specular.g = original_color.specular.g * light[i].color.g / 255;
    out_color.specular.b = original_color.specular.b * light[i].color.b / 255;

    ll = {light[i].v.x, light[i].v.y, light[i].v.z};
    if (light[i].w == 1) {
      l = l_vector(intersect(ray, t), light[i]);
      h = UnitVector(l + v);
      phong = PhongGenerator(out_color, n, l, h);
      // Check Attenuation
      if (isAttlight(light[i])) {
        phong.r = phong.r / Attlight(light[i], distance(ll - intersect(ray, t)));
        phong.g = phong.g / Attlight(light[i], distance(ll - intersect(ray, t)));
        phong.b = phong.b / Attlight(light[i], distance(ll - intersect(ray, t)));
      }

      // Shadow Happens HERE
      shadow = ShadowGenerator(s, sphere, light[i], size, intersect(ray, t));
      phong.r = shadow * phong.r;
      phong.g = shadow * phong.g;
      phong.b = shadow * phong.b;
      returnColor = returnColor + phong;

    } else {
      l = UnitVector({-light[i].v.x, -light[i].v.y, -light[i].v.z});
      ll = l;
      h = UnitVector(ll + v);
      phong = PhongGenerator(out_color, n, l, h);

      shadow = ShadowGenerator(s, sphere, light[i], size, intersect(ray, t));
      phong.r = shadow * phong.r;
      phong.g = shadow * phong.g;
      phong.b = shadow * phong.b;

      returnColor = returnColor + phong;
    }
  }

  return returnColor;
}




// add all the lights.
ColorType Shade_Ray_Sphere (MaterialColor m, RayType ray, float t, SphereType *sphere,
   SphereType s, Light *l, Spotlight *s_l, int *size) {
    ColorType c;    // return color that adds all the light sources.
    ColorType returnColor;

    c = Light_Sphere(m, ray, t, sphere, s, l, size);
    c = c + SpotLight_Sphere(m, ray, t, sphere, s, s_l, size);

    returnColor.r = m.a * m.object.r;
    returnColor.g = m.a * m.object.g;
    returnColor.b = m.a * m.object.b;

    return returnColor + c;
}



// RAY TRACE
ColorType Trace_Ray (RayType ray, SphereType *sphere,
      MaterialColor *color, Light *light, Spotlight *spotlight, int *size) {
  float         b, c, t;
  float         final_t = -1.0;
  ColorType     returnColor = color[0].object;
  int           size_s = size[1];

  // Sphere
  for (int i = 0; i < size_s; i ++) {
    b = ray.dx * (ray.x - sphere[i].x) + ray.dy * (ray.y - sphere[i].y) + ray.dz * (ray.z - sphere[i].z);
    c = pow((ray.x - sphere[i].x), 2) + pow((ray.y - sphere[i].y), 2) + pow((ray.z - sphere[i].z), 2) - pow(sphere[i].r, 2);

    if (pow(b,2) - c < 0) {
      continue;
    } else {
      t = -b + sqrt(pow(b,2) - c) < -b - sqrt(pow(b,2) - c) ? -b + sqrt(pow(b,2) - c) : -b - sqrt(pow(b,2) - c);
      if (t < 0) {
        continue;
      } else if (final_t == -1) {
        final_t = t;
        returnColor = Shade_Ray_Sphere(color[sphere[i].m], ray, final_t, sphere, sphere[i], light, spotlight, size);
      } else {
        if (final_t > t) {
          final_t = t;
          returnColor = Shade_Ray_Sphere(color[sphere[i].m], ray, final_t, sphere, sphere[i], light, spotlight, size);
        }
      }
    }
  }
  return clamp(returnColor);
}

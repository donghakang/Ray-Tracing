#include <cmath>
#include "vectorMath.hpp"
#include "Shading.hpp"
#include "Texture.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;

#define EPSOLIN 0.0001

MaterialColor ColorToMat (ColorType c) {
  MaterialColor m;
  m.object = c;
  m.specular = {1, 1, 1};
  m.a = 0.5;
  m.d = 0.5;
  m.s = 0.5;
  m.n = 10;

  return m;
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


bool isInShadow (Light l, Vector intersection, Function *f, SphereType *s, int *size) {
  int count_sphere = size[1];
  int count_triangle = size[6];
  float dist;
  Vector light_dir;
  Vector n, p;

  float b, c, t, t1, t2;
  float alpha, beta, gamma;

  // check for sphere shadow
  // directional light
  if (l.w == 1) {
    dist = distance(l.v - intersection);   // distance between light and intersection
    light_dir = UnitVector(l.v - intersection);
  } else {
    light_dir = UnitVector(m(l.v, -1));
  }

  intersection.x = intersection.x + light_dir.x * EPSOLIN;
  intersection.y = intersection.y + light_dir.y * EPSOLIN;
  intersection.z = intersection.z + light_dir.z * EPSOLIN;


  for (int j = 0; j < count_sphere; j ++) {
    b = light_dir.x * (intersection.x - s[j].x) + light_dir.y * (intersection.y - s[j].y)
        + light_dir.z * (intersection.z - s[j].z);
    c = pow((intersection.x - s[j].x), 2) + pow((intersection.y - s[j].y), 2)
        + pow((intersection.z - s[j].z), 2) - pow(s[j].r, 2);
    t1 = -b + sqrt(pow(b,2) - c);
    t2 = -b - sqrt(pow(b,2) - c);

    if (t1 > 0 || t2 > 0) {
      return true;
    } else {
      continue;
    }
  }
  // check for triangle shadow
  for (int k = 0; k < count_triangle; k++) {
    n = n_vector(f[k]);

    if (fabs(dotProduct(n, light_dir) - 0) < EPSOLIN) continue;
    else {
      t = -(dotProduct(n, intersection) + calculateD(n, f[k].v1)) / (dotProduct(n, light_dir));
      if (t < 0) continue;
      else {
        p = {intersection.x + t * light_dir.x,
             intersection.y + t * light_dir.y,
             intersection.z + t * light_dir.z};
        alpha = Alpha(f[k], p, calculateArea(f[k]));
        beta  = Beta(f[k], p, calculateArea(f[k]));
        gamma = Gamma(f[k], p, calculateArea(f[k]));

        if (checkInPlane(alpha, beta, gamma)) {
          return true;
        } else continue;
      }
    }
  }
  return false;
}

// SPOTLIGHT for sphere only
float ShadowGenerator (SphereType *sphere, SphereType s, Spotlight l, int* size, Vector o) {
  int        count_sphere = size[1];
  Spotlight  light = l;

  // SOFT SHADDOW
  float      count = 0;
  float      random_x, random_y, random_z;

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

// Sphere
float ShadowGenerator (SphereType *sphere, Function *function, SphereType s, Light l, int* size, Vector o) {
  int     count_sphere = size[1];

  // SOFT SHADOW
  // float   random_x, random_y, random_z;
  // float   count = 0;
  // Light   light = l;

  for (int i = 0; i < count_sphere; i++ ){
    if (sphere[i].x == s.x && sphere[i].y == s.y && sphere[i].z == s.z) {
      continue;
    } else {
      if (isInShadow(l, o, function, sphere, size)) {
        return 0;   // returns 0 if it creates shadow

        // SOFT SHADOW
        // for (int j = 0; j < 100; j ++) {
        //   random_x = rand() / (float)RAND_MAX * .002 - .001;
        //   random_y = rand() / (float)RAND_MAX * .002 - .001;
        //   random_z = rand() / (float)RAND_MAX * .002 - .001;
        //
        //   light.v = {l.v.x + random_x, l.v.y + random_y, l.v.z + random_z};
        //   if (isInShadow(l, o, function, sphere, size))
        //     count ++;
        //   else
        //     continue;
        // }
      }
    }
  }
  return 1;
  // return (100 - count) / 100.0;     // returns 1 if there is no intersection with spheres
}

// TRIANGLE
float ShadowGenerator(SphereType *sphere, Function *function, Function f, Light l, int* size, Vector o) {
  int     count_triangle = size[6];

  // SOFT SHADOW
  // float   count = 0;
  // float   random_x, random_y, random_z;
  // Light   light = l;

  for (int i = 0; i < count_triangle; i++) {
    if (function[i].v1.x == f.v1.x && function[i].v1.y == f.v1.y && function[i].v1.z == f.v1.z &&
        function[i].v2.x == f.v2.x && function[i].v2.y == f.v2.y && function[i].v2.z == f.v2.z &&
        function[i].v3.x == f.v3.x && function[i].v3.y == f.v3.y && function[i].v3.z == f.v3.z) {
      continue;
    } else {
      if (isInShadow(l, o, function, sphere, size)) {
        return 0;   // returns 0 if it creates shadow

        // SOFT SHADOW
        // for (int j = 0; j < 100; j ++) {
        //   random_x = rand() / (float)RAND_MAX * .002 - .001;
        //   random_y = rand() / (float)RAND_MAX * .002 - .001;
        //   random_z = rand() / (float)RAND_MAX * .002 - .001;
        //
        //   light.v = {l.v.x + random_x, l.v.y + random_y, l.v.z + random_z};
        //   if (isInShadow(l, o, function, sphere, size))
        //     count ++;
        //   else
        //     continue;
        // }
      }
    }
  }
  return 1;
  // return (100 - count) / 100.0;
}



// SPOTLIGHTS
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

// LIGHT for SPHERE
ColorType Light_Sphere(MaterialColor original_color, RayType ray, float t,
  SphereType *s, Function* f, SphereType sphere, Light *light, int *size) {

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
      shadow = ShadowGenerator(s, f, sphere, light[i], size, intersect(ray, t));
      phong.r = shadow * phong.r;
      phong.g = shadow * phong.g;
      phong.b = shadow * phong.b;
      returnColor = returnColor + phong;

    } else {
      l = UnitVector({-light[i].v.x, -light[i].v.y, -light[i].v.z});
      ll = l;
      h = UnitVector(ll + v);
      phong = PhongGenerator(out_color, n, l, h);

      shadow = ShadowGenerator(s, f, sphere, light[i], size, intersect(ray, t));
      phong.r = shadow * phong.r;
      phong.g = shadow * phong.g;
      phong.b = shadow * phong.b;

      returnColor = returnColor + phong;
    }
  }

  return returnColor;
}


// LIGHT for TRIANGLE
ColorType Light_Triangle(MaterialColor original_color, RayType ray, float t,
  SphereType *s, Function *f, Function function, Light *light, int *size) {

  int           count_light = size[3];
  Vector        n;
  Vector        l, ll;
  Vector        h;
  Vector        v = {ray.x, ray.y, ray.z};
  ColorType     phong       = {0, 0, 0};
  ColorType     returnColor = {0, 0, 0};
  float         shadow;
  MaterialColor out_color;
  float         alpha, beta, gamma;

  Vector        p = {ray.x + t * ray.dx, ray.y + t * ray.dy, ray.z + t * ray.dz};

  // if one of the function misses normal vector, it automatically finds another normal vector
  if ((function.vn1.x == 0 && function.vn1.y == 0 && function.vn1.z == 0) ||
      (function.vn2.x == 0 && function.vn2.y == 0 && function.vn2.z == 0) ||
      (function.vn3.x == 0 && function.vn3.y == 0 && function.vn3.z == 0)) {
    n = n_vector(function);
  }
  // else it is smooth shaiding.
  else {
    alpha = Alpha(function, p, calculateArea(function));
    beta  = Beta(function, p, calculateArea(function));
    gamma = Gamma(function, p, calculateArea(function));

    n = n_vector(function, alpha, beta, gamma);
    // n = n_vector(function);
  }

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
      shadow = ShadowGenerator(s, f, function, light[i], size, intersect(ray, t));
      phong.r = shadow * phong.r;
      phong.g = shadow * phong.g;
      phong.b = shadow * phong.b;

      returnColor = returnColor + phong;

    } else {
      l = UnitVector({-light[i].v.x, -light[i].v.y, -light[i].v.z});
      ll = l;
      h = UnitVector(ll + v);
      phong = PhongGenerator(out_color, n, l, h);

      shadow = ShadowGenerator(s, f, function, light[i], size, intersect(ray, t));
      phong.r = shadow * phong.r;
      phong.g = shadow * phong.g;
      phong.b = shadow * phong.b;

      returnColor = returnColor + phong;
    }
  }

  return returnColor;
}




// CONNECTION TO THE LIGHT
// SPHERE
ColorType Shade_Ray_Sphere (MaterialColor m, RayType ray, float t, SphereType *sphere,
   SphereType s, Function* function, Light *l, Spotlight *s_l, int *size) {
    ColorType c;    // return color that adds all the light sources.
    ColorType returnColor;

    c = Light_Sphere(m, ray, t, sphere, function, s, l, size);
    // c = c + SpotLight_Sphere(m, ray, t, sphere, s, s_l, size);

    returnColor.r = m.a * m.object.r;
    returnColor.g = m.a * m.object.g;
    returnColor.b = m.a * m.object.b;

    return returnColor + c;
}
// TRIANGLE
ColorType Shade_Ray_Triangle (MaterialColor m, RayType ray, float t, SphereType *sphere,
  Function* function, Function f, Light *l, int *size) {
    ColorType c;
    ColorType returnColor;

    c = Light_Triangle(m, ray, t, sphere, function, f, l, size);

    returnColor.r = m.a * m.object.r;
    returnColor.g = m.a * m.object.g;
    returnColor.b = m.a * m.object.b;

    return returnColor + c;
}




// RAY TRACE
ColorType Trace_Ray (RayType ray, SphereType *sphere, Function* function,
      TexturePicture *texture, MaterialColor *color, Light *light, Spotlight *spotlight, int *size) {
  float         b, c, t;
  float         final_t = -1.0;
  ColorType     returnColor = color[0].object;
  int           size_s = size[1];

  // Triangle variables
  int           size_f = size[6]; // function entry
  float         d;
  Vector        n;
  Vector        origin = {ray.x, ray.y, ray.z};
  Vector        direction = {ray.dx, ray.dy, ray.dz};
  Vector        p;
  float         alpha, beta, gamma;

  // for texture
  int           w, h;
  ColorType     textureColor;
  MaterialColor pictureColor;

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
        p = {ray.x + final_t * ray.dx, ray.y + final_t * ray.dy, ray.z + final_t * ray.dz};

        if (color[sphere[i].m].texture == -1) {
          returnColor = Shade_Ray_Sphere(color[sphere[i].m], ray, final_t, sphere, sphere[i], function, light, spotlight, size);
        } else {
          w = texture[color[sphere[i].m].texture].w;
          h = texture[color[sphere[i].m].texture].h;

          textureColor = TexturePosition(texture[color[sphere[i].m].texture].picture, w, h, sphere[i], p);
          returnColor = textureColor;
          pictureColor = ColorToMat(textureColor);
          pictureColor.texture = color[sphere[i].m].texture;
          returnColor = Shade_Ray_Sphere(pictureColor, ray, final_t, sphere, sphere[i], function, light, spotlight, size);
        }
      } else {
        if (final_t > t) {
          final_t = t;
          p = {ray.x + final_t * ray.dx, ray.y + final_t * ray.dy, ray.z + final_t * ray.dz};
          // returnColor = Shade_Ray_Sphere(color[sphere[i].m], ray, final_t, sphere, sphere[i], light, spotlight, size);
          if (color[sphere[i].m].texture == -1) {
            returnColor = Shade_Ray_Sphere(color[sphere[i].m], ray, final_t, sphere, sphere[i], function, light, spotlight, size);
          } else {
            w = texture[color[sphere[i].m].texture].w;
            h = texture[color[sphere[i].m].texture].h;

            textureColor = TexturePosition(texture[color[sphere[i].m].texture].picture, w, h, sphere[i], p);
            returnColor = textureColor;
            pictureColor = ColorToMat(textureColor);
            pictureColor.texture = color[sphere[i].m].texture;
            returnColor = Shade_Ray_Sphere(pictureColor, ray, final_t, sphere, sphere[i], function, light, spotlight, size);
          }
        }
      }
    }
  }

  // Triangles
  for (int j = 0; j < size_f; j ++) {
    n = n_vector(function[j]);
    d = calculateD(n, function[j].v1);

    if (fabs(dotProduct(n, direction) - 0) < EPSOLIN) continue;
    else {
      // intersection between ray and plane
      t = -(dotProduct(n, origin) + d) / (dotProduct(n, direction));

      if (t < 0)  continue;
      else {
        p = {ray.x + t * ray.dx, ray.y + t * ray.dy, ray.z + t * ray.dz};

        // alpha, beta, gamma
        alpha = Alpha(function[j], p, calculateArea(function[j]));
        beta  = Beta(function[j], p, calculateArea(function[j]));
        gamma = Gamma(function[j], p, calculateArea(function[j]));

        // check if the point is in the plane.
        if (checkInPlane(alpha, beta, gamma)) {
          if (final_t == -1) {
            final_t = t;
            if (color[function[j].m].texture == -1) {
              returnColor = Shade_Ray_Triangle(color[function[j].m], ray, final_t, sphere, function, function[j], light, size);
            }
            else {
              w = texture[color[function[j].m].texture].w;
              h = texture[color[function[j].m].texture].h;
              textureColor = TexturePosition(texture[color[function[j].m].texture].picture, w, h,
                                function[j], alpha, beta, gamma);
              returnColor = textureColor;
              pictureColor = ColorToMat(textureColor);
              pictureColor.texture = color[function[j].m].texture;
              returnColor = Shade_Ray_Triangle(pictureColor, ray, final_t, sphere, function, function[j], light, size);
            }
          } else {
            // continue if there is no point in the plane.
            if (final_t > t) {
              final_t = t;
              if (color[function[j].m].texture == -1) {
                returnColor = Shade_Ray_Triangle(color[function[j].m], ray, final_t, sphere, function, function[j], light, size);
              }
              else {
                w = texture[color[function[j].m].texture].w;
                h = texture[color[function[j].m].texture].h;
                textureColor = TexturePosition(texture[color[function[j].m].texture].picture, w, h,
                                  function[j], alpha, beta, gamma);
                returnColor = textureColor;
                pictureColor = ColorToMat(textureColor);
                pictureColor.texture = color[function[j].m].texture;
                returnColor = Shade_Ray_Triangle(pictureColor, ray, final_t, sphere, function, function[j], light, size);
              }
            }
          }
        }
      }
    }
  }
  return clamp(returnColor);
}

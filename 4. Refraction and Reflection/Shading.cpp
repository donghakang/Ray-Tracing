#include <cmath>
#include "VectorMath.hpp"
#include "Shading.hpp"
#include "SpecialTexture.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;

#define EPSOLIN 0.0001

MaterialColor ColorToMat (ColorType c) {
  MaterialColor m;
  m.object = c;
  m.specular = {1, 1, 1};
  m.a = 0.2;
  m.d = 1;
  m.s = 0.1;
  m.n = 10;
  m.opacity = 1.0;
  m.index_of_refraction = 1.0;

  return m;
}


// Calculates Phong Illumination
ColorType PhongGenerator (MaterialColor m, Vector n, Vector l, Vector h) {
  ColorType i;    // return Color
  i.r = m.d * m.object.r * dotProduct(n, l) + m.s * m.specular.r * pow(dotProduct(n, h), m.n);
  i.g = m.d * m.object.g * dotProduct(n, l) + m.s * m.specular.g * pow(dotProduct(n, h), m.n);
  i.b = m.d * m.object.b * dotProduct(n, l) + m.s * m.specular.b * pow(dotProduct(n, h), m.n);
  return i;   // Phong does not contain ambient color since it's in Light Ray.
}

bool  IsShadow (Object *object, Object current, Light light, int count_object) {
  float b, c, t, t1, t2;
  float alpha, beta, gamma;

  Vector n, p;
  Vector intersection = current.intersection;
  Vector light_dir    = lVector(intersection, light);

  SphereType sphere;
  Function   triangle;

  // Avoid Shadow acne.
  intersection.x = intersection.x + (light_dir.x * EPSOLIN);
  intersection.y = intersection.y + (light_dir.y * EPSOLIN);
  intersection.z = intersection.z + (light_dir.z * EPSOLIN);

  for (int i = 0; i < count_object; i ++) {
    // sphere
    if (object[i].objectType == 0) {
      sphere   =   object[i].sphere;
      b = light_dir.x * (intersection.x - sphere.x) +
          light_dir.y * (intersection.y - sphere.y) +
          light_dir.z * (intersection.z - sphere.z);
      c = pow((intersection.x - sphere.x), 2) +
          pow((intersection.y - sphere.y), 2) +
          pow((intersection.z - sphere.z), 2) - pow(sphere.r, 2);
      t1 = -b + sqrt(pow(b,2) - c);
      t2 = -b - sqrt(pow(b,2) - c);
      if (t1 > 0 || t2 > 0) return true;
      else                  continue;
    }
    // triangle
    else {
      triangle   =   object[i].triangle;
      n = nVector(triangle);
      if (fabs(dotProduct(n, light_dir) - 0) < EPSOLIN) continue;
      else {
        t = -(dotProduct(n, intersection) + calculateD(n, triangle.v1)) / (dotProduct(n, light_dir));
        if (t < 0) continue;
        else {
          p = {intersection.x + t * light_dir.x,
               intersection.y + t * light_dir.y,
               intersection.z + t * light_dir.z};
          alpha = Alpha(triangle, p, calculateArea(triangle));
          beta  = Beta(triangle, p, calculateArea(triangle));
          gamma = Gamma(triangle, p, calculateArea(triangle));

          if (checkInPlane(alpha, beta, gamma)) return true;
          else continue;
        }
      }
    }
  }
  return false;
}

// Check whether or not if it is in shadow and returns a number
float ShadowGenerator(Object *object, Object current, Light light, int count_object) {
  if (IsShadow(object, current, light, count_object)) return 0.0;
  else return 1.0;
}


ColorType Lighting (RayType ray, Object *object, Light *light, int *scene, int pos) {
  int           count_light = scene[1];
  Vector        eye      =  {ray.x, ray.y, ray.z};
  Vector        n;
  Vector        l;
  Vector        v;
  Vector        h;

  Object        current  =  object[pos];

  ColorType     phong    =  {0, 0, 0};
  float         shadow;

  MaterialColor out_color;
  MaterialColor original_color = current.material;
  ColorType     returnColor    = {0, 0, 0};

  Function      function;
  float         alpha, beta, gamma;
  Vector        p;

  for (int i = 0; i < count_light; i++) {
    out_color            = original_color;
    out_color.specular.r = original_color.specular.r * light[i].color.r;
    out_color.specular.g = original_color.specular.g * light[i].color.g;
    out_color.specular.b = original_color.specular.b * light[i].color.b;

    // sphere
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
    l    =    lVector(current.intersection, light[i]);
    v    =    unitVector(eye - current.intersection);
    h    =    unitVector(l + v);

    // Phong Illumination
    phong  = PhongGenerator(out_color, n, l, h);
    // Shadow Detecter
    shadow      = ShadowGenerator(object, current, light[i], scene[0]);
    phong       = shadow * phong;
    returnColor = returnColor + phong;
  }

  returnColor = returnColor + (original_color.a * original_color.object);
  return returnColor;
}


ColorType Shade_Ray (RayType ray, Object* object, Light *light,
  TexturePicture *texture, int *scene, ColorType background, int pos) {
    ColorType c;
    // get texture color
    int           w, h;
    ColorType     textureColor;

    if (pos == -1) {
      return background;
    }

    if (object[pos].texture_index > -1) {
      int texture_pos = object[pos].texture_index;
      w = texture[texture_pos].w;
      h = texture[texture_pos].h;

      textureColor = TexturePosition(texture[texture_pos].picture, w, h, object[pos]);
      // returnColor = textureColor
      object[pos].material = ColorToMat(textureColor);
    }

    c = Lighting(ray, object, light, scene, pos);
    // c = object[pos].material.object;
    // c + Fresnel Reflectance Coefficient + Illumination by reflection ray R


    return c;
}


ColorType Reflection (RayType ray, Object *object, Light *light,
  TexturePicture *texture, int *scene, ColorType background, int pos, int depth) {

  Object current   =  object[pos];
  Vector n_vector  =  nVector(current);
  Vector i_vector  =  iVector(ray);
  Vector r_vector  =  Specular_Reflection(n_vector, i_vector);
  RayType new_ray  =  NewRayReflection(n_vector, current.intersection, r_vector);
  float  fresnel   =  Fresnel_Reflectance(current.material.index_of_refraction, n_vector, i_vector);
  ColorType returnColor =  Trace_Ray(new_ray, object, light, texture, scene, background, depth - 1);
  returnColor.clamp();
  return fresnel * returnColor;
}



ColorType Refraction (RayType ray, Object *object, Light *light,
  TexturePicture *texture, int *scene, ColorType background, int pos, int depth) {

  Object current   =  object[pos];
  float  ni        =  1.0;    // for air.
  float  nt        =  current.material.index_of_refraction;

  float  opacity   =  current.material.opacity;
  Vector n_vector  =  nVector(current);
  Vector i_vector  =  iVector(ray);
  if (dotProduct(n_vector, -1 * i_vector) > 0) {

    swap(ni, nt);
    if (depth == 4) {
      cout << depth << ":" << ni << ", " << nt << endl;
    }
    n_vector = -1 * n_vector;
  }
  Vector t_vector  =  Transparent_Vector(ni, nt, n_vector, i_vector);
  float  fresnel   =  Fresnel_Reflectance(nt, n_vector, i_vector);
  RayType new_ray  =  NewRayRefraction(n_vector, current.intersection, t_vector);

  ColorType returnColor = Trace_Ray(new_ray, object, light, texture, scene, background, depth - 1);
  returnColor.clamp();
  return ((1-fresnel) * (1-opacity)) * returnColor;
}


// RAY TRACE
ColorType Trace_Ray (RayType ray, Object *object, Light *light,
  TexturePicture *texture, int *scene, ColorType background, int depth) {
  float         b, c;
  float         t1, t2;
  float         t = 0.0;
  float         final_t = -1.0;

  int           num_object = scene[0];    // number of object

  float         d;
  Vector        n;
  Vector        origin = {ray.x, ray.y, ray.z};
  Vector        direction = {ray.dx, ray.dy, ray.dz};
  Vector        p;
  float         alpha, beta, gamma;

  // for texture

  ColorType     returnColor;
  SphereType    sphere;
  Function      triangle;
  int           pos = -1;

  for (int i = 0; i < num_object; i ++) {
    // if sphere
    if (object[i].objectType == 0) {
      sphere = object[i].sphere;
      b = ray.dx * (ray.x - sphere.x) + ray.dy * (ray.y - sphere.y) + ray.dz * (ray.z - sphere.z);
      c = pow((ray.x - sphere.x), 2) + pow((ray.y - sphere.y), 2) + pow((ray.z - sphere.z), 2) - pow(sphere.r, 2);
      if (pow(b,2) - c < 0)  continue;
      else {
        t1 = -b + sqrt(pow(b,2) - c);
        t2 = -b - sqrt(pow(b,2) - c);
        if (t1 < 0 && t2 < 0)  continue;
        else {
          if      (t1 < 0 || t2 > 0) t = t2;
          else if (t1 > 0 || t2 < 0) t = t1;
          else    t = t1 < t2 ? t1 : t2;
          object[i].t = t;
          object[i].intersection = intersect(ray, t);
          if (final_t == -1) {
            final_t = t;
            pos = i;              // compare t.
          } else {
            if (final_t > t) {
              final_t = t;
              pos = i;
            }
          }
        }
      }
    }
    // if triangle
    else {      // object[i].objectType == 1
      triangle = object[i].triangle;

      n = nVector(triangle);
      d = calculateD(n, triangle.v1);

      if (fabs(dotProduct(n, direction) - 0) < EPSOLIN) continue;
      else {
        t = -(dotProduct(n, origin) + d) / (dotProduct(n, direction));
        if (t < 0) continue;
        else {
          p = intersect(ray, t);
          alpha = Alpha(triangle, p, calculateArea(triangle));
          beta  = Beta(triangle, p, calculateArea(triangle));
          gamma = Gamma(triangle, p, calculateArea(triangle));

          if (checkInPlane(alpha, beta, gamma)) {
            object[i].t = t;
            object[i].intersection = p;
            if (final_t == -1) {
              final_t = t;
              pos = i;
            } else {
              if (final_t > t) {
                final_t = t;
                pos = i;
              }
            }
          }
        }
      }
    }
  }

  returnColor = Shade_Ray(ray, object, light, texture, scene, background, pos);

  Object current = object[pos];
  Vector raydir  = unitVector({ray.dx, ray.dy, ray.dz});
  // cout << depth << endl;
  if (pos == -1) {
    return background;
  } else if (depth == 0) {
    return returnColor;
  } else {
    ColorType refraction = {0,0,0};
    ColorType reflection = {0,0,0};
    if (dotProduct(nVector(current), raydir) <= 0) {
      reflection = (current.material.s * current.material.specular) *
                   Reflection(ray, object, light, texture, scene, background, pos, depth);
      returnColor = returnColor + reflection;
    }
    if (current.material.opacity < 1.0) {
      refraction = Refraction(ray, object, light, texture, scene, background, pos, depth);
      returnColor = returnColor + refraction;
    }



    return returnColor;
  }
}

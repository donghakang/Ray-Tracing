#include <cmath>
#include "vectorMath.h"
#include <iostream>
using namespace std;
// ||v1||
float distance (Vector v1) {
  return sqrt(pow(v1.x, 2) + pow(v1.y, 2) + pow(v1.z, 2));
}

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


ColorType BackgroundColor(ColorType back, int x, int i) {
  int  a = ceil(x / 255.0);
  // if (i % a == 0) {
  //   back.r ++;
  //   if (back.r == 255) {
  //     back.r = 0;
  //   }
  // }
  // cout << a << endl;
  bool up_r = true;
  bool up_b = true;
  bool up_g = true;
  if (i % a == 0) {
    if (back.r == 255) {
      up_r = false;
    } else if (back.r == 0) {
      up_r = true;
    }
    if (back.g == 255) {
      up_g = false;
    } else if (back.g == 0) {
      up_g = true;
    }
    if (back.b == 255) {
      up_b = false;
    } else if (back.b == 0) {
      up_b = true;
    }

    if (up_r) {
      back.r += 2;
    } else {
      back.r -= 5;
    }
    if (up_g) {
      back.g += 7;
    } else {
      back.g -= 4;
    }
    if (up_b) {
      back.b += 8;
    } else {
      back.b -= 3;
    }
  }
  return back;
}

ColorType Trace_Ray (RayType ray, SphereType *sphere, ColorType *color, ColorType back, int size_s) {
  float         b, c, t;
  float         final_t = -1.0;
  ColorType     returnColor = back;

  for (int i = 0; i < size_s; i ++) {
    b = ray.dx * (ray.x - sphere[i].x) + ray.dy * (ray.y - sphere[i].y) + ray.dz * (ray.z - sphere[i].z);
    c = pow((ray.x - sphere[i].x), 2) + pow((ray.y - sphere[i].y), 2) + pow((ray.z - sphere[i].z), 2) - pow(sphere[i].r, 2);

    if (pow(b,2) - c < 0) {
      // retunColor = BackgroundColor();
      continue;
    } else {
      t = -b + sqrt(pow(b,2) - c) < -b - sqrt(pow(b,2) - c) ? -b + sqrt(pow(b,2) - c) : -b - sqrt(pow(b,2) - c);
      if (final_t == -1) {
        final_t = t;
        returnColor = color[sphere[i].m];
      } else {
        if (final_t > t) {
          final_t = t;
          returnColor = color[sphere[i].m];
        }
      }
    }
  }
  return returnColor;
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

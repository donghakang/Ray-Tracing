#include <string>

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define EPSOLIN 0.0001

ColorType** readTexture (std::string filename);
ColorType TexturePosition(ColorType** texture, int w, int h, Object obj);
// ******************************************************
// *************  A S S I G N M E N T  1 D **************
// ******************************************************

float Fresnel_Reflectance(float n, Vector n_vector, Vector i_vector);
Vector Specular_Reflection(Vector n_vector, Vector i_vector);

Vector Transparent_Vector(float ni, float nt, Vector n_vector, Vector i_vector);

RayType NewRayReflection(Vector n, Vector intersection, Vector reflection);
RayType NewRayRefraction(Vector n, Vector intersection, Vector refraction);
#endif  // TEXTURE_HPP

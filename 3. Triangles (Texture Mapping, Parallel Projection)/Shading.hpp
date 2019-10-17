#ifndef SHADING_HPP
#define SHADING_HPP

#define EPSOLIN 0.0001

void pv (Vector v);

MaterialColor ColorToMat (ColorType c);


ColorType PhongGenerator (MaterialColor m, Vector n, Vector l, Vector h);

// Attenuation
bool isAttlight (Light light);
bool isAttspotlight (Spotlight spotlight);
float Attlight (Light l, float distance);
float Attspotlight (Spotlight l, float distance);
//

// bool isInShadow (Spotlight light, Vector origin, SphereType sphere);
// bool isInShadow (Light light, Vector origin, SphereType sphere);
// bool isInShadow (Light light, Vector origin, Function function);
bool isInShadow (Light l, Vector intersection, Function *f, SphereType *s, int *size);

float ShadowGenerator(SphereType *sphere, SphereType s, Light l, int* size, Vector o);
float ShadowGenerator(SphereType *sphere, Function *function, SphereType s, Spotlight l, int* size, Vector o);
float ShadowGenerator(SphereType *sphere, Function *function, Function f, Light l, int* size, Vector o);



ColorType SpotLight_Sphere(MaterialColor original_color, RayType ray, float t,
  SphereType *s, SphereType sphere, Spotlight *spotlight, int *count_spotlight);

ColorType Light_Sphere(MaterialColor original_color, RayType ray, float t,
  SphereType *s, Function *f, SphereType sphere, Light *light, int count_light);
ColorType Light_Triangle(MaterialColor original_color, RayType ray, float t,
  SphereType *s, Function *f, Function function, Light *light, int *size);

ColorType Shade_Ray_Sphere (MaterialColor m, RayType ray, float t, SphereType *sphere,
  SphereType s, Function* function, Light *l, Spotlight *s_l, int *size);
ColorType Shade_Ray_Triangle (MaterialColor m, RayType ray, float t, SphereType *sphere,
  Function* function, MaterialColor *color, Light *light, int *size);


ColorType Trace_Ray (RayType ray, SphereType *sphere, Function *function,
      TexturePicture *texutre, MaterialColor *color, Light *light, Spotlight *spotlight, int *size);


#endif  // SHADING_HPP

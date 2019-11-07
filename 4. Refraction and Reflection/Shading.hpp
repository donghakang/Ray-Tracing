#ifndef SHADING_HPP
#define SHADING_HPP

#define EPSOLIN 0.0001

MaterialColor ColorToMat (ColorType c);

ColorType PhongGenerator (MaterialColor m, Vector n, Vector l, Vector h);

bool  IsShadow (Object *object, Object current, Light light, int count_object);
float ShadowGenerator(Object *object, Object current, Light light, int count_object);
ColorType Lighting (RayType ray, Object *object, Light *light, int *scene, int pos);
ColorType Shade_Ray (RayType ray, Object* object, Light *light,
          TexturePicture *texture, int *scene, ColorType background, int pos);
ColorType Trace_Ray (RayType ray, Object *objects, Light *light,
          TexturePicture *texture, int *scene, ColorType background, int depth);
ColorType Reflection (RayType ray, Object *object, Light *light,
  TexturePicture *texture, int *scene, ColorType background, int pos, int depth);
ColorType Refraction (RayType ray, Object *object, Light *light,
  TexturePicture *texture, int *scene, ColorType background, int pos, int depth);
#endif  // SHADING_HPP

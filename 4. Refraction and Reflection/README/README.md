# Assignment 1d

Dongha Kang, kangx637
November 6th, 2019


## Installation

To compile the program. Simple use make file in terminal
```bash
make all
```
or
```bash
make
```
Once it compiles, compiler generates the object file called **kangx637**. To run the program, type ./kangx637 in terminal.
Also, one input file is required.
```bash
./kangx637 <input.txt>
```


## Write-up

For this part of assignment, I generated Refraction, Reflection of the objects and
Depth of Field Effects.


### Reflection
By using recursive function for reflection. It produced a picture below.
Instead of adding the color directly, I multiplied the color with specular variables of the material.

<img src="reflection.png" width="50%" height="50%">

### Refraction
Problems that I had
- if I use only refraction (no reflection). It creates the image that refracts. However, index of refraction value seems to be different
<img src="refraction.png" width="50%" height="50%">
- if I change the index of refraction value. The image seems to be correct except the image has NaN values.
<img src="refraction error.png" width="50%" height="50%">

***code***
```c++
ColorType Refraction (RayType ray, Object *object, Light *light,
  TexturePicture *texture, int *scene, ColorType background, int pos, int depth) {

  Object current   =  object[pos];
  float  ni        =  1.0;    // for air.
  float  nt        =  current.material.index_of_refraction;

  float  opacity   =  current.material.opacity;
  Vector n_vector  =  nVector(current);
  Vector i_vector  =  iVector(ray);
  // when the ray is inside the object
  if (dotProduct(n_vector, -1 * i_vector) > 0) {
    swap(ni, nt);
    n_vector = -1 * n_vector;
  }
  Vector t_vector  =  Transparent_Vector(ni, nt, n_vector, i_vector);
  float  fresnel   =  Fresnel_Reflectance(nt, n_vector, i_vector);
  RayType new_ray  =  NewRayRefraction(n_vector, current.intersection, t_vector);

  ColorType returnColor = Trace_Ray(new_ray, object, light, texture, scene, background, depth - 1);
  returnColor.clamp();
  return ((1-fresnel) * (1-opacity)) * returnColor;
}
```

```c++
// Transparent Vector (T Vector)
Vector Transparent_Vector(float ni, float nt, Vector n_vector, Vector i_vector) {
  float refract      = ni/nt;
  float cosine_theta = dotProduct(n_vector, i_vector);

  Vector temp1 = sqrt(1.0 - (pow(refract, 2) * (1 - pow(cosine_theta, 2)))) * (-1 * n_vector);
  Vector temp2 = refract * ((cosine_theta * n_vector) - i_vector);

  return unitVector(temp1 + temp2);
}
```

### Depth of Field
By having viewdist value in txt file, it enables the depth of field image. What I did was that I set the original intersection point with original eye value with distance. Then, I changed eye value randomly 15 times pointing at the intersection point. Then I averaged the color.

<img src="no DOF.png" width="50%" height="50%">
<img src="DOF.png" width="50%" height="50%">

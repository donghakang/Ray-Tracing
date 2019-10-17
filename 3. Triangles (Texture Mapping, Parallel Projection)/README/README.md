# Assignment 1c

Dongha Kang, kangx637
October 16th, 2019


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

For this part of assignment, I generated triangle object. By using triangle objects
I was able to make different type of objects such as cylinder, cube.
This is a simple example that was made with triangle. (cube)

<img src="cube.png" width="50%" height="50%">



### Shading / Soft Shading
For Triangle objects, it was possible for user to provide the the objects' own vertex normal.
If the users do not give the vertex normal, the program will just calculate the normal of the plane with given three vertices.
On the other side, if the users provide the vertex normal, it is possible to prevent from more angled shading.

Below is the picture of using NO soft shading (no vertex normal given), and soft shading (vertex normal given).
<img src="hard_cylinder.png" width="40%" height="40%">
<img src="soft_cylinder.png" width="40%" height="40%">

If there are no vertex normal given. It will generate more angled obejct since everything is created with triangle.
Very small triangles might prevent the look of this angled object, but it will also requires numerous calculations.


### Texture Mapping
Assumed that input texture has a texture coordinates like the below, where upper left is (0,0), and lower right is (1, 1);
<img src="texture_coordinates.png" width="50%" height="50%">

I first stored the pictures' color coordinates in the array and mapped it when they had vt value in *f*.
For example,
```
v 1 1 0
v 1 -1 0
v -1 -1 0
v -1 1 0

vt 0 0
vt 0 1
vt 1 1
vt 1 0

f 1/1 2/2 3/3
f 1/1 3/3 4/4
```
becomes the picture below.

<img src="texture.png" width="50%" height="50%">

As the object got bigger or the window size got bigger, the texture became more clear.
<img src="cube_texture.png" width="40%" height="40%"> <img src="sphere_texture.png" width="40%" height="40%">


### Parallel Projection
For Parallel Projection instead of having distance between eye and viewing window to be *1.0*, I made the
distance between eye and viewing window to be ***0.0***.
Before, my ray started at eye position with the direction being the unit vector of *viewing window - eye*.
For Parallel projection, ray starts from viewing window with direction being the unit vector of the viewing direciton.
The picture will be affected when viewing window size is changed since all the ray starts at the viewing window.
Therefore, changing **vfov** value will affect the view of the parallel projection.
- original picture  
<img src="og_parallel.png" width="60%" height="60%">

- vfov = 90 degree
<img src="parallel90.png" width="60%" height="60%">

- vfov = 120 degree
<img src="parallel120.png" width="60%" height="60%">

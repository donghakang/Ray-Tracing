
#include <iostream>
#include <fstream>
#include <cmath>
#include "VectorMath.hpp"
#include "File.hpp"
#include "Shading.hpp"
#include "SpecialTexture.hpp"

using namespace std;

int main (int argc, char *argv[]) {

  if (argc <= 1) {
    cout << "ERROR: Text file not available. " << endl;
    exit(1);
  }


  //============================================================
  //             COUNT DATA FOR  ERROR CHECKING
  //============================================================
  string   filename = argv[1];
  ifstream countfile;

  // only for counting.
  int           count_eye = 0;
  int           count_view = 0;
  int           count_vfov = 0;
  int           count_up = 0;
  int           count_imsize = 0;
  // to create arrays
  int           count_light  = 1;
  int           count_object = 1;
  int           count_color = 1;
  int           count_v = 1;
  int           count_vn = 1;
  int           count_vt = 1;
  int           count_texture = 1;
  string        word;

  printTextFileError(filename);

  countfile.open(filename);
  // check rather readfile is empty
  if (countfile.good() == false) {
    cout << "ERROR: Text file not available. " << endl;
    exit(1);
  }
  if (isEmptyFile(countfile)) {
    cout << "ERROR: Empty File." << endl;
    exit(1);
  }

  // check for the array size
  while (countfile >> word) {
    if (word.compare("v") == 0) count_v++;
    else if (word.compare("vn") == 0) count_vn++;
    else if (word.compare("vt") == 0) count_vt++;
    else if (word.compare("f") == 0) count_object++;
    else if (word.compare("imisze") == 0) count_imsize ++;
    else if (word.compare("eye") == 0) count_eye ++;
    else if (word.compare("updir") == 0) count_up++;
    else if (word.compare("viewdir") == 0) count_view++;
    else if (word.compare("vfov") == 0) count_vfov++;
    else if (word.compare("mtlcolor") == 0) count_color++;
    else if (word.compare("sphere") == 0) count_object++;
    else if (word.compare("light") == 0) count_light++;
    else if (word.compare("texture") == 0) count_texture ++;
  }

  countfile.close();
  countfile.clear();



  if (count_imsize > 1) {
    cout << "Error: Requires only one IMSIZE" << endl;
    exit(1);
  }
  if (count_eye > 1) {
    cout << "Error: Requires only one EYE" << endl;
    exit(1);
  }
  if (count_up > 1) {
    cout << "Error: Requires only one UPDIR" << endl;
    exit(1);
  }
  if (count_view > 1) {
    cout << "Error: Requires only one VIEWDIR" << endl;
    exit(1);
  }
  if (count_vfov > 1) {
    cout << "Error: Requires only one VFOV" << endl;
    exit(1);
  }



  //============================================================
  //           READ DATA TO USE FOR RAY CASTING
  //============================================================

  // Basic variables
  Vector              eye;
  Vector              viewdir, updir;
  float               vfov;
  int                 imsize_x, imsize_y;

  // using variables
  ColorType           bkgcolor = {0, 0, 0};
  MaterialColor       mtlcolor;

  SphereType          sphere;     // sphere

  Vector              vertex;     // triangle
  Vector              vertex_n;
  Texture             vertex_t;
  Function            function;

  Light               light;

  ifstream            openTexture;      // reads texture file

  // arrays
  Light               *ListOfLights         = new Light[count_light];
  Object              *ListOfObjects        = new Object[count_object];
  Vector              *ListOfVertex         = new Vector[count_v];
  Vector              *ListOfVertexNormal   = new Vector[count_vn];
  Texture             *ListOfTexture        = new Texture[count_vt];
  TexturePicture      *ListOfTexturePicture = new TexturePicture[count_texture];

  // additional useful variables
  int                 light_entry = 0;
  int                 object_entry = 0;
  int                 vertex_entry = 0;
  int                 vertex_n_entry = 0;
  int                 vertex_t_entry = 0;
  int                 texture_entry = 0;

  bool                isParallel = false;     // for parallel function
  int                 pos;                    // for function.
  string              t;                      // for function.
  // texture and material
  string              texture_filename;
  bool                setting = false;        // if true, material color enables for objects.
  bool                enable_texture;
  ifstream            readfile;

  int                 depth  =  5;            // for recursion.

  // depth of view
  bool                depth_of_field = false;
  float               viewdist;

  readfile.open(filename);


  while (readfile >> word) {
    // Check whether or not it is valid statement. ex) eye..
    if (!isNumber(word)) {
      // starting to store data.
      if (word.compare("eye") == 0) {
        readfile >> word;
        printNotNumberError(word);
        eye.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        eye.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        eye.z = stof(word);
      } else if (word.compare("viewdir") == 0) {
        readfile >> word;
        printNotNumberError(word);
        viewdir.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        viewdir.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        viewdir.z = stof(word);
      } else if (word.compare("updir") == 0) {
        readfile >> word;
        printNotNumberError(word);
        updir.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        updir.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        updir.z = stof(word);
      } else if (word.compare("vfov") == 0) {
        readfile >> word;
        printNotNumberError(word);
        vfov = stof(word);
        printFOVError(vfov);
        vfov = vfov * PI / 180;       // degree 2 radian
      } else if (word.compare("imsize") == 0) {
        readfile >> word;
        printNotNumberError(word);
        imsize_x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        imsize_y = stof(word);
        printImageSizeError(imsize_x, imsize_y);
      } else if (word.compare("bkgcolor") == 0) {
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.r = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.g = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.b = stof(word);
      } else if (word.compare("mtlcolor") == 0) {
        enable_texture = false;
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.object.r = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.object.g = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.object.b = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.specular.r = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.specular.g = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.specular.b = stof(word);

        // clamp the colors, just in case;
        mtlcolor.object.clamp();
        mtlcolor.specular.clamp();

        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.a = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.d = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.s = stof(word);
        printZeroToOneError(mtlcolor.a, mtlcolor.d, mtlcolor.s);
        readfile >> word;
        printNotNumberError(word);
        mtlcolor.n = stof(word);
        readfile >> word;
        printNotNumberError(word);
        mtlcolor.opacity = stof(word);
        readfile >> word;
        printNotNumberError(word);
        mtlcolor.index_of_refraction = stof(word);
        setting = true;     // now object can be set.
      } else if (word.compare("sphere") == 0) {
        if (!setting) {
          cout << "ERROR: Material Color has to come before the Object information. " << endl;
          cout << "       Material Color information has to come first in text file." << endl;
          exit(1);
        } else {
          readfile >> word;
          printNotNumberError(word);
          sphere.x = stof(word);
          readfile >> word;
          printNotNumberError(word);
          sphere.y = stof(word);
          readfile >> word;
          printNotNumberError(word);
          sphere.z = stof(word);
          readfile >> word;
          printNotNumberError(word);
          sphere.r = stof(word);

          ListOfObjects[object_entry].sphere = sphere;
          if (enable_texture) {
              ListOfObjects[object_entry].texture_index = texture_entry;
          } else {
              ListOfObjects[object_entry].material = mtlcolor;
              ListOfObjects[object_entry].texture_index = -1;
          }
          ListOfObjects[object_entry].objectType = 0;
          object_entry ++;
        }
      } else if (word.compare("light") == 0) {
        readfile >> word;
        printNotNumberError(word);
        light.v.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        light.v.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        light.v.z = stof(word);
        readfile >> word;
        printNotNumberError(word);
        light.w = stof(word);
        printLightError (light.w);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.r = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.g = stof(word);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.b = stof(word);
        ListOfLights[light_entry] = light;
        light_entry ++;
      } else if (word.compare("v") == 0) {
        readfile >> word;
        printNotNumberError(word);
        vertex.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        vertex.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        vertex.z = stof(word);
        ListOfVertex[vertex_entry] = vertex;
        vertex_entry ++;
      } else if (word.compare("vn") == 0) {
        readfile >> word;
        printNotNumberError(word);
        vertex_n.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        vertex_n.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        vertex_n.z = stof(word);
        ListOfVertexNormal[vertex_n_entry] = vertex_n;
        vertex_n_entry ++;
      } else if (word.compare("vt") == 0) {
        readfile >> word;
        printNotNumberError(word);
        vertex_t.u = stof(word);
        readfile >> word;
        printNotNumberError(word);
        vertex_t.v = stof(word);
        ListOfTexture[vertex_t_entry] = vertex_t;
        vertex_t_entry ++;
      } else if (word.compare("f") == 0) {
        if (!setting) {
          cout << "ERROR: Material Color has to come before the Object information. " << endl;
          cout << "       Material Color information has to come first in text file." << endl;
          exit(1);
        }

        readfile >> word;
        if (isNumber(word)) {
          function.v1 = ListOfVertex[stoi(word) - 1];
          function.vn1 = {0, 0, 0};
        } else {
          pos = word.find("/");             // 1/2/3
          t = word.substr(0, pos);
          printNotNumberError(t);
          function.v1 = ListOfVertex[stoi(t) - 1];
          word = word.substr(pos + 1);
          pos = word.find("/");
          t = word.substr(0, pos);
          if (t.compare("") != 0) {
            printNotNumberError(t);
            function.vt1 = ListOfTexture[stoi(t) - 1];
          }
          word = word.substr(pos + 1);
          if (word.compare("") != 0) {
            printNotNumberError(word);
            function.vn1 = ListOfVertexNormal[stoi(word) - 1];
          } else {
            function.vn1 = {0, 0, 0};
          }
        }

        readfile >> word;
        if (isNumber(word)) {
          function.v2 = ListOfVertex[stoi(word) - 1];
          function.vn2 = {0, 0, 0};
        } else {
          pos = word.find("/");             // 1/2/3
          t = word.substr(0, pos);
          printNotNumberError(t);
          function.v2 = ListOfVertex[stoi(t) - 1];
          word = word.substr(pos + 1);
          pos = word.find("/");
          t = word.substr(0, pos);
          if (t.compare("") != 0) {
            printNotNumberError(t);
            function.vt2 = ListOfTexture[stoi(t) - 1];
          }
          word = word.substr(pos + 1);
          if (word.compare("") != 0) {
            printNotNumberError(word);
            function.vn2 = ListOfVertexNormal[stoi(word) - 1];
          } else {
            function.vn2 = {0, 0, 0};
          }
        }

        readfile >> word;
        if (isNumber(word)) {
          function.v3 = ListOfVertex[stoi(word) - 1];
          function.vn3 = {0, 0, 0};
        } else {
          pos = word.find("/");             // 1/2/3
          t = word.substr(0, pos);
          printNotNumberError(t);
          function.v3 = ListOfVertex[stoi(t) - 1];
          word = word.substr(pos + 1);
          pos = word.find("/");
          t = word.substr(0, pos);
          if (t.compare("") != 0) {
            printNotNumberError(t);
            function.vt3 = ListOfTexture[stoi(t) - 1];
          }
          word = word.substr(pos + 1);
          if (word.compare("") != 0) {
            printNotNumberError(word);
            function.vn3 = ListOfVertexNormal[stoi(word) - 1];
          } else {
            function.vn3 = {0, 0, 0};
          }
        }

        ListOfObjects[object_entry].triangle   = function;
        ListOfObjects[object_entry].objectType = 1;
        if (enable_texture) {
            ListOfObjects[object_entry].texture_index = texture_entry;
        } else {
            ListOfObjects[object_entry].material = mtlcolor;
            ListOfObjects[object_entry].texture_index = -1;
        }
        object_entry ++;
      } else if (word.compare("texture") == 0) {
        texture_entry ++;   // this enables texture.
        enable_texture = true;
        readfile >> word;
        texture_filename = word;
        if (!isPPMFile(texture_filename)) {
          cout << "ERROR: Wrong PPM File for texture" << endl;
          exit(1);
        }

        openTexture.open(texture_filename);
        openTexture >> word;
        openTexture >> word;
        printNotNumberError(word);
        ListOfTexturePicture[texture_entry].w = stoi(word);
        openTexture >> word;
        printNotNumberError(word);
        ListOfTexturePicture[texture_entry].h = stoi(word);
        ListOfTexturePicture[texture_entry].picture = readTexture(texture_filename);

        openTexture.close();
        openTexture.clear();
        setting = true;
      } else if (word.compare("parallel") == 0) {
        isParallel = true;
      } else if (word.compare("viewdist") == 0) {
        depth_of_field = true;
        readfile >> word;
        printNotNumberError(word);
        viewdist = stof(word);
      } else {
        cout << "ERROR: incorrect information" << endl;
        cout << "       Should have info name first rather than numbers." << endl;
        exit(1);
      }
    } else {
      cout << "ERROR: incorrect information" << endl;
      cout << "       Should have string first rather than numbers." << endl;
      exit(1);
    }
  }
  readfile.close();
  readfile.clear();     // close readfile.

  // Error Checking
  printParallelVectorError(updir, viewdir);
  printParallelVectorError(viewdir, updir);
  printZeroLengthVectorError(updir);
  printZeroLengthVectorError(viewdir);


  //============================================================
  //                 VIEWING PARAMETERS
  //============================================================


  float             aspect_ratio;              // imsize_x / imsize_y;
  float             w, h;                      // length of viewing window width and height
  Vector            u, v;                      // u, v vector
  Vector            ur, ul, ll, lr;            // viewing window
  Vector            view_width, view_height;
  Vector            view_center_w, view_center_h;
  Vector            view_direction;

  int               dist;                      // parallel projection
  int               scene[2];                  // 0: how many objects.
                                               // 1. how many lights

  RayType           ray;                       // eye -> intersection
  ColorType         returnColor = {0,0,0};     // final color that returns



  aspect_ratio = (float)(imsize_x) / (float)(imsize_y);   // aspect ratio
  h = 2.0 * tan(vfov / 2.0);                              // view height length
  w = h * aspect_ratio;                                   // view width length
  u = uVector(updir, viewdir);                           // u vector
  v = vVector(u, viewdir);                               // v vector


  // set view window .. d = 1
  if (!isParallel) dist = 1.0;
  else dist = 0.0;              // Parallel

  ul = eye + (dist * unitVector(viewdir)) - ((w/2.0) * u) + ((h/2.0) * v);
  ur = eye + (dist * unitVector(viewdir)) + ((w/2.0) * u) + ((h/2.0) * v);
  ll = eye + (dist * unitVector(viewdir)) - ((w/2.0) * u) - ((h/2.0) * v);
  lr = eye + (dist * unitVector(viewdir)) + ((w/2.0) * u) - ((h/2.0) * v);


  view_width    = (ur - ul) / (imsize_x - 1.0);
  view_height   = (ll - ul) / (imsize_y - 1.0);
  view_center_w = 0.5 * view_width;
  view_center_h = 0.5 * view_height;


  scene[0] = object_entry;
  scene[1] = light_entry;

  //============================================================
  //          RAYCAST CALCULATION AND OUTPUT
  //============================================================
  ofstream          savefile;       // for the output file.

  // for depth of view
  Vector            DOFpoint;
  Vector            new_origin;
  RayType           DOFray;
  ColorType         tempColor = {0,0,0};

  savefile.open(txtToPPM(filename));
  savefile << "P3" << endl;
  savefile << imsize_x << " " << imsize_y << endl;     // followed by width and height (px)
  savefile << 255 << endl;                             // followed by 255 (r,g,b)

  if (!depth_of_field) {
    for (int i = 0; i < imsize_y; i ++) {
      for (int j = 0; j < imsize_x; j ++ ) {
        view_direction = ul + (j * view_width) + (i * view_height) + view_center_w + view_center_h;
        if (!isParallel) {
          ray = rayDirection(eye, view_direction);
        } else {
          ray = {view_direction.x, view_direction.y, view_direction.z,
          unitVector(viewdir).x, unitVector(viewdir).y, unitVector(viewdir).z};
        }
        returnColor = Trace_Ray(ray, ListOfObjects, ListOfLights, ListOfTexturePicture, scene, bkgcolor, depth);
        returnColor.clamp();
        savefile << (int)(255 * returnColor.r) << " " << (int)(255 * returnColor.g) << " " << (int)(255 * returnColor.b) << " ";
      }
    }
  }
  else {  /* depth of field */
    for (int i = 0; i < imsize_y; i ++) {
      for (int j = 0; j < imsize_x; j ++ ) {
        view_direction = ul + (j * view_width) + (i * view_height) + view_center_w + view_center_h;
        if (!isParallel) {
          ray = rayDirection(eye, view_direction);
        } else {
          ray = {view_direction.x, view_direction.y, view_direction.z,
          unitVector(viewdir).x, unitVector(viewdir).y, unitVector(viewdir).z};
        }

        DOFpoint.x = ray.x + viewdist * ray.dx;
        DOFpoint.y = ray.y + viewdist * ray.dy;
        DOFpoint.z = ray.z + viewdist * ray.dz;

        returnColor = returnColor + Trace_Ray(ray, ListOfObjects, ListOfLights, ListOfTexturePicture, scene, bkgcolor, depth);
        returnColor.clamp();

        for (int k = 0; k < 15; k++) {
          new_origin   = {ray.x, ray.y, ray.z};
          new_origin.x = ray.x + (((float)rand() / (RAND_MAX * 5.0)) - 0.1);
          new_origin.y = ray.y + (((float)rand() / (RAND_MAX * 5.0)) - 0.1);     // locating random eyes
          new_origin.z = ray.z + (((float)rand() / (RAND_MAX * 5.0)) - 0.1);
          DOFray = rayDirection(new_origin, DOFpoint);
          tempColor = Trace_Ray(DOFray, ListOfObjects, ListOfLights, ListOfTexturePicture, scene, bkgcolor, depth);
          tempColor.clamp();
          returnColor = returnColor + tempColor;
        }
        returnColor = returnColor / 16.0;
        savefile << (int)(255 * returnColor.r) << " " << (int)(255 * returnColor.g) << " " << (int)(255 * returnColor.b) << " ";
      }
    }
  }
  //
  // view_direction = ul + (249 * view_width) + (249 * view_height) + view_center_w + view_center_h;
  // ray = {view_direction.x, view_direction.y, view_direction.z,
  // unitVector(viewdir).x, unitVector(viewdir).y, unitVector(viewdir).z};
  // returnColor = Trace_Ray(ray, ListOfObjects, ListOfLights, ListOfTexturePicture, scene, bkgcolor, depth);
  // returnColor.clamp();

  // deallocation
  delete[] ListOfLights;
  delete[] ListOfObjects;
  delete[] ListOfVertex;
  delete[] ListOfVertexNormal;
  delete[] ListOfTexture;
  delete[] ListOfTexturePicture;


  return 0;
}

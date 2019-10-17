// Dongha Kang
// September 11th, 2019
// CSCI 5706, University of Minnesota

#include <iostream>
#include <fstream>
#include <cmath>
#include "vectorMath.hpp"
#include "File.hpp"
#include "Shading.hpp"
#include "Texture.hpp"

using namespace std;




int main(int argc, char *argv[]) {

  if (argc <= 1) {
    cout<< "ERROR: Text file not available. " << endl;
    exit(1);
  }

  string        filename = argv[1];
  ifstream      countfile, readfile;
  ofstream      savefile;

  // only for counting.
  int           count_eye = 0;
  int           count_view = 0;
  int           count_vfov = 0;
  int           count_up = 0;
  int           count_imsize = 0;

  // to create array
  int           count_color = 1;
  int           count_sphere = 1;
  int           count_light = 1;
  int           count_spotlight = 1;
  int           count_v = 1;
  int           count_f = 1;
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
    else if (word.compare("f") == 0) count_f++;
    else if (word.compare("imisze") == 0) count_imsize ++;
    else if (word.compare("eye") == 0) count_eye ++;
    else if (word.compare("updir") == 0) count_up++;
    else if (word.compare("viewdir") == 0) count_view++;
    else if (word.compare("vfov") == 0) count_vfov++;
    else if (word.compare("mtlcolor") == 0) count_color++;
    else if (word.compare("sphere") == 0) count_sphere++;
    else if (word.compare("light") == 0) count_light++;
    else if (word.compare("spotlight") == 0) count_spotlight++;
    else if (word.compare("texture") == 0) {
      count_color++;
      count_texture++;
    }
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

  readfile.open(filename);


  // FILE CHECK ENDED HERE.

  Vector            eye;
  Vector            viewdir, updir;
  float             vfov;
  int               imsize_x, imsize_y;
  MaterialColor     bkgcolor;
  MaterialColor     mtlcolor;
  SphereType        sphere;
  Light             light;
  Spotlight         spotlight;

  // 0: color, 1: circle, 2: ellipsoid, 3: light, 4: spotlight
  int               number_of_objects[10];
  int               color_entry = 1;
  int               sphere_entry = 0;          // sphere Entry related to List of Sphere
  int               light_entry = 0;
  int               spotlight_entry = 0;
  RayType           ray;                       // store eye, and direction
  ColorType         returnColor;               // ColorType returnColor;

  MaterialColor     *ListOfColors     = new MaterialColor[count_color + 1];         // background color
  SphereType        *ListOfSphere     = new SphereType[count_sphere];
  Light             *ListOfLights     = new Light[count_light];
  Spotlight         *ListOfSpotlight  = new Spotlight[count_spotlight];


  // ******************************************************
  // *************  A S S I G N M E N T  1 C **************
  // ******************************************************
  Vector            vertex;     // vertex that represents triangles
  Vector            vertex_n;
  Texture           vertex_t;

  Function          function;   // functions that deals with the vertexes
  Vector            *ListOfVertex         = new Vector[count_v];   //
  Vector            *ListOfVertexNormal   = new Vector[count_vn];
  Texture           *ListOfTexture        = new Texture[count_vt];
  Function          *ListOfFunctions      = new Function[count_f];
  TexturePicture    *ListOfTexturePicture = new TexturePicture[count_texture];

  int               vertex_entry = 0;
  int               vertex_n_entry = 0;
  int               vertex_t_entry = 0;
  int               function_entry = 0;
  int               texture_entry = 0;

  string            textureFileName;
  ifstream          openTexture;

  bool              isParallel = false;

  // ******************************************************
  // For Calculations
  // ******************************************************

  float             aspect_ratio;              // imsize_x / imsize_y;
  float             w, h;                      // length of viewing window width and height
  Vector            u, v;                      // u, v vector
  Vector            ur, ul, ll, lr;            // viewing window
  Vector            view_width, view_height;
  Vector            view_center_w, view_center_h;
  Vector            view_direction;

  int               pos;
  string            t;        // temporary string for function.


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
        vfov = vfov * PI / 180;
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
        bkgcolor.object.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.object.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        bkgcolor.object.b = int(stof(word) * 255);
        ListOfColors[0] = bkgcolor;
      } else if (word.compare("mtlcolor") == 0) {
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.object.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.object.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.object.b = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.specular.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.specular.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        mtlcolor.specular.b = int(stof(word) * 255);
        clamp(mtlcolor.specular);
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
        mtlcolor.texture = -1;
        ListOfColors[color_entry] = mtlcolor;
        color_entry ++;
      } else if (word.compare("sphere") == 0) {
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
        sphere.m = color_entry - 1;           // save the color data.
        ListOfSphere[sphere_entry] = sphere;
        sphere_entry ++;
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
        printLightEror (light.w);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.b = int(stof(word) * 255);
        light.c.x = -1;
        light.c.y = -1;
        light.c.z = -1;
        ListOfLights[light_entry] = light;
        light_entry ++;
      } else if (word.compare("spotlight") == 0) {
        readfile >> word;
        printNotNumberError(word);
        spotlight.v.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.v.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.v.z = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.dir.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.dir.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.dir.z = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.theta = stof(word);
        spotlight.theta = spotlight.theta * PI / 180;
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        spotlight.color.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        spotlight.color.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        spotlight.color.b = int(stof(word) * 255);
        spotlight.c.x = -1;
        spotlight.c.y = -1;
        spotlight.c.z = -1;
        ListOfSpotlight[spotlight_entry] = spotlight;
        spotlight_entry ++;
      } else if (word.compare("attlight") == 0) {
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
        printLightEror (light.w);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        light.color.b = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        light.c.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        light.c.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        light.c.z = stof(word);
        ListOfLights[light_entry] = light;
        light_entry ++;
      } else if (word.compare("attspotlight") == 0) {
        readfile >> word;
        printNotNumberError(word);
        spotlight.v.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.v.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.v.z = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.dir.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.dir.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.dir.z = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.theta = stof(word);
        spotlight.theta = spotlight.theta * PI / 180;
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        spotlight.color.r = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        spotlight.color.g = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        printValueError (stof(word));
        spotlight.color.b = int(stof(word) * 255);
        readfile >> word;
        printNotNumberError(word);
        spotlight.c.x = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.c.y = stof(word);
        readfile >> word;
        printNotNumberError(word);
        spotlight.c.z = stof(word);
        ListOfSpotlight[spotlight_entry] = spotlight;
        spotlight_entry ++;
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

        function.m = color_entry - 1;           // save the color data.
        ListOfFunctions[function_entry] = function;
        function_entry ++;

      } else if (word.compare("texture") == 0) {
        readfile >> word;
        textureFileName = word;
        if (!isPPMFile(textureFileName)) {
          cout << "ERROR: Wrong PPM File for texture" << endl;
          exit(1);
        }
        ListOfColors[color_entry].texture = texture_entry;

        openTexture.open(textureFileName);
        openTexture >> word;
        openTexture >> word;
        printNotNumberError(word);
        ListOfTexturePicture[texture_entry].w = stoi(word);
        openTexture >> word;
        printNotNumberError(word);
        ListOfTexturePicture[texture_entry].h = stoi(word);
        ListOfTexturePicture[texture_entry].picture = readTexture(textureFileName);

        openTexture.close();
        openTexture.clear();

        texture_entry ++;
        color_entry++;
      } else if (word.compare("parallel") == 0) {
        isParallel = true;
      }
        else {
        cout << "ERROR: incorrect information" << endl;
        cout << "       should have eye, viewdir, updir, vfov, imsize followed by numbers" << endl;
        exit(1);
      }
    } else {
      cout << "ERROR: incorrect information" << endl;
      cout << "       Should have info name first rather than numbers." << endl;
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




  aspect_ratio = (float)(imsize_x) / (float)(imsize_y);   // aspect ratio
  h = 2.0 * tan(vfov / 2.0);                              // view height length
  w = h * aspect_ratio;                                   // view width length
  u = u_vector(updir, viewdir);                           // u vector
  v = v_vector(u, viewdir);                               // v vector

  int dist;
  // set view window .. d = 1
  if (!isParallel) dist = 1.0;
  else dist = 0.0;

  ul = eye + m(UnitVector(viewdir), dist) - m(u, w/2.0) + m(v, h/2.0);
  ur = eye + m(UnitVector(viewdir), dist) + m(u, w/2.0) + m(v, h/2.0);
  ll = eye + m(UnitVector(viewdir), dist) - m(u, w/2.0) - m(v, h/2.0);
  lr = eye + m(UnitVector(viewdir), dist) + m(u, w/2.0) - m(v, h/2.0);

  view_width = d(ur - ul, imsize_x - 1.0);
  view_height = d(ll - ul, imsize_y - 1.0);
  view_center_w = m(view_width, 0.5);             // ∆ch
  view_center_h = m(view_height, 0.5);            // ∆cv



  number_of_objects[0] = color_entry;
  number_of_objects[1] = sphere_entry;
  // number_of_objects[2] = ellipsoid_entry;
  number_of_objects[3] = light_entry;
  number_of_objects[4] = spotlight_entry;
  number_of_objects[5] = vertex_entry;
  number_of_objects[6] = function_entry;

  // Reading Texture



  // OUTPUT
  savefile.open(txtToPPM(filename));
  savefile << "P3" << endl;
  savefile << imsize_x << " " << imsize_y << endl;     // followed by width and height (px)
  savefile << 255 << endl;                             // followed by 255 (r,g,b)

  for (int i = 0; i < imsize_y; i ++) {
    for (int j = 0; j < imsize_x; j ++ ) {
      view_direction = ul + m(view_width, j) + m(view_height, i) + view_center_w + view_center_h;
      if (!isParallel) {
        ray = rayDirection(eye, view_direction);
      } else {
        ray = {view_direction.x, view_direction.y, view_direction.z,
               UnitVector(viewdir).x, UnitVector(viewdir).y, UnitVector(viewdir).z};
      }
      returnColor = Trace_Ray(ray, ListOfSphere, ListOfFunctions, ListOfTexturePicture, ListOfColors, ListOfLights, ListOfSpotlight, number_of_objects);
      savefile << returnColor.r << " " << returnColor.g << " " << returnColor.b << " ";
    }
  }

  delete[] ListOfColors;
  delete[] ListOfFunctions;
  delete[] ListOfTexturePicture;
  delete[] ListOfSphere;
  delete[] ListOfLights;
  delete[] ListOfSpotlight;
  delete[] ListOfVertex;
  delete[] ListOfVertexNormal;
  delete[] ListOfTexture;

  return 0;
}

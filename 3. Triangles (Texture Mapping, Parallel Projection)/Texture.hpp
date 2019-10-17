#include <string>

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define EPSOLIN 0.0001

ColorType** readTexture (std::string filename);


ColorType TexturePosition(ColorType** texture, int w, int h,
        SphereType sphere, Vector point);
ColorType TexturePosition(ColorType** texture, int w, int h,
        Function function, float alpha, float beta, float gamma);

#endif  // TEXTURE_HPP

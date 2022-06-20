#pragma once

class Texture
{
public:
    char *pixels;

    static Texture *BlackTexture(unsigned int width, unsigned int height)
    {
        auto tex = new Texture();
        tex->pixels = new char[width * height * 3];

        for (int i = 0; i < width * height * 3; i++)
        {
            tex->pixels[i] = 255;
        }

        return tex;
    }
};
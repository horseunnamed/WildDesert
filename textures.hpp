#ifndef TEXTURES_H
#define TEXTURES_H

#include "main_header.hpp"

class TextureCache
{
private:

    map<string, Texture> textures;

public:

    Texture* get(string fileName)
    {
        auto textureEntry = textures.find(fileName);
        if (textureEntry == textures.end())
        {
            if (load(fileName))
            {
                return &textures[fileName];
            }
            else
            {
                return nullptr;
            }
        }

        return &(textureEntry->second);
    }

private:

    bool load(string fileName)
    {
        return textures[fileName].loadFromFile(fileName);
    }
};


#endif

#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "main_header.hpp"

class GameMap
{

private:
    Texture* tileset;

    int width; // in tiles
    int height; // in tiles
    int tileWidth; // in pixels
    int tileHeight; // in pixels

    vector<int*> tileLayers;
    vector<RenderTexture*> textures;

    vector<FloatRect> walls;

public:

    GameMap(string filename, TextureCache* textureCache)
    {
        XMLDocument doc;
        doc.LoadFile(filename.c_str());
        if (doc.ErrorID())
        {
            cout << "Can't load file " << filename << endl;
        }

        XMLElement* mapElement = doc.FirstChildElement("map");
        mapElement->QueryIntAttribute("width", &width);
        mapElement->QueryIntAttribute("height", &height);
        mapElement->QueryIntAttribute("tilewidth", &tileWidth);
        mapElement->QueryIntAttribute("tileheight", &tileHeight);

        parseTileset(mapElement->FirstChildElement("tileset"), textureCache);

        // parse layers
        XMLElement* layerElement = mapElement->FirstChildElement("layer");
        while (layerElement != nullptr)
        {
            parseTileLayer(layerElement);
            layerElement = layerElement->NextSiblingElement("layer");
        }

        XMLElement* objectgroupElement = mapElement->FirstChildElement("objectgroup");
        while (objectgroupElement != nullptr)
        {
            parseObjectLayer(objectgroupElement);
            objectgroupElement = objectgroupElement->NextSiblingElement("objectgroup");
        }

        drawTextures();

        cout << filename << " was loaded successfully" << endl;
    }

    ~GameMap()
    {
        for (size_t i = 0; i < tileLayers.size(); ++i)
        {
            delete tileLayers[i];
        }

        for (size_t i = 0; i < textures.size(); ++i)
        {
            delete textures[i];
        }
    }

    void drawToWindow(RenderWindow* renderWindow)
    {
        for (size_t i = 0; i < textures.size(); ++i)
        {
            Sprite sprite(textures[i]->getTexture());
            renderWindow->draw(sprite);
        }
    }

    vector<FloatRect>* getWalls()
    {
        return &walls;
    }

private:

    void parseTileset(XMLElement* tilesetElement, TextureCache* textureCache)
    {
        XMLElement* imageElement = tilesetElement->FirstChildElement("image");
        string source = imageElement->Attribute("source");
        tileset = textureCache->get(source);
    }


    void parseTileLayer(XMLElement* layerElement)
    {
        int* newLayer = new int[height * width];

        XMLElement* dataElement = layerElement->FirstChildElement("data");
        XMLElement* tileElement = dataElement->FirstChildElement("tile");
        int counter = 0;
        while (tileElement != nullptr)
        {
            tileElement->QueryIntAttribute("gid", &newLayer[counter++]);
            tileElement = tileElement->NextSiblingElement("tile");
        }

        tileLayers.push_back(newLayer);
    }

    void parseObjectLayer(XMLElement* objectgroupElement)
    {
        XMLElement* objectElement = objectgroupElement->FirstChildElement("object");
        while (objectElement != nullptr)
        {
            float x, y, width, height;
            objectElement->QueryFloatAttribute("x", &x);
            objectElement->QueryFloatAttribute("y", &y);
            objectElement->QueryFloatAttribute("width", &width);
            objectElement->QueryFloatAttribute("height", &height);

            cout << x << " " << y << " " << width << " " << height << endl;

            objectElement = objectElement->NextSiblingElement("object");

            walls.push_back(FloatRect(x, y, width, height));
        }
    }

    Vector2i getPositionForTile(int ind)
    {
        return Vector2i((ind % width) * tileWidth, (ind / width) * tileHeight);
    }

    IntRect getRectForGid(int gid)
    {
        IntRect result;
        gid -= 1;
        int tilesetWidth = tileset->getSize().x / tileWidth; // in tiles!
        result.left = (gid % tilesetWidth) * tileWidth;
        result.top = (gid / tilesetWidth) * tileHeight;
        result.width = tileWidth;
        result.height = tileHeight;
        return result;
    }

    void drawTextures()
    {
        Sprite tile;
        tile.setTexture(*tileset);
        for (size_t i = 0; i < tileLayers.size(); ++i)
        {
            int* layer = tileLayers[i];
            RenderTexture* renderTexture = new RenderTexture(); // target for rendering map layer
            renderTexture->create(width * tileWidth, height * tileHeight, false);

            for (int j = 0; j < width * height; ++j)
            {
                if (layer[j] > 0)
                {
                    tile.setTextureRect(getRectForGid(layer[j]));
                    Vector2i tilePos = getPositionForTile(j);
                    tile.setPosition(tilePos.x, tilePos.y);
                    renderTexture->draw(tile);
                }
            }
            renderTexture->display();

            textures.push_back(renderTexture);
        }
    }

};

#endif

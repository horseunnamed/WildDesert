#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "main_header.hpp"

class Animation
{
private:

    Texture* spriteSheet;
    string name;
    vector<int> frames;
    Vector2u spriteSize; // in pixels
    Vector2u framePadding;
    Time frameDuration = milliseconds(150);
    Time startTime;
    Time currentTime;

    bool flipped;

public:

    Animation(string _name, Texture* _spriteSheet, Vector2u _spriteSize, Vector2u _framePadding, vector<int> _frames) :
        spriteSheet(_spriteSheet),
        name(_name),
        frames(_frames),
        spriteSize(_spriteSize),
        framePadding(_framePadding)
    {
        flipped = false;
    }

    IntRect getRect()
    {
        int frameNum =
            ((int) ((currentTime - startTime) / frameDuration)) % frames.size();
        int frame = frames[frameNum];

        Vector2u textureSize = spriteSheet->getSize();

        int sw = textureSize.x / spriteSize.x; // amount of sprites in width of texture
        int sh = textureSize.y / spriteSize.y; // ... in height

        int x = (frame % sw) * spriteSize.x + framePadding.x;
        int y = (frame / sh) * spriteSize.y + framePadding.y;

        return IntRect(
                   flipped ? (x + spriteSize.x - framePadding.x) : x,
                   y,
                   flipped ? (-spriteSize.x + framePadding.x) : (spriteSize.x - framePadding.x),
                   spriteSize.y - framePadding.y);
    }

    void setCurrentTime(Time time)
    {
        currentTime = time;
    }

    void start(Time time)
    {
        startTime = time;
    }

    Texture* getSpriteSheet()
    {
        return spriteSheet;
    }

    Vector2u getSpriteSize()
    {
        return spriteSize;
    }

    void setFlipped(bool value)
    {
        flipped = value;
    }
};

class AnimationManager
{
private:

    map<string, Animation> animations;
    TextureCache* textureCache;

public:

    AnimationManager(TextureCache* _textureCache) :
        textureCache(_textureCache)
    {

    }

    bool load(string fileName)
    {
        ifstream input(fileName);
        if (!input)
        {
            cout << "Can't load sprite file " + fileName << endl;
            return false;
        }

        string textureFile;
        input >> textureFile; 	// first line - path to spritesheet
        // WARN: not dealing with whitespaces in path!

        Texture* spriteSheet = textureCache->get(textureFile);

        Vector2u spriteSize;
        input >> spriteSize.x; // sprite size in px goes next
        input >> spriteSize.y;

        Vector2u framePadding; // padding for frame by horizontal and vertical directions
        input >> framePadding.x;
        input >> framePadding.y;

        int animationsNum; // total amount of animations that describes below
        input >> animationsNum;

        for (int animationInd = 0; animationInd < animationsNum; ++animationInd)
        {
            string animationId; // string identifier of this animation for caching
            input >> animationId;

            int framesNum; // total amount of frames
            input >> framesNum;

            vector<int> frames; // frames ids itself in right-down direction
            for (int frameInd = 0; frameInd < framesNum; ++frameInd)
            {
                int frame;
                input >> frame;
                frames.push_back(frame);
            }

            animations.insert(pair<string, Animation>(animationId,
                              Animation(animationId, spriteSheet, spriteSize, framePadding, frames)));
        }

        return true;
    }

    Animation* get(string animationId)
    {
        auto animationsEntry = animations.find(animationId);
        if (animationsEntry != animations.end())
            return &(animationsEntry->second);
        return nullptr;
    }

    void updateAll(Time currentTime)
    {
        for (auto it = animations.begin(); it != animations.end(); ++it)
        {
            (it->second).setCurrentTime(currentTime);
        }
    }
};

#endif

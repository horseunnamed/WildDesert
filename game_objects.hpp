#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include "main_header.hpp"

class GameObject
{
protected:
    FloatRect* body;
    Sprite* sprite;
    Animation* animation;

    bool flipped;

public:

    GameObject(FloatRect* _body, Sprite* _sprite) : body(_body), sprite(_sprite), flipped(false)
    {
        syncSpriteAndBody();
    }

    ~GameObject()
    {
        delete body;
        delete sprite;
        // do not delete animation because it is AnimationManager's care
    }

    bool move(Vector2f value)
    {
        if (body != nullptr)
        {
            body->left += value.x;
            body->top += value.y;

            syncSpriteAndBody();

            return true;
        }

        return false;
    }

    void drawToWindow(RenderWindow* window)
    {
        if (sprite != nullptr)
        {
            if (animation != nullptr)
            {
                animation->setFlipped(flipped);
                sprite->setTexture(*(animation->getSpriteSheet()));
                sprite->setTextureRect(animation->getRect());
            }

            window->draw(*sprite);
        }
    }

    void collideWith(GameObject* anotherObject)
    {

    }

    void setAnimation(Animation* animation)
    {
        this->animation = animation;
    }

    void setFlipped(bool value)
    {
        flipped = value;
    }

    FloatRect* getBody()
    {
        return body;
    }

    Vector2f getPosition()
    {
        return (body != nullptr) ? Vector2f(body->left, body->top) : Vector2f(0, 0);
    }

protected:

    void syncSpriteAndBody()
    {
        if (sprite != nullptr && body != nullptr)
        {
            sprite->setPosition(body->left, body->top);
        }
    }

};

class Player : public GameObject
{
private:

public:
    Vector2f velocity;

    Player() : GameObject(new FloatRect(0, 0, 16, 20), new Sprite())
    {
        velocity.x = 1;
        velocity.y = 1;

        flipped = false;
    }

};

#endif

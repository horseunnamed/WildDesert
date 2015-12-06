#ifndef GAME_H
#define GAME_H

#include "main_header.hpp"

class Game
{
private:
    Clock timer;

    TextureCache* textureCache;
    AnimationManager* animationManager;

    GameMap* gameMap;
    Player player;

public:

    Game()
    {
        textureCache = new TextureCache();
        animationManager = new AnimationManager(textureCache);

        animationManager->load("res/player1_sprite.txt");

        gameMap = new GameMap("level.tmx", textureCache);

        timer.restart();

        player.setAnimation(animationManager->get("walk_unarmed"));
    }

    ~Game()
    {
        delete textureCache;
        delete animationManager;
        delete gameMap;
    }

    void iteration()
    {
        Vector2f direction;

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            direction.x = player.velocity.x;
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            direction.x = -player.velocity.x;
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            direction.y = -player.velocity.y;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            direction.y = player.velocity.y;
        }

        if (direction.x != 0 || direction.y != 0)
        {
            player.setAnimation(animationManager->get("walk_unarmed"));
            if (direction.x > 0)
                player.setFlipped(false);
            else if (direction.x < 0)
                player.setFlipped(true);

            moveObject(&player, Vector2f(direction.x, 0));
            moveObject(&player, Vector2f(0, direction.y));
        }
        else
        {
            player.setAnimation(animationManager->get("stand_unarmed"));
        }

        animationManager->updateAll(timer.getElapsedTime());
    }

    void rendering(RenderWindow* window)
    {
        View camera(window->getDefaultView());
        camera.setCenter(player.getPosition());
        camera.zoom(0.5);

        window->setView(camera);
        gameMap->drawToWindow(window);
        player.drawToWindow(window);
    }

private:

    void moveObject(GameObject* object, Vector2f direction)
    {
        if (object->move(direction))
        {
            auto walls = gameMap->getWalls();
            for (auto wallsIt = walls->begin(); wallsIt != walls->end(); ++wallsIt)
            {
                if (wallsIt->intersects(*(object->getBody())))
                {
                    object->move(-direction);
                    return;
                }
            }
        }
    }

};

#endif

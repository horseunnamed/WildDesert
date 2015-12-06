#include "main_header.hpp"

int main()
{
    RenderWindow window(VideoMode(640, 480), "Window");

    window.setFramerateLimit(60);

    Game game;

    Event windowEvent;

    while(window.isOpen())
    {
        while(window.pollEvent(windowEvent))
        {
            if (windowEvent.type == Event::Closed)
            {
                window.close();
                break;
            }
        }

        game.iteration();
        game.rendering(&window);

        window.display();
        window.clear();
    }

    return 0;
}

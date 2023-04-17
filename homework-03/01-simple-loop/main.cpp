#include <SDL3/SDL.h>
#include <iostream>
#include <map>

std::map<SDL_Keycode, const char*> match{ { SDLK_w, "Up" },
                                          { SDLK_a, "Left" },
                                          { SDLK_s, "Down" },
                                          { SDLK_d, "Right" },
                                          { SDLK_LCTRL, "first-button" },
                                          { SDLK_SPACE, "second-button" },
                                          { SDLK_ESCAPE, "select" },
                                          { SDLK_RETURN, "start" } };

int main(int, char*[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    SDL_Window* window =
        SDL_CreateWindow("Homework-03-01", 300, 300, SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        return 1;
    }

    bool      run = true;
    SDL_Event Event;

    while (run)
    {
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_EVENT_QUIT)
            {
                run = false;
            }
            if (match.contains(Event.key.keysym.sym))
            {
                if (Event.type == SDL_EVENT_KEY_DOWN)
                {
                    std::cout << match[Event.key.keysym.sym] << " is pressed!"
                              << std::endl;
                }
                if (Event.type == SDL_EVENT_KEY_UP)
                {
                    std::cout << match[Event.key.keysym.sym] << " is released!"
                              << std::endl;
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

#include "engine.h"
#include <SDL3/SDL.h>
#include <map>

std::map<SDL_Keycode, const char*> match{ { SDLK_w, "Up" },
                                          { SDLK_a, "Left" },
                                          { SDLK_s, "Down" },
                                          { SDLK_d, "Right" },
                                          { SDLK_LCTRL, "first-button" },
                                          { SDLK_SPACE, "second-button" },
                                          { SDLK_ESCAPE, "select" },
                                          { SDLK_RETURN, "start" } };
SDL_Window* window;
static bool is_exist = false;

void Engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("Could not initialize!");
    }

    window = SDL_CreateWindow("Homework-03-02", 300, 300, SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        throw std::runtime_error("Сould not create a window!");
    }
}

bool Engine::read_events()
{
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_EVENT_QUIT)
        {
            return false;
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
    return true;
}

void Engine::clean()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Engine* create_engine()
{
    if (is_exist)
    {
        throw std::runtime_error("Engine exists!");
    }
    is_exist = true;
    return new Engine();
}

void delete_engine(Engine* engine)
{
    if (is_exist)
    {
        if (engine == nullptr)
        {
            throw std::runtime_error("Engine is nullptr");
        }
        else
        {
            is_exist = false;
            delete engine;
        }
    }
    else
    {
        throw std::runtime_error("Engine does not exist!");
    }
}

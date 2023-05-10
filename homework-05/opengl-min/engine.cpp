#include "engine.hpp"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <map>

std::map<SDL_Keycode, int> match{
    { SDLK_w, 1 }, { SDLK_a, 2 }, { SDLK_s, 3 }, { SDLK_d, 4 }
};

void* load_func(const char* name)
{
    SDL_FunctionPointer func_ptr = SDL_GL_GetProcAddress(name);
    return reinterpret_cast<void*>(func_ptr);
}

void check_errors()
{
    int num_of_error = static_cast<int>(glGetError());
    if (num_of_error != GL_NO_ERROR)
    {
        switch (num_of_error)
        {
            case GL_INVALID_ENUM:
                std::cerr << "Error: GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                std::cerr << "Error: GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                std::cerr << "Error: GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr << "Error: GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr << "Error: GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_OVERFLOW:
                std::cerr << "Error: GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                std::cerr << "Error: GL_STACK_UNDERFLOW";
                break;
        }
    }
}

SDL_Window*   window         = nullptr;
SDL_GLContext opengl_context = nullptr;
static bool   is_exist       = false;
int           choose_color   = -1;

void Engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("Engine could not initialize!");
    }

    window = SDL_CreateWindow("Homework-05-02", 300, 300, SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        throw std::runtime_error("Engine could not create a window!");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    opengl_context = SDL_GL_CreateContext(window);

    if (opengl_context == nullptr)
    {
        throw std::runtime_error("Engine could not create OpenGL ES context!");
    }

    if (!gladLoadGLES2Loader(load_func))
    {
        throw std::runtime_error(
            "Engine could not initialize OpenGL ES functions!");
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
                choose_color = match[Event.key.keysym.sym];
            }
        }
    }
    return true;
}

void Engine::clean()
{
    SDL_GL_DeleteContext(opengl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::update()
{
    switch (choose_color)
    {
        case 1:
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 2:
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case 3:
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case 4:
            glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
            break;
        default:
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            break;
    }
    check_errors();
    glClear(GL_COLOR_BUFFER_BIT);
    check_errors();
    SDL_GL_SwapWindow(window);
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
